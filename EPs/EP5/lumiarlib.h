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


#define INI_DEFAULT_PORT 8080
#define INI_DEFAULT_BASE "./html"

#define INI_DEFAULT_PWM_HIGH 0xFFFFFFFFu
#define INI_DEFAULT_PWM_LOW  0x00000000u

#define INI_DEFAULT_LDR_HIGH 0xFFFFFFFFu
#define INI_DEFAULT_LDR_LOW  0x00000000u


/**
 * Guarda as configurações do arquivo ini.
 */
struct config
{
	char base[PATH_MAX];
	unsigned short port;
	struct calibration pwm;
	struct calibration ldr;
};


/**
 * Guarda os valores de calibração.
 */
struct calibration
{
	unsigned int high;
	unsigned int low;
};


int parseini(struct config *c);


#endif /* LUMIARLIB_H */
