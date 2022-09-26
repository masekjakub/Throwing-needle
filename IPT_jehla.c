#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define pi 3.141592653589

double spaceBetweenRows = 500;
long needleSize = 410;
long numberOfRows = 104;
long numberOfThrows = 2000000;

double* calculateRows(double array[]){
    for (int i=0;i<numberOfRows;i++){
        array[i]=spaceBetweenRows*i+spaceBetweenRows/2;
    }
    return array;
}

int isOnPaper(double point, double paperSize){
    if(point >= 0 && point <= paperSize){
        return 1;
    }
    return 0;
}

int isHit(double rows[], double startPoint, long startIndex, long endIndex, double heightDiff, double paperSize)
{
    long mid = (startIndex + endIndex) / 2;
    double endPoint = startPoint + heightDiff;

    if (endIndex == -1){
        if (startPoint < rows[mid] && endPoint > rows[mid]){
            return 1;
        }
        return 0;
    }

    if (startIndex > endIndex){
        printf("Error: Binary search failed");
        return 0;
    }

    if (startPoint >= rows[mid] && startPoint < rows[mid]+spaceBetweenRows){
        if (endPoint < rows[mid] || endPoint >= rows[mid]+spaceBetweenRows){
            return 1;
        }
        return 0;
    }

    if (startPoint < rows[mid]){
        return isHit(rows, startPoint, 0, mid-1, heightDiff, paperSize);
    }
    if (startPoint > rows[mid])
    {
        return isHit(rows, startPoint, mid+1, endIndex, heightDiff, paperSize);
    }

    printf("ERROR: StartPoint not found between rows");
    return 0;
}

double throwNeedles(double rows[], unsigned int paperSize)
{
    double needleStartPoint, heightDiff, angle, hits = 0;

    for (int i=0;i<numberOfThrows;i++){
        int OnPaper = 0;
        while (!OnPaper){
            srand(clock()+i*angle);
            needleStartPoint = (rand() % ((paperSize)*10+1))/10;
            srand(clock() - i - angle);
            angle = (rand() % 3600)/10;
            heightDiff = needleSize * sin(angle);
            OnPaper = isOnPaper(needleStartPoint+heightDiff,paperSize);
        }

        if (isHit(rows, needleStartPoint, 0, numberOfRows - 1, heightDiff, paperSize))
        {
            hits++;
        }
    }
    return hits;
}

int main(int argc, char *argv[]){
    clock_t startTime = clock();

    double paperSize = spaceBetweenRows * numberOfRows;
    double *listOfRows;
    listOfRows = (double *)malloc(numberOfRows*sizeof(double));
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