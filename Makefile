all: 
	gcc -std=gnu11 -Wall -pedantic -O2 buffons_needle.c -o buffons_needle -lm
cuda:
	nvcc -arch=sm_70 -lcurand -O3 --machine 64 --use_fast_math buffons_needle.cu -o buffons_needleCUDA
rm:
	rm -rf buffons_needle