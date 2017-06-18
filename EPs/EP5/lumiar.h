/* -----------------------------------------------------------------------------
 * Title:       lumiar
 * File:        lumiar.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
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

#define LUMIAR_STATE_ON 1
#define LUMIAR_STATE_STANDBY 0
#define LUMIAR_STATE_DEFAULT 0

#define	LUMIAR_MODE_AUTO 1
#define LUMIAR_MODE_MANUAL 0
#define LUMIAR_MODE_DEFAULT 1

#define LUMIAR_VALUE_MIN 0
#define LUMIAR_VALUE_MAX 100
#define LUMIAR_VALUE_DEFAULT 50


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
 * Guarda as configurações de comando do sistema.
 */
struct lumiarState
{
	int state;       /* System state */
	int mode;        /* System operating mode */
	int pwmValue;    /* PWM value sent to LED (depends on mode) */
	int userValue;   /* PWM value by user */
	int luminosity;  /* LDR reading */
};


/* Protótipos de funções */
int parseConfig(struct lumiarConfig *c);
int initConfig(struct lumiarConfig *c);


#endif /* LUMIAR_H */
