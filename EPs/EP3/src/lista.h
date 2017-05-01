/** @file 	lista.h
 *  @brief 	Implementa a estrutura de dados lista com suporte
 *          a concorrência
 *         	
 *         	Repository: https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/30
 */

#ifndef __LISTA__H
#define __LISTA__H

#define L_ERRO 0
#define L_OK 1

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

/**
 *  Estrutura que implementa um item de uma lista ligada
 */
typedef struct item {
	/*@{*/
	void * conteudo; /**< Ponteiro para o conteúdo do item */
	struct item * proximo; /**< Ponteiro para o próximo item */
	/*@}*/
} Item;

/**
 *  Estrutura que implementa uma lista ligada
 */
typedef struct lista {
	/*@{*/
	Item * inicio; /**< Ponteiro para o primeiro item da lista */
	int nitens; /**< Quantidade de elementos na lista */
	sem_t sem_mutex; /**< Semáforo para controle de acesso à fila */
	sem_t sem_nitens; /**< Semaforo para controle de elementos na lista */
	/*@}*/
} Lista;


/** @brief cria uma nova lista já inicializada
 *
 *  @return o ponteiro para uma lista inicializada
 */
Lista * NewLista();

/** @brief adiciona um novo item a uma lista em uma posição
 *         predeterminada
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @param indice indice em que o novo item deve ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int InsereNovoItemIndice(Lista *L, void * conteudo, int indice);

/** @brief adiciona um novo item ao início de uma lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int PushInicio(Lista *L, void * conteudo);

/** @brief adiciona um novo item ao fim de uma lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int PushFim(Lista *L, void * conteudo);

/** @brief remove um item de uma lista em uma posição
 *         predeterminada
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param indice indice do item a ser removido
 *  @return ponteiro para o conteúdo do item removido 
 *                   (NULL em caso de erro)
 */
void * RemoveItemIndice(Lista *L, int indice);

/** @brief Remove o primeiro item da lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo do item removido 
 *                   (NULL em caso de erro)
 */
void * PopInicio(Lista *L);

/** @brief Remove o último item da lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo do item removido 
 *                   (NULL em caso de erro)
 */
void * PopFim(Lista *L);

/** @brief determina o ponteiro para o conteúdo de um item em 
 *         um determinado índice da lista
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo requisitado (NULL em caso de erro)
 */
void * GetConteudoIndice(Lista *L, int indice);

/** @brief imprime o estado atual da lista
 *
 *  @param L ponteiro para a Lista a ser impressa
 */
void PrintLista(Lista *L);

/** @brief determina o tamanho da lista
 *
 *  @param L ponteiro para a Lista a ser avaliada
 *  @return o tamanho da lista
 */
int TamLista(Lista *L);


#endif