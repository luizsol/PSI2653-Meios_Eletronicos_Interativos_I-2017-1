/** @file 	fila.c
 *  @brief 	Implementa a estrutura de dados fila com suporte
 *          a concorrência
 *         	
 *         	Repository: 
 *			https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/30
 */

#include "fila.h"

/** @brief Cria uma nova Fila já inicializada
 *
 *  @return o ponteiro para uma Fila inicializada
 */
Fila * NewFila(){
	Fila * nFila = malloc(sizeof(Fila));
	nFila->lista = NewLista();
	return nFila;
}

/** @brief Adiciona um novo Item ao fim da Fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int PushFila(Fila *F, void * conteudo){
	return PushFim(F->lista, conteudo);
}

/** @brief Retira um Item do inicio da Fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @return ponteiro para o conteudo removido da Fila
 */
void * PopFila(Fila *F){
	return PopInicio(F->lista);
}

/** @brief Imprime o estado atual da fila
 *
 *  @param F ponteiro para a Fila a ser impressa
 */
void PrintFila(Fila *F){
	PrintLista(F->lista);
}

/** @brief Determina o tamanho da Fila
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return o tamanho da Fila
 */
int TamFila(Fila *F){
	return TamLista(F->lista);
}