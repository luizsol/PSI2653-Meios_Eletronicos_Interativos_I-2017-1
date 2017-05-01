/** @file 	gui.h
 *  @brief 	Implementa uma interface grafica generica com 
 *			suporte a concorrência
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/30
 */

#include "gui.h"

void AtualizaChat();
void RemoveTextoChat();

/** @brief Inicializa as janelas da interface
 *
 *  @param modo MODOCLIENTE OU MODOSERVIDOR
 */
void InitGUI(int modo){

	sem_init(&sem_chat, 0, 1);
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

	keypad(stdscr, TRUE);	/* Habilitando as teclas F1..F12*/

	AtualizaDimPos(modo);	/* Atualizando as variaveis de 	*
    						 * posicionamento				*/

    refresh();	/* Atualizando o terminal para mostrar as 	*
    			 * janelas 									*/

	GUIjanelaTitulo = NovaJanela(alturaTitulo, larguraTitulo,
								inicioYTitulo, inicioXTitulo);

    GUIjanelaChat = NovaJanela(alturaChat, larguraChat, 
    							inicioYChat, inicioXChat);
    if(modo == MODOCLIENTE){
		GUIjanelaInput = NovaJanela(alturaInput, larguraInput, 
									inicioYInput, inicioXInput
									);

		mvwprintw(GUIjanelaTitulo, 1, (COLS-2-27)/2, 
					"Cliente Chat (F1 para sair)");
    } else {
    	mvwprintw(GUIjanelaTitulo, 1 , (COLS-2-27)/2,
    				"Servidor Chat (F1 para sair)");
    }
    wrefresh(GUIjanelaTitulo);
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
		alturaInput = 2+((COLS-2)/TAMMAXINPUT +1);
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
	box(GUIjanelaChat, 0 , 0);	/* 0, 0 gives default 		*
							 * characters for the vertical	*
							 * and horizontal lines			*/

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
		for(int j = 0; j < nLinhas-1; j++, yCursor--){
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
							+ j*(larguraChat - 2 - padding);

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
	}
	wrefresh(GUIjanelaChat);
}