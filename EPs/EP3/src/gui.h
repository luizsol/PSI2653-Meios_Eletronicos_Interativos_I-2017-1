/** @file 	gui.h
 *  @brief 	Implementa uma interface grafica generica com suporte
 *          a concorrência
 *         	
 *         	Repository: https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/30
 */

#ifndef __GUI__H
#define __GUI__H

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TAMMAXINPUT 79	/** Tamanho maximo do texto de usuario*/
#define MODOCLIENTE 0
#define MODOSERVIDOR 1

WINDOW * GUIjanelaTitulo;	/** Janela para descricao e instucoes do 
							 *  programa
							 */
WINDOW * GUIjanelaChat;	/** Janela de apresentacao do texto do
						 *  chat
						 */
WINDOW * GUIjanelaInput;	/** Janela de escrita do texto do chat */

/** Variaveis de dimensionamento e posicionamento da janela de
 *  titulo
 */
int alturaTitulo, larguraTitulo, inicioYTitulo, inicioXTitulo;
/** Variaveis de dimensionamento e posicionamento da janela de
 *  chat
 */
int alturaChat, larguraChat, inicioYChat, inicioXChat;
/** Variaveis de dimensionamento e posicionamento da janela de
 *  input
 */
int alturaInput, larguraInput, inicioYInput, inicioXInput;

/** @brief Inicializa as janelas da interface
 *
 *  @param modo MODOCLIENTE OU MODOSERVIDOR
 */
void InitJanelas(int modo);

/** @brief Inicializa as janelas da interface
 *
 */
void AtualizaDimPos(int modo);

/** @brief Inicializa as janelas da interface
 *
 *  @param altura altura da janela a ser criada
 *  @param largura largura da janela a ser criada
 *  @param inicioY linha em que a janela deve ser colocada
 *  @param inicioX coluna em que a janela deve ser colocada
 *  @return o ponteiro para a janela criada
 */
WINDOW * NovaJanela(int altura, int largura, int inicioY, 
					int inicioX);


#endif