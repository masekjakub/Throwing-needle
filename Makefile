all: 
	gcc -std=c99 -Wall -pedantic -O2 IPT_jehla.c -o IPT_jehla -lm

clean:
	rm -rf *.o