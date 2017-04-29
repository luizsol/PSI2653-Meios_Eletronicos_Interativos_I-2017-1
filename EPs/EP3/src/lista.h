/** @file 	lista.h
 *  @brief 	Implementa a estrutura de dados lista com suporte
 *          a concorrência
 *         	
 *         	Repository: https://github.com/luizsol/MEI
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/29
 */

#ifndef __LISTA__H
#define __LISTA__H

#define ERRO 0
#define OK 1

#include <stdlib.h>
#include <semaphore.h>

/**
 *  Estrutura que implementa um item de uma lista duplamente ligada
 */
typedef struct item {
	/*@{*/
	struct item * anterior; /**< Ponteiro para o item anterior */
	void * conteudo; /**< Ponteiro para o conteúdo do item */
	struct item * proximo; /**< Ponteiro para o próximo item */
	/*@}*/
} Item;

/**
 *  Estrutura que implementa uma lista duplamente ligada
 */
typedef struct lista {
	/*@{*/
	Item * inicio; /**< Ponteiro para o primeiro item da lista */
	Item * fim; /**< Ponteiro para o último item da lista */
	int nitens; /**< Quantidade atual de elementos na lista */
	sem_t sem_mutex; /**< Semáforo para controle de acesso à fila */
	/*@}*/
} Lista;

/** @brief cria um novo item já inicializada
 *  
 *  @param conteudo o ponteiro para o conteúdo a ser inserido
 *  @return o ponteiro para um objeto inicializada
 */
Item * NewItem(void * conteudo);

/** @brief inicializa uma nova Fila
 *
 *  @param I ponteiro para um item a ser inicializada
 *  @param conteudo o ponteiro para o conteúdo a ser inserido
 */
void InitItem(Item *I, void * conteudo);

/** @brief cria uma nova lista já inicializada
 *
 *  @return o ponteiro para uma lista inicializada
 */
Lista * NewLista();

/** @brief inicializa uma nova Fila
 *
 *  @param L ponteiro para a Lista a ser inicializada
 */
void InitLista(Lista *L);

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

/** @brief determina o ponteiro para um item em um determinado
 *         índice da lista
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o item requisitado (NULL em caso de erro)
 */
Item * GetItemIndice(Lista *L, int indice);

/** @brief determina o ponteiro para o conteúdo de um item em 
 *         um determinado índice da lista
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo requisitado (NULL em caso de erro)
 */
void * GetConteudoIndice(Lista *L, int indice);


#endif