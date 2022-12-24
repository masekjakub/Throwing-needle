#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <curand.h>
#define pi 3.141592653589
#define dabs(x) ((x) < 0 ? -(x) : (x))
#define UNSIGNED_INT_MAX 4294967296

// settings
double spaceBetweenRows = 10;
double needleSize = 7.853981633975;

int numberOfIterations = 280;
int CUDAthreads = 1024;
int CUDAblocks = 4096;

long numberOfThrowsPerIteration = CUDAthreads*CUDAblocks;
long numberOfThrows = numberOfThrowsPerIteration * numberOfIterations;;

__global__ void throwNeedles(curandGenerator_t gen, unsigned int *hitsArr, unsigned int *randArr, double spaceBetweenRows, double needleSize, long numberOfThrowsPerIteration, long numberOfThrows)
{
    double midPoint, heightDiff;
    long index = blockIdx.x * blockDim.x + threadIdx.x;
    if (index > numberOfThrowsPerIteration)
    {
        return;
    }
    
    double angle = randArr[index]; 
    midPoint = angle / (UNSIGNED_INT_MAX / spaceBetweenRows);
    heightDiff = needleSize / 2 * sin(angle);
    heightDiff = dabs(heightDiff);

    double height = dabs(spaceBetweenRows/2 - midPoint);
    if (heightDiff >= height)
    {
        hitsArr[index] += 1;
    }
}

int main(int argc, char *argv[])
{
    unsigned int *randArr, *hitsArr;
    time_t startTime, endTime;
    time(&startTime);

    cudaMallocManaged(&randArr, sizeof(unsigned int)*numberOfThrowsPerIteration);
    cudaMallocManaged(&hitsArr, sizeof(unsigned int)*numberOfThrowsPerIteration);
    cudaMemset(&hitsArr, 0, numberOfThrowsPerIteration);
    curandGenerator_t gen;
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen, time(NULL)*clock());

    long hits = 0;

    for (int thread = 0; thread < numberOfIterations; thread++){
        curandGenerate(gen, randArr, numberOfThrowsPerIteration);

        throwNeedles<<<CUDAblocks, CUDAthreads>>>(gen,hitsArr, randArr, spaceBetweenRows, needleSize, numberOfThrowsPerIteration, numberOfIterations*numberOfThrowsPerIteration);
        
    }
    cudaDeviceSynchronize();

    time(&endTime);
    double timeElapsed = difftime(endTime, startTime);

    for(long i = 0; i < numberOfThrowsPerIteration; i++)
    {
        hits += hitsArr[i];
    }

    time(&endTime);
    timeElapsed = difftime(endTime, startTime);

    double calculatedPi = 2 * needleSize * numberOfThrows / (hits * spaceBetweenRows);

    printf("Calculated pi: %lf\n", calculatedPi);
    printf("Actual pi:     %lf\n", pi);
    printf("Difference:    %lf\n", pi - calculatedPi);
    printf("Hit rate:      %lf\n", (double)hits / numberOfThrows);
    printf("Hits:          %ld\n", hits);
    printf("Expected hits: %ld\n", (long)(2 * needleSize * numberOfThrows / (pi * spaceBetweenRows)));
    printf("Program time:  %d sec\n", (int)timeElapsed);
    printf("\n");

    cudaFree(randArr);
    cudaFree(hitsArr);
    return 0;
}