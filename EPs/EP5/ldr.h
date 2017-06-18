/* -----------------------------------------------------------------------------
 * Title:       ldr
 * File:        ldr.h
 * Author:      Luiz Sol, Gabriel Crabbé
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
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


#define UPERIOD 500000 // Período entre observações (500[ms])


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


/* Protótipos de funções */
void *ldrService(void *config);
int getLuminosity();


#endif /* LDR_H */
