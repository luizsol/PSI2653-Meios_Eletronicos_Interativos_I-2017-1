// Arquivo: resolv-serv-udp.c

#include <stdio.h>
#include <netdb.h>

extern int    errno;

int main()
  {
  char                servicenamep[80];
  struct servent    * serventp;

  printf("Entre com o nome do servico: "); 
  scanf("%s",servicenamep);
  serventp = getservbyname(servicenamep,"udp"); 
  if (serventp == NULL) /* Erro */
     perror("Erro na chamada getservbyname()");    
  else /* ok */
    {
    printf("Nome      = %s\n",serventp->s_name);
    printf("Porta     = %d\n",ntohs(serventp->s_port));
    printf("Protocolo = %s\n",serventp->s_proto);
    }
  } 

