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
	initConfig(&sconf);
	parseConfig(&sconf);

	/* Create threads */
	pthread_t pwm, ldr, webserver;
	printf("Launching dedicated threads\n");
	pthread_create(&pwm, NULL, pwm, (void *) sconf.pwm);
	pthread_create(&ldr, NULL, ldr_initandrun, (void *) sconf.ldr);
	pthread_create(&webserver, NULL, webserver, (void *) sconf);

	/* Main loop */
	for(;;)
	{

	}

	return 0;
}
