/** @file 	fila.c
 *  @brief 	Implementa a estrutura de dados Fila por meio de
 *              um buffer circular com funções reentrantes
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author 	Volnys Borges Bernal
 * 		Luiz Sol (luizedusol@gmail.com)
 *		Caio Vinícius Batista Pereira
 *  @date	2017/05/11
 */

#include "fila.h"

/** @brief inicializa uma nova Fila
 *
 *  @param F ponteiro para a Fila a ser inicializada
 *  @param size tamanho maximo da fila
 */
void InitFila(Fila *F, int size){
	F->inicio = 0;
	F->fim    = 0;
	F->nitens = 0;
	if(size <= TAMMAXFILA)
		F->tammax = size; //Definindo o tamanho maximo da fila
	else{
		fprintf(stderr, "O tamanho da fila nao pode exceder %d. Utilizando o valor maximo (%d).\n", TAMMAXFILA, TAMMAXFILA);
		F->tammax = TAMMAXFILA;
	}
	sem_init(&(F->sem_mutex), 0, 1); /*Inicializando o semáforo
	de mutex com o valor de 0 para que somente 1 thread possa
	alterar a fila por vez*/
	sem_init(&(F->sem_fila_cheia), 0, F->tammax); /*Inicializando o
	semáforo de controle de fila cheia com TAMMAXFILA (pois a fila está
	inicialmente vazia) */
	sem_init(&(F->sem_fila_vazia), 0, 0); /*Inicializando o
	semáforo de controle de fila vazia com -1 (pois a fila está
	inicialmente vazia) */
}

/** @brief imprime a situação atual da fila
 *
 *  @param F ponteiro para a Fila a ser impressa
 */
void PrintFila(Fila *F){
	sem_wait(&(F->sem_mutex));
	printf("[");
	for (int i = 0; i < F->nitens;)
	{
		//printf("%d", F->buffer[(i+F->inicio)%F->tammax]);
		i++;
		if(i < F->nitens)
			printf(", ");
	}
	printf("]\n");
	sem_post(&(F->sem_mutex));
}

/** @brief determina se uma fila está vazia
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return TRUE se a fila estiver vazia, FALSE c.c.
 */
int FilaVazia(Fila *F){
	if (F->nitens == 0)
		return(TRUE);
	else
		return(FALSE);
}

/** @brief determina se uma fila está cheia
 *
 *  @param F ponteiro para a Fila a ser avaliada
 *  @return TRUE se a fila estiver cheia, FALSE c.c.
 */
int FilaCheia(Fila *F){
	if (F->nitens == F->tammax)
		return(TRUE);
	else
		return(FALSE);
}

/** @brief insere um item no fim da fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @param item valor a ser inserido na fila
 */
void InserirFila(Fila *F, *Elemento item){
	sem_wait(&(F->sem_fila_cheia));
	sem_wait(&(F->sem_mutex));

	F->buffer[F->fim] = item; //alocando o item no fim da fila
	F->fim = (F->fim +1) % F->tammax; //movendo o índice de fim de fila
	F->nitens = F->nitens + 1; //atualizando o tamanho da fila

	sem_post(&(F->sem_mutex)); /* Fim do acesso à região crítica */

	sem_post(&(F->sem_fila_vazia));/* Tendo inserido um item na fila, 
	vamos atualizar o semáforo de fila vazia, acordando os consumidores
	bloquados pela fila vazia */

}


/** @brief remove um item do início da fila
 *
 *  @param F ponteiro para a Fila a ser modificada
 *  @return o valor do elemento retirado da fila
 */
int RetirarFila(Fila *F){
	Elemento* item;
	sem_wait(&(F->sem_fila_vazia)); /* ... e aguarde a
	a sinalização de fila não vazia */
	sem_wait(&(F->sem_mutex)); 


    	item = F->buffer[F->inicio]; //armazenando o valor no início da fila pq?
	F->inicio = (F->inicio + 1) % F->tammax; //movendo o índice de início de fila
	F->nitens = F->nitens - 1; //atualizando o tamanho da fila

	sem_post(&(F->sem_mutex)); /* Fim do acesso à região crítica */
	sem_post(&(F->sem_fila_cheia));/* Tendo consumido um item da fila, 
	vamos atualizar o semáforo de fila cheia, acordando os produtores
	bloquados pela fila cheia */

	

	return(item);
	
}


