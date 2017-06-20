/* -----------------------------------------------------------------------------
 * Title:       pwm
 * File:        pwm.h
 * Author:      Gabriel Crabbé -- Caio Pereira
 * Version:     0.0 (2017-06-12)
 * Date:        2017-06-12
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef PWM_H
#define PWM_H


#define INPUT 0
#define OUTPUT 1

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
void *pwmService(void *conf);
int setOperatingPoint(int val);
int export_gpio(int pin);
int direction_gpio(int pin, int direction);
int unexport_gpio(int pin);


#endif /* PWM_H */
