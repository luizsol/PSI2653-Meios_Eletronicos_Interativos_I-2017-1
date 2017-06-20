# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=gnu11

# Enable warnings
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic

# Library includes
LIBS = -lpthread -lwiringPi

# Directories
TDIR = test

# Goals
.DEFAULT_GOAL := main

# Object files
queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

lumiarlib.o: lumiarlib.c lumiarlib.h
	$(CC) $(CFLAGS) -c lumiarlib.c

serverlib.o: serverlib.c serverlib.h
	$(CC) $(CFLAGS) -c serverlib.c

ldr.o: ldr.c ldr.h
	$(CC) $(CFLAGS) -c ldr.c

main.o: serverlib.h queue.h main.c
	$(CC) $(CFLAGS) -c main.c 

# Tests
test-ldr.o: ldr.h $(TDIR)/test-ldr.c
	$(CC) $(CFLAGS) -c $(TDIR)/test-ldr.c

test-ldr: test-lrd.o
	$(CC) $(CFLAGS) test-ldr.o -o test-ldr $(LIBS)

# All
all main: main.o lumiarlib.o queue.o serverlib.o htmllib.o ldr.o
	$(CC) $(CFLAGS) serverlib.o queue.o htmllib.o main.o lumiarlib.o ldr.o -o web $(LIBS)

clean:
	rm -f *.o
