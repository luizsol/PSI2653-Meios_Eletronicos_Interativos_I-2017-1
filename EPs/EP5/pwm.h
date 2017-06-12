/* -----------------------------------------------------------------------------
 * Title:       pwm
 * File:        pwm.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-11)
 * Date:        2017-06-11
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef PWM_H
#define PWM_H


/**
 * Struct de configuração do PWM.
 */
struct pwmConfig
{
	unsigned int outputPin;
	unsigned int lowValue;
	unsigned int highValue;
};


/* Protótipos de funções */
int pwmService(void *conf);


#endif /* PWM_H */
