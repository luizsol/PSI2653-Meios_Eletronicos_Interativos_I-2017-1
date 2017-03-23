// Programa temporizador.
//
// Importante: não utilize alarmes de temporização junto com a função sleep()
// Sleep também faz uso de alarmes de temporização e o resultado pode não ser o esperado 

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

unsigned count = 0; // Contador de recebimento de sinal SIGINT (cntr-C).

void mypause(int sign) 
  {
  switch (sign) 
    {
    case SIGALRM:
      printf("Recebido alarme do temporizador (3 segundos) ...\n");
      // Reprograma novo alarme para daqui a 3 segundos
      alarm(3); 
      break;
    case SIGINT:
      count++;
      if (count == 1)
         {
         // Primeiro sinal SIGINT
         printf("\nAinda estou ativo .... primeiro Cntr-C recebido ... programando temporizador ...\n");
         // Programa alarme para daqui a 3 segundos
         alarm(3); // programa alarme para daqui a 3 segundos
         }
      else
         printf("\nAinda estou ativo ... recebido Cntr-C %d vezes ...\n", count);
      break;
    case SIGQUIT:
      printf("\nRecebido Cntr-\\  ...  vou encerrar! \n\n");
      exit(0);
      break;
    }
  }

int main() 
  {
  printf("Programa temporizador\n");
  printf("   Digite cntr-C para iniciar temporizador\n");
  printf("   Digite cntr-\\ para encerrar\n");
  printf("\n");

  signal(SIGALRM, mypause);  // Trata sinal de alarme do temporizador
  signal(SIGINT , mypause);  // Trata sinal de interrupção (cntr-C) 
  signal(SIGQUIT, mypause);  // Trata sinal SIGQUIT (cntr-\)

  while (1) 
    {
    }
  return (0);
  }

