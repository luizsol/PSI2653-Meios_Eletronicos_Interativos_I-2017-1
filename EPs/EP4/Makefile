#Makefile

#Compilador a ser utilizado
CC=gcc

#Local dos arquivos de teste
TDIR =test

#Flag para que o compilador mostre todos os warnings e erros
CFLAGS =-Wall
CFLAGS +=-Wextra
#Flag para o standard de C que utilizaremos
CFLAGS +=-std=gnu99
#Flag para ser extra rigoroso com a compilacao
CFLAGS +=-pedantic

#Biblioteca a ser utilizada (Pthread)
LIBS=-lpthread

.DEFAULT_GOAL := main

serverlib.o: serverlib.c serverlib.h
	$(CC) $(CFLAGS) -c serverlib.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

main.o: serverlib.h queue.h main.c
	$(CC) $(CFLAGS) -c main.c 

test-files.o: serverlib.h $(TDIR)/test-files.c
	$(CC) $(CFLAGS) -c $(TDIR)/test-files.c

test-files: test-files.o serverlib.o
	$(CC) $(CFLAGS) serverlib.o test-files.o -o test-files $(LIBS)

all main: main.o queue.o serverlib.o 
	$(CC) $(CFLAGS) serverlib.o queue.o main.o -o web $(LIBS)

clean:
	rm -f *.o web test-files
