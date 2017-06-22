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
pwm.o: pwm.c pwm.h
	$(CC) $(CFLAGS) -c pwm.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

web.o: web.c web.h
	$(CC) $(CFLAGS) -c web.c

lumiar.o: lumiar.c lumiar.h
	$(CC) $(CFLAGS) -c lumiar.c

ldr.o: ldr.c ldr.h
	$(CC) $(CFLAGS) -c ldr.c

main.o: web.h ldr.h pwm.h queue.h main.c
	$(CC) $(CFLAGS) -c main.c 

# Tests
test-lrd.o: ldr.h $(TDIR)/test-ldr.c
	$(CC) $(CFLAGS) -c $(TDIR)/test-ldr.c

test-ldr: test-lrd.o ldr.o
	$(CC) $(CFLAGS) test-ldr.o ldr.o -o test-ldr $(LIBS)

# All
all main: main.o lumiar.o pwm.o queue.o web.o ldr.o
	$(CC) $(CFLAGS) queue.o web.o pwm.o main.o lumiar.o ldr.o -o web $(LIBS)

clean:
	rm -f *.o
