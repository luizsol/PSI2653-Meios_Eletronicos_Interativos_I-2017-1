/* -----------------------------------------------------------------------------
 * Title:       main
 * File:        main.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

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
	struct lumiarState lstate;
	struct webDriver wdriver;
	initDriver(&wdriver, &(sconf.web));

	/* Create threads */
	pthread_t pwmThread, ldrThread, webThread;
	printf("Launching dedicated threads\n");
	pthread_create(&webThread, NULL, webService, (void *) &wdriver);
	pthread_create(&ldrThread, NULL, ldrService, (void *) &sconf.ldr);
	pthread_create(&pwmThread, NULL, pwmService, (void *) &sconf.pwm);

	/* Main loop */
	for(;;)
	{
		/* Get request from server */
		sem_wait(wdriver.mutex);
		if(memcmp(lstate, wdriver.current, sizeof(lstate)))
		{
			memcpy(lstate, wdriver.current, sizeof(lstate));
			sem_post(wdriver.mutex);
		}
		else
		{
			sem_post(wdriver.mutex);
		}


		/* Parse lumiarState */
		if(lstate.state == LUMIAR_STATE_ON)
		{
			if(lstate.mode == LUMIAR_MODE_AUTO)
			{
				lstate.luminosity = getLuminosity();
				setOperatingPoint(lstate.luminosity);
			}
		}
		else
		{
			sleep(1);
		}


	}

	return 0;
}
