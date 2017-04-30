/** @file 	teste_fila.c
 *  @brief 	Programa produtor-consumidor para testar a biblioteca fila.h
 *			
 *			Esse programa implementa o EP2 com a biblioteca fila.h
 *              
 * 			./teste_fila -p <numero de produtores> -c <numero de consumidores>
 *
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author Volnys Borges Bernal
 * 			Luiz Sol (luizedusol@gmail.com)
 *			Caio Vinícius Batista Pereira
 *  @date	2017/04/30
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "fila.h"

#define TOTAL_PROD 99

#define MAX_PRODUTORES	100 //Maior numero possivel de produtores
#define MAX_CONSUMIDORES 100 //Maior numero possivel de consumidores

#define STD_PRODUTORES 3 //Quantidade padrao de produtores
#define STD_CONSUMIDORES 3 //Quantidade padrao de consumidores

#define PRINT_FILA //Imprime o estado da fila em um intervalo regular de tempo


//Fila a ser utilizada pelos produtores e consumidores
Fila * F;

//Prototipos das funcoes

void * consumidor(void* iid);
void * produtor(void* iid);
void cria_consumidores(int n);
void cria_produtores(int n);
void processa_parametros(int argc, char* argv[]);

//Array das threads de produtores e consumidores
pthread_t* produtores[MAX_PRODUTORES];
pthread_t* consumidores[MAX_CONSUMIDORES];

//Numero de produtores e consumidores criados
int n_produtores;
int n_consumidores;


/** @brief função main
 *
 *  Instancia e executa a simulação
 *
 *  @param argc números de parâmetros na chamada do programa
 *  @param argv parâmetros utilizadas na chamada do programa
 */
int main(int argc, char* argv[]){
	

	printf("Programa Produtor-Consumidor\n");

	processa_parametros(argc, argv);

	//Aguardando o fim das threads para encerrar o programa
	//pthread_join(consumidores[0],NULL); //Nunca vai acontercer. Garante a espera
	
	while(1){
		PrintFila(F);
		sleep(1);	
	}
	
	printf("Terminado processo Produtor-Consumidor.\n\n");
}

/** @brief simula um produtor
 *
 *  Essa função quando executada como uma thread simula
 *  um produtor que a cada 2 segundos produz um item e
 *  aguarda o momento certo para inseri-lo na fila. O 
 *  produtor irá encerrar sua produção assim que o número
 *  de itens produzidos for igual a TOTAL_PROD.
 *
 *  @param o id do produtor
 */
void * produtor(void* iid){
	int id = (int) iid;
	int i = 0; //contador da quantidade de itens produzidos
	int * item; //variável que contém o último ítem produzido

	printf("Inicio produtor %d \n",id);

	while (i < TOTAL_PROD){
		item = malloc(sizeof(int));
		*item = i + (id*1000); //item produzido nessa iteração
		while(ERRO == PushFila(F, (void*)item)){}
		printf("Produtor %d inseriu item %d\n", id, *item);
		i++;
		sleep(1);
	}
	printf("Produtor %d terminado \n", id); 
	return NULL;
}


/** @brief simula um consumidor
 *
 *  Essa função quando executada como uma thread simula
 *  um consumidor que a cada 2 segundos tenta consumir 
 *  um item da fila.
 *
 *  @param o id do consumidor
 */
void * consumidor(void* iid){
	int id = (int) iid;
	printf("Inicio consumidor %d \n",id);
	int * item; //variável que contém o último item consumido
	while (1){
		
		item = NULL;
		while(item == NULL){
			item = (int*)PopFila(F);
		}
		printf("Consumidor %d consumiu item %d\n", id, *item);
		free(item);
		sleep(2);
	}
	printf("Consumidor %d terminado \n", id); 
	return NULL;
}


/** @brief cria e inicializa as threads de consumidores
 *
 *  @param n quantidade de consumidores a serem criados
 */
void cria_consumidores(int n){
	printf("Disparando threads consumidores\n");
	for(int i = 0; i < n; i++){
		consumidores[i] = malloc(sizeof(pthread_t));
		pthread_create(consumidores[i], NULL, consumidor,(void*)(i+1));
		printf("Thread de consumidor %d disparada\n", i+1);
	}
}

/** @brief cria e inicializa as threads de produtores
 *
 *  @param n quantidade de produtores a serem criados
 */
void cria_produtores(int n){
	printf("Disparando threads produtores\n");
	for(int i = 0; i < n; i++){
		produtores[i] = malloc(sizeof(pthread_t));
		pthread_create(produtores[i], NULL, produtor,(void*)(i+1));
		printf("Thread de produtor %d disparada\n", i+1);
	}
}


/** @brief processa os parametros utilizados na chamada do programa
 *
 *  Essa função ira procurar pelo parametro "-p" seguido de um int para
 *  determinar o numero de produtores, pelo parametro "-c" seguido de 
 *  um int para determinar o numero de consumidores e pelo parametro "-f"
 *  seguido de um int para determinar o tamanho max da fila. Para cada parametro
 *  que nao for passado o programa utilizara o valor STD_CONSUMIDORES para
 *  o numero de consumidores, STD_PRODUTORES para o numero de produtores e 
 *  STD_FILA para o tamanho maximo da fila
 */
void processa_parametros(int argc, char* argv[]){
	n_produtores = n_consumidores = 0;
	char opt;
	while ((opt = getopt(argc, argv, "p:c:f:")) != -1){
		switch(opt) {
    		case 'p':
			n_produtores = atoi(optarg);
			if(n_produtores <= 0 || n_produtores > MAX_PRODUTORES){
				fprintf(stderr, "%d fora do intervalo de produtores aceito. Alterando para o valor padrao (%d).\n", n_produtores, STD_PRODUTORES);	
				n_produtores = STD_PRODUTORES;
			}
			break;
		case 'c':
			n_consumidores = atoi(optarg);
			if(n_consumidores <= 0 || n_consumidores > MAX_CONSUMIDORES){
				fprintf(stderr, "%d fora do intervalo de consumidores aceito. Alterando para %d.\n", n_consumidores, STD_CONSUMIDORES);	
				n_consumidores = STD_CONSUMIDORES;
			}
			break;
			
		case '?':
    		/* Casos em que o usuario utiliza um parametro mas nao determina um valor
     		* $ ./prod_cons -p -c 10
     		*/
			if (optopt == 'p') {
    				fprintf(stderr, "Valor do parametro \"p\" nao fornecido. Utilizando o valor padrao (%d).\n", STD_PRODUTORES);
    			
  			} else if (optopt == 'c') {
     				fprintf(stderr, "Valor do parametro \"c\" nao fornecido. Utilizando o valor padrao (%d).\n", STD_CONSUMIDORES);
  			} else {
     				printf("\nParametro %c invalido\n", optopt);
  			}
			break;
		}
	}
	if(n_produtores == 0)
		n_produtores = STD_PRODUTORES;
	if(n_consumidores == 0)
		n_consumidores = STD_CONSUMIDORES;

	printf("Numero de produtores: %d\n", n_produtores);
	printf("Numero de consumidores: %d\n", n_consumidores);
	
	F = NewFila(); //Inicializando a lista
	cria_consumidores(n_consumidores); //Inicializando Consumidores
	cria_produtores(n_produtores); //Inicializando Produtores

}
