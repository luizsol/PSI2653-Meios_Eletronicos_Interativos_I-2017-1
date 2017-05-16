/** @file 	fila.h
 *  @brief 	Implementa a estrutura de dados fila com suporte
 *          a concorrência
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/30
 */

#ifndef __FILA__H
#define __FILA__H

/* Includes 												*/
#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

/**
 *  Estrutura que implementa uma fila a partir de uma lista
 */
typedef struct fila {
	/*@{*/
	Lista * lista; 	/**< Ponteiro para a lista que			*
					 *   implementa a fila 					*/
	/*@}*/
} Fila;

/** @brief Cria uma nova Fila já inicializada
 *
 *  @return o ponteiro para uma Fila inicializada
 */
Fila * NewFila();

/** @brief Adiciona um novo Item ao fim da Fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int PushFila(Fila *F, void * conteudo);

/** @brief Retira um Item do inicio da Fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @return ponteiro para o conteudo removido da Fila
 */
void * PopFila(Fila *F);

/** @brief Imprime o estado atual da fila
 *
 *  @param F ponteiro para a Fila a ser impressa
 */
void PrintFila(Fila *F);

/** @brief Determina o tamanho da Fila
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return o tamanho da Fila
 */
int TamFila(Fila *F);


#endif