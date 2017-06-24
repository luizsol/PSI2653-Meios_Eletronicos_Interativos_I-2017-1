// Arquivo: resolv-proto.c

#include <stdio.h>
#include <netdb.h>

extern int    errno;

int main()
  {
  char                protonamep[80];
  struct protoent    * protoentp;

  printf("Entre com o nome do protocolo: "); 
  scanf("%s",protonamep);
  protoentp = getprotobyname(protonamep); 
  if (protoentp == NULL) /* Erro */
     perror("Erro na chamada getprotobyname()");    
  else /* ok */
    {
    printf("Nome      = %s\n",protoentp->p_name);
    printf("Protocolo = %d\n",protoentp->p_proto);
    }
  } 

