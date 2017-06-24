#include <signal.h>

void handler(sig)
   int sig;
   {
   printf("Chegou um sinal %d \n",sig);
   }

int main()
    {
    signal(2,handler);
    printf("Programa que trata o sinal INTR \n");
    printf("Aguardando ...\n");
    while (1);
    }
