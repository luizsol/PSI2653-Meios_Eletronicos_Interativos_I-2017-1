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

/** @brief Inicializa as janelas da interface
 *
 *  @param modo MODOCLIENTE OU MODOSERVIDOR
 */
void InitJanelas(int modo){

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
									inicioYInput, inicioXInput);

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
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}