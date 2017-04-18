/** @file 	fila.h
 *  @brief 	Implementa a estrutura de dados Fila por meio de
 *			um buffer circular
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author Volnys Borges Bernal
 * 			Luiz Sol (luizedusol@gmail.com)
 *			Caio Vinícius Batista Pereira
 *  @date	2017/04/18
 */

#ifndef __FILA__H
#define __FILA__H

#include <stdio.h>
#include <stdlib.h>

#define TAMMAXFILA 4 //Tamanho da fila
#define  FALSE 0
#define  TRUE  1

/**
 *  Estrutura que implementa a Fila com buffer circular
 */
typedef struct fila {
	/*@{*/
	int buffer[TAMMAXFILA]; /**< Array que armazena os intens da fila */
	int inicio; /**< Índice do primeiro elemento da fila */
	int fim; /**< Índice do primeiro slot livre no fim da fila */
	int nitens; /**< Quantidade atual de elementos na fila */
	int tammax; /**< Tamanho máximo da fila */
	/*@}*/
} Fila;

/** @brief inicializa uma nova Fila
 *
 *  @param F ponteiro para a Fila a ser inicializada
 */
void InitFila(Fila *F);

/** @brief determina se uma fila está vazia
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return TRUE se a fila estiver vazia, FALSE c.c.
 */
int FilaVazia(Fila *F);

/** @brief determina se uma fila está cheia
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return TRUE se a fila estiver cheia, FALSE c.c.
 */
int FilaCheia(Fila *F);

/** @brief insere um item no fim da fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @param item valor a ser inserido na fila
 */
void InserirFila(Fila *F, int item);

/** @brief remove um item do início da fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @return o valor do elemento retirado da fila
 */
int RetirarFila(Fila *F);

#endif

	

