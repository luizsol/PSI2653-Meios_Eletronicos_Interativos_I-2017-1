/** @file 	main.c
 *  @brief 	Programa produtor-consumidor com mutex, wait & signal
 *			
 *			Esse programa se utiliza de threads, mutex e
 * 			variáveis de condição para implementar uma 
 *			simulação do clássico problema produtor-consumidor 
 *			
 *			Utilize-o com os seguintes parametros [opcionais]
 *			de configuracao:
 *              
 * 			./prod_cons -p <numero de produtores> -c <numero de consumidores> -f <tam. max da fila>
 *
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author Volnys Borges Bernal
 * 			Luiz Sol (luizedusol@gmail.com)
 *			Caio Vinícius Batista Pereira
 *  @date	2017/04/26
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

#define TOTAL_PROD 10

#define MAX_PRODUTORES	100 //Maior numero possivel de produtores
#define MAX_CONSUMIDORES 100 //Maior numero possivel de consumidores

#define STD_PRODUTORES 3 //Quantidade padrao de produtores
#define STD_CONSUMIDORES 3 //Quantidade padrao de consumidores
#define STD_FILA 4 //Tamanho padrao da fila

#define PRINT_FILA //Imprime o estado da fila em um intervalo regular de tempo


//Fila a ser utilizada pelos produtores e consumidores
Fila f;

//Prototipos das funcoes

void consumidor(int id);
void produtor(int id);
void cria_consumidores(int n);
void cria_produtores(int n);
void processa_parametros(int argc, char* argv[]);

//Array das threads de produtores e consumidores
pthread_t* produtores[MAX_PRODUTORES];
pthread_t* consumidores[MAX_CONSUMIDORES];

//Numero de produtores e consumidores criados
int n_produtores;
int n_consumidores;
int max_fila;


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
		PrintFila(&f);
		sleep(2);	
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
void produtor(int id){
	int i=0; //contador da quantidade de itens produzidos
	int item; //variável que contém o último ítem produzido

	printf("Inicio produtor %d \n",id);

	while (i < TOTAL_PROD){
		item = i + (id*1000); //item produzido nessa iteração
		
		InserirFila(&f,item);
		printf("Produtor %d inseriu item %d\n", id, item);
		i++;
		sleep(2);
	}
	printf("Produtor %d terminado \n", id); 
}


/** @brief simula um consumidor
 *
 *  Essa função quando executada como uma thread simula
 *  um consumidor que a cada 2 segundos tenta consumir 
 *  um item da fila.
 *
 *  @param o id do consumidor
 */
void consumidor(int id){
	int item; //variável que contém o último item consumido

	printf("Inicio consumidor %d \n",id);

	while (1){
		item = RetirarFila(&f);
		printf("Consumidor %d consumiu item %d\n", id, item);
		sleep(2);
	}
	printf("Consumidor %d terminado \n", id); 
}


/** @brief cria e inicializa as threads de consumidores
 *
 *  @param n quantidade de consumidores a serem criados
 */
void cria_consumidores(int n){
	printf("Disparando threads consumidores\n");
	for(int i = 0; i < n; i++){
		consumidores[i] = malloc(sizeof(pthread_t));
		pthread_create(consumidores[i], NULL, (void*) consumidor,i+1);
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
		pthread_create(produtores[i], NULL, (void*) produtor,i+1);
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
	n_produtores = n_consumidores = max_fila = 0;
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
		case 'f':
			max_fila = atoi(optarg);
			if(max_fila <= 0){
				fprintf(stderr, "%d fora do intervalo de tamanho de fila aceito aceito. Alterando para %d.\n", max_fila, STD_FILA);	
				max_fila = 4;
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
  			} else if (optopt == 'f') {
				fprintf(stderr, "Valor do parametro \"f\" nao fornecido. Utilizando o valor padrao (%d).\n", STD_FILA);		
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
	if(max_fila == 0)
		max_fila = STD_FILA;

	printf("Numero de produtores: %d\n", n_produtores);
	printf("Numero de consumidores: %d\n", n_consumidores);
	printf("Tamanho maximo da fila: %d\n", max_fila);
	
	InitFila(&f, max_fila);//Inicializando a Fila
	cria_consumidores(n_consumidores); //Inicializando Consumidores
	cria_produtores(n_produtores); //Inicializando Produtores

}




