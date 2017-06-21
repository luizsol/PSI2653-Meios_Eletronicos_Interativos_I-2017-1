/* -----------------------------------------------------------------------------
 * Title:       pwm
 * File:        pwm.c
 * Author:      Gabriel Crabbé, Caio Pereira
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
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
   Estipula quando a escala de 0 a 100?
 */
void *pwmService(void *conf)
{

	if(wiringPiSetup() ==-1)
		perror("Falha em configurar");
	pinMode(outputPin,PWM_OUTPUT);
	lowValue=0;
	HighValue=1000;
	return NULL;
}


/**
 * Função de interface com a main.
   Valores de 0-100
 */
int setOperatingPoint(int val)
{
	val=(highValue-lowValue)*val/100+lowValue; //set range 0 -100
	pwmWrite(1,val);


	return 0;
}











