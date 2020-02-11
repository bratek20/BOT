#include "ScreenHelperCUDA.h"
#include "BOT.h"
#include "Params.h"

#include <stdio.h>

using namespace std;

__constant__ float MATCH_THRESHOLD;

__device__ __forceinline__ int matches(int num, int area) {
    return (float)num / area >= MATCH_THRESHOLD;
};

__device__ __forceinline__ int posToIdx(int x, int y, dim3 tabSize) {
    return y * tabSize.x + x;
}

__device__ float3 colorAt(float* tab, int x, int y, dim3 tabSize) {
    int idx = posToIdx(x, y, tabSize);
    float3 ans;
    ans.x = tab[3 * idx];
    ans.x = tab[3 * idx + 1];
    ans.x = tab[3 * idx + 2];
    return ans;
}

__device__ __forceinline__ float3 colorAt(float3* tab, int x, int y, dim3 tabSize) {
    return tab[posToIdx(x, y, tabSize)];
}

__global__ void matchRect(float* screen, dim3 screenSize, float* rect, dim3 rectSize, int* ans) {
    int idx = blockIdx.x * blockDim.x  + threadIdx.x;
    int hit = 0, cnt = 0;
    int startPointY = idx / screenSize.x;
    int startPointX = idx % screenSize.x;
    int rectArea = rectSize.x * rectSize.y;
    if (idx >= screenSize.x * screenSize.y) {
        return;
    }

    for (int y = startPointY; y < startPointY + rectSize.y && y < screenSize.y; y++) {
        for (int x = startPointX; x < startPointX + rectSize.x && x < screenSize.x; x++) {
            float3 sc = colorAt(screen, x, y, screenSize);
            float3 rc = colorAt(rect, x - startPointX, y - startPointY, rectSize);
            
            hit += sc.x == rc.x && sc.y == rc.y && sc.z == rc.z;
            cnt++;

            int possibleBest = hit + rectArea - cnt;
            if (!matches(possibleBest, rectArea)) {
                ans[idx] = 0;
                return;
            }
        }
    }
    
    ans[idx] = matches(hit, rectArea);
}

__global__ void matchRect32(float* globalScreen, dim3 globalScreenSize, float* rect, dim3 rectSize, int* ans) {
    int threadStartY = threadIdx.x / 32;
    int threadStartX = threadIdx.x % 32;
    dim3 screenSize = dim3(32 * 2, 32 * 2);
    __shared__ float3 screen[32 * 2 * 32 * 2];
    int rectArea = rectSize.x * rectSize.y;
    
    int blockLength = 32;
    int blocksPerLine = ceilf(globalScreenSize.x / blockLength);
    int blockStartY = (blockIdx.x / blocksPerLine) * blockLength;
    int blockStartX = (blockIdx.x % blocksPerLine) * blockLength;
    if (blockStartX >= globalScreenSize.x || blockStartY >= globalScreenSize.y) {
        return;
    }

    for (int y = threadStartY; y < screenSize.y; y += 32) {
        for (int x = threadStartX; x < screenSize.x; x += 32) {
            int globalX = blockStartX + x;
            int globalY = blockStartY + y;
            if (globalX < globalScreenSize.x && globalY < globalScreenSize.y) {
                screen[posToIdx(x, y, screenSize)] = colorAt(globalScreen, globalX, globalY, globalScreenSize);
            }
        }
    }
    __syncthreads();

    int hit = 0;
    int cnt = 0;
    int idx = posToIdx(blockStartX + threadStartX, blockStartY + threadStartY, globalScreenSize);
    for (int y = threadStartY; y < threadStartY + rectSize.y; y++) {
        for (int x = threadStartX; x < threadStartX + rectSize.x; x++) {
            float3 sc = colorAt(screen, x, y, screenSize);
            float3 rc = colorAt(rect, x - threadStartX, y - threadStartY, rectSize);

            hit += sc.x == rc.x && sc.y == rc.y && sc.z == rc.z;
            cnt++;

            int possibleBest = hit + rectArea - cnt;
            if (!matches(possibleBest, rectArea)) {
                ans[idx] = 0;
                return;
            }
        }
    }

    ans[idx] = matches(hit, rectArea);
}

constexpr int MAX_ANS_SIZE = 1920 * 1080 * 42;
int ans[MAX_ANS_SIZE];

Point ScreenHelperCUDA::find(const BmpRect& rect, bool forceSlow) {
    const Screen& screen = BOT::screen;

    // Choose which GPU to run on, change this on a multi-GPU system.
    if (cudaSetDevice(0) != cudaSuccess) {
        Logger::error("ScreenHelperCUDA::find()", "cudaSetDevice failed!");
    }

    float* h_screen = reinterpret_cast<float*>(screen._screen);
    float* h_rect = reinterpret_cast<float*>(rect._colors);

    dim3 screenSize = dim3(screen.width(), screen.height());
    dim3 rectSize = dim3(rect.width(), rect.height());
    int ansSize = screen.size();

    float* d_screen = mallocAndCpy(h_screen, screenSize.x * screenSize.y * 3);
    float* d_rect = mallocAndCpy(h_rect, rectSize.x * rectSize.y * 3);
    int* d_ans = checkMalloc<int>(ansSize);

    cudaMemcpyToSymbol(MATCH_THRESHOLD, &Params::MATCH_THRESHOLD, sizeof(float));



    if (rectSize.x == 32 && rectSize.y == 32 && !forceSlow) {
        int threads = 32 * 32;
        int blockLength = 32;
        int blocks = static_cast<int>(ceil(screen.width() / blockLength) * ceil(screen.height() / blockLength));
        Logger::info("ScreenHelperCUDA::find()", "kernel matchRect32");
        Logger::info("ScreenHelperCUDA::find()", "blocks = " + to_string(blocks) + ", threads = " + to_string(threads));
        matchRect32 <<<blocks, threads >>> (d_screen, screenSize, d_rect, rectSize, d_ans);
    }
    else {
        int threads = 512;
        int blocks = static_cast<int>(ceil(screen.size() / threads));
        Logger::info("ScreenHelperCUDA::find()", "kernel matchRect");
        Logger::info("ScreenHelperCUDA::find()", "blocks = " + to_string(blocks) + ", threads = " + to_string(threads));
        matchRect <<<blocks, threads >>> (d_screen, screenSize, d_rect, rectSize, d_ans);
    }

    cudaDeviceSynchronize();

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        Logger::error("ScreenHelperCUDA::find()", "kernel failed");
    }

    checkCpy(ans, d_ans, ansSize, cudaMemcpyDeviceToHost);
    
    cudaFree(d_screen);
    cudaFree(d_rect);
    cudaFree(d_ans);

    for (int i = 0; i < ansSize; i++) {
        if (ans[i]) {
            Logger::info("ScreenHelperCUDA::find()", "!!! found !!!");
            return Point(i % screen.width(), i / screen.width());
        }
    }
    Logger::info("ScreenHelperCUDA::find()", "not found");
    return Point();
}
