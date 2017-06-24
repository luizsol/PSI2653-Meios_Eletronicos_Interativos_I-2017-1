#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
    {
    pid_t pid;

    printf("Programa de disparo de processos filho. \n");
    pid = fork();
    if (pid == 0)
       {
       printf("Processo filho executando. \n");
       while (1)
	 {
         printf("Filho. \n");
	 sleep(2);
	 }
       }
    else 
       {
       printf("Processo pai executando: pid do filho = %d\n",pid);
       while (1)
         {
	 printf("Pai. \n");
	 sleep(2);
         }
       }
    }
