# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c11

# Enable warnings
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic

# Library includes
LIBS = -lpthread

# Goals
.DEFAULT_GOAL := main

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

lumiarlib.o: lumiarlib.c lumiarlib.h
	$(CC) $(CFLAGS) -c lumiarlib.c

serverlib.o: serverlib.c serverlib.h
	$(CC) $(CFLAGS) -c serverlib.c

main.o: serverlib.h queue.h main.c
	$(CC) $(CFLAGS) -c main.c 

test-files.o: serverlib.h htmllib.h $(TDIR)/test-files.c
	$(CC) $(CFLAGS) -c $(TDIR)/test-files.c

test-files: test-files.o serverlib.o htmllib.o
	$(CC) $(CFLAGS) serverlib.o htmllib.o test-files.o -o test-files $(LIBS)

all main: main.o lumiarlib.o queue.o serverlib.o htmllib.o
	$(CC) $(CFLAGS) serverlib.o queue.o htmllib.o main.o lumiarlib.o -o web $(LIBS)

clean:
	rm -f *.o
