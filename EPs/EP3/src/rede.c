/** @file 	rede.h
 *  @brief 	Implementa uma interface para utilizacao de
 * 			sockets UDP
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/05/01
 */

#include "rede.h"

void * _ThreadRX(void * arg);
void * _ThreadTX(void * arg);
void * _ThreadCorreios(void * arg);

/** @brief Inicializa um socket de comunicacao
 *
 *  @param porta porta de comunicacao
 *  @return status da operação
 */
int  InitSocket(int porta){
	ChatSocket * chatSocket;

	chatSocket = socketTXRX = malloc(sizeof(ChatSocket));
	outbox = NewFila();
	inbox = NewFila();
	//TODO colocar o DestroyFila correspondente
	

	/* Struct genérico para conexão com todas as interfaces	*
	 * locais 												*/
	struct sockaddr_in mylocal_addr;
	mylocal_addr.sin_family = AF_INET;
	mylocal_addr.sin_addr.s_addr = INADDR_ANY;
	mylocal_addr.sin_port = htons(porta); //TODO pode ter erro aqui!
	chatSocket->status = -1;
	
	/* Criacao do socket 	 								*/
	chatSocket->sd = socket(PF_INET,SOCK_DGRAM,0);
	
	if(chatSocket->sd < 0){	/* Erro na criacao do socket 	*/
		free(chatSocket);
		return L_ERRO;
	}

	chatSocket->status = bind(chatSocket->sd,
						(struct sockaddr *) &mylocal_addr,
						sizeof(struct sockaddr_in));

	if(chatSocket->status != 0){	/* Erro ao conectar o 	*
									 * socket 				*/
		free(chatSocket);
		return L_ERRO;
	}

	chatSocket->socketThreadTX = malloc(sizeof(pthread_t));
	chatSocket->socketThreadRX = malloc(sizeof(pthread_t));


	pthread_create(chatSocket->socketThreadRX, NULL,
			_ThreadRX, NULL);
	pthread_create(chatSocket->socketThreadTX, NULL,
			_ThreadTX, NULL);
    
	return L_OK;
}

/** @brief Cria e adiciona uma RawMsg à fila de envios
 *
 *  @param mensagem a mensagem a ser enviada
 *  @param s_addr o IP de destino da mensagem
 *  @param sin_port a porta de destino da mensagem
 *  @return o status da operação
 */
int EnviaRawMsg(char * mensagem, unsigned long s_addr,
	 unsigned short sin_port){
		RawMsg * nMsg = malloc(sizeof(RawMsg));
		nMsg->msg[0] = '\0';
		strcpy(nMsg->msg, mensagem);

		nMsg->fromTo.sin_family = AF_INET;
		nMsg->fromTo.sin_port = sin_port;
		nMsg->fromTo.sin_addr.s_addr = s_addr;

		return PushFila(outbox, (void *) nMsg);
}

/** @brief Continuamente envia as mensagens da fila outbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadRX(void * arg){
	unsigned int sizeSock = sizeof(struct sockaddr_in);
	int sizeMsg = MAXMSGSIZE * sizeof(char);
	int status = -1;
	RawMsg * nMsg;

	while (socketTXRX->status == 0){
		nMsg = malloc(sizeof(RawMsg));
		nMsg->msg[0] = '\0';
		
		status = recvfrom(socketTXRX->sd, 
						nMsg->msg,
						sizeMsg,
						0,
						(struct sockaddr *)&(nMsg->fromTo),
						&sizeSock);
		if(status > 0){
			PushFila(inbox, (void *) nMsg);
		}
	}
	return NULL;
}

/** @brief Continuamente insere as mensagens na fila inbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadTX(void * arg){
	int status = -1;
	RawMsg * nMsg;
	int sizeSock = sizeof(struct sockaddr);
	int tentativas;

	while(socketTXRX->status == 0){
		
		nMsg = (RawMsg *) PopFila(outbox);
		if(nMsg != NULL){
			tentativas = MAXTENTATIVAS;
			status = -1;

			while(tentativas > 0 && status <= 0){
				tentativas--;
				status = sendto(socketTXRX->sd,
							nMsg->msg,
							strlen(nMsg->msg)+1,
							0,
							(struct sockaddr *) &(nMsg->fromTo),
							sizeSock);
			}
			free(nMsg);
		}
	}
	return NULL;
}

