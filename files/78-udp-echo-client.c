#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>


int main()
	{
        int    sd;              // socket descriptor
        char   txbuffer[80];    // buffer de transmissao
        char   rxbuffer[80];    // buffer de recepcao
        char   hostnamep[80];   // nome do servidor
        void * hostaddress;     // ponteiro p/ endereco IP do servido, formato netwok (4 byes) 
        int    serverport;      // porta UDP do servidor
        int    status;          // resultado da chamada
        struct hostent   * hostentryp;      // estrutura para traducao de nomes
        struct sockaddr_in serversockaddr;  // socket address do servidor

	printf("Entre com um nome ou um endereço IP: ");
	scanf("%s",hostnamep);

	printf("Porta: ");
	scanf("%d",&serverport);

        // SOCKET
        sd = socket(PF_INET, SOCK_DGRAM, 0); 
        if (sd < 0)
           {
           perror("ERRO na chamada socket()");
           exit(1); 
           }

    //************************************
    //* Converte nome de host ou endereco IP para network byte order 
    //************************************                          
    
    hostentryp = gethostbyname(hostnamep);
    if (hostentryp != NULL)
        {
        // conseguiu converteri nome em endereco IP: copia os 4 bytes do endereço IP
        hostaddress = hostentryp->h_addr;
        bcopy(hostaddress, (char *)&serversockaddr.sin_addr, hostentryp->h_length);
        }
    else
        {
        // Nao conseguiu converter nome para endereco IP
        // Verifica se hostname informado é um endereço IP
    
        status = inet_pton(AF_INET,hostnamep,&serversockaddr.sin_addr);
        if (status <=0)
            {
            perror("ERRO: inet_pton()");
            printf("Nome de host ou endereço IP inválido \n");
            return(-1);
            }
        }
        
        //********************************
        // CONECT
        //********************************

        // define endereco do parceiro 
        serversockaddr.sin_family = AF_INET;
        serversockaddr.sin_port   = htons(serverport);
           
        // ativa connect()
        status = connect(sd, (struct sockaddr *) &serversockaddr, sizeof(serversockaddr));
        if (status != 0)
           {
           perror("ERRO na chamada connect()");
           exit(1); 
           }
    
        fgets(txbuffer,sizeof(txbuffer),stdin);
        while (1)
           {
           printf(">> ");
           fgets(txbuffer,sizeof(txbuffer),stdin);
           //scanf("%80s\n",txbuffer);

           printf("sending (chamada send() ... \n");
           status =  send(sd, txbuffer, strlen(txbuffer)+1, 0);
           if (status < 0)
              {
              perror("Erro na chamada send()");
              //exit(1); 
              }

           status =  recv(sd, rxbuffer, sizeof(rxbuffer), 0);
           if (status < 0)
              {
              perror("ERRO na chamada recv() ");
              //exit(1); 
              }
           rxbuffer[status+1]='\0';
           printf("\n<< %s",rxbuffer);
           }        
	}
