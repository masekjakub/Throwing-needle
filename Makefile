all: 
	gcc -std=c99 -g IPT_jehla.c -o IPT_jehla -lm

clean:
	rm -rf *.o