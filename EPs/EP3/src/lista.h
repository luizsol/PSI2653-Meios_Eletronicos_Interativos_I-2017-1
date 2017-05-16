/** @file 	lista.h
 *  @brief 	Implementa a estrutura de dados lista com suporte
 *          a concorrência
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/30
 */

#ifndef __LISTA__H
#define __LISTA__H

/* Includes 												*/
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

/* Constantes da biblioteca 								*/
#define L_ERRO 0
#define L_OK 1

/**
 *  Estrutura que implementa um item de uma lista ligada
 */
typedef struct item {
	/*@{*/
	void * conteudo; /**< Ponteiro para o conteúdo do item 	*/
	struct item * proximo; /**< Ponteiro para o próximo item*/
	/*@}*/
} Item;

/**
 *  Estrutura que implementa uma lista ligada
 */
typedef struct lista {
	/*@{*/
	Item * inicio; 	/**< Ponteiro para o primeiro item da 	*
					 *   lista 								*/
	int nitens; /**< Quantidade de elementos na lista 		*/
	sem_t sem_mutex;/**< Semáforo para controle de acesso à *
					 *   fila 								*/
	sem_t sem_nitens;	/**< Semaforo para controle de 		*
						 *   elementos na lista 			*/
	/*@}*/
} Lista;


/** @brief Cria uma nova Lista já inicializada
 *
 *  @return o ponteiro para uma Lista inicializada
 */
Lista * NewLista();

/** @brief Adiciona um novo item a uma Lista em uma posição
 *         predeterminada
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @param indice indice em que o novo Item deve ser inserido
 *  @return status da operação (L_OK ou L_ERRO)
 */
int InsereNovoItemIndice(Lista *L, void * conteudo, int indice);

/** @brief Adiciona um novo item ao início de uma Lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (L_OK ou L_ERRO)
 */
int PushInicio(Lista *L, void * conteudo);

/** @brief Adiciona um novo Item ao fim de uma lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (L_OK ou L_ERRO)
 */
int PushFim(Lista *L, void * conteudo);

/** @brief Remove um Item de uma Lista em uma posição
 *         predeterminada
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param indice indice do item a ser removido
 *  @return ponteiro para o conteúdo do item removido 
 *                   (NULL em caso de L_ERRO)
 */
void * RemoveItemIndice(Lista *L, int indice);

/** @brief Remove o primeiro Item da Lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo do Item removido 
 *                   (NULL em caso de L_ERRO)
 */
void * PopInicio(Lista *L);

/** @brief Remove o último item da lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo do item removido 
 *                   (NULL em caso de L_ERRO)
 */
void * PopFim(Lista *L);

/** @brief Determina o ponteiro para o conteúdo de um Item em 
 *         um determinado índice da Lista
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo requisitado (NULL em 
 *          caso de L_ERRO)
 */
void * GetConteudoIndice(Lista *L, int indice);

/** @brief Imprime o estado atual da Lista
 *
 *  @param L ponteiro para a Lista a ser impressa
 */
void PrintLista(Lista *L);

/** @brief Determina o tamanho da Lista
 *
 *  @param L ponteiro para a Lista a ser avaliada
 *  @return o tamanho da Lista
 */
int TamLista(Lista *L);


#endif