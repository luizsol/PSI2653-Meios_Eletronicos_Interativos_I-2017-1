/** @file 	fila.c
 *  @brief 	Implementa a estrutura de dados fila com suporte
 *          a concorrência
 *         	
 *         	Repository: https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/30
 */

#include "fila.h"

/** @brief cria uma nova fila já inicializada
 *
 *  @return o ponteiro para uma fila inicializada
 */
Fila * NewFila(){
	Fila * nFila = malloc(sizeof(Fila));
	nFila->lista = NewLista();
	return nFila;
}

/** @brief adiciona um novo item ao fim da fila
 *
 *  @param F ponteiro para a fila a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int PushFila(Fila *F, void * conteudo){
	return PushFim(F->lista, conteudo);
}

/** @brief retira um item do inicio da fila
 *
 *  @param F ponteiro para a fila a ser modificada
 *  @return ponteiro para o conteudo removido da fila
 */
void * PopFila(Fila *F){
	return PopInicio(F->lista);
}

/** @brief imprime o estado atual da fila
 *
 *  @param F ponteiro para a Fila a ser impressa
 */
void PrintFila(Fila *F){
	PrintLista(F->lista);
}

/** @brief determina o tamanho da fila
 *
 *  @param F ponteiro para a fila a ser avaliada
 *  @return o tamanho da fila
 */
int TamFila(Fila *F){
	return TamLista(F->lista);
}