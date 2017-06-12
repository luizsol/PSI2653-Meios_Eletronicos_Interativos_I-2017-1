/* -----------------------------------------------------------------------------
 * Title:       ldr
 * File:        ldr.h
 * Author:      Luiz Sol, Gabriel Crabbé
 * Version:     0.0 (2017-06-11)
 * Date:        2017-06-11
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

/*
 * Para instalar wiringPi:
 * git clone git://git.drogon.net/wiringPi
 * cd wiringPi
 * ./build
 */

#ifndef LDR_H
#define LDR_H

#include <semaphore.h>


#define UPERIOD 500000 // Período entre observações (500[ms])


/**
 * Struct de controle do LDR.
 */
struct ldrDriver
{
	int status;
	int value;
	struct ldrConfig config;
	sem_t mutex;
};


/**
 * Struct de configuração do LDR.
 */
struct ldrConfig
{
	unsigned int inputPin;
	unsigned int outputPin;
	unsigned int lowValue;
	unsigned int highValue;
};


/**
 * Variável  de controle do LDR.
 */
extern struct ldrDriver ldr;


/* Protótipos de funções */
int ldr_initandrun(void *conf);


#endif /* LDR_H */
