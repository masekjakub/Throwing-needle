all: 
	gcc -std=gnu11 -Wall -pedantic -O2 buffons_needle.c -o buffons_needle -lm
cuda:
	nvcc -arch=sm_70 -lcurand -o buffons_needleCUDA buffons_needle.cu
rm:
	rm -rf buffons_needle