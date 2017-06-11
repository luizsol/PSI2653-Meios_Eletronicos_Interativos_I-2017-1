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

#define LDR_OUT_PIN 	1 // Defines dos pinos utilizados pelo LDR
#define LDR_IN_PIN 		2 // Podemos alterar para que sejam definidos no .ini


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

struct calibration calibracao; // Escrever os valores de calibração aqui

// Variáveis de LDR
unsigned int luminosidade; // Valor lido pelo LDR baseado na calibração
unsigned int ldr_driver_running;	// Status da thread do driver de ldr.
								// Escreva 0 para parar

const int ldr_in_pin, ldr_out_pin; // LDR le essas variáveis para inicializar IO

int parseini(struct config *c);


#endif /* LUMIARLIB_H */
