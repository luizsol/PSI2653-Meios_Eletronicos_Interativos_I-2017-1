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
#include <wiringPi.h>
#include <unistd.h> 
#include <stdio.h>

int _luminosidade;

// Assinaturas
int ldr_init(LDRConfig * config);

int ldr_init(LDRConfig * config){
	int setup = wiringPiSetupGpio();
	
	if(setup == -1){
		puts("Erro na inicializacao do GPIO");
	} else {
		puts("GPIO inicializado com sucesso");
	}
	
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

	int timediff;  // u seconds;

	// Discharge RC circuit for a given time
	digitalWrite(_config->outputPin, LOW);
	delayMicroseconds(UPERIOD);

	while(1){
		// Collect timestamp
		timediff = 0;

		// Charge RC circuit
		digitalWrite(_config->outputPin, HIGH);

		// Collect timestamp when input = 1
		do{
			timediff += SAMPLEPERIOD;
			usleep(SAMPLEPERIOD);
		} while(digitalRead(_config->inputPin) == LOW);

		// Discharge RC circuit for a given time
		digitalWrite(_config->outputPin, LOW);
		delayMicroseconds(UPERIOD);

		timediff = timediff/1000;

		// More light -> less resistance -> less timediff
		_luminosidade = (100 * 
						(_config->highValue - timediff - _config->lowValue)) 
						/ (_config->highValue - _config->lowValue);

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
