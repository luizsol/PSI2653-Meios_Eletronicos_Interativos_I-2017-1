#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int i = 1; 

int main()
    {
    pid_t status;

    printf("Programa de disparo de processos filho. \n");
    status = fork();
    printf("Apos a chamada fork(). \n");
    while (1)
        {
        printf("  Status = %5d,   i = %d \n",status,i);
        i++;
	  sleep(2);
        }
    }
