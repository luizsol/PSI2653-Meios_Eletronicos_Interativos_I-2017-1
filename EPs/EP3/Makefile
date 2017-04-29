#Makefile
CC=gcc

#Local dos arquivos fonte
IDIR =src
#Destino dos arquivos .o
ODIR=obj

#Flag de localizacao do codigo fonte
CFLAGS =-I $(IDIR) 
#Flag para que o compilador mostre todos os warnings e erros
CFLAGS +=-Wall
#Flag para o standard de C que utilizaremos
CFLAGS +=-std=c99
#Flag para ser extra rigoroso com a compilacao
CFLAGS +=-pedantic

#Biblioteca a ser utilizada (Pthread)
LIBS=-lpthread


.DEFAULT_GOAL := main

lista $(ODIR)/lista.o: $(IDIR)/lista.c $(IDIR)/lista.h
	$(CC) $(CFLAGS) -c $(IDIR)/lista.c -o $(ODIR)/lista.o

fila $(ODIR)/fila.o: $(IDIR)/fila.c $(IDIR)/fila.h $(ODIR)/lista.o
	$(CC) $(CFLAGS) -c $(IDIR)/fila.c -o $(ODIR)/fila.o

gui $(ODIR)/gui.o: $(IDIR)/gui.c $(IDIR)/gui.h $(ODIR)/lista.o $(ODIR)/fila.o
	$(CC) $(CFLAGS) -c $(IDIR)/gui.c -o $(ODIR)/gui.o

#main.o: fila.o main.c
#	gcc -std=gnu11 -Wall -pedantic -c main.c 

#all main: main.o fila.o
#	gcc -std=gnu11 -Wall -pedantic fila.o main.o -o prod_cons -lpthread

all main: $(ODIR)/lista.o

clean:
	rm -f $(ODIR)/* cliente-chat servidor-chat
