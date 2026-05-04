#include <iostream>
#include <cuda.h>
using namespace std;

#define WIDTH 16

__global__ void matMul(int *A, int *B, int *C, int width) {

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < width && col < width) {
        int sum = 0;

        for (int k = 0; k < width; k++) {
            sum += A[row * width + k] * B[k * width + col];
        }

        C[row * width + col] = sum;
    }
}

int main() {

    int N = WIDTH * WIDTH;
    int size = N * sizeof(int);

    int A[N], B[N], C[N];

    for (int i = 0; i < N; i++) {
        A[i] = 1;
        B[i] = 1;
    }

    int *dA, *dB, *dC;

    cudaMalloc(&dA, size);
    cudaMalloc(&dB, size);
    cudaMalloc(&dC, size);

    cudaMemcpy(dA, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, B, size, cudaMemcpyHostToDevice);

    dim3 block(16, 16);
    dim3 grid(WIDTH / 16, WIDTH / 16);

    matMul<<<grid, block>>>(dA, dB, dC, WIDTH);

    cudaDeviceSynchronize();

    cudaMemcpy(C, dC, size, cudaMemcpyDeviceToHost);

    cout << "Matrix Multiplication Result:\n";

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            cout << C[i * WIDTH + j] << " ";
        }
        cout << endl;
    }

    cudaFree(dA);
    cudaFree(dB);
    cudaFree(dC);

    return 0;
}
