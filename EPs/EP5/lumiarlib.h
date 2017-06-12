/* -----------------------------------------------------------------------------
 * Title:       lumiarlib
 * File:        lumiarlib.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-11)
 * Date:        2017-06-11
 * Description: Eexercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef LUMIARLIB_H
#define LUMIARLIB_H

#include <linux/limits.h>

#include "ldr.h"
#include "pwm.h"


#define INI_DEFAULT_PORT 8080
#define INI_DEFAULT_BASE "./html"

#define INI_DEFAULT_PWM_HIGH 0xFFFFFFFFu
#define INI_DEFAULT_PWM_LOW  0x00000000u

#define INI_DEFAULT_PWM_PIN_OUT 4

#define INI_DEFAULT_LDR_HIGH 0xFFFFFFFFu
#define INI_DEFAULT_LDR_LOW  0x00000000u

#define INI_DEFAULT_LDR_PIN_IN 	1
#define INI_DEFAULT_LDR_PIN_OUT 2


/**
 * Guarda as configurações do arquivo ini.
 */
struct config
{
	char base[PATH_MAX];
	unsigned short port;
	struct pwmConfig pwm;
	struct ldrConfig ldr;
};


/* Protótipos de funções */
int parseConfig(struct config *c);
int initConfig(struct config *c);


#endif /* LUMIARLIB_H */
