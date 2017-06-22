/* -----------------------------------------------------------------------------
 * Title:       pwm
 * File:        pwm.c
 * Author:      Gabriel Crabbé, Caio Pereira
 * Version:     0.0 (2017-06-21)
 * Date:        2017-06-21
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include "lumiar.h"


/**
 * Thread principal do driver PWM.
 */
void *pwmService(void *config)
{
	 /*if(wiringPiSetup() == -1)
		perror("Falha em configurar"); // Race condition, right here
	                                   // What if this thread runs before
	                                   // LDR?*/
	struct pwmConfig *p=(struct pwmConfig*)config;
	printf("d1 - pinc: %d \n",(int)p->outputPin);
	pinMode((int)p->outputPin, PWM_OUTPUT);
	
	return NULL;
}


/**
 * Função de interface com a main.
 * Valores de 0-100
 */
int setOperatingPoint(int val,struct pwmConfig *p)
{
	//printf("a1  - val: %d\n",val);
	float aux=(p->highValue - p->lowValue) * val / 100.0 + p->lowValue;
	printf("a1  - aux: %f\n",aux);
	val = (long)aux; //set range 0 -100
	printf("21  - val: %d\n",val);
	printf("a3 - pin: %d\n",(int)p->outputPin);
	pwmWrite((int)p->outputPin, val);  


	return 0;
}











