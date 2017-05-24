#Makefile

.DEFAULT_GOAL := main

serverlib.o: serverlib.c serverlib.h
	gcc -std=c99 -Wall -Wextra -c serverlib.c

queue.o: serverlib.o queue.c
	gcc -std=c99 -Wall -Wextra -c queue.c

main.o: serverlib.o queue.o main.c
	gcc -std=c99 -Wall -Wextra -c main.c 

all main: main.o queue.o serverlib.o 
	gcc -std=c99 Wall -Wextra serverlib.o queue.o main.o -o web -lpthread

clean:
	rm -f *.o web
