#Makefile

.DEFAULT_GOAL := main

elemento.o: elemento.c elemento.o
	gcc -std=gnu11 -Wall -pedantic -c elemento.c

fila.o: elemento.o fila.c
	gcc -std=gnu11 -Wall -pedantic -c fila.c

ex.o: fila.o ex.c
	gcc -std=gnu11 -Wall -pedantic -c ex.c 

all main: ex.o fila.o
	gcc -std=gnu11 -Wall -pedantic fila.o ex.o -o ex -lpthread

clean:
	rm -f *.o ex
