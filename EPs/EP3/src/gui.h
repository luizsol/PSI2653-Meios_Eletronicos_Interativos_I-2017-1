/** @file 	gui.h
 *  @brief 	Implementa uma interface grafica generica com 
 *			suporte a concorrência
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/05/01
 */

#ifndef __GUI__H
#define __GUI__H

/* Includes 												*/
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "fila.h"

/* Constantes da biblioteca 								*/
#define TAMMAXINPUT 79	/* Tamanho maximo do texto de 		*
						 * usuario							*/
#define TAMNOME 10	/* Tamanho maximo do nome de usuario 	*/
#define MODOCLIENTE 0
#define MODOSERVIDOR 1


/* Variáveis globais da biblioteca 							*/
WINDOW * GUIjanelaTitulo;	/* Janela para descricao e 		*
							 * instrucoes do programa		*/

WINDOW * GUIjanelaChat;	/* Janela de apresentacao do texto	* 
						 * do chat							*/

WINDOW * GUIjanelaInput;/* Janela de escrita do texto do 	*
						 * chat 							*/

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

Lista * textosChat;	/* Textos que serao apresentados no chat*/

Fila * filaInput; 	/* Fila contendo os inputs gerados pelo *
					 * usuario 								*/

int numLinhasChat; /* Numero de linhas ja ocupadas na janela*
					* de chat 								*/

sem_t sem_chat; 	/* Semaforo de controle da janela de 	*
					 * chat 								*/

char * inputBuffer;	/* Variavel de armazenamento do input 	*/

pthread_t * inputThread; /* Thread de processamento de input*/

volatile int execGUI; 	/* Flag que determina se a interface*
						 * gráfica está em execução 		*/

int inputEnable; 	/* Determina se o programa deve 		*
					 * processar inputs gerados pelo usuário*/

/** @brief Inicializa as janelas da interface
 *
 *  @param modo MODOCLIENTE OU MODOSERVIDOR
 */
void InitGUI(int modo);

/** @brief Calcula as dimensões e posições dos elementos da
 *         interface gráfica
 *
 *  @param modo MODOCLIENTE OU MODOSERVIDOR
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

/** @brief Insere um novo texto na janela de chat
 *
 *  @param texto texto a ser inserido
 */
void InsereTextoChat(char * texto);

#endif