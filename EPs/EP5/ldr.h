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
typedef struct ldrConfig{
	unsigned int inputPin;
	unsigned int outputPin;
	unsigned int lowValue;
	unsigned int highValue;
} LDRConfig ;

/**
 * Thread principal do driver LDR.
 *
 * @param  sconf ponteiro para a struct config
 * @return       ponteiro NULL
 */
void * ldrService(void * config);

/* Determina a luminosidade do ambiente
 *
 * @return o valor da luminosidade entre 0 e 100
 */
int getLuminosity(void);


#endif /* LDR_H */
