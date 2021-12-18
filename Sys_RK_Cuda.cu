#include <cmath>
#include <iostream>
#include <cuda.h>
#include <cuda_runtime_api.h>

using namespace std;

__global__ void calculateK(float* k1, float* k2, float* k3, float* k4, float a, float b, float xn, float yn, float h, int i) {
    k1[i] = h * cos(a * xn) - b * yn;
    k2[i] = h * (cos(a * (xn + h / 2.0)) - b * (yn + k1[i] * h / 2.0));
    k3[i] = h * (cos(a * (xn + h / 2.0)) - b * (yn + k2[i] * h / 2.0));
    k4[i] = h * (cos(a * (xn + h)) - b * (yn + k3[i] * h));
}

__global__ void calculateRez(float* yn, float* k1, float* k2, float* k3, float* k4, float h, int i) {
    yn[i] = yn[i] + (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6.0;
}

int main() {

    srand(NULL);

    float x0 = 0, h = 0.0002, y0 = 0.8;
    float xn = x0;
    int finish = 10000, m = 100;

    float* k1, * k2, * k3, * k4, * yn;
    float* c_k1, * c_k2, * c_k3, * c_k4, * c_yn;

    k1 = (float*)malloc(m * sizeof(float));
    k2 = (float*)malloc(m * sizeof(float));
    k3 = (float*)malloc(m * sizeof(float));
    k4 = (float*)malloc(m * sizeof(float));
    yn = (float*)malloc(m * sizeof(float));

    for (int i = 0; i < finish; i++) {

        float a = rand() % 1000;
        float b = rand() % 1000;

        cudaMalloc((void**)&c_k1, m * sizeof(float));
        cudaMalloc((void**)&c_k2, m * sizeof(float));
        cudaMalloc((void**)&c_k3, m * sizeof(float));
        cudaMalloc((void**)&c_k4, m * sizeof(float));
        cudaMalloc((void**)&c_yn, m * sizeof(float));

        cudaMemcpy(k1, c_k1, m * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(k2, c_k2, m * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(k3, c_k3, m * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(k4, c_k4, m * sizeof(float), cudaMemcpyHostToDevice);
        cudaMemcpy(yn, c_yn, m * sizeof(float), cudaMemcpyHostToDevice);

        calculateK << <1, m >> > (c_k1, c_k2, c_k3, c_k4, a, b, xn, y0, h, i);
        calculateRez << <1, m >> > (c_yn, c_k1, c_k2, c_k3, c_k4, h, i);

        cudaMemcpy(c_k1, k1, m * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(c_k2, k2, m * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(c_k3, k3, m * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(c_k4, k4, m * sizeof(float), cudaMemcpyDeviceToHost);
        cudaMemcpy(c_yn, yn, m * sizeof(float), cudaMemcpyDeviceToHost);

        for (int i = 0; i < finish; i++) {
           cout << yn[i] << " ";
        }

        cout << "\n";

        xn += h;
    }

    free(k1);
    free(k2);
    free(k3);
    free(k4);
    free(yn);

    cudaFree(c_k1);
    cudaFree(c_k2);
    cudaFree(c_k3);
    cudaFree(c_k4);
    cudaFree(c_yn);


    return 0;
}