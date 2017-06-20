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




/**
 * Struct de configuração do PWM.
 */
struct pwmConfig
{
	unsigned int outputPin=1;
	unsigned int lowValue;
	unsigned int highValue;
};


/* Protótipos de funções */
void *pwmService(void *conf);
int setOperatingPoint(int val);



#endif /* PWM_H */
