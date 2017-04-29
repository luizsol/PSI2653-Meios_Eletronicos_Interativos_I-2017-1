#include "lista.h"

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
	I->anterior = I->proximo = NULL;
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
	L->inicio = L->fim = NULL;
	L->nitens = 0;
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
	if(indice >= L->nitens){ //Índice inválido
		free(nItem); //Apagando item criado
		sem_post(&(F->sem_mutex));
		return ERRO;
	}
	
	Item * addr = GetItemIndice(L, indice);

	addr->anterior->proximo = nItem; /*Fazendo com que o valor "proximo"
	do item anterior a esse seja igual ao endereço do item sendo inserido*/
	nItem->anterior = addr->anterior; /*Fazendo com que o valor "anterior"
	do novo item seja igual ao valor "anterior" do item sendo deslocado*/
	addr->anterior = nItem;
	nItem->proximo = addr;
	L->nitens++;
	sem_post(&(F->sem_mutex));
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
	/*TODO: otimizar essa parte criando uma opção de percorrer a lista
	de trás para frente a depender do indice e do tamanho da lista*/
	Item * addr = GetItemIndice(L, L->nitens - 1);

	addr->proximo = nItem;
	nItem->anterior = addr;

	L->nitens++;
	sem_post(&(F->sem_mutex));
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
	sem_wait(&(L->sem_mutex));
	if(indice >= L->nitens - 1){
		sem_post(&(F->sem_mutex));
		return NULL;
	}

	Item * addr = GetItemIndice(L, indice);

	/*Linkando os itens anterior e próximo a esse 
	um ao outro*/
	addr->anterior->proximo = addr->proximo;
	addr->proximo->anterior = addr->anterior;

	//Guardando o endereço do conteúdo contido nesse item
	void * conteudo = addr->conteudo;
	//Apagando o item
	free(addr);
	//Atualizando o tamanho da lista
	L->nitens--;

	sem_post(&(F->sem_mutex));
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
	sem_wait(&(L->sem_mutex));
	if(L->nitens == 0){
		sem_post(&(F->sem_mutex));
		return NULL;
	}

	Item * addr = GetItemIndice(L, L->nitens-1);

	/*Atualizando a referência do índice anterior*/
	addr->anterior->proximo = NULL;

	//Guardando o endereço do conteúdo contido nesse item
	void * conteudo = addr->conteudo;
	//Apagando o item
	free(addr);
	//Atualizando o tamanho da lista
	L->nitens--;

	sem_post(&(F->sem_mutex));
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
	if(indice >= L->nitens){
		return NULL;
	}
	/*TODO: otimizar essa parte criando uma opção de percorrer a lista
	de trás para frente a depender do indice e do tamanho da lista*/
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
		sem_post(&(F->sem_mutex));
		return NULL;
	}
	sem_post(&(F->sem_mutex));
	return addr->conteudo;
}