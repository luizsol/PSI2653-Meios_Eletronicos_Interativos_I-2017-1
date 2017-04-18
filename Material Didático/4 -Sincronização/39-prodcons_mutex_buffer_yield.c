// arquivo: prodcons.c
// descricao: Programa produtor-consumidor com mutex
// Utiliza a biblioteca pthreads.
// para compilar:  cc -o phtread pthread.c -lpthread

#include <pthread.h>

#define  FALSE 0
#define  TRUE  1


// Declaracao das variaveis de condicao:
pthread_mutex_t  mutex;

//Buffer
#define  BUFFERVAZIO 0
#define  BUFFERCHEIO 1
int buffer;
int estado = BUFFERVAZIO;


void produtor(int id)
        {
	int i=0;
	int item;
        int aguardar;

	printf("Inicio produtor %d \n",id);
	while (i < 10)
	    {
            //produzir item
	    item = i + (id*1000);

            pthread_mutex_lock(&mutex);
            while (estado == BUFFERCHEIO)
	        {
                pthread_mutex_unlock(&mutex);
                pthread_yield();
                pthread_mutex_lock(&mutex);
	        }

	    //inserir item
            printf("Produtor %d inserindo item %d\n", id, item); 
            buffer = item;
	    estado = BUFFERCHEIO;
	    
	    pthread_mutex_unlock(&mutex);
	    i++;
	    sleep(1);
	    }
	printf("Produtor %d terminado \n", id); 
        }

void consumidor(int id)
        {
	int item;
	int aguardar;

	printf("Inicio consumidor %d \n",id);
	while (1)
	    {
            // retirar item da fila
            pthread_mutex_lock(&mutex);
            while (estado == BUFFERVAZIO)
                {
                pthread_mutex_unlock(&mutex);
                pthread_yield();
                pthread_mutex_lock(&mutex);
	        } 
	    item = buffer;
	    estado = BUFFERVAZIO;
	    pthread_mutex_unlock(&mutex);

	    // processar item
            printf("Consumidor %d consumiu item %d\n", id, item); 

 	    sleep(1);
	    }
	printf("Consumidor %d terminado \n", id); 
        }

int main()
{ 
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

	pthread_join(prod1,NULL);
	pthread_join(prod2,NULL);
	pthread_join(prod3,NULL);
	pthread_join(cons1,NULL);
	pthread_join(cons2,NULL);
	
        printf("Terminado processo Produtor-Consumidor.\n\n");
}


