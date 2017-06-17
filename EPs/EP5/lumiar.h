/* -----------------------------------------------------------------------------
 * Title:       lumiar
 * File:        lumiar.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-12)
 * Date:        2017-06-12
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef LUMIAR_H
#define LUMIAR_H


#include <linux/limits.h>

#include "ldr.h"
#include "pwm.h"
#include "web.h"


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
struct lumiarConfig
{
	struct webConfig web;
	struct pwmConfig pwm;
	struct ldrConfig ldr;
};


/**
 * Guarda as configurações do arquivo ini.
 */
struct lumiarState
{
	int state;
	int mode;
	int value;
	int luminosity;
	char statechar[11];
	char modechar[11]
	char valuechar[11];
	char luminositychar[11];
};


/* Protótipos de funções */
int parseConfig(struct lumiarConfig *c);
int initConfig(struct lumiarConfig *c);


#endif /* LUMIAR_H */
