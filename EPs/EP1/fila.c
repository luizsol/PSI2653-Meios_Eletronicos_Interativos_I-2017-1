/** @file 	fila.c
 *  @brief 	Implementa a estrutura de dados Fila
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author Volnys Borges Bernal
 * 			Luiz Sol (luizedusol@gmail.com)
 *			Caio Vinícius Batista Pereira
 *  @date	2017/04/18
 */

#include "fila.h"

/** @brief inicializa uma nova Fila
 *
 *  @param F ponteiro para a Fila a ser inicializada
 */
void InitFila(Fila *F){
	F->inicio = 0;
	F->fim    = 0;
	F->nitens = 0;
	F->tammax = TAMMAXFILA;
}

/** @brief determina se uma fila está vazia
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return TRUE se a fila estiver vazia, FALSE c.c.
 */
int FilaVazia(Fila *F){
	if (F->nitens == 0)
		return(TRUE);
	else
		return(FALSE);
}

/** @brief determina se uma fila está cheia
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return TRUE se a fila estiver cheia, FALSE c.c.
 */
int FilaCheia(Fila *F){
	if (F->nitens == F->tammax)
		return(TRUE);
	else
		return(FALSE);
}

/** @brief insere um item no fim da fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @param item valor a ser inserido na fila
 */
void InserirFila(Fila *F, int item){
	if (FilaCheia(F)){
		printf("ERRO: tentando inserir em fila cheia. \n");
		exit(1);
	}
	F->buffer[F->fim] = item; //alocando o item no fim da fila
	F->fim = (F->fim +1) % F->tammax; //movendo o índice de fim de fila
	F->nitens = F->nitens + 1; //atualizando o tamanho da fila
}

/** @brief remove um item do início da fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @return o valor do elemento retirado da fila
 */
int RetirarFila(Fila *F){
	int item;
	if (FilaVazia(F)){
		printf("ERRO: tentando retirar de fila vazia. \n");
		exit(1);
	}
    item = F->buffer[F->inicio]; //armazenando o valor no início da fila
	F->inicio = (F->inicio + 1) % F->tammax; //movendo o índice de início de fila
	F->nitens = F->nitens - 1; //atualizando o tamanho da fila
	return(item);
}

