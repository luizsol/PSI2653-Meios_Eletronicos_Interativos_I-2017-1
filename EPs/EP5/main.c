/* -----------------------------------------------------------------------------
 * Title:       main
 * File:        main.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-11)
 * Date:        2017-06-11
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "lumiarlib.h"


/**
 * Main thread.
 */
int main(int argc, char *argv[])
{
	/* Parse configuration file */
	struct config sconf;
	if(parseini(&sconf) < 0)
	{
		sconf.port = INI_DEFAULT_PORT;
		strcpy(sconf.base, INI_DEFAULT_BASE);

		sconf.pwm.high = INI_DEFAULT_PWM_HIGH;
		sconf.pwm.low  = INI_DEFAULT_PWM_LOW; 
		sconf.ldr.high = INI_DEFAULT_LDR_HIGH;
		sconf.ldr.low  = INI_DEFAULT_LDR_LOW;
	}

	/* Create threads */
	pthread_t pwm, ldr, webserver;
	printf("Launching dedicated threads\n");
	pthread_create(&pwm, NULL, worker, (void *) );

	return 0;
}
