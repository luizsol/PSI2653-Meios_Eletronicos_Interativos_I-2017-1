#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

extern int    errno;

#define IPMAXSTRSIZE  16

//##########################################################
// main
//##########################################################

#define  BUFFERSIZE  80
#define  MSGMAXSIZE     512 

int main()
    {
    int                sd;                 // socket descriptor  
    int                status;             // estado
    int                size;
    char               buffer[BUFFERSIZE]; // buffer temporario
    char               msg[MSGMAXSIZE];       // buffer temporario
    int                serverport;         // porta (formato host)
    struct sockaddr_in mysocketaddr;
    struct sockaddr_in fromaddr;
    struct hostent   * hostentryp;         // host entry (p/ traducao nome<->ip)
    struct servent   * serventryp;         // host entry (p/ traducao nome<->ip)

    mysocketaddr.sin_family = AF_INET;
    mysocketaddr.sin_addr.s_addr   = INADDR_ANY;

    //####################
    // servico/porta  
    //####################

    // Requisita nome do servico  
    printf("SERVICENAME: ");
    scanf("%s",buffer);

    // Verifica se string informada representa um inteiro
    serverport = atoi(buffer);
    if (serverport > 65535)
        {
        printf("Valor de porta invalido. \n");
        exit(1);
        }
    else if (serverport > 0)    
        mysocketaddr.sin_port = htons((unsigned short int) serverport);
    else if (serverport <= 0)
        {
        // String informada nao e' um inteiro
        serventryp = getservbyname(buffer,"udp");
        if (serventryp != NULL)
            {
            mysocketaddr.sin_port = serventryp->s_port;
            }
        else
            {
            printf("Nome do servico (ou porta) invalido. \n");
            exit(1);
            }
        }

    //####################
    // imprime IP e porta  
    //####################
    printf("Endereco IP do servidor = %s \n",
            inet_ntop(AF_INET,&mysocketaddr.sin_addr,buffer,BUFFERSIZE));
    printf("Porta do servidor = %hu \n",ntohs(mysocketaddr.sin_port));

    /****************************************************************/
    /* Socket(): Criacao do socket                                  */
    /****************************************************************/
    sd = socket(PF_INET,SOCK_DGRAM,0); 
    if (sd < 0)
        {
        printf("Erro na criação do socket. \n");
        perror("Descricao do erro");
        exit(1);
        }

    /****************************************************************/
    /* Bind(): associa o socket a um IP e a uma porta               */
    /****************************************************************/
    status=bind(sd,(struct sockaddr *)&mysocketaddr,sizeof(struct sockaddr_in));
    if (status < 0)
        {
        perror("Erro na chamada bind()");
        exit(1);
        }

    while (1)
       {
       size = sizeof(fromaddr);
       status=recvfrom(sd,msg,MSGMAXSIZE,0,(struct sockaddr *)&fromaddr,&size);
       if (status < 0)
          {
          perror("ERRO no recebimento de datagramas UDP \n");
          exit(1); 
          }
       printf("<< %s",msg);
       fflush(stdout);

       status = sendto(sd,msg,strlen(msg)+1,0,(struct sockaddr *) &fromaddr,sizeof(fromaddr));
       if (status < 0)
          {
          printf("ERRO no envio de datagramas UDP \n");
          exit(1); 
          }
       }        
    }
