/** @file 	fila.h
 *  @brief 	Implementa a estrutura de dados Fila por meio de
 *              um buffer circular com funções reentrantes
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author 	Volnys Borges Bernal
 * 		Luiz Sol (luizedusol@gmail.com)
 *		Caio Vinícius Batista Pereira
 *  @date	2017/05/11
 */

#ifndef __FILA__H
#define __FILA__H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "elemento.h"

#define TAMMAXFILA 10 //Tamanho da fila 10 -- especificacoes do programa
#define  FALSE 0
#define  TRUE  1
#define  DEBUG 1 //Modo debug

/**
 *  Estrutura que implementa a Fila com buffer circular
 */
typedef struct fila {
	/*@{*/
	Elemento* buffer[TAMMAXFILA]; /**< Array que armazena os elementos da fila */
	int inicio; /**< Índice do primeiro elemento da fila */
	int fim; /**< Índice do primeiro slot livre no fim da fila */
	int nitens; /**< Quantidade atual de elementos na fila */
	int tammax; /**< Tamanho máximo da fila */
	sem_t sem_mutex; /**< Semáforo para implementação de mutex */
	sem_t sem_fila_cheia; /**< Semáforo de controle de fila cheia */
	sem_t sem_fila_vazia; /**< Semáforo de controle de fila vazia */
	/*@}*/
} Fila;

/** @brief inicializa uma nova Fila
 *
 *  @param F ponteiro para a Fila a ser inicializada
 *  @param size tamanho maximo da fila
 */
void InitFila(Fila *F, int size);

/** @brief imprime a situação atual da fila
 *
 *  @param F ponteiro para a Fila a ser impressa
 */
void PrintFila(Fila *F);

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
void InserirFila(Fila *F, *Elemento item);

/** @brief remove um item do início da fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @return o valor do elemento retirado da fila
 */
int RetirarFila(Fila *F);

#endif

	


