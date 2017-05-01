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

void * _ThreadRX(void * chatSocket);
void * _ThreadTX(void * chatSocket);

/** @brief Inicializa um socket de comunicacao
 *
 *  @param IP IP do endpoint
 *  @param porta porta de comunicacao
 *  @param nome nome do endpoint do socket 
 *  @return ponteiro para o socket criado
 */
ChatSocket *  InitSocket(char * IP, char * porta,
	char * nome, int modo){

	ChatSocket * chatSocket = malloc(sizeof(ChatSocket));

	/* Inicializando variaveis da struct do socket 			*/
	chatSocket->IP = malloc(100*sizeof(char));
	chatSocket->serviceEntry = NULL;
	chatSocket->hostEntry = NULL;
	chatSocket->status = -1;
	chatSocket->filaBuffer = NewFila();
	chatSocket->nome = malloc(100*sizeof(char));
	strcpy(chatSocket->nome, nome);

	/* Traduz nome do servico para numero da porta 			*/
	chatSocket->serviceEntry = 
		getservbyname(chatSocket->porta,"udp");

	if (chatSocket->serviceEntry == NULL){	/* Erro na 		*
											 * obtencao do 	*
											 * socket 		*/
		free(chatSocket);
		return NULL;
	}

	/* Traducao do valor da porta 							*/
	chatSocket->porta = 
		ntohs(chatSocket->serviceEntry->s_port);

	if (chatSocket->porta <= 0){/* Valor da porta invalido	*/
		free(chatSocket);
		return NULL;
	}

	chatSocket->netByteOrderedServerPort = 
		htons(chatSocket->porta);

	/* Converte endereco IP 								*/
	chatSocket->hostEntry = gethostbyname(IP);

	if (chatSocket->hostEntry == NULL){	/* Falha na traducao*
										 * do endereco de IP*/
		free(chatSocket);
		return NULL;
	}

	bcopy(chatSocket->hostEntry->h_addr,
			(char *)&(chatSocket->socketAddress.sin_addr),
			chatSocket->hostEntry->h_length);

	/* Copiando o valor traduzido do IP para socketTX.IP 	*/
	inet_ntop(AF_INET,
				&(chatSocket->socketAddress.sin_addr),
				socketTX.IP,
				IPMAXSTRSIZE)

	/* Criacao do socket 	 								*/
	chatSocket->sd = socket(PF_INET,SOCK_DGRAM,0);
	if(chatSocket->sd < 0){	/* Erro na criacao do socket 	*/
		free(chatSocket);
		return NULL;
	}

	/* Conectando o socket 									*/
	chatSocket->socketAddress.sin_family = /* Tipo de socket*/
		AF_INET;

	chatSocket->socketAddress.sin_port =		/* Valor da */
		chatSocket->netByteOrderedServerPort;	/* porta 	*/
	
	chatSocket->status = connect(chatSocket->sd,
						(struct sockaddr *)
							&(chatSocket->socketAddress),
						sizeof(chatSocket->socketAddress));

	if(chatSocket->status != 0){	/* Erro ao conectar o 	*
									 * socket 				*/
		free(chatSocket);
		return NULL;
	}

	chatSocket->socketThread = malloc(sizeof(pthread_t));
	if(modo == MODORX){
		pthread_create(chatSocket->socketThread, NULL,
			_ThreadRX, (void *)chatSocket);
	} else {
		pthread_create(chatSocket->socketThread, NULL,
			_ThreadTX, (void *)chatSocket);
	}
    

	return chatSocket;
}

/** @brief continuamente 
 *
 *  @param socketTX socket a ser utilizado
 *  @return status da operacao
 */
void * _ThreadRX(void * chatSocket){
	while(((ChatSocket * )chatSocket)->status != 0){
		RecebeMensagem((ChatSocket * ) chatSocket);
	}
}

void * _ThreadTX(void * chatSocket){
	while(((ChatSocket * )chatSocket)->status != 0){
		EnviaMensagem((ChatSocket * ) chatSocket);
	}
}

/** @brief Envia a primeira mensagem na filaBuffer de um 
 *  	   socketTX
 *
 *  @param socketTX socket a ser utilizado
 *  @return status da operacao
 */
int EnviaMensagem(ChatSocket * socketTX){
	if(socketTX->status != 0){	/* Socket nao esta conectado*/
		return L_ERRO;
	}
	char * conteudo = (char *) PopFila(socketTX->filaBuffer);
	
	int status = send(socketTX->sd,
					conteudo, strlen(conteudo), 0);

	free(conteudo);

	if (status < 0){
		return L_ERRO;
	}

	return L_OK;
}

