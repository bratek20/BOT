#pragma once

#include "Screen.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "Logger.h"

class ScreenHelperCUDA {
public:
    static Point find(const BmpRect& rect);

private:
    template<typename T>
    static T*  mallocAndCpy(T* h_tab, int size) {
        T* d_tab = checkMalloc<T>(size);
        checkCpy(d_tab, h_tab, size);
        return d_tab;
    }

    template<typename T>
    static T* checkMalloc(int size) {
        T* d_tab;
        cudaError_t cudaStatus = cudaMalloc(&d_tab, size * sizeof(T));
        if (cudaStatus != cudaSuccess) {
            Logger::error("ScreenHelperCUDA::find()", "cudaMalloc failed!");
            return nullptr;
        }

        return d_tab;
    }

    template<typename T>
    static void checkCpy(T* dst, T* src, int size, cudaMemcpyKind kind = cudaMemcpyHostToDevice) {
        cudaError_t cudaStatus = cudaMemcpy(dst, src, size * sizeof(T), kind);
        if (cudaStatus != cudaSuccess) {
            Logger::error("ScreenHelperCUDA::find()", "cudaMemcpy failed!");
        }
    }
};