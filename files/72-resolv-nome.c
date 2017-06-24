// Programa resolv-nome.c

#include <stdio.h>
#include <netdb.h>

extern int    errno;

int main()
  {
  char                hostnamep[80];
  struct hostent    * hostentp;
  printf("Entre com o nome do host: "); 
  scanf("%s",hostnamep);
  hostentp = gethostbyname(hostnamep); 
  if (hostentp == NULL) /* Erro */
     perror("Erro na chamada gethostbyname()");    
  else /* ok */
    {
    printf("Endereco  = %d.%d.%d.%d \n",
       ((unsigned char *) hostentp->h_addr)[0],
       ((unsigned char *) hostentp->h_addr)[1],
       ((unsigned char *) hostentp->h_addr)[2],
       ((unsigned char *) hostentp->h_addr)[3]);
    }
  }
