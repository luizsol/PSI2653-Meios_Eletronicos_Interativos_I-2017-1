#Makefile
CC=gcc

#Local dos arquivos fonte
IDIR =src
#Local dos arquivos de teste
TDIR =test
#Destino dos arquivos .o
ODIR=obj

#Flag de localizacao do codigo fonte
CFLAGS =-I $(IDIR) 
#Flag para que o compilador mostre todos os warnings e erros
CFLAGS +=-Wall
#Flag para o standard de C que utilizaremos
CFLAGS +=-std=gnu99
#Flag para ser extra rigoroso com a compilacao
CFLAGS +=-pedantic

#Biblioteca a ser utilizada (Pthread)
LIBS=-lpthread -lncurses


.DEFAULT_GOAL := main

### LISTA
$(ODIR)/lista.o: $(IDIR)/lista.c $(IDIR)/lista.h
	$(CC) $(CFLAGS) -c $(IDIR)/lista.c -o $(ODIR)/lista.o

#Teste da biblioteca de listas
$(ODIR)/teste_lista.o: $(IDIR)/lista.h $(TDIR)/teste_lista.c
	$(CC) $(CFLAGS) -c $(TDIR)/teste_lista.c -o $(ODIR)/teste_lista.o
lista: $(ODIR)/teste_lista.o $(ODIR)/lista.o
	$(CC) $(CFLAGS) $(ODIR)/teste_lista.o $(ODIR)/lista.o -o teste_lista $(LIBS)

### FILA
$(ODIR)/fila.o: $(IDIR)/fila.c $(IDIR)/fila.h $(IDIR)/lista.h
	$(CC) $(CFLAGS) -c $(IDIR)/fila.c -o $(ODIR)/fila.o

#Teste da biblioteca de filas
$(ODIR)/teste_fila.o: $(IDIR)/fila.h $(TDIR)/teste_fila.c
	$(CC) $(CFLAGS) -c $(TDIR)/teste_fila.c -o $(ODIR)/teste_fila.o
fila: $(ODIR)/lista.o $(ODIR)/teste_fila.o $(ODIR)/fila.o
	$(CC) $(CFLAGS) $(ODIR)/lista.o $(ODIR)/teste_fila.o $(ODIR)/fila.o -o teste_fila $(LIBS)

### GUI
$(ODIR)/gui.o: $(IDIR)/gui.c $(IDIR)/gui.h
	$(CC) $(CFLAGS) -c $(IDIR)/gui.c -o $(ODIR)/gui.o

#Teste da biblioteca de filas
$(ODIR)/teste_gui.o: $(IDIR)/gui.h $(IDIR)/fila.h $(TDIR)/teste_gui.c
	$(CC) $(CFLAGS) -c $(TDIR)/teste_gui.c -o $(ODIR)/teste_gui.o
gui: $(ODIR)/lista.o $(ODIR)/fila.o $(ODIR)/gui.o $(ODIR)/teste_gui.o
	$(CC) $(CFLAGS) $(ODIR)/lista.o $(ODIR)/fila.o $(ODIR)/gui.o $(ODIR)/teste_gui.o -o teste_gui $(LIBS)

#main.o: fila.o main.c
#	gcc -std=gnu11 -Wall -pedantic -c main.c 

#all main: main.o fila.o
#	gcc -std=gnu11 -Wall -pedantic fila.o main.o -o prod_cons -lpthread

all main: $(ODIR)/lista.o

clean:
	rm -f $(ODIR)/*.o cliente-chat servidor-chat teste_lista teste_fila teste_gui
