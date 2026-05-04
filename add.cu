#include <iostream.h>
#include <cuda.h>
using namespace std;

__global__ void add(int* A, int* B, int* C, int size) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < size) {
        C[tid] = A[tid] + B[tid];
    }
}

void initialize(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 10;
    }
}

void print(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        cout << vector[i] << " ";
    }
    cout << endl;
}

int main() {
    int N = 4;
    int* A = new int[N];
    int* B = new int[N];
    int* C = new int[N];

    initialize(A, N);
    initialize(B, N);

    cout << "Vector A: ";
    print(A, N);
    cout << "Vector B: ";
    print(B, N);

    int *X, *Y, *Z;
    size_t bytes = N * sizeof(int);
    cudaMalloc((void **)&X, bytes);
    cudaMalloc((void **)&Y, bytes);
    cudaMalloc((void **)&Z, bytes);

    cudaMemcpy(X, A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(Y, B, bytes, cudaMemcpyHostToDevice);

    dim3 dimBlock(256);
    dim3 dimGrid((N + dimBlock.x - 1) / dimBlock.x);

    add<<>>(X, Y, Z, N);
    cudaDeviceSynchronize();

    cudaMemcpy(C, Z, bytes, cudaMemcpyDeviceToHost);

    cout << "Addition: ";
    print(C, N);

    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);

    return 0;
}
     
