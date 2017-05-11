/** @file 	main.c
 *  @brief 	Programa produtor-consumidor com mutex, wait & signal
 *			
 *			Esse programa se utiliza de threads, mutex e
 * 			variáveis de condição para implementar uma 
 *			simulação do clássico problema produtor-consumidor 
 *
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author Volnys Borges Bernal
 * 			Luiz Sol (luizedusol@gmail.com)
 *			Caio Vinícius Batista Pereira
 *  @date	2017/04/18
 */

#include <stdio.h>
#include <pthread.h>
//#include "fila.h"
#include<sys/socket.h>
#include<stdlib.h>
#include<netdb.h>
#include<string.h>
#define TOTAL_PROD 10

//variaveis globais



int status; //status da chamada 
struct sockaddr_in serveraddr; //endereço do servidor
int serverport; //porta
char stringIP[20]; //endereço ip




//RECEPTOR

void receptor(int sd){

//socket

//connect
serveraddr.sin_family=AF_INET;

//int serverport;
//char stringIP[20];

//serverport=1000;
//stringIP[20]="192.168.1.35";

//printf("Digite a porta UDP:");
//scanf("%d",&serverport);
//printf("Digite o endereco IP do servidor: ");
//scanf("%s",stringIP);

serveraddr.sin_port= htons(serverport);
status=inet_pton(AF_INET,stringIP,&serveraddr.sin_addr);
if(status <= 0)
	perror("Erro na conversao do endereco IP");

status=connect(sd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

if (status != 0)
	perror("Erro na chamada connect");

//send
/*
char txbuffer[80];

printf("Digite a mensagem: ");
scanf("%s",txbuffer);

status = send (sd,txbuffer, strlen(txbuffer)+1, 0);
if (status <0)
	perror("Erro na chamada send");
*/
//recv

char rxbuffer[80];

status=recv(sd, rxbuffer, sizeof(rxbuffer), 0);
if (status <0)
	perror("Erro na chamada recv");
printf("MSG recebida: %s\n", rxbuffer);


//close

status=close(sd);
if(status == -1)
	perror("Error na chamada close");

}


void transmissor(int sd){


//socket
//connect
serveraddr.sin_family=AF_INET;

//int serverport;
//char stringIP[20];
//serverport=1000;

//printf("Digite a porta UDP:");
//scanf("%d",&serverport);
//printf("Digite o endereco IP do servidor: ");
//scanf("%s",stringIP);

serveraddr.sin_port= htons(serverport);
status=inet_pton(AF_INET,stringIP,&serveraddr.sin_addr);
if(status <= 0)
	perror("Erro na conversao do endereco IP");

status=connect(sd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

if (status != 0)
	perror("Erro na chamada connect");

//send

char txbuffer[80];

printf("Digite a mensagem: ");
scanf("%s",txbuffer);

status = send (sd,txbuffer, strlen(txbuffer)+1, 0);
if (status <0)
	perror("Erro na chamada send");

//recv
/*
char rxbuffer[80];

status=recv(sd, rxbuffer, sizeof(rxbuffer), 0);
if (status <0)
	perror("Erro na chamada recv");
printf("MSG recebida: %s\n", rxbuffer);
*/

//close

status=close(sd);
if(status == -1)
	perror("Error na chamada close");




}





//Mutex que será utilizado para controlar o acesso à região crítica
//pthread_mutex_t  mutex;

//Declaracao das variaveis de condicao:
//pthread_cond_t filacheia, filavazia;

//Fila a ser utilizada pelos produtores e consumidores
//Fila f;






/** @brief função main
 *
 *  Instancia e executa a simulação
 *
 *  @param argc números de parâmetros na chamada do programa
 *  @param argv parâmetros utilizadas na chamada do programa
 */


int main(int argc, char* argv[]){
	//InitFila(&f);//Inicializando a Fila
	
	//Inicializando as variáveis de condição
	//pthread_cond_init(&filacheia,NULL);
	//pthread_cond_init(&filavazia,NULL);

	//Declaração das threads de transmissores e receptores
	pthread_t tx1;
	pthread_t rx1;

	printf("Programa Produtor-Consumidor\n");

	//printf("Iniciando variaveis de sincronizacao.\n");
	//pthread_mutex_init(&mutex,NULL);
	
	int sd; //socket descriptor - não global
		//o socket é gerado na main para gerar varios clientes com sd distintos
	sd=socket(PF_INET, SOCK_DGRAM,0);
	if (sd== -1)
		{
		perror("Erro na chamada socket");
		exit(1);
		}

		


	printf("Digite a porta UDP:");
	scanf("%d",&serverport);
	printf("Digite o endereco IP do servidor: ");
	scanf("%s",stringIP);


	printf("Disparando threads transmissores\n");
	pthread_create(&tx1, NULL, (void*) transmissor,sd);
	
	printf("Disparando threads receptores\n");
	pthread_create(&rx1, NULL, (void*) receptor,sd);

	//Aguardando o fim das threads para encerrar o programa
	pthread_join(tx1,NULL);
	pthread_join(rx1,NULL);
	
	printf("Terminado processo .\n\n");
}
