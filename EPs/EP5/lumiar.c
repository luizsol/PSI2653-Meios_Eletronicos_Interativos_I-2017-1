/* -----------------------------------------------------------------------------
 * Title:       lumiar
 * File:        lumiar.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-12)
 * Date:        2017-06-12
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lumiar.h"


/**
 * Lê as configurações relacionadas a porta e endereço base do servidor salvas
 * no arquivo lumiar.ini
 *
 * @param  c Ponteiro para a struct lumiarConfig. Deve estar inicializado.
 * @return   O número de configurações lidas, negativo em caso de erro.
*/
int parseConfig(struct lumiarConfig *c)
{
	FILE *f;
	f = fopen("./lumiar.ini", "r");
	if(f == NULL)
		return -1;

	int count;
	char line[256];
	char *strp;

	for(count = 0;; count++)
	{
		/* Check for end-of-file or read errors */
		strp = fgets(line, sizeof(line), f);
		if(strp == NULL)
		{
			if(ferror(f))
				return -1;
			if(feof(f))
				break;
		}

		/* Parse line */
		line[strcspn(line, "\n")] = '\0';
		strp = strtok(line, "=");

		if(!strcmp(line, "port"))
			sscanf(strtok(NULL, " "), "%hu", &c->web.port);
		else if(!strcmp(line, "base"))
			strcpy(c->web.base, strtok(NULL, " "));
		else if(!strcmp(line, "pwm.high"))
			sscanf(strtok(NULL, " "), "%u", &c->pwm.highValue);
		else if(!strcmp(line, "pwm.low"))
			sscanf(strtok(NULL, " "), "%u", &c->pwm.lowValue);
		else if(!strcmp(line, "pwm.out"))
			sscanf(strtok(NULL, " "), "%hhu", &c->pwm.outputPin);
		else if(!strcmp(line, "ldr.high"))
			sscanf(strtok(NULL, " "), "%u", &c->ldr.highValue);
		else if(!strcmp(line, "ldr.low"))
			sscanf(strtok(NULL, " "), "%u", &c->ldr.lowValue);
		else if(!strcmp(line, "ldr.in"))
			sscanf(strtok(NULL, " "), "%hhu", &c->ldr.inputPin);
		else if(!strcmp(line, "ldr.out"))
			sscanf(strtok(NULL, " "), "%hhu", &c->ldr.outputPin);
		else
			count--; /* Do not increase count in this iteration */
	}

	return count;
}


/**
 * Inicializa as configurações com seus valores padrão.
 *
 * @param  c Ponteiro para a struct lumiarConfig.
 * @return   Zero em caso de sucesso, não nulo em caso de erro.
*/
int initConfig(struct lumiarConfig *c)
{
	if(c == NULL)
		return -1;

	c->web.port = INI_DEFAULT_PORT;
	const char *tmp = INI_DEFAULT_BASE;
	strcpy(c->web.base, tmp);

	c->pwm.highValue = INI_DEFAULT_PWM_HIGH;
	c->pwm.lowValue  = INI_DEFAULT_PWM_LOW;
	c->pwm.outputPin = INI_DEFAULT_PWM_PIN_OUT;

	c->ldr.highValue = INI_DEFAULT_LDR_HIGH;
	c->ldr.lowValue  = INI_DEFAULT_LDR_LOW;
	c->ldr.inputPin  = INI_DEFAULT_LDR_PIN_IN;
	c->ldr.outputPin = INI_DEFAULT_LDR_PIN_OUT;

	return 0;
}
