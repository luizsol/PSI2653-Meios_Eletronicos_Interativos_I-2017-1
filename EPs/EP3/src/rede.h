/** @file 	rede.h
 *  @brief 	Implementa uma interface para utilizacao de
 * 			sockets UDP
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
#include "fila.h"

#define STDPORTA 10000	/* Porta para comunicacao UDP padrao*/
#define IPMAXSTRSIZE   16
#define MAXMSGSIZE 	200  
#define BROADCAST "0.0.0.0"	/* IP para envio de datagramas 	*
							 * em broadcast					*/

#define MODOTX 1
#define MODORX 0

#define ESPACADOR "> "
#define NOMESRV "SERVIDOR  "

/**
 *  Estrutura que implementa uma fila a partir de uma lista
 */
typedef struct chatSocket{
	/*@{*/
	int porta; /**< Porta da comunicacao 					*/
	char * IP; /**< IP do endpoint							*/
	struct servent * serviceEntry;	/**< Service entry 		*/
	int status;	/**< Status de conexao do socket			*/
	unsigned short int  netByteOrderedServerPort; /**< ? 	*/
	struct hostent * hostEntry; /**< Host entry 			*/
	struct sockaddr_in  socketAddress;	/**< Socket Address	*/
	int sd; /**< Socket descriptor							*/
	Fila * filaBuffer; /**< Fila de mensagem do socket 		*/
	char * nome;	/**< Nome do endpoint do socket 		*/
	pthread_t * socketThread; 	/**< Thread de execucao do 	*
								 *   socket					*/
	/*@}*/
} ChatSocket;

/**
 *  Estrutura para controle de threads de comunicacao
 */
typedef struct threadStruct{
	/*@{*/
	int id;	/**< ID dessa thread 		 					*/
	int modo; /**< Modo de operacao da thread				*/
	ChatSocket * tSocket;	/**< Socket utilizado pela 		*
							 *   thread 					*/

	struct threadStruct reciproca;	/**< Estrutura da thread*
									 *   reciproca 			*/

	volatile int alive; /**< Controle de execucao da thread */
	Fila * filaBuffer;	/**< Controle de execucao da thread */
	/*@}*/
} ThreadStruct;

Lista * listaSockets;	/* Lista de sockets em execucao 	*/

Fila * filaMensagens;	/* Fila de mensagens a serem 		*
						 * apresentadas pelo chat. Quando	*
						 * inicializado deve conter o 		*
						 * endereco da filaInput do GUI 	*/

/** @brief Inicializa um socket de comunicacao
 *
 *  @param IP IP do endpoint
 *  @param porta porta de comunicacao
 *  @param nome nome do endpoint do socket 
 *  @return ponteiro para o socket criado
 */
ChatSocket *  InitSocket(char * IP, char * porta,
	char * nome);

/** @brief Envia a primeira mensagem na filaBuffer de um 
 *  	   socket
 *
 *  @param socketTX socket a ser utilizado
 *  @return status da operacao
 */
int EnviaMensagem(ChatSocket * socketTX);

/** @brief Recebe uma mensagem do socketRX e insere na fila
 *
 *  @param socketRX socket a ser utilizado
 *  @return status da operacao
 */
int RecebeMensagem(ChatSocket * socketRX);


int RunCliente(Fila * input);

int RunServidor(Fila * input);

#endif