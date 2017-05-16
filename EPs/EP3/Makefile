#  @file       Makefile
#  @brief      Automatiza a compilação do EP3 de MEI
#              
#              Utilização: execute o comando `make`
#                      
#              Repository: 
#				https://github.com/luizsol/MEI
#  @author     Luiz Sol (luizedusol@gmail.com)
#  @date       2017/05/16
#

#Compilador a ser utilizado
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

#Target padrão de compilação (make sem parâmetros)
.DEFAULT_GOAL := all

### LISTA
$(ODIR)/lista.o: $(IDIR)/lista.c $(IDIR)/lista.h
	mkdir -p $(ODIR)
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
$(ODIR)/gui.o: $(IDIR)/gui.c $(IDIR)/gui.h $(IDIR)/fila.h
	$(CC) $(CFLAGS) -c $(IDIR)/gui.c -o $(ODIR)/gui.o

#Teste da biblioteca GUI sem sockets
$(ODIR)/teste_gui.o: $(IDIR)/gui.h $(IDIR)/fila.h $(TDIR)/teste_gui.c
	$(CC) $(CFLAGS) -c $(TDIR)/teste_gui.c -o $(ODIR)/teste_gui.o
gui: $(ODIR)/lista.o $(ODIR)/fila.o $(ODIR)/gui.o $(ODIR)/teste_gui.o
	$(CC) $(CFLAGS) $(ODIR)/lista.o $(ODIR)/fila.o $(ODIR)/gui.o $(ODIR)/teste_gui.o -o teste_gui $(LIBS)

### REDE
$(ODIR)/rede.o: $(IDIR)/rede.c $(IDIR)/rede.h $(IDIR)/fila.h $(IDIR)/lista.h $(IDIR)/gui.h
	$(CC) $(CFLAGS) -c $(IDIR)/rede.c -o $(ODIR)/rede.o

#Servidor UDP de echo
$(ODIR)/echo_rede.o: $(TDIR)/echo_rede.c
	$(CC) $(CFLAGS) -c $(TDIR)/echo_rede.c -o $(ODIR)/echo_rede.o
echo: $(ODIR)/echo_rede.o
	$(CC) $(CFLAGS) $(ODIR)/echo_rede.o -o echo_rede $(LIBS)

#Compilação do cliente
$(ODIR)/cliente.o: $(IDIR)/rede.h $(IDIR)/fila.h $(IDIR)/lista.h $(IDIR)/gui.h $(IDIR)/cliente.c $(IDIR)/cliente.h
	$(CC) $(CFLAGS) -c $(IDIR)/cliente.c -o $(ODIR)/cliente.o
all cliente-chat: $(ODIR)/lista.o $(ODIR)/fila.o $(ODIR)/gui.o $(ODIR)/rede.o $(ODIR)/cliente.o
	$(CC) $(CFLAGS) $(ODIR)/lista.o $(ODIR)/fila.o $(ODIR)/gui.o $(ODIR)/rede.o $(ODIR)/cliente.o -o cliente-chat $(LIBS)

##Remoção de todos os arquivos gerados por compilação
clean:
	rm -f $(ODIR)/*.o cliente-chat teste_lista teste_fila teste_gui techo_rede
