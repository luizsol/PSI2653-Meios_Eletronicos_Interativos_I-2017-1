/** @file 	rede.h
 *  @brief 	Implementa uma interface para utilização de
 * 			sockets UDP
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/05/01
 */

#include "rede.h"

/*	Assinaturas de funcoes de uso exclusivamente interno 	*/
int InitSocket(int porta);
int TipoRawMsg(char * conteudo);
int AdicionaHost(char * nome, unsigned long s_addr,
	 unsigned short sin_port);
ChatHost * BuscaHostPorIP(unsigned long s_addr);
int RemoveHostPorIP(unsigned long s_addr);
void BroadcastMsg(char * msg);
void ParseMensagemDOWN(RawMsg * mensagem);
void ParseMensagemOKOK(RawMsg * mensagem);
int ValidarBUSY(char * conteudo);
int ValidarBYE(char * conteudo);
int ValidarOKOK(char * conteudo);
int ValidarTEST(char * conteudo);
int ValidarDOWN(char * conteudo);
void * _threadKeepAlive(void * host);
void * _ThreadRX(void * arg);
void * _ThreadTX(void * arg);
void * _ThreadCliente(void * servidor);
void * _ThreadCorreios(void * arg);

/** @brief Inicializa o socket e as variáveis de controle de
 *         um cliente de chat
 *
 *  @param srvIP endereço IPV4 do servidor
 *  @param porta porta a ser utilizada na comunicação
 *  @param nome nome que o cliente deseja utilizar no chat
 */
void InitCliente(char * srvIP, char * porta, char * nome){
	InitGUI(MODOCLIENTE);
	if(modoDebug){
		InsereTextoChat("[InitCliente]");
	}
	ipServidor = inet_addr(srvIP);
	portaServidor = htons(atoi(porta));
	/* Iniciando Socket 									*/
	InitSocket(htons(atoi(porta)));
	/* Adiciona Servido a lista de hosts 					*/
	int status = AdicionaHost(NOMESRV,
					inet_addr(srvIP),
					htons(atoi(porta)));
	if(status == L_ERRO){
		InsereTextoChat(
			"Erro ao adicionar SRV a lista de hosts");
		endwin();	/* Termina modo curses 					*/
		exit(1);
	}
	/* Solicitando entrada na sala 							*/
	conectadoSRV = L_OK;
	threadCliente = malloc(sizeof(pthread_t));
	pthread_create(threadCliente, NULL,
			_ThreadCliente,
			 (void * ) BuscaHostPorIP(inet_addr(srvIP)));
	char pkt[MAXMSGSIZE];
	pkt[0] = '\0';
	strcat(pkt, "USER:");
	strcat(pkt, nome);
	BroadcastMsg(pkt);
}

/** @brief Inicializa um socket de comunicacao
 *
 *  @param porta porta de comunicacao
 *  @return status da operação
 */
int  InitSocket(int porta){
	if(modoDebug){
		InsereTextoChat("[InitSocket]");
	}
	ChatSocket * chatSocket;

	chatSocket = socketTXRX = malloc(sizeof(ChatSocket));
	outbox = NewFila();
	inbox = NewFila();
	listaHosts = NewLista();
	sem_init(&sem_mutex_listaHosts, 0, 1);
	

	/* Struct genérico para conexão com todas as interfaces	*
	 * locais 												*/
	struct sockaddr_in mylocal_addr;
	mylocal_addr.sin_family = AF_INET;
	mylocal_addr.sin_addr.s_addr = INADDR_ANY;
	mylocal_addr.sin_port = htons(porta);
	chatSocket->status = -1;
	
	/* Criacao do socket 	 									*/
	chatSocket->sd = socket(AF_INET,SOCK_DGRAM,0);
	
	if(chatSocket->sd < 0){	/* Erro na criacao do socket 	*/
		free(chatSocket);
		endwin();	/* Termina modo curses 					*/
		execGUI = 0;
		return L_ERRO;
	}

	chatSocket->status = bind(chatSocket->sd,
						(struct sockaddr *) &mylocal_addr,
						sizeof(struct sockaddr_in));

	if(chatSocket->status != 0){	/* Erro ao conectar o 	*
									 * socket 				*/
		free(chatSocket);
		endwin();	/* Termina modo curses 					*/
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
	if(modoDebug){
		InsereTextoChat("[EnviaRawMsg]");
	}

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
	if(modoDebug){
		InsereTextoChat("[TipoRawMsg]");
	}
	if(*(conteudo) == 'B'){ /* BUSY ou BYE 					*/
		if(*(conteudo + 1) == 'U'){
			if (ValidarBUSY(conteudo) == L_OK){
				return BUSY;
			}
		} else if(*(conteudo + 1) == 'Y'){
			if(ValidarBYE(conteudo) == L_OK){
				return BYE;
			}
		}
		return -1;
	} else if(*(conteudo) == 'T'){ /* TEST 					*/
		if(*(conteudo + 1)  == 'E'){
			if(ValidarTEST(conteudo) == L_OK){
				return TEST;	
			}
		}
		return -1;
	} else if(*(conteudo) == 'O'){ /* OKOK 					*/
		if(*(conteudo + 1) == 'K'){
			if(ValidarOKOK(conteudo) == L_OK){
				return OKOK;
			}
		}
		return -1;
	} else if(*(conteudo) == 'D'){ /* DOWN 					*/
		if(*(conteudo + 1) == 'O'){
			if(ValidarDOWN(conteudo) == L_OK){
				return DOWN;
			}
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
	if(modoDebug){
		InsereTextoChat("[AdicionaHost]");
	}
	ChatHost * nHost = malloc(sizeof(ChatHost));
	nHost->s_addr = s_addr;
	nHost->sin_port = sin_port;
	nHost->nome[0] = '\0';
	strcpy(nHost->nome, nome);

	if(PushFim(listaHosts, nHost) == L_OK){
		nHost->keepAlive = malloc(sizeof(pthread_t));
		pthread_create(nHost->keepAlive, NULL,
			_threadKeepAlive, (void * ) nHost);
		
		InsereTextoChat("[Conectado]");
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
	if(modoDebug){
		InsereTextoChat("[BuscaHostPorIP]");
	}
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
	if(modoDebug){
		InsereTextoChat("[RemoveHostPorIP]");
	}
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
					free(item);
					return L_OK;
			}
	}
	sem_post(&sem_mutex_listaHosts);
	return L_ERRO;
}

/** @brief Envia uma mensagem para todos os hosts da 
           listaHosts
 *
 *  @param msg a mensagem a ser enviada
 */
void BroadcastMsg(char * msg){
	if(modoDebug){
		InsereTextoChat("[BroadcastMsg]");
	}
	ChatHost * host;
	sem_wait(&sem_mutex_listaHosts);
	for(int i = 0; i < TamLista(listaHosts); i++){
		host = (ChatHost * )GetConteudoIndice(listaHosts, i);
		EnviaRawMsg(msg, host->s_addr, host->sin_port);
	}
	sem_post(&sem_mutex_listaHosts);
}

/** @brief Processa as mensagens DOWN
 *
 *  @param mensagem a mensagem a ser processada
 */
void ParseMensagemDOWN(RawMsg * mensagem){
	if(modoDebug){
		InsereTextoChat("[ParseMensagemDOWN]");
	}
	ChatHost * host = 
				BuscaHostPorIP(
					mensagem->fromTo.sin_addr.s_addr);
	if(host != NULL){
		char * texto = malloc(MAXMSGSIZE*sizeof(char));
		texto[0] = '\0';
		strncat (texto, (mensagem->msg + 5), 10);
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
	if(modoDebug){
		InsereTextoChat("[ParseMensagemOKOK]");
	}
	ChatHost * host = 
				BuscaHostPorIP(
					mensagem->fromTo.sin_addr.s_addr);
	if(host != NULL){
		host->alive = MAXTENTATIVAS;
	}
}


/** @brief Verifica a integridade do pacote BUSY
 *
 *  @param conteudo a mensagem recebida
 *  @return L_OK se for um pacote BUSY válido, L_ERRO c.c.
 */
int ValidarBUSY(char * conteudo){
	if(modoDebug){
		InsereTextoChat("[ValidarBUSY]");
	}
	if(strlen(conteudo) == 5){
		if(*(conteudo) == 'B'){
			if(*(conteudo + 1) == 'U'){
				if(*(conteudo + 2) == 'S'){
					if(*(conteudo + 3) == 'Y'){
						if(*(conteudo + 4) == ':'){
							if(*(conteudo + 5) == '\0'){
								return L_OK;
							} else {
								if(modoDebug){
									InsereTextoChat(
										"[ValidarBUSY]: Quinta letra errada (\\0).");
								}
							}
						} else {
							if(modoDebug){
								InsereTextoChat(
									"[ValidarBUSY]: Quinta letra errada (:).");
							}
						}
					} else {
						if(modoDebug){
							InsereTextoChat(
								"[ValidarBUSY]: Quarta letra errada (Y).");
						}
					}
				} else {
					if(modoDebug){
						InsereTextoChat(
							"[ValidarBUSY]: Terceira letra errada (S).");
					}
				}
			} else {
				if(modoDebug){
					InsereTextoChat("[ValidarBUSY]: Segunda letra errada (U).");
				}
			}
		} else {
			if(modoDebug){
				InsereTextoChat("[ValidarBUSY]: Primeira letra errada (B).");
			}
		}
	} else {
		if(modoDebug){
			InsereTextoChat("[ValidarBUSY]: Tamanho do pacote errado.");
		}
	}
	return L_ERRO;
}

/** @brief Verifica a integridade do pacote BYE
 *
 *  @param conteudo a mensagem recebida
 *  @return L_OK se for um pacote BYE válido, L_ERRO c.c.
 */
int ValidarBYE(char * conteudo){
	if(modoDebug){
		InsereTextoChat("[ValidarBYE]");
	}
	if(strlen(conteudo) == 5){
		if(*(conteudo) == 'B'){
			if(*(conteudo + 1) == 'Y'){
				if(*(conteudo + 2) == 'E'){
					if(*(conteudo + 3) == ' '){
						if(*(conteudo + 4) == ':'){
							if(*(conteudo + 5) == '\0'){
								return L_OK;
							} else {
								if(modoDebug){
									InsereTextoChat(
										"[ValidarBYE]: Quinta letra errada (\\0).");
								}
							}
						} else {
							if(modoDebug){
								InsereTextoChat(
									"[ValidarBYE]: Quinta letra errada (:).");
							}
						}
					} else {
						if(modoDebug){
							InsereTextoChat(
								"[ValidarBYE]: Quarta letra errada ( ).");
						}
					}
				} else {
					if(modoDebug){
						InsereTextoChat(
							"[ValidarBYE]: Terceira letra errada (E).");
					}
				}
			} else {
				if(modoDebug){
					InsereTextoChat("[ValidarBYE]: Segunda letra errada (Y).");
				}
			}
		} else {
			if(modoDebug){
				InsereTextoChat("[ValidarBYE]: Primeira letra errada (B).");
			}
		}
	} else {
		if(modoDebug){
			InsereTextoChat("[ValidarBYE]: Tamanho do pacote errado.");
		}
	}
	return L_ERRO;
}

/** @brief Verifica a integridade do pacote OKOK
 *
 *  @param conteudo a mensagem recebida
 *  @return L_OK se for um pacote OKOK válido, L_ERRO c.c.
 */
int ValidarOKOK(char * conteudo){
	if(modoDebug){
		InsereTextoChat("[ValidarOKOK]");
	}
	if(strlen(conteudo) == 5){
		if(*(conteudo) == 'O'){
			if(*(conteudo + 1) == 'K'){
				if(*(conteudo + 2) == 'O'){
					if(*(conteudo + 3) == 'K'){
						if(*(conteudo + 4) == ':'){
							if(*(conteudo + 5) == '\0'){
								return L_OK;
							} else {
								if(modoDebug){
									InsereTextoChat(
										"[ValidarOKOK]: Quinta letra errada (\\0).");
								}
							}
						} else {
							if(modoDebug){
								InsereTextoChat(
									"[ValidarOKOK]: Quinta letra errada (:).");
							}
						}
					} else {
						if(modoDebug){
							InsereTextoChat(
								"[ValidarOKOK]: Quarta letra errada ( ).");
						}
					}
				} else {
					if(modoDebug){
						InsereTextoChat(
							"[ValidarOKOK]: Terceira letra errada (E).");
					}
				}
			} else {
				if(modoDebug){
					InsereTextoChat("[ValidarOKOK]: Segunda letra errada (Y).");
				}
			}
		} else {
			if(modoDebug){
				InsereTextoChat("[ValidarOKOK]: Primeira letra errada (B).");
			}
		}
	} else {
		if(modoDebug){
			InsereTextoChat("[ValidarOKOK]: Tamanho do pacote errado.");
		}
	}
	return L_ERRO;
}

/** @brief Verifica a integridade do pacote TEST
 *
 *  @param conteudo a mensagem recebida
 *  @return L_OK se for um pacote TEST válido, L_ERRO c.c.
 */
int ValidarTEST(char * conteudo){
	if(modoDebug){
		InsereTextoChat("[ValidarTEST]");
	}
	if(strlen(conteudo) == 5){
		if(*(conteudo) == 'T'){
			if(*(conteudo + 1) == 'E'){
				if(*(conteudo + 2) == 'S'){
					if(*(conteudo + 3) == 'T'){
						if(*(conteudo + 4) == ':'){
							if(*(conteudo + 5) == '\0'){
								return L_OK;
							} else {
								if(modoDebug){
									InsereTextoChat(
										"[ValidarTEST]: Quinta letra errada (\\0).");
								}
							}
						} else {
							if(modoDebug){
								InsereTextoChat(
									"[ValidarTEST]: Quinta letra errada (:).");
							}
						}
					} else {
						if(modoDebug){
							InsereTextoChat(
								"[ValidarTEST]: Quarta letra errada (T).");
						}
					}
				} else {
					if(modoDebug){
						InsereTextoChat(
							"[ValidarTEST]: Terceira letra errada (S).");
					}
				}
			} else {
				if(modoDebug){
					InsereTextoChat("[ValidarTEST]: Segunda letra errada (E).");
				}
			}
		} else {
			if(modoDebug){
				InsereTextoChat("[ValidarTEST]: Primeira letra errada (T).");
			}
		}
	} else {
		if(modoDebug){
			InsereTextoChat("[ValidarTEST]: Tamanho do pacote errado.");
		}
	}
	return L_ERRO;
}

/** @brief Verifica a integridade do pacote DOWN
 *
 *  @param conteudo a mensagem recebida
 *  @return L_OK se for um pacote DOWN válido, L_ERRO c.c.
 */
int ValidarDOWN(char * conteudo){
	/*"DOWN:" = 5,"<nome do usuário>" = 10, ":" = 1, '\0'=1		*/
	if(modoDebug){
		InsereTextoChat("[ValidarDOWN]");
	}
	if(strlen(conteudo) >= 17){
		if(*(conteudo) == 'D'){
			if(*(conteudo + 1) == 'O'){
				if(*(conteudo + 2) == 'W'){
					if(*(conteudo + 3) == 'N'){
						if(*(conteudo + 4) == ':'){
							if(*(conteudo + 15) == ':'){
								if(*(
									conteudo + strlen(conteudo))
									 == '\0'){
									return L_OK;
								} else {
									if(modoDebug){
									InsereTextoChat(
										"[ValidarDOWN]: Ultimo caracter errado (\\0).");
								}
								}
							} else {
								if(modoDebug){
									InsereTextoChat(
										"[ValidarDOWN]: Decima sexta letra errada (:).");
								}
							}
						} else {
							if(modoDebug){
								InsereTextoChat(
									"[ValidarDOWN]: Quinta letra errada (:).");
							}
						}
					} else {
						if(modoDebug){
							InsereTextoChat(
								"[ValidarDOWN]: Quarta letra errada (N).");
						}
					}
				} else {
					if(modoDebug){
						InsereTextoChat(
							"[ValidarDOWN]: Terceira letra errada (W).");
					}
				}
			} else {
				if(modoDebug){
					InsereTextoChat("[ValidarDOWN]: Segunda letra errada (O).");
				}
			}
		} else {
			if(modoDebug){
				InsereTextoChat("[ValidarDOWN]: Primeira letra errada (D).");
			}
		}
	} else {
		if(modoDebug){
			InsereTextoChat("[ValidarDOWN]: Tamanho do pacote errado.");
		}
	}
	return L_ERRO;
}

/** @brief Continuamente verifica se o host está online
 *
 *  @param host ponteiro para o host a ser monitorado
 *  @return NULL
 */
void * _threadKeepAlive(void * host){
	if(modoDebug){
		InsereTextoChat("[_threadKeepAlive]");
	}
	sleep(2);
	ChatHost * myHost = (ChatHost *) host;
	unsigned long myHostIP = myHost->s_addr;
	while(myHost->alive > 0){
		EnviaRawMsg("TEST:", myHost->s_addr, myHost->sin_port);
		myHost->alive--;
		sleep(PERIODOTEST);
		if(BuscaHostPorIP(myHostIP) != NULL &&
			myHost->alive < 1){ /* O host não respondeu		*
								 * depois de MAXTENTATIVAS 	*/
			if(conectadoSRV == L_OK){
				conectadoSRV = L_ERRO;
			}
			RemoveHostPorIP(myHostIP);
			return NULL;

		}
	}
	if(modoDebug){
		InsereTextoChat("[_threadKeepAlive]: FIM");
	}
	return NULL;
}

/** @brief Continuamente envia as mensagens da fila outbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadRX(void * arg){
	if(modoDebug){
		InsereTextoChat("[_ThreadRX]");
	}
	unsigned int sizeSock = sizeof(struct sockaddr_in);
	int sizeMsg = MAXMSGSIZE * sizeof(char);
	int status = 0;
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
			if(modoDebug){
				char strRecebido[200];
				sprintf(strRecebido,
					"[_ThreadRX] Mensagem recebida:\"%s\"",
				 	nMsg->msg);
				InsereTextoChat(strRecebido);
			}
			PushFila(inbox, (void *) nMsg);
		}
	}
	if(modoDebug){
		InsereTextoChat("[_ThreadRX]: FIM");
	}
	return NULL;
}

/** @brief Continuamente insere as mensagens na fila inbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadTX(void * arg){
	if(modoDebug){
		InsereTextoChat("[_ThreadTX]");
	}
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
				if(modoDebug){
					char strEnviado[200];
					sprintf(strEnviado,
						"[_ThreadTX] Mensagem enviada:\"%s\"",
					 	nMsg->msg);
					InsereTextoChat(strEnviado);
				}
			}
			free(nMsg);
		}
	}
	if(modoDebug){
		InsereTextoChat("[_ThreadTX]: FIM");
	}
	return NULL;
}

/** @brief Continuamente transfere msgs da filaInput para o 
 *         outbox
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadCliente(void * servidor){
	if(modoDebug){
		InsereTextoChat("[_ThreadCliente]");
	}
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
	if(modoDebug){
		InsereTextoChat("[_ThreadCliente]: FIM");
	}
	return NULL;
}

/** @brief Continuamente decide o que fazer com as msgs 
 *         recebidas
 *
 *  @param arg NULL
 *  @return NULL
 */
void * _ThreadCorreios(void * arg){
	if(modoDebug){
		InsereTextoChat("[_ThreadCorreios]");
	}
	RawMsg * nMsg;
	while(socketTXRX->status == 0 && execGUI == 1){
		nMsg = (RawMsg *) PopFila(inbox);
		if (nMsg != NULL){
			switch(TipoRawMsg(nMsg->msg)){
			case TEST:
				EnviaRawMsg("OKOK:", 
					nMsg->fromTo.sin_addr.s_addr,
					nMsg->fromTo.sin_port);
				break;
			case DOWN:
				ParseMensagemDOWN(nMsg);
				break;
			case BUSY:
				InsereTextoChat(
					"[Servidor cheio. Tente mais tarde.]");
			case BYE:
				execGUI = 0;				
				break;
			case OKOK:
				ParseMensagemOKOK(nMsg);
				conectadoSRV = 1;
				break;
			default:
				if(modoDebug){
					InsereTextoChat(
						"[Mensagem Invalida recebida do servidor]");
				}
				break;
			}
			free(nMsg);
		}
	}
	if(modoDebug){
		InsereTextoChat("[_ThreadCorreios]: FIM");
	}
	/* Garantindo que o programa irá terminar mesmo que 	*
	 * haja erro no envio do pacote de BYE ao tornar o 		*
	 * socket assíncrono 									*/
	fcntl(socketTXRX->sd, F_SETFL, O_ASYNC);
	InsereTextoChat("[Saindo]");
	EnviaRawMsg("EXIT:", ipServidor, portaServidor);
	sleep(2);
	endwin();	/* Termina modo curses 									*/
	exit(0);
	return NULL;
}