/** @brief Recebe uma mensagem do socketRX e insere na fila
 *
 *  @param socketRX socket a ser utilizado
 *  @return status da operacao
 */
int RecebeMensagem(ChatSocket * socketRX){
	if(socketRX->status != 0){	/* Socket nao esta conectado*/
		return L_ERRO;
	}
	
	char * conteudo = malloc(MAXMSGSIZE*sizeof(char));
	char * mensagem = malloc(MAXMSGSIZE*sizeof(char));

	int isSrv = strcmp(NOMESRV, socketRX->nome);

	if(isSrv != 0){ /* A mensagem nao veio de um servidor	*/
												 
		strcpy(conteudo, socketRX->nome);
		strcpy(conteudo, ESPACADOR);
	}

	int status =  recv(socketRX->sd, mensagem, MAXMSGSIZE, 0);

	//TODO: Adicionar processamento da msg

	if (status < 0){
		free(conteudo);
		free(mensagem);
		return L_ERRO;
	}

	strcpy(conteudo, mensagem);

	return PushFila(socketRX->filaBuffer,
		(void *)conteudo);
}

int RunCliente(Fila * input){
	//inicializa socketTX com ip do srv com nome srv
	//inicializa socketRX com ip do srv com nome srv
	//manda msg de conexao para o servidor
	//recebe msg de confirmacao
	//mostra conectado no chat

	//inicia thread tx
	//inicia thread rx
}

void * _ThreadRx(void * threadStruct){
	while( ((ThreadStruct)(threadStruct))->alive ){
		while(L_ERRO == RecebeMensagem(
				((ThreadStruct)(threadStruct))->tSocket)
			){}
		char * pacote = (char * ) PopFila(
			((ThreadStruct)(threadStruct))->tSocket->filaBuffer);
		switch(*pacote){
			case 'U':	/* USER ou UP						*/
				if(*(pacote +1) == 'S'){	/* USER			*/
				//TODO
				//inicializa socketTX com ip e nome do usr
				//if(nusers == maxusers)
					//envia BUSY
				//else
					//inicializa socketRX com ip e nome do usr
					//inicia thread rx
					//inicia thread tx
					//insere nome+"entrou" no chat
					//envia OKOK
					//nusers++
					//insere nome+"entrou" na lista de todas as threads tx
				} else if(*(pacote +1) == 'P'){	/* UP		*/
					//insere msg na lista de todas as threads tx
					//insere nome+msg no chat
				}
				break;
			case 'E':	/* EXIT								*/
				//insere nome+"saiu" na lista de todas as threads tx
				//envia BYE
				//mata thread tx desse usuario
				//nusers--
				return NULL;
			case 'T':	/* TEST								*/
				//envia OKOK para esse usuario
				break;
			case 'B':	/* BUSY ou BYE						*/
				if(*(pacote +1) == 'U'){	/* BUSY 		*/
					//espera 1s
					//envia user+nome para esse usuario
				} else if(*(pacote +1) == 'Y'){	/* BYE		*/
					//insere "desconectado" no chat
					//online = 0
					//mata thread tx desse usuario
					//return
				}
				break;
			case 'D':	/* DOWN								*/
				//insere msg no chat
				break;
			case 'O':	/* OKOK								*/
				//online = 1
				break;
		}
		free(pacote);
	}

	return NULL;
}


//threadrx
	//loop 1
		//recebe pacote
		//pacote == user
			//inicializa socketTX com ip e nome do usr
			//if(nusers == maxusers)
				//envia BUSY
			//else
				//inicializa socketRX com ip e nome do usr
				//inicia thread rx
				//inicia thread tx
				//insere nome+"entrou" no chat
				//envia OKOK
				//nusers++
				//insere nome+"entrou" na lista de todas as threads tx
		//pacote == up
			//insere msg na lista de todas as threads tx
			//insere nome+msg no chat
		//pacote == exit
			//insere nome+"saiu" na lista de todas as threads tx
			//envia BYE
			//mata thread tx desse usuario
			//nusers--
			//return
		//pacote == test
			//envia OKOK para esse usuario
		//pacote == down
			//insere msg no chat
		//pacote == busy
			//espera 1s
			//envia user+nome para esse usuario
		//pacote == bye
			//insere "desconectado" no chat
			//online = 0
			//mata thread tx desse usuario
			//return
		//pacote == okok
			//online = 1

//threadtx
	//loop 1
		//envio = poplista da lista de tx
		//envia envio
		//free(envio)