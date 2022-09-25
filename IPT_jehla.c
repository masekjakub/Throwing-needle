#include <stdio.h>
#include <stdlib.h>

int calculateRows(float array[], int numberOfRows, int spaceBetweenRows){
    for (int i=0;i<numberOfRows;i++){
        array[i]=spaceBetweenRows*i+spaceBetweenRows/2;
    }
    return array;
}

void cleanRows(float array[], int numberOfRows){
    for (int i=0;i<numberOfRows;i++){
        free(&array[i]);
    }
}
int main(int argc, char *argv[]){
    //settings
    int spaceBetweenRows=30000;
    int needleSize = 20000;
    int numberOfRows = 1000;
    int numberOfThrows = 1500000;

    float *listOfRows;
    listOfRows = (float *)malloc(numberOfRows*sizeof(float));
    *listOfRows = calculateRows(numberOfRows, numberOfRows, spaceBetweenRows);

    for (int i=0;i<numberOfRows;i++){
        printf("%f",listOfRows[i]);
    }

    free(listOfRows);
    return 0;
}

