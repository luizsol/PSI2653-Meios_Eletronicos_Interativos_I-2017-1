// arquivo: pthread.c
// descricao: Programa para teste de threads. Utiliza a biblioteca pthreads.
// para compilar:  cc -o phtread pthread.c -lpthread

#include <stdio.h>
#include <pthread.h>
int i=0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

imprimir_msg(char *nome)
      {
        while (i<10)
              {
                pthread_mutex_lock(&mymutex);
                printf("Thread %s - %d\n", nome, i);
                i++;
                pthread_mutex_unlock(&mymutex);
                sleep(2);
              }
        printf("Thread %s terminado \n", nome);
      }

int main()
      {
        pthread_t thread1;
        pthread_t thread2;

        printf("Programa de teste de pthreads \n");

      printf("Disparando primeiro thread\n");
        pthread_create(&thread1, NULL, (void*) imprimir_msg,"thread_1");

      printf("Disparando segundo thread\n");
        pthread_create(&thread2, NULL, (void*) imprimir_msg,"thread_2");

        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);

      printf("Terminando processo\n");
      }



