/* -----------------------------------------------------------------------------
 * Title:       main
 * File:        main.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-20)
 * Date:        2017-06-20
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
	/* UNCOMMENT LINES BELOW FOR DEPLOYMENT */
	// pthread_create(&ldrThread, NULL, ldrService, (void *) &sconf.ldr);
	// pthread_create(&pwmThread, NULL, pwmService, (void *) &sconf.pwm);

	/* Local flags and variables */
	int hasChanged = 0;
	int readLuminosity;
	const struct timespec slp =
	{
		tv_sec = 0;
		tv_nsec = 100000000L;  /* 100 ms */
	};


	/* Main loop */
	for(;;)
	{
		/* Get request from server */
		sem_wait(&(wdriver.mutex));
		if(memcmp(lstate, wdriver.current, sizeof(lstate)))
		{
			memcpy(lstate, wdriver.current, sizeof(lstate));
			sem_post(&(wdriver.mutex));
			hasChanged = 1;
		}
		else
		{
			sem_post(&(wdriver.mutex));
			hasChanged = 0;
		}

		/* Get luminosity */
		/* UNCOMMENT LINE BELOW FOR DEPLOYMENT */
		//readLuminosity = getLuminosity();
		/* DELETE LINE BELOW FOR DEPLOYMENT */
		readLuminosity = (int) time(NULL) % 101;
		if(lstate.luminosity != readLuminosity)
		{
			lstate.luminosity = readLuminosity;
			hasChanged = 1;
		}

		/* Send command */
		if(hasChanged)
		{
			if(lstate.state == LUMIAR_STATE_ON)
			{
				if(lstate.mode == LUMIAR_MODE_AUTO)
					lstate.pwmValue = LUMIAR_VALUE_MAX - readLuminosity;
				else
					lstate.pwmValue = lstate.userValue
			}
			else
			{
				lstate.pwmValue = LUMIAR_VALUE_MIN;
			}

			/* UNCOMMENT LINE BELOW FOR DEPLOYMENT */
			// setOperatingPoint(lstate.pwmValue);
		}

		/* Send back to server */
		if(hasChanged)
		{
			sem_wait(&(wdriver.mutex));
			memcpy(wdriver.current, lstate, sizeof(lstate));
			sem_post(&(wdriver.mutex));
		}

		/* Sleep */
		nanosleep(&slp, (struct timespec *) NULL);
	}

	return 0;
}
