/** @file 	main.c
 *  @brief 	Programa produtor-consumidor com mutex, wait & signal
 *			
 *			Esse programa se utiliza de threads, mutex e
 * 			variáveis de condição para implementar uma 
 *			simulação do clássico problema produtor-consumidor 
 *
 *         	Source: PSI2653 - Meios Eletrônicos Interativos I
 *         	Repository: https://github.com/luizsol/MEI
 *  @author Volnys Borges Bernal
 * 			Luiz Sol (luizedusol@gmail.com)
 *			Caio Vinícius Batista Pereira
 *  @date	2017/04/18
 */

#include <stdio.h>
#include <pthread.h>
#include "fila.h"

#define TOTAL_PROD 10


//Mutex que será utilizado para controlar o acesso à região crítica
pthread_mutex_t  mutex;

//Declaracao das variaveis de condicao:
pthread_cond_t filacheia, filavazia;

//Fila a ser utilizada pelos produtores e consumidores
Fila f;

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
	int aguardar; //flag local de estado

	printf("Inicio produtor %d \n",id);

	while (i < TOTAL_PROD){
		item = i + (id*1000); //item produzido nessa iteração
		pthread_mutex_lock(&mutex); /*produtor acessa a região crítica
		a fim de verificar se pode inserir um item na fila*/
		do{
			aguardar = FALSE;
			if (FilaCheia(&f)){ 
				aguardar = TRUE;
				pthread_cond_wait(&filacheia, &mutex);/*se a fila 
				estiver cheia o produtor irá se bloquear e aguardar
				até que algum consumidor acorde todos os produtores.
				Ainda assim é importante verificar novamente se a fila
				está cheia pois pode ser o caso que outro produtor 
				que tenha sido acordado antes desse tenha novamente 
				enchido a fila*/
			}
		} while (aguardar == TRUE);
		/*Nesse ponto o produtor pode garantir que há espaço na fila
		e que só ele tem acesso à região crítica. Hora de inserir seu
		item na fila*/
		printf("Produtor %d inserindo item %d\n", id, item);
		InserirFila(&f,item);
		/*Tendo inserido o item na fila o produtor deve sinalizar a 
		todos os consumidores bloqueados que há um novo item nesta*/
		pthread_cond_signal(&filavazia);
		/*Como não é mais necessário o acesso à região crítica este
		produtor deve desbloquear o mutex*/
		pthread_mutex_unlock(&mutex);
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
	int aguardar; //flag local de estado

	printf("Inicio consumidor %d \n",id);

	while (1){
		// retirar item da fila
		pthread_mutex_lock(&mutex);/*consumidor acessa a região 
		crítica a fim de verificar se pode consumir um item da fila*/
		do{
			aguardar = FALSE;
			if (FilaVazia(&f)){
				aguardar = TRUE;
				pthread_cond_wait(&filavazia, &mutex);/*se a fila 
				estiver vazia o consumidor irá se bloquear e aguardar
				até que algum produtor acorde todos os consumidores.
				Ainda assim é importante verificar novamente se a fila
				está vazia pois pode ser o caso que outro consumidor 
				que tenha sido acordado antes desse tenha novamente 
				esvaziado a fila*/
			}
		} while (aguardar == TRUE);
		/*Nesse ponto o consumidor pode garantir que há ao menos um 
		item na fila e que só ele tem acesso à região crítica. Hora 
		de consumir um item da fila*/
		item = RetirarFila(&f);
		/*Tendo consumido um item da fila o consumidor deve sinalizar
		a todos os produtores bloqueados que há um novo slot nesta*/
		pthread_cond_signal(&filacheia);
		/*Como não é mais necessário o acesso à região crítica este
		consumidor deve desbloquear o mutex*/
		pthread_mutex_unlock(&mutex);

		printf("Consumidor %d consumiu item %d\n", id, item); 
		sleep(2);
	}
	printf("Consumidor %d terminado \n", id); 
}


/** @brief função main
 *
 *  Instancia e executa a simulação
 *
 *  @param argc números de parâmetros na chamada do programa
 *  @param argv parâmetros utilizadas na chamada do programa
 */
int main(int argc, char* argv[]){
	InitFila(&f);//Inicializando a Fila
	
	//Inicializando as variáveis de condição
	pthread_cond_init(&filacheia,NULL);
	pthread_cond_init(&filavazia,NULL);

	//Declaração das threads de produtores e consumidores
	pthread_t prod1;
	pthread_t prod2;
	pthread_t prod3;
	pthread_t cons1;
	pthread_t cons2;

	printf("Programa Produtor-Consumidor\n");

	printf("Iniciando variaveis de sincronizacao.\n");
	pthread_mutex_init(&mutex,NULL);

	printf("Disparando threads produtores\n");
	pthread_create(&prod1, NULL, (void*) produtor,1);
	pthread_create(&prod2, NULL, (void*) produtor,2);
	pthread_create(&prod3, NULL, (void*) produtor,3);

	printf("Disparando threads consumidores\n");
	pthread_create(&cons1, NULL, (void*) consumidor,1);
	pthread_create(&cons2, NULL, (void*) consumidor,2);

	//Aguardando o fim das threads para encerrar o programa
	pthread_join(prod1,NULL);
	pthread_join(prod2,NULL);
	pthread_join(prod3,NULL);
	pthread_join(cons1,NULL);
	pthread_join(cons2,NULL);
	
	printf("Terminado processo Produtor-Consumidor.\n\n");
}



