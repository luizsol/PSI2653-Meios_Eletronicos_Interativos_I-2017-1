#Makefile

.DEFAULT_GOAL := main

fila.o: fila.c fila.h
	gcc -std=gnu11 -Wall -pedantic -c fila.c

main.o: fila.o main.c
	gcc -std=gnu11 -Wall -pedantic -c main.c 

all main: main.o fila.o
	gcc -std=gnu11 -Wall -pedantic fila.o main.o -o phtread -lpthread

clean:
	rm -f *.o phtread