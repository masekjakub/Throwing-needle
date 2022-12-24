#include "buffons_needle.h"
#define pi 3.141592653589
#define dabs(x) ((x) < 0 ? -(x) : (x))
#define waitForAllChildren(numberOfChildren) for (int i = 0; i < numberOfChildren; i++) wait(NULL)
#define initMem(mem) if ((mem = mmap(NULL, sizeof *mem, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)\
        fprintf(stderr, "Memory not allocated");

// settings
double spaceBetweenRows = 10;
double needleSize = 7.853981633975;
double numberOfRows = 1;
long numberOfThrowsPerThread = 1023*256*75;
int numberOfThreads = 80;

long numberOfThrows;
long *hitsGlobal;
sem_t *writeSem = NULL;

void clearSemAndMem()
{
    sem_close(writeSem);
    sem_unlink("writeSem");
    munmap(hitsGlobal, sizeof *hitsGlobal);
}

void initSem(char *name, int value)
{
    if ((writeSem = sem_open(name, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
        fprintf(stderr,"Semaphore %s not created\n", name);
        clearSemAndMem();
        exit(1);
    }
}

long getPaperSize()
{
    double paperSize = spaceBetweenRows * numberOfRows;
    if (paperSize > RAND_MAX)
    {
        printf("ERROR: Paper is too big\n");
        clearSemAndMem();
        exit(1);
    }
    return paperSize;
}

void generateRows(double *array[])
{
    for (int i = 0; i < numberOfRows; i++)
    {
        (*array)[i] = spaceBetweenRows * i + spaceBetweenRows / 2;
    }
}

int isHit(double rows[], double midPoint, long startIndex, long endIndex, double heightDiff, double paperSize)
{
    long index = (startIndex + endIndex) / 2;

    if (midPoint >= rows[index] - spaceBetweenRows / 2 && midPoint <= rows[index] + spaceBetweenRows / 2)
    {
        double height = dabs(rows[index] - midPoint);
        if (heightDiff >= height)
        {
            return 1;
        }
        return 0;
    }

    if (startIndex >= endIndex)
    {
        printf("Error: Binary search failed\n");
        return 0;
    }

    if (midPoint < rows[index])
        return isHit(rows, midPoint, 0, index - 1, heightDiff, paperSize);
    else if (midPoint > rows[index])
        return isHit(rows, midPoint, index + 1, endIndex, heightDiff, paperSize);

    printf("ERROR: StartPoint not found between rows\n");
    return 0;
}

void asyncWrite(long hits)
{
    sem_wait(writeSem);
    *hitsGlobal += hits;
    sem_post(writeSem);
}

void throwNeedles(double rows[], double paperSize)
{
    double midPoint, heightDiff, angle;
    long hits = 0;

    srand(clock() * getpid()*time(NULL));
    for (int i = 0; i < numberOfThrowsPerThread; i++)
    {
        angle = rand();
        midPoint = (double)(angle / (RAND_MAX / paperSize));

        heightDiff = needleSize / 2 * sin(angle);
        heightDiff = dabs(heightDiff);

        if (isHit(rows, midPoint, 0, numberOfRows, heightDiff, paperSize))
        {
            hits++;
        }
    }
    asyncWrite(hits);
}

int main(int argc, char *argv[])
{
    clearSemAndMem();
    time_t startTime, endTime;
    time(&startTime);

    initSem("writeSem", 1);
    initMem(hitsGlobal);

    numberOfThrows = numberOfThrowsPerThread * numberOfThreads;
    double paperSize = getPaperSize();
    double *listOfRows = (double *)malloc(numberOfRows * sizeof(double));
    generateRows(&listOfRows);

    pid_t pid;
    for (int i = 0; i < numberOfThreads; i++)
    {
        pid = fork();
        if (pid == 0) // children only
        {
            throwNeedles(listOfRows, paperSize);
            return 0;
        }
    }

    waitForAllChildren(numberOfThreads);

    time(&endTime);
    double time = difftime(endTime, startTime);

    double calculatedPi = 2 * needleSize * numberOfThrows / (*hitsGlobal * spaceBetweenRows);

    printf("Calculated pi: %lf\n", calculatedPi);
    printf("Actual pi:     %lf\n", pi);
    printf("Difference:    %lf\n", pi - calculatedPi);
    printf("Hit rate:      %lf\n", (double)*hitsGlobal / numberOfThrows);
    printf("Hits:          %ld\n", *hitsGlobal);
    printf("Expected hits: %ld\n", (long)(2 * needleSize * numberOfThrows / (pi * spaceBetweenRows)));
    printf("Program time:  %d sec\n", (int)time);
    printf("\n");

    clearSemAndMem();
    free(listOfRows);
    return 0;
}