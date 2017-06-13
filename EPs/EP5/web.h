/* -----------------------------------------------------------------------------
 * Title:       web
 * File:        web.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-12)
 * Date:        2017-06-12
 * Description: Eexercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef WEB_H
#define WEB_H


#include <semaphore.h>

#include "lumiarlib.h"


#define WORKER_THREADS 3


/**
 * Struct de configuração do webserver.
 */
struct webConfig
{
	char base[PATH_MAX];
	unsigned short port;
};


/**
 * Struct de controle do webserver.
 */
struct webDriver
{
	struct lumiarState current;
	struct lumiarState command;
	struct webConfig *config;
	sem_t mutex;
};


/* Protótipos de funções */
int initDriver(struct webDriver *d, struct webConfig *c);


#endif /* WEB_H */
