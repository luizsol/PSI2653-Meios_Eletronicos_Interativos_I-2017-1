#include <signal.h>

void handler(sig)
   int sig;
   {
   printf("Sinal %d \n",sig);
   }

int main()
    {
    int i;
    for (i=1; i<20; i++)
       signal(i,handler);
    printf("Programa que trata todos os sinais\n");
    while (1);
    }
