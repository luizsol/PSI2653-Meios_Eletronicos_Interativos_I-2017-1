/* -----------------------------------------------------------------------------
 * Title:       pwm
 * File:        pwm.h
 * Author:      Gabriel Crabbé, Caio Pereira
 * Version:     0.0 (2017-06-20)
 * Date:        2017-06-20
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
	unsigned char outputPin;
	unsigned int lowValue;
	unsigned int highValue;
};


/* Protótipos de funções */
void *pwmService(void *config);
int setOperatingPoint(int val, struct pwmConfig *p);


#endif /* PWM_H */
