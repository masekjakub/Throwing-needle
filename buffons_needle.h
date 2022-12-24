
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

// function definitions
void clearSemAndMem();
void initSem(char *, int );
long getPaperSize();
void generateRows(double **);
int isHit(double *, double , long , long , double , double );
void asyncWrite(long);
void throwNeedles(double *, double);