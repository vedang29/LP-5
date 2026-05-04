#include <iostream>
#include <cuda.h>

using namespace std;

__global__ void add(int *A, int *B, int *C, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
!./add.out

    if (i < N) {
        C[i] = A[i] + B[i];
    }
}

void init(int *arr, int N) {
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 10;
}

void print(int *arr, int N) {
    for (int i = 0; i < N; i++)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {

    int N = 8;
    size_t size = N * sizeof(int);

    int *hA = new int[N];
    int *hB = new int[N];
    int *hC = new int[N];

    init(hA, N);
    init(hB, N);

    cout << "Vector A: ";
    print(hA, N);

    cout << "Vector B: ";
    print(hB, N);

    int *dA, *dB, *dC;

    cudaMalloc(&dA, size);
    cudaMalloc(&dB, size);
    cudaMalloc(&dC, size);

    cudaMemcpy(dA, hA, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, hB, size, cudaMemcpyHostToDevice);

    int block = 256;
    int grid = (N + block - 1) / block;

    add<<<grid, block>>>(dA, dB, dC, N);

    cudaDeviceSynchronize();

    cudaMemcpy(hC, dC, size, cudaMemcpyDeviceToHost);

    cout << "Result: ";
    print(hC, N);

    cudaFree(dA);
    cudaFree(dB);
    cudaFree(dC);

    delete[] hA;
    delete[] hB;
    delete[] hC;

    return 0;
}
