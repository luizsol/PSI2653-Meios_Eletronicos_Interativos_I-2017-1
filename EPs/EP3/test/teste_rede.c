/** @file       teste_rede.c
 *  @brief      Programa para testar a biblioteca rede.h
 *                      
 *              Repository: 
 *				https://github.com/luizsol/MEI
 *  @author     Luiz Sol (luizedusol@gmail.com)
 *  @date       2017/05/01
 */

#include "rede.h"

///////////////////////////////////////

// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <netdb.h>

extern int    errno;

#define IPMAXSTRSIZE   16

//###############################################################
// openActiveUDPport
//     Descricao  
//         Abre uma porta UDP no modo ativo
//     Argumentos 
//         hostnamep   : nome do host ou endereco ip 
//         servicenamep: nome do servico ou numero da porta 
//     Retorno    
//         sucesso: socket descriptor
//         erro   : < 0
//###############################################################

int openActiveUDPport(char *hostnamep, char *servicenamep)
    {
    int                 serverport;
    unsigned short int  netbyteordered_serverport;
    unsigned long  int  ipaddr;
    int                 socketdescriptor;
    int                 status;
    char                ipstring[IPMAXSTRSIZE];
    struct sockaddr_in  socketaddress;
    struct servent    * serviceentryp;
    struct hostent    * hostentryp;

    /************************************/
    /* Traduz nome do servico para numero da porta */
    /************************************/
    serviceentryp = getservbyname(servicenamep,"udp");
    if (serviceentryp == NULL){
        // Nao conseguiu converter o nome do servico para numero de porta.
		// Verifica se foi informado numero da porta */
        serverport = atoi(servicenamep);
        if (serverport <= 0){
            printf("Nome do servico (ou porta) invalido. \n");
            return(-1);
        }
        netbyteordered_serverport = htons(serverport);
    } else{
        serverport = ntohs(serviceentryp->s_port);
        if (serverport <= 0){
            printf("Numero de porta invalida. \n");
            return(-1);
        }
        netbyteordered_serverport = htons(serverport);
    }
    printf("Porta do servidor    = %hu \n",serverport);

    /************************************/
    /* Converte endereco IP              */
    /************************************/
  
    hostentryp = gethostbyname(hostnamep); 
    if (hostentryp == NULL){
		// Nao conseguiu converter nome para endereco IP
        // Verifica se hostname informado � um endere�o IP
        status = inet_pton(AF_INET,hostnamep,&socketaddress.sin_addr);
        if (status <=0)
            {
            perror("ERRO: intet_pton()");
            return(-1);
            }
    } else {
        bcopy(hostentryp->h_addr, (char *)&socketaddress.sin_addr, hostentryp->h_length);
    }
    printf("Endereco IP do servidor = %s \n",inet_ntop(AF_INET,&socketaddress.sin_addr,ipstring,IPMAXSTRSIZE));

    /****************************************************************/
    /* Socket(): Criacao do socket                                  */
    /****************************************************************/
    socketdescriptor = socket(PF_INET,SOCK_DGRAM,0); 
    if (socketdescriptor < 0)
        {
        printf("Erro na cria��o do socket. \n");
        perror("Descricao do erro");
        return (-1);
        }

    /****************************************************************/
    /* connect(): */
    /****************************************************************/
    socketaddress.sin_family = AF_INET;                  /* tipo do socket */
    socketaddress.sin_port   = netbyteordered_serverport;   /* porta          */

    status = connect(socketdescriptor,(struct sockaddr*)&socketaddress,sizeof(socketaddress));
    if (status != 0)
        {
        printf("Erro na chamada Connect().\n");
        perror("Descricao do erro");
        return(-1);
        }
    return(socketdescriptor);
    }


int main(){
 //        int  sd;
 //        char buffer[80];
 //        int  status;
	// int  n;

 //        char hostnamep[80];
	// char servicenamep[80];

	// printf("HOSTNAME: ");
	// scanf("%s",hostnamep);

	// printf("SERVICENAME: ");
	// scanf("%s",servicenamep);

 //        sd = openActiveUDPport(hostnamep, servicenamep);
 //        if (sd < 0)
 //           {
 //           printf("ERRO no estabelecimento da sess�o UDP \n");
 //           exit(1); 
 //           }
        
 //        while (1)
 //           {
 //           printf(">> ");
 //           scanf("%80s",buffer);

 //           status =  send(sd, buffer, strlen(buffer), 0);
 //           if (status < 0)
 //              {
 //              printf("ERRO no envido de datagramas UDP \n");
 //              exit(1); 
 //              }

 //           n =  recv(sd, buffer, 80, 0);
 //           if (n < 0)
 //              {
 //              printf("ERRO no envido de datagramas UDP \n");
 //              exit(1); 
 //              }
	//    buffer[n]='\0';
 //           printf("<< %s \n",buffer);
 //           }        
	InitSocket(&socketTX, BROADCAST, STDPORTA);
	while(1){
		AdicionaMensagem("Viva!\n");
	}
}
