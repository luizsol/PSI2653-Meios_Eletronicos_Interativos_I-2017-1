/** @file 	rede.h
 *  @brief 	Implementa uma interface para utilização de
 * 			sockets UDP
 *
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/05/01
 */

#ifndef __REDE__H
#define __REDE__H

/* Includes 												*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sys/fcntl.h>
#include "fila.h"
#include "gui.h"

/* Constantes da biblioteca 								*/

/* Definição de parâmetros de rede 							*/
#define STDPORTA 		10000	/* Porta padrao 			*/
#define IPMAXSTRSIZE   	16
#define BROADCAST 		"0.0.0.0"	/* IP de broadcast 		*/
#define MAXTENTATIVAS	3
#define PERIODOTEST		30
#define MAXHOSTS		3

/* Definição dos tipos de mensagens 						*/
#define USER 			91
#define UP 				92
#define EXIT 			93
#define TEST 			94
#define OKOK 			95
#define DOWN 			96
#define BUSY 			97
#define BYE				98

/* Definição dos parâmetros de mensagem						*/
#define MAXMSGSIZE 		200	/* max de caracteres da msg		*/
#define MAXNAMESIZE		11	/* max de caracteres de nome 	*/
#define ESPACADOR 		"> "
#define NOMESRV 		"SERVIDOR  "

/**
 *  Estrutura que implementa uma fila a partir de uma lista
 */
typedef struct chatSocket{
	/*@{*/
	int porta; /**< Porta da comunicacao 					*/
	int status;	/**< Status de conexao do socket			*/
	int sd; /**< Socket descriptor							*/
	pthread_t * socketThreadTX;
	pthread_t * socketThreadRX;
	/*@}*/
} ChatSocket;

/**
 *  Estrutura armazena as mensagens recebidas pelos sockets
 */
typedef struct rawMsg{
	/*@{*/
	char msg[MAXMSGSIZE];	/**< Conteúdo da mensagem 		*/
	struct sockaddr_in fromTo;	/**< IP da mensagem			*/
	/*@}*/
} RawMsg;

/**
 *  Estrutura armazena um host conectado
 */
typedef struct chatHost{
	/*@{*/
	unsigned long s_addr;	/**< IP do host 				*/
	unsigned short sin_port;	/**< porta de comunicação	*/
	char nome[MAXNAMESIZE];	/**< Nome do Host				*/
	volatile int alive;	/**< Status da conexão com o host	*/
	pthread_t * keepAlive;	/**< Watchdog associado ao host */
	/*@}*/
} ChatHost;

/* Variáveis globais da biblioteca 							*/
ChatSocket * socketTXRX; /* Socket utilizado pelo software	*/
Fila * inbox; /* Fila de mensagens recebidas 				*/
Fila * outbox; /* Fila de mensagens a serem enviadas 		*/
Lista * listaHosts; /* Lista de hosts conectados 			*/
sem_t sem_mutex_listaHosts; /* Mutex da listaHost 			*/
unsigned long ipServidor; /* IP do servidor 				*/
unsigned short portaServidor; /* Porta de comunicacao do srv*/

int modoDebug; /* Flag para geração de mensagens de debug 	*/

volatile int conectadoSRV; /* Status da comunicacao com srv */

pthread_t * threadCorreios; /* Thread que processa os 		*
							 * pacotes recebidos 			*/
pthread_t * threadCliente;	/* Thread que gera pacotes a 	*
							 * serem enviados ao servidor 	*/

/** @brief Inicializa o socket e as variáveis de controle de
 *         um cliente de chat
 *
 *  @param srvIP endereço IPV4 do servidor
 *  @param porta porta a ser utilizada na comunicação
 *  @param nome nome que o cliente deseja utilizar no chat
 */
void InitCliente(char * srvIP, char * porta, char * nome);

/** @brief Cria e adiciona uma RawMsg à fila de envios
 *
 *  @param mensagem a mensagem a ser enviada
 *  @param s_addr o IP de destino da mensagem
 *  @param sin_port a porta de destino da mensagem
 *  @return o status da operação
 */
int EnviaRawMsg(char * mensagem, unsigned long s_addr,
	 unsigned short sin_port);

#endif