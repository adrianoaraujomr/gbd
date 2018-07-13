all: main

heap.o: heap.c heap.h
	gcc -c heap.c

buff.o: buff.c buff.h heap.h
	gcc -c buff.c

hash.o: hash.c hash.h buff.h -lm
	gcc -c hash.c -lm

piii.o: piii.c piii.h hash.h -lm
	gcc -c piii.c -lm

main.o: main.c piii.h -lm
	gcc -c main.c -lm

main: main.o piii.o hash.o buff.o heap.o -lm

clean:
	rm *.o
