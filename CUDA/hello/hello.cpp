#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "kernel.cuh"
#include <stdio.h>

__global__ 
void addArrays(int* a, int* b,int* c)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

int main()
{
    const int count = 5;
    const int size = count * sizeof(int);
    int ha[] = {1,2,3,4,5};
    int hb[] = {10,20,30,40,50};
    int hc[count];

    int *da, *db, *dc;
    cudaMalloc(&da,size);
    cudaMalloc(&db,size);
    cudaMalloc(&dc,size);

    cudaMemcpy(da,ha,size,cudaMemcpyHostToDevice);
    cudaMemcpy(db,hb,size,cudaMemcpyHostToDevice);

    //addArrays <<<1,count>>>(da,db,dc);
    runKernel(addArrays,count,da,db,dc);
    cudaMemcpy(hc,dc,size,cudaMemcpyDeviceToHost);

    for(int i = 0; i < count;i++)
        printf("%d ",hc[i]);
}