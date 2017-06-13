/* -----------------------------------------------------------------------------
 * Title:       main
 * File:        main.c
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
#include <pthread.h>

#include "lumiar.h"


/**
 * Main thread.
 */
int main(int argc, char *argv[])
{
	/* Parse configuration file */
	struct lumiarConfig sconf;
	initConfig(&sconf);
	parseConfig(&sconf);

	/* Webserver driver */
	struct webDriver wdriver;
	initDriver(&wdriver, &(sconf.web));

	/* Create threads */
	pthread_t pwmThread, ldrThread, webThread;
	printf("Launching dedicated threads\n");
	// pthread_create(&webThread, NULL, webserver, (void *) &wdriver);
	pthread_create(&ldrThread, NULL, ldr_initandrun, (void *) &sconf.ldr);
	// pthread_create(&pwmThread, NULL, pwm, (void *) &sconf.pwm);

	/* Main loop */
	for(;;)
	{
		sem_wait();
	}

	return 0;
}
