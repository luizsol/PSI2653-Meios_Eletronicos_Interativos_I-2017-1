/* -----------------------------------------------------------------------------
 * Title:       ldr
 * File:        ldr.c
 * Author:      Luiz Sol, Gabriel Crabbé
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include "ldr.h"
#include <pthread.h>
#include <time.h>
#include <wiringPi.h>


//#include "lumiar.h"

int _luminosidade;

// Assinaturas
int ldr_init(LDRConfig * config);

int ldr_init(LDRConfig * config){
	wiringPiSetup();
	piHiPri(99);	// Aumenta a prioridade de execução do programa. Só funciona
				// se o programa for executado como root, c.c. nada ocorre.

	pinMode(config->outputPin, OUTPUT);
	pullUpDnControl(config->outputPin, PUD_OFF);

	pinMode(config->inputPin, INPUT);
	pullUpDnControl(config->inputPin, PUD_OFF);

	return 1;

}

void * ldrService(void * config){
	LDRConfig * _config = (LDRConfig *) config;
	ldr_init(_config);

	unsigned long time_stamp1, time_stamp2;
	int timediff;

	// Discharge RC circuit for a given time
	digitalWrite(_config->outputPin, LOW);
	delayMicroseconds(UPERIOD);

	while(1){
		// Collect timestamp
		time_stamp1 = (unsigned long) time(NULL);

		// Charge RC circuit
		digitalWrite(_config->outputPin, HIGH);

		// Collect timestamp when input = 1
		do{
			time_stamp2 = (unsigned long) time(NULL);
		} while(! digitalRead(_config->inputPin) );

		// Discharge RC circuit for a given time
		digitalWrite(_config->outputPin, LOW);
		delayMicroseconds(UPERIOD);

		timediff = (int) (time_stamp2 - time_stamp1);

		// More light -> less resistance -> less timediff
		_luminosidade = (100 * (_config->highValue - timediff - _config->lowValue)) 
								/ _config->highValue;

		if(_luminosidade <= 0){
			_luminosidade = 0;
		}
	}

	return NULL;
}

/**
 * Retorna a luminosidade atual. Bloqueante.
 *
 * @return luminosidade, entre 0 e 100.
 */
int getLuminosity(void){
	return _luminosidade;
}
