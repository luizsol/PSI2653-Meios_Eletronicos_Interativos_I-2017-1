/** @file 	rede.h
 *  @brief 	Implementa uma interface para utilizacao de
 * 			sockets UDP
 *         	
 *			Principio de funcionamento: no início são instanci-
 *			adas dois sockets, um TX e outro RX, e juntamente a
 *			eles são criadas duas threads (threadTX e threadRX).
 *			A threadRX lê as mensagens da fila inbox e envia aos
 *			seus devidos destinatários, já a threadTX lê as men-
 *			sagens da fila outbox e as envia aos seus respecti-
 *			vos destinatários.
 *			
 *			Além dessas duas threads no início do programa é 
 *			criada outra chamada threadCorreios que é responsá-
 *			vel por centralizar a tomada de decisão em relação
 *			às mensagens recebidas. Ele também é responsável
 *			por manter o registro de todos os endereços e nomes
 *			dos correspondentes na lista listaHosts.
 *
 *			Sempre que for iniciada a comunicação com um novo
 *			host será iniciada uma nova thread responsável por
 *			implementar uma máquina de estados referente ao 
 *			estado atual da comunicação com seu respectivo
 *			host. Dentre as tarefas dessa thread, encontra-se
 *			a de periodicamente checar o status da conexão
 *			(TEST). 
 *
 *
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/05/01
 */

#ifndef __REDE__H
#define __REDE__H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "fila.h"


/* Definição de parâmetros de rede 							*/
#define STDPORTA 		10000	/* Porta padrao 			*/
#define IPMAXSTRSIZE   	16
#define BROADCAST 		"0.0.0.0"	/* IP de broadcast 		*/
#define MODOTX 			1
#define MODORX 			0
#define MAXTENTATIVAS	3

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
	struct sockaddr_in fromTo;	/**< IP da mensagem				*/
	/*@}*/
} RawMsg;

ChatSocket * socketTXRX;

Fila * inbox;
Fila * outbox;
Lista * listaHosts;

pthread_t * threadCorreios;

Fila * filaMensagens;	/* Fila de mensagens a serem 		*
						 * apresentadas pelo chat. Quando	*
						 * inicializado deve conter o 		*
						 * endereco da filaInput do GUI 	*/

/** @brief Inicializa um socket de comunicacao
 *
 *  @param porta porta de comunicacao
 *  @param modo modo de operação do socket 
 *  @return status da operação
 */
int  InitSocket(int porta);

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