/** @file 	gui.h
 *  @brief 	Implementa uma interface grafica generica com 
 *			suporte a concorrência
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/05/01
 */

#include "gui.h"

/* Cabecalhos de funcoes nao declaradas no .h 				*/
void AtualizaChat();
void RemoveTextoChat();
void * InputManager(void * arg);
void InputUpdate();
void InputErase();

/** @brief Inicializa as janelas da interface
 *
 *  @param modo MODOCLIENTE OU MODOSERVIDOR
 */
void InitGUI(int modo){

    execGUI = 1;	/* Flag de execucao da GUI 				*/

	sem_init(&sem_chat, 0, 1);	/* Inicializando semaforo 	*
								 * de controle da GUI 		*/

	if(modo == MODOCLIENTE){
		/* Inicializando a variavel de buffer de entrada 	*/
		inputBuffer = malloc((TAMMAXINPUT+1)*sizeof(char));
		InputErase();
		filaInput = NewFila();
	}

	/* Inicializando a variavel de buffer do chat 			*/
	inputBuffer = malloc((TAMMAXINPUT+1)*sizeof(char));
	textosChat = NewLista();	/* Inicializando a lista de *	
								 * textos que serao 		*
								 * mostrados na janela de 	*
								 * chat 					*/
	numLinhasChat = 0;

	initscr();	/* Inicia o modo curses						*/

	cbreak();	/* Desativando o character	buffer pois		*
				 * queremos tratar todas as entradas. Aviso:*
				 * isso desativa as interrupcoes de teclado	*
				 * como ^C 									*/

	noecho();	/* Desativando o echo do terminal 			*/
	curs_set(0);	/* Desativando o cursor 				*/

	keypad(stdscr, TRUE);	/* Habilitando as teclas F1..F12*/

	AtualizaDimPos(modo);	/* Atualizando as variaveis de 	*
    						 * posicionamento				*/

    refresh();	/* Atualizando o terminal para mostrar as 	*
    			 * janelas 									*/

	/* Inicializando Janelas 								*/
	GUIjanelaTitulo = NovaJanela(alturaTitulo, larguraTitulo,
								inicioYTitulo, inicioXTitulo);

    GUIjanelaChat = NovaJanela(alturaChat, larguraChat, 
    							inicioYChat, inicioXChat);
    if(modo == MODOCLIENTE){
		GUIjanelaInput = NovaJanela(alturaInput, larguraInput, 
								inicioYInput, inicioXInput);

		mvwprintw(GUIjanelaTitulo, 1, (COLS-2-27)/2, 
					"Cliente Chat (F1 para sair)");
    } else {
    	mvwprintw(GUIjanelaTitulo, 1 , (COLS-2-27)/2,
    				"Servidor Chat (F1 para sair)");
    }

    wrefresh(GUIjanelaTitulo);

    /* Inicializando a thread de manipulacao de input 		*/
    inputThread = malloc(sizeof(pthread_t));
    pthread_create(inputThread, NULL, InputManager, NULL);
}

/** @brief Inicializa as janelas da interface
 *
 */
void AtualizaDimPos(int modo){
	alturaTitulo = 3;
	larguraInput = COLS;
	inicioYInput = 0;
	inicioXInput = 0;

	if(modo == MODOCLIENTE){
		alturaInput = 2 + (TAMMAXINPUT/(COLS-2) + 1);
		larguraInput = COLS;
		inicioYInput = LINES-alturaInput;  
		inicioXInput = 0;
	} else {
		alturaInput = 0;
		larguraInput = 0;
		inicioYInput = 0;
		inicioXInput = 0;
	}

	alturaChat = LINES - alturaTitulo - alturaInput;
	larguraChat = COLS;
	inicioYChat = alturaTitulo;
	inicioXChat = 0;
}

/** @brief Inicializa as janelas da interface
 *
 *  @param altura altura da janela a ser criada
 *  @param largura largura da janela a ser criada
 *  @param inicioY linha em que a janela deve ser colocada
 *  @param inicioX coluna em que a janela deve ser colocada
 *  @return o ponteiro para a janela criada
 */
WINDOW * NovaJanela(int altura, int largura, int inicioY, 
					int inicioX){
	WINDOW *local_win;

	local_win = newwin(altura, largura, inicioY, inicioX);
	box(local_win, 0 , 0);	/* 0, 0 gives default characters*
							 * for the vertical and 		*
							 * horizontal lines				*/
	wrefresh(local_win);

	return local_win;
}

/** @brief Determina o numero de linhas necessario para
 *         imprimir um texto no chat
 *
 *  @param texto texto a ser avaliado
 *  @return o numero de linhas necessario
 */
int NumeroLinhas(char * texto){
	int size = strlen(texto);
	int nlinhas = 1;
	/* contabilizando a primeira linha que tem o nome do 	*
	 * usuario alem do texto do chat 						*/
	size = size - larguraChat - 2;
	while(size > 0){
		size = size - ((larguraChat - 2) - (TAMNOME + 2));
		nlinhas++;
	}
	return nlinhas;
}

/** @brief Insere um novo texto na janela de chat
 *
 *  @param texto texto a ser inserido
 */
void InsereTextoChat(char * texto){
	char * conteudo = malloc((TAMNOME + 2 + TAMMAXINPUT + 1)
							*sizeof(char));
	strcpy(conteudo, texto);
	sem_wait(&sem_chat);
	numLinhasChat += NumeroLinhas(conteudo);
	PushInicio(textosChat, (void *)conteudo);
	while(numLinhasChat > alturaChat - 2){
		RemoveTextoChat();
	}
	AtualizaChat();
	sem_post(&sem_chat);
}

/** @brief Remove o texto mais antigo da lista do chat
 *
 */
void RemoveTextoChat(){
	char * conteudo = (char *)PopFim(textosChat);
	numLinhasChat -= NumeroLinhas(conteudo);
	free(conteudo);
}

/** @brief Atualiza a janela de chat
 *
 */
void AtualizaChat(){
	werase(GUIjanelaChat);
	box(GUIjanelaChat, 0 , 0);

	int tamLista = TamLista(textosChat);
	int xCursor = 1;
	int yCursor = alturaChat - 1; 	/* Posicionando o cursor*
									 * na parte de baixo da *
									 * janela de chat 		*/
	
	int padding = TAMNOME + 2;	/* Padding do texto que for *
								 * impresso alem da primeira*
								 * linha 					*/

	for(int i = 0; i < tamLista; i++){ /* Percorrendo a 	*
										* lista do item mais*
										* recente ao mais 	*
										* antigo			*/

		/* Texto a ser impresso nessa iteracao				*/
		char * texto = (char *)GetConteudoIndice(textosChat, 
												i);
		/* Numero de linhas necessarias para imprimir esse 	*
		 * texto 											*/
		int nLinhas = NumeroLinhas(texto);

		yCursor -= nLinhas;	/* Posicionando o cursor na 	*
							 * linha onde o texto comecara 	*
							 * a ser impresso 				*/

		/* Imprimindo os primeiro (larguraChat - 2) 		*
		 * caracteres da primeira linha do texto na posicao *
		 * xCursor yCursor 									*/
		mvwprintw(GUIjanelaChat, yCursor, xCursor, "%.*s", 
					larguraChat - 2, texto);

		

		/* Caso esse texto use mais que uma linha 			*/
		for(int j = 0; j < nLinhas-1; j++){
			xCursor = padding + 1;	/* Movendo o cursor para*/
			yCursor++;				/* a proxima linha 		*
									 * levando em 			*
								 	 * consideracao a 		*
									 * indentacao do texto 	*/
			
			/* Calculando o indice de onde a impressao ira 	*
			 * continuar 									*/
			int idxInicio = larguraChat - 2	/* caracteres	*
											 * impressos na *
											 * na primeira	*
											 * linha 		*/

							/* caracteres impressos nas 	*
							 * outras linhas 				*/
							+ j*((larguraChat - 2) - padding);

			/* Imprimindo o restante do texto 				*/
			mvwprintw(GUIjanelaChat, 
						yCursor, 
						xCursor, 
						"%.*s", 
						larguraChat - 2 - padding, 
						texto + idxInicio);

			xCursor = 1;
		}
		xCursor = 1;
		yCursor -= (nLinhas-1);
	}
	wrefresh(GUIjanelaChat);
}

/** @brief Apaga todo o conteudo do buffer de input
 *
 */
void InputErase(){
	for(int i = 0; i < (TAMMAXINPUT+1); i++){
		*(inputBuffer + i) = '\0';
	}
}

/** @brief Funcao responsavel por atualizar a janela de input
 *
 */
void InputUpdate(){
	int xCursor = 1;
	int yCursor = 1;
	int tamBuffer = strlen(inputBuffer);
	int nLinhas = tamBuffer/(larguraInput - 2);
	if(strlen(inputBuffer)%(larguraInput - 2) > 0){
		nLinhas++;
	}
	werase(GUIjanelaInput);
	box(GUIjanelaInput, 0 , 0);

	for(int i = 0; i < nLinhas; i++, yCursor++){
		mvwprintw(GUIjanelaInput, 
					yCursor, 
					xCursor, 
					"%.*s", 
					larguraChat - 2, 
					inputBuffer + i*(larguraInput - 2));
	}

	wrefresh(GUIjanelaInput);
}

/** @brief Funcao responsavel por tratar inputs do usuario
 *
 */
void * InputManager(void * arg){
	int ch;
	int idx = 0;
	char * conteudo;
	while((ch = getch()) != KEY_F(1)){
		switch(ch){
			case KEY_BACKSPACE:
				if(idx > 0){
					idx--;
					*(inputBuffer + idx) = '\0';
					InputUpdate();
				}
				break;
			case 10: /* \n */
				conteudo =
					malloc((TAMMAXINPUT+1)*sizeof(char));
				strcpy(conteudo, inputBuffer);
				PushFila(filaInput, conteudo);
				InputErase();
				idx = 0;
				InputUpdate();
				break;
			default:
				if(ch > 31 && ch < 127	/* Caracteres ASCII */
					&& idx < TAMMAXINPUT-1){
					*(inputBuffer + idx) = ch;
					idx++;
					InputUpdate();
				}
				break;
		}
	}
	endwin();	/* Termina modo curses 						*/
	execGUI = 0;
	exit(0);
	
	return NULL;
}