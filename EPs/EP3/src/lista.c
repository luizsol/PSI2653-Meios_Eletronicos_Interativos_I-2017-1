/** @file 	lista.c
 *  @brief 	Implementa a estrutura de dados lista com suporte
 *          a concorrência
 *         	
 *         	Repository: https://github.com/luizsol/MEI/tree/master/EPs/EP3
 *  @author	Luiz Sol (luizedusol@gmail.com)
 *  @date	2017/04/29
 */

#include "lista.h"

//Assinaturas de funcoes de uso exclusivamente interno
Item * NewItem(void * conteudo);
void InitItem(Item *I, void * conteudo);
void InitLista(Lista *L);
Item * GetItemIndice(Lista *L, int indice);

/** @brief cria um novo item já inicializada
 *  
 *  @param conteudo o ponteiro para o conteúdo a ser inserido
 *  @return o ponteiro para um objeto inicializada
 */
Item * NewItem(void * conteudo){
	Item * nItem = malloc(sizeof(Item));
	InitItem(nItem, conteudo);
	return nItem;
}

/** @brief inicializa uma nova Fila
 *
 *  @param I ponteiro para um item a ser inicializada
 *  @param conteudo o ponteiro para o conteúdo a ser inserido
 */
void InitItem(Item *I, void * conteudo){
	I->proximo = NULL;
	I->conteudo = conteudo;
}

/** @brief cria uma nova lista já inicializada
 *
 *  @return o ponteiro para uma lista inicializada
 */
Lista * NewLista(){
	Lista * nLista = malloc(sizeof(Lista));
	InitLista(nLista);
	return nLista;
}

/** @brief inicializa uma nova Fila
 *
 *  @param L ponteiro para a Lista a ser inicializada
 */
void InitLista(Lista *L){
	L->inicio = NULL;
	sem_init(&(L->sem_nitens), 0, 0);
	sem_init(&(L->sem_mutex), 0, 1);
}

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
int InsereNovoItemIndice(Lista *L, void * conteudo, int indice){
	Item * nItem = NewItem(conteudo); //Criando novo item a ser add
	sem_wait(&(L->sem_mutex));
	if(indice > TamLista(L)){ //Índice inválido
		free(nItem); //Apagando item criado
		sem_post(&(L->sem_mutex));
		return ERRO;
	}
	if(TamLista(L) == 0){//Lista vazia
		L->inicio = nItem;
		L->nitens++;
		sem_post(&(L->sem_nitens));
		sem_post(&(L->sem_mutex));
		return OK;
	}
	if(indice != 0){ //Indice nao e 0 nem a lista nao esta vazia
		Item * addr = GetItemIndice(L, indice-1);
		nItem->proximo = addr->proximo;
		addr->proximo = nItem;
	} else { //Indice e 0 e a lista nao esta vazia
		nItem->proximo = L->inicio;
		L->inicio = nItem;
	}
	L->nitens++;
	sem_post(&(L->sem_mutex));
	sem_post(&(L->sem_nitens));
	
	return OK;
}

/** @brief adiciona um novo item ao início de uma lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int PushInicio(Lista *L, void * conteudo){
	return InsereNovoItemIndice(L, conteudo, 0);
}

/** @brief adiciona um novo item ao fim de uma lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @param conteudo endereço do conteúdo a ser inserido
 *  @return status da operação (OK ou ERRO)
 */
int PushFim(Lista *L, void * conteudo){
	Item * nItem = NewItem(conteudo); //Criando novo item a ser add
	sem_wait(&(L->sem_mutex));
	if(TamLista(L) == 0){//Lista vazia
		L->inicio = nItem;
		L->nitens++;
		sem_post(&(L->sem_nitens));
		sem_post(&(L->sem_mutex));
		return OK;
	}
	//Lista nao vazia
	Item * addr = GetItemIndice(L, TamLista(L)-1);
	addr->proximo = nItem;
	L->nitens++;
	sem_post(&(L->sem_nitens));
	sem_post(&(L->sem_mutex));
	return OK;
}

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
void * RemoveItemIndice(Lista *L, int indice){
	if(indice >= TamLista(L)){
		return NULL;
	}
	sem_wait(&(L->sem_nitens));
	sem_wait(&(L->sem_mutex));
	Item * deletado;
	if(indice == 0){
		deletado = L->inicio;
		L->inicio = L->inicio->proximo;
	} else { 
		Item * addr = GetItemIndice(L, indice-1);
		deletado = addr->proximo;
		addr->proximo = deletado->proximo;
	}
	L->nitens--;
	sem_post(&(L->sem_mutex));
	//Guardando o endereço do conteúdo contido nesse item
	void * conteudo = deletado->conteudo;
	//Apagando o item
	free(deletado);
	return conteudo;
}

/** @brief Remove o primeiro item da lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo do item removido 
 *                   (NULL em caso de erro)
 */
void * PopInicio(Lista *L){
	return RemoveItemIndice(L, 0);
}

/** @brief Remove o último item da lista
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo do item removido 
 *                   (NULL em caso de erro)
 */
void * PopFim(Lista *L){
	sem_wait(&(L->sem_nitens));
	sem_wait(&(L->sem_mutex));
	Item * deletado;
	if(TamLista(L) == 1){ /* Removendo o unico elemento da
	lista */
		deletado = L->inicio;
		L->inicio = NULL;
	} else {/* Removendo o ultimo elemento de uma lista com 
	mais de 1 elemento */
		Item * addr = GetItemIndice(L, TamLista(L) - 2);
		/* Item anterior ao que vamos remover*/
		deletado = addr->proximo;
		addr->proximo = NULL;
	}
	L->nitens--;	
	sem_post(&(L->sem_mutex));
	//Guardando o endereço do conteúdo contido nesse item
	void * conteudo = deletado->conteudo; //Erro!
	//Apagando o item
	free(deletado);
	return conteudo;
}

/** @brief determina o ponteiro para um item em um determinado
 *         índice da lista
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o item requisitado (NULL em caso de erro)
 */
Item * GetItemIndice(Lista *L, int indice){
	Item * addr = L->inicio;
	for(int i = 0; i < indice; i++){
		addr = addr->proximo;
	}
	return addr;
}

/** @brief determina o ponteiro para o conteúdo de um item em 
 *         um determinado índice da lista
 *
 *  OBS: o primeiro indice da lista é 0
 *
 *  @param L ponteiro para a Lista a ser modificada
 *  @return ponteiro para o conteúdo requisitado (NULL em caso de erro)
 */
void * GetConteudoIndice(Lista *L, int indice){
	sem_wait(&(L->sem_mutex));
	Item * addr = GetItemIndice(L, indice);
	if(addr == NULL){
		sem_post(&(L->sem_mutex));
		return NULL;
	}
	sem_post(&(L->sem_mutex));
	return addr->conteudo;
}

/** @brief imprime o estado atual da lista
 *
 *  @param L ponteiro para a Lista a ser impressa
 */
void PrintLista(Lista *L){
	sem_wait(&(L->sem_mutex));
	printf("[");
	for(int i = 0; i < TamLista(L);){
		int item = *((int*)(GetItemIndice(L, i)->conteudo));
		printf("%d", item);
		i++;
		if(i <TamLista(L))
			printf(",");
	}
	printf("]\n");
	sem_post(&(L->sem_mutex));
}

/** @brief determina o tamanho da lista
 *
 *  @param L ponteiro para a Lista a ser avaliada
 *  @return o tamanho da lista
 */
int TamLista(Lista *L){
	return L->nitens;
}