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

int InitSocket(int porta);
int TipoRawMsg(char * conteudo);
int AdicionaHost(char * nome, unsigned long s_addr,
	 unsigned short sin_port);
ChatHost * BuscaHostPorIP(unsigned long s_addr);
int RemoveHostPorIP(unsigned long s_addr);
void BroadcastMsg(char * msg);
char * ParseMensagemUP(RawMsg * mensagem);
void ParseMensagemDOWN(RawMsg * mensagem);
void ParseMensagemOKOK(RawMsg * mensagem);
void ParseMensagemUSER(RawMsg * mensagem);
void * _threadKeepAlive(void * host);
void * _ThreadRX(void * arg);
void * _ThreadTX(void * arg);
void * _ThreadCliente(void * servidor);
void * _ThreadCorreios(void * arg);


void InitCliente(char * srvIP, char * porta, char * nome){
	InitGUI(MODOCLIENTE);
	InsereTextoChat("InitCliente");
	//Iniciando Socket
	InitSocket(htons(atoi(porta)));
	//Adiciona Servido a lista de hosts
	AdicionaHost(NOMESRV,inet_addr(srvIP),htons(atoi(porta)));
	//Solicitando entrada na sala
	char pkt[MAXMSGSIZE];
	pkt[0] = '\0';
	strcat(pkt, "USER:");
	strcat(pkt, nome);
	EnviaRawMsg(pkt, inet_addr(srvIP),htons(atoi(porta)));
	conectadoSRV = L_OK;
	threadCliente = malloc(sizeof(pthread_t));
	pthread_create(threadCliente, NULL,
			_ThreadCliente,
			 (void * ) BuscaHostPorIP(inet_addr(srvIP)));
}

void InitServidor(char * porta){
	InitGUI(MODOSERVIDOR);
	InsereTextoChat("InitServidor");
	InitSocket(htons(atoi(porta)));
	conectadoSRV = L_ERRO;
}

/** @brief Inicializa um socket de comunicacao
 *
 *  @param porta porta de comunicacao
 *  @return status da operação
 */
int  InitSocket(int porta){
	InsereTextoChat("InitSocket");
	ChatSocket * chatSocket;

	chatSocket = socketTXRX = malloc(sizeof(ChatSocket));
	outbox = NewFila();
	inbox = NewFila();
	listaHosts = NewLista();
	sem_init(&sem_mutex_listaHosts, 0, 1);
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
		endwin();	/* Termina modo curses 						*/
		execGUI = 0;
		puts("Erro no socket()");
		return L_ERRO;
	}

	chatSocket->status = bind(chatSocket->sd,
						(struct sockaddr *) &mylocal_addr,
						sizeof(struct sockaddr_in));

	if(chatSocket->status != 0){	/* Erro ao conectar o 	*
									 * socket 				*/
		free(chatSocket);
		endwin();	/* Termina modo curses 						*/
		execGUI = 0;
		puts("Erro no bind()");
		return L_ERRO;
	}

	chatSocket->socketThreadTX = malloc(sizeof(pthread_t));
	chatSocket->socketThreadRX = malloc(sizeof(pthread_t));
	threadCorreios = malloc(sizeof(pthread_t));


	pthread_create(chatSocket->socketThreadRX, NULL,
			_ThreadRX, NULL);
	pthread_create(chatSocket->socketThreadTX, NULL,
			_ThreadTX, NULL);
	pthread_create(threadCorreios, NULL,
			_ThreadCorreios, NULL);
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

	InsereTextoChat("EnviaRawMsg");
	RawMsg * nMsg = malloc(sizeof(RawMsg));
	nMsg->msg[0] = '\0';
	strcpy(nMsg->msg, mensagem);

	nMsg->fromTo.sin_family = AF_INET;
	nMsg->fromTo.sin_port = sin_port;
	nMsg->fromTo.sin_addr.s_addr = s_addr;

	return PushFila(outbox, (void *) nMsg);
}

/** @brief Determina o tipo da mensagem recebida
 *
 *  @param conteudo o texto da mensagem
 *  @return o tipo da mensagem
 */
int TipoRawMsg(char * conteudo){
	InsereTextoChat("TipoRawMsg");
	if(*(conteudo) == 'B'){ /* BUSY ou BYE 					*/
		if(*(conteudo + 1) == 'U'){
			return BUSY;
		} else if(*(conteudo + 1) == 'Y'){
			return BYE;
		}
		return -1;
	} else if(*(conteudo) == 'U'){ /* USER ou UP			*/
		if(*(conteudo + 1)  == 'S'){
			return USER;
		} else if(conteudo[1] == 'P'){
			return UP;
		}
		return -1;
	} else if(*(conteudo)  == 'E'){ /* EXIT 					*/
		if(*(conteudo + 1) == 'X'){
			return EXIT;
		}
		return -1;
	} else if(*(conteudo) == 'T'){ /* TEST 					*/
		if(*(conteudo + 1)  == 'E'){
			return TEST;
		}
		return -1;
	} else if(*(conteudo) == 'O'){ /* OKOK 					*/
		if(*(conteudo + 1) == 'K'){
			return OKOK;
		}
		return -1;
	} else if(*(conteudo) == 'D'){ /* DOWN 					*/
		if(*(conteudo + 1) == 'O'){
			return DOWN;
		}
		return -1;
	}
	return -1;
}

/** @brief Adiciona um novo host à listaHosts
 *
 *  @param nome o nome do novo host
 *  @param s_addr o IP do novo host
 *  @param sin_port a porta do novo host
 *  @return o status da operação
 */
int AdicionaHost(char * nome, unsigned long s_addr,
	 unsigned short sin_port){
	InsereTextoChat("AdicionaHost");
	ChatHost * nHost = malloc(sizeof(ChatHost));
	nHost->s_addr = s_addr;
	nHost->sin_port = sin_port;
	nHost->nome[0] = '\0';
	strcpy(nHost->nome, nome);

	if(PushFim(listaHosts, nHost) == L_OK){
		nHost->keepAlive = malloc(sizeof(pthread_t));
		pthread_create(nHost->keepAlive, NULL,
			_threadKeepAlive, (void * ) nHost);
		if(strcmp(nHost->nome, NOMESRV) != 0){	/* Não estamos	*
												 * conectando a *
												 * um servidor	*/
			char msg[MAXMSGSIZE];
			msg[0] = '\0';
			strcpy(msg, nHost->nome);
			strcat(msg, " conectado(a).");
			InsereTextoChat(msg);
			BroadcastMsg(msg);
		} else {
			InsereTextoChat("[Conectado]");
		}
		
		
		return L_OK;
	}

	return L_ERRO;
}

/** @brief Busca um host na listaHosts por IP
 *
 *  @param s_addr o IP do host
 *  @return o ponteiro para o host
 */
ChatHost * BuscaHostPorIP(unsigned long s_addr){
	InsereTextoChat("BuscaHostPorIP");
	ChatHost * host = NULL;
	sem_wait(&sem_mutex_listaHosts);
	for(int i = 0; 
		i < TamLista(listaHosts) && host == NULL;
		i++){

			if(((ChatHost * )
				GetConteudoIndice(listaHosts, i))->s_addr
				== s_addr){

					host = (ChatHost * )
						GetConteudoIndice(listaHosts, i);
			}
	}
	sem_post(&sem_mutex_listaHosts);
	return host;
}

/** @brief Remove um host da listaHosts por IP
 *
 *  @param s_addr o IP do host
 *  @return o status da operação
 */
int RemoveHostPorIP(unsigned long s_addr){
	InsereTextoChat("RemoveHostPorIP");
	sem_wait(&sem_mutex_listaHosts);
	for(int i = 0; i < TamLista(listaHosts); i++){
		if(((ChatHost * )
			GetConteudoIndice(listaHosts, i))->s_addr
			== s_addr){

					ChatHost * item =  
						RemoveItemIndice(listaHosts, i);
					sem_post(&sem_mutex_listaHosts);
					item->alive = -1;
					char msg[MAXMSGSIZE];
					msg[0] = '\0';
					strcpy(msg, item->nome);
					strcat(msg, " desconectado(a).");
					BroadcastMsg(msg);
					EnviaRawMsg("BYE :", item->s_addr,
						item->sin_port);
					//Pode dar problema aqui...
					free(item);
					return L_OK;
			}
	}
	sem_post(&sem_mutex_listaHosts);
	return L_ERRO;
}

/** @brief Envia uma mensagem para todos os hosts da listaHosts
 *
 *  @param msg a mensagem a ser enviada
 */
void BroadcastMsg(char * msg){
	InsereTextoChat("BroadcastMsg");
	ChatHost * host;
	sem_wait(&sem_mutex_listaHosts);
	for(int i = 0; i < TamLista(listaHosts); i++){
		host = (ChatHost * )GetConteudoIndice(listaHosts, i);
		EnviaRawMsg(msg, host->s_addr, host->sin_port);
	}
	sem_post(&sem_mutex_listaHosts);
	InsereTextoChat(msg);
}

/** @brief Processa as mensagens UP
 *
 *  @param mensagem a mensagem a ser processada
 */
char * ParseMensagemUP(RawMsg * mensagem){
	InsereTextoChat("ParseMensagemUP");
	ChatHost * host = 
				BuscaHostPorIP(mensagem->fromTo.sin_addr.s_addr);
	char * texto = malloc(MAXMSGSIZE*sizeof(char));
	texto[0] = '\0';
	strcat(texto, "DOWN:");
	strcat(texto, host->nome);
	while(strlen(texto) < 15){
		strcat(texto, " ");
	}
	strcat(texto, ":");
	strcat(texto, (mensagem->msg + 5));
	return texto;
}

/** @brief Processa as mensagens DOWN
 *
 *  @param mensagem a mensagem a ser processada
 */
void ParseMensagemDOWN(RawMsg * mensagem){
	InsereTextoChat("ParseMensagemDOWN");
	ChatHost * host = 
				BuscaHostPorIP(mensagem->fromTo.sin_addr.s_addr);
	if(host != NULL){
		char * texto = malloc(MAXMSGSIZE*sizeof(char));
		texto[0] = '\0';
		strncat (texto, (mensagem->msg + 5), 10);	/* Add nome do 	*
												 * usuario		*/
		strcat(texto, ESPACADOR);
		strcat(texto, (mensagem->msg + 16));
		InsereTextoChat(texto);
		free(texto);
	}
}

/** @brief Processa as mensagens OKOK
 *
 *  @param mensagem a mensagem a ser processada
 */
void ParseMensagemOKOK(RawMsg * mensagem){
	InsereTextoChat("ParseMensagemOKOK");
	ChatHost * host = 
				BuscaHostPorIP(mensagem->fromTo.sin_addr.s_addr);
	if(host != NULL){
		host->alive = MAXTENTATIVAS;
	}
}

/** @brief Processa as mensagens USER
 *
 *  @param mensagem a mensagem a ser processada
 */
void ParseMensagemUSER(RawMsg * mensagem){
	if(conectadoSRV == L_OK){	/* Rodando no modo cliente.		*/
		return;
	}

	if(TamLista(listaHosts) >= MAXHOSTS){
		InsereTextoChat("Novo usuario recusado");
		EnviaRawMsg("BUSY:", mensagem->fromTo.sin_addr.s_addr,
			mensagem->fromTo.sin_port);
	} else {
		InsereTextoChat("Novo usuario adicionado");
		AdicionaHost(mensagem->msg + 5,	/* O nome começa no i=5	*/
			mensagem->fromTo.sin_addr.s_addr,
			mensagem->fromTo.sin_port);
		EnviaRawMsg("OKOK:",
			mensagem->fromTo.sin_addr.s_addr, 
			mensagem->fromTo.sin_port);
	}
}

