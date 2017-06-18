/* -----------------------------------------------------------------------------
 * Title:       ldr
 * File:        ldr.c
 * Author:      Luiz Sol, Gabriel Crabbé
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include <pthread.h>
#include <time.h>
#include <wiringPi.h>

#include "ldr.h"


// Variáveis locais
pthread_t _thread_ldr;

// Assinaturas
int ldr_init(void);
int _ldr_run(void);


int ldr_initandrun(void){
	ldr_init();
	_thread_ldr = malloc(sizeof(pthread_t));
	ldr_driver_running = 1;
	pthread_create(threadCliente, NULL,	_ldr_run, NULL);
	return 1;
}

int ldr_init(void){
	wiringPiSetup();
	piHiPri(99);// Aumenta a prioridade de execução do programa. Só funciona se o
				// programa for executado como root, c.c. nada ocorre.

	pinMode(ldr_out_pin, OUTPUT);
	pullUpDnControl(ldr_out_pin, PUD_OFF);

	pinMode(ldr_in_pin, INPUT);
	pullUpDnControl(ldr_in_pin, PUD_OFF);

	return 1;

}

void * _ldr_run(void * arg){
	unsigned long time_stamp1, time_stamp2;
	int _luminosidade, timediff;

	// Discharge RC circuit for a given time
	digitalWrite(ldr_out_pin, LOW);
	delayMicroseconds(UPERIOD);

	while(ldr_driver_running){
		// Collect timestamp
		time_stamp1 = (unsigned long)time(NULL);

		// Charge RC circuit
		digitalWrite(ldr_out_pin, HIGH);

		// Collect timestamp when input = 1
		do{
			time_stamp2 = (unsigned long)time(NULL);
		} while(! digitalRead(ldr_in_pin))

		// Discharge RC circuit for a given time
		digitalWrite(ldr_out_pin, LOW);
		delayMicroseconds(UPERIOD);

		timediff = (int) (time_stamp2 - time_stamp1);

		// More light -> less resistance -> less timediff
		_luminosidade = (100 * (ldr.high - timediff - ldr.low)) / ldr.high;

		if(_luminosidade >= 0){
			luminosidade = _luminosidade;
		} else {
			luminosidade = 0;
		}
	}

	return NULL;
}


/**
 * Thread principal do driver LDR.
 *
 * @param  sconf ponteiro para a struct config
 * @return       ponteiro NULL
 */
void *ldrService(void *config)
{
	return NULL
}


/**
 * Retorna a luminosidade atual. Bloqueante.
 *
 * @return luminosidade, entre 0 e 100.
 */
int getLuminosity()
{
	return 0
}
