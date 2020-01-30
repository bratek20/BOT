#include "ScreenHelperCUDA.h"
#include "BOT.h"
#include "Params.h"

#include <stdio.h>

using namespace std;

__constant__ float MATCH_THRESHOLD;

__device__ int matches(int num, int area) {
    return (float)num / area >= MATCH_THRESHOLD;
};

__device__ float3 colorAt(float* tab, dim3 tabSize, int x, int y) {
    int idx = y * tabSize.x + x;
    float3 ans;
    ans.x = tab[3 * idx];
    ans.x = tab[3 * idx + 1];
    ans.x = tab[3 * idx + 2];
    return ans;
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
            float3 sc = colorAt(screen, screenSize, x, y);
            float3 rc = colorAt(rect, rectSize, x - startPointX, y - startPointY);
            if (sc.x == rc.x && sc.y == rc.y && sc.z == rc.z) {
                hit++;
            }
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

Point ScreenHelperCUDA::find(const BmpRect& rect) {
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

    int threads = 512;
    int blocks = static_cast<int>(ceil(screen.size() / threads));
    Logger::info("ScreenHelperCUDA::find()", "blocks = " + to_string(blocks) + ", threads = " + to_string(threads));

    matchRect << <blocks, threads >> > (d_screen, screenSize, d_rect, rectSize, d_ans);
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