/** @brief Continuamente verifica se o host está online
 *
 *  @param host ponteiro para o host a ser monitorado
 *  @return NULL
 */
void * _threadKeepAlive(void * host){
	InsereTextoChat("_threadKeepAlive");
	ChatHost * myHost = (ChatHost *) host;
	unsigned long myHostIP = myHost->s_addr;
	while(BuscaHostPorIP(myHostIP) != NULL &&
		myHost->alive > 0){
		EnviaRawMsg("TEST:", myHost->s_addr, myHost->sin_port);
		myHost->alive--;
		sleep(PERIODOTEST);
		if(BuscaHostPorIP(myHostIP) != NULL &&
			myHost->alive < 1){ /* O host não respondeu			*
								 * depois de MAXTENTATIVAS 		*/
			if(conectadoSRV == L_OK){
				conectadoSRV = L_ERRO;
			}
			RemoveHostPorIP(myHostIP);
			return NULL;

		}
	}
	return NULL;
}

/** @brief Continuamente envia as mensagens da fila outbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadRX(void * arg){
	InsereTextoChat("_ThreadRX");
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
			InsereTextoChat("Mensagem recebida:");
			InsereTextoChat(nMsg->msg);
			PushFila(inbox, (void *) nMsg);
		}
	}
	InsereTextoChat("FIM _ThreadRX");
	return NULL;
}

/** @brief Continuamente insere as mensagens na fila inbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadTX(void * arg){
	InsereTextoChat("_ThreadTX");
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
				InsereTextoChat("Mensagem enviada:");
				InsereTextoChat(nMsg->msg);
			}
			free(nMsg);
		}
	}
	InsereTextoChat("FIM _ThreadRX");
	return NULL;
}

/** @brief Continuamente transfere msgs da filaInput para o outbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadCliente(void * servidor){
	InsereTextoChat("_ThreadCliente");
	char msg[MAXMSGSIZE];
	ChatHost * srv = (ChatHost *) servidor;
	while(conectadoSRV == L_OK){
		msg[0] = '\0';
		char * texto = (char * ) PopFila(filaInput);
		if(texto != NULL){
			strcat(msg, "UP  :");
			strcat(msg, texto);
			EnviaRawMsg(msg, srv->s_addr, srv->sin_port);
		}
	}
	return NULL;
}

/** @brief Continuamente decide o que fazer com as msgs recebidas
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadCorreios(void * arg){
	InsereTextoChat("_ThreadCorreios");
	RawMsg * nMsg;
	ChatHost * host;
	while(socketTXRX->status == 0){
		nMsg = (RawMsg *) PopFila(inbox);
		if (nMsg != NULL){
			InsereTextoChat("Mensagem Recebida");
			switch(TipoRawMsg(nMsg->msg)){
			case USER:
				ParseMensagemUSER(nMsg);
				break;
			case UP:
				host = BuscaHostPorIP(nMsg->fromTo.sin_addr.s_addr);
				if(host	!= NULL){ /* Host está de fato registrado		*/
					char * text = ParseMensagemUP(nMsg);
					BroadcastMsg(text);
					free(text);
				}
				break;
			case EXIT:
				RemoveHostPorIP(nMsg->fromTo.sin_addr.s_addr);
				break;
			case TEST:
				EnviaRawMsg("OKOK:", nMsg->fromTo.sin_addr.s_addr,
						 nMsg->fromTo.sin_port);
				break;
			case DOWN:
				ParseMensagemDOWN(nMsg);
				break;
			case BUSY:
				printf("Servidor cheio. Tente mais tarde.\n");
			case BYE:
				endwin();	/* Termina modo curses 						*/
				execGUI = 0;
				exit(0);
				break;
			case OKOK:
				ParseMensagemOKOK(nMsg);
				conectadoSRV = 1;
				break;
			}
			free(nMsg);
		}
	}
	InsereTextoChat("FIM _ThreadCorreios");
	return NULL;
}