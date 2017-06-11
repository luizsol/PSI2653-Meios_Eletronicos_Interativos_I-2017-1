/* -----------------------------------------------------------------------------
 * Title:       lumiarlib
 * File:        lumiarlib.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-11)
 * Date:        2017-06-11
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include "lumiarlib.h"


/** 
 * Lê as configurações relacionadas a porta e endereço base do servidor salvas
 * no arquivo lumiar.ini
 *
 * @param  c Ponteiro para a struct config. Deve estar inicializado em zero.
 * @return   O número de configurações lidas, -1 em caso de erro de arquivo.
*/
int parseini(struct config *c)
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
			sscanf(strtok(NULL, " "), "%hu", &c->port);
		else if(!strcmp(line, "base"))
			strcpy(c->base, strtok(NULL, " "));
		else if(!strcmp(line, "pwm.high"))
			sscanf(strtok(NULL, " "), "%u", &c->pwm.high);
		else if(!strcmp(line, "pwm.low"))
			sscanf(strtok(NULL, " "), "%u", &c->pwm.low);
		else if(!strcmp(line, "ldr.high"))
			sscanf(strtok(NULL, " "), "%u", &c->ldr.high);
		else if(!strcmp(line, "ldr.low"))
			sscanf(strtok(NULL, " "), "%u", &c->ldr.low);
		else
			count--; /* Do not increase count in this iteration */
	}

	return count;
}
