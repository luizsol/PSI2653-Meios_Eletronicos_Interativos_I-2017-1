#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>

extern int    errno;

#define QLEN           1    // Fila de requisicoes de conexoes pendentes
#define IPMAXSTRSIZE  16

//##########################################################
// main
//##########################################################

#define  STRINGSIZE   90 
#define  BUFFERSIZE  512 

void perdaconexao(int sig)
    {
    printf("Sinal %d \n",sig);
    printf("Perda de conexao \n");
    }

int main()
    {
    int                sd;                 // socket descriptor  
    int                newsd;              // novo socket descriptor  
    int                status;             // estado
    int                size;               // tamanho
    char               string[STRINGSIZE]; // buffer temporario para string
    char               buffer[BUFFERSIZE]; // buffer temporario para dados
    int                serverport;         // porta (formato host)
    struct sockaddr_in serversocketaddr;   // end. socket (IP+porta) do servidor
    struct sockaddr_in clientaddr;         // end. socket (IP+porta) do cliente
    struct hostent   * hostentryp;         // host entry (p/ traducao nome<->ip)
    struct servent   * serventryp;         // host entry (p/ traducao nome<->ip)

    serversocketaddr.sin_family      = AF_INET;    //address family internet
    serversocketaddr.sin_addr.s_addr = INADDR_ANY; //todos ends. IPs do servidor

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
        serversocketaddr.sin_port = htons((unsigned short int) serverport);
    else if (serverport <= 0)
        {
        // String informada nao e' um inteiro
        serventryp = getservbyname(buffer,"tcp");
        if (serventryp != NULL)
            {
            serversocketaddr.sin_port = serventryp->s_port;
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
            inet_ntop(AF_INET,&serversocketaddr.sin_addr,buffer,STRINGSIZE));
    printf("Porta do servidor = %hu \n",ntohs(serversocketaddr.sin_port));

    /****************************************************************/
    /* Socket(): Criacao do socket                                  */
    /****************************************************************/
    sd = socket(PF_INET,SOCK_STREAM,0); // tcp
    if (sd < 0)
        {
        printf("Erro na criacao do socket. \n");
        perror("Descricao do erro");
        exit(1);
        }

    /****************************************************************/
    /* Bind(): associa o socket a um IP e a uma porta               */
    /****************************************************************/
    status=bind(sd,(struct sockaddr *)&serversocketaddr,sizeof(struct sockaddr_in));
    if (status < 0)
        {
        perror("Erro na chamada bind()");
        exit(1);
        }

    /********************************************************************/
    /* listen(): Ativa o socket no modo passivo para aceite de conexoes */
    /********************************************************************/
    status = listen(sd,QLEN);
    if (status != 0)
        {
        perror("Erro na chamada listen()");
        exit(1);
        }

    /****************************************************************/
    /* accept(): aceita uma conexao da fila de conexoes pendentes   */
    /****************************************************************/
    size = sizeof(clientaddr);
    newsd = accept(sd,(struct sockaddr *) &clientaddr,(socklen_t *) &size);
    if (newsd < 0)
        {
        perror("Erro na chamada accept()");
        exit(1);
        }

    signal(SIGPIPE,perdaconexao);
    signal(SIGIO,perdaconexao);

    /****************************************************************/
    /* read() & write():                */
    /****************************************************************/
    while (1)
       {
       status=read(newsd,buffer,BUFFERSIZE);
       if (status < 0)
          {
          perror("ERRO no recebimento de segmento TCP \n");
          exit(1); 
          }
       printf("<< %s \n",buffer);

       status = write(newsd,buffer,strlen(buffer));
       if (status < 0)
          {
          printf("ERRO no envio de segmento TCP \n");
          exit(1); 
          }
       }        
    }
