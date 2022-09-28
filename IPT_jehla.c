#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define pi 3.141592653589

//settings
double spaceBetweenRows = 5000000;
long needleSize = 4999000;
long numberOfRows = 100;
long numberOfThrows = 80000000;

double* calculateRows(double array[]){
    for (int i=0;i<numberOfRows;i++){
        array[i]=spaceBetweenRows*i+spaceBetweenRows/2;
    }
    return array;
}

double fabs(double x){
    if (x < 0){
        return -x;
    }
    return x;
}

int isHit(double rows[], double midPoint, long startIndex, long endIndex, double heightDiff, double paperSize)
{
    long index = (startIndex + endIndex) / 2;
    if (startIndex > endIndex){
        printf("Error: Binary search failed\n");
        return 0;
    }

    if (midPoint >= rows[index] - spaceBetweenRows / 2 && midPoint <= rows[index] + spaceBetweenRows / 2){
        double height = fabs(rows[index] - midPoint);
        if (heightDiff >= height){
            return 1;
        }
        return 0;
    }

    if (midPoint < rows[index])
    {
        return isHit(rows, midPoint, 0, index - 1, heightDiff, paperSize);
    }
    if (midPoint > rows[index])
    {
        return isHit(rows, midPoint, index + 1, endIndex, heightDiff, paperSize);
    }
    printf("ERROR: StartPoint not found between rows\n");
    return 0;
}

double throwNeedles(double rows[], unsigned long paperSize)
{
    double midPoint, heightDiff, angle, hits = 0;

    srand(clock());
    for (int i=0;i<numberOfThrows;i++){
        midPoint = ((double)(rand() % (paperSize*10))/10);
        angle = rand() % 720;
        heightDiff = (double)needleSize/2 * sin(angle);
        heightDiff = fabs(heightDiff);

        if (isHit(rows, midPoint, 0, numberOfRows, heightDiff, paperSize))
        {
            hits++;
        }
    }
    return hits;
}

int main(int argc, char *argv[]){
    clock_t startTime = clock();

    unsigned long paperSize = spaceBetweenRows * numberOfRows;
    if (paperSize > RAND_MAX)
    {
        printf("ERROR: Paper is too big\n");
    }
    double *listOfRows = (double *)malloc(numberOfRows*sizeof(double));
    listOfRows = calculateRows(listOfRows);
    double hits = throwNeedles(listOfRows, paperSize);
    double calculatedPi = (2 * needleSize * numberOfThrows) / (hits * spaceBetweenRows);

    clock_t endTime = clock();
    double time = endTime-startTime;

    printf("Calculated pi: %lf\n", calculatedPi);
    printf("Actual pi:     %lf\n", pi);
    printf("Difference:    %lf\n", pi - calculatedPi);
    printf("Hit rate:      %lf\n", (hits / numberOfThrows));
    printf("Hits:          %lf\n", hits);
    printf("Expected hits: %lf\n", (2 * needleSize * numberOfThrows) / (pi * spaceBetweenRows));
    printf("Program time:  %lf sec\n", ((double)time) / CLOCKS_PER_SEC);
    printf("\n");
    free(listOfRows);
    return 0;
}