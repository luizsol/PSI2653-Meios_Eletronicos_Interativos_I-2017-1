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


#define DEFAULT_LUMIAR_STATE 0
#define DEFAULT_LUMIAR_MODE 1
#define DEFAULT_LUMIAR_VALUE 50


#define ON 1
#define STANDBY 0
#define	AUTO 1
#define MANUAL 0


#define WORKER_THREADS 3
#define BUFFERSIZE 1000000


struct request
{
	char msg[BUFFERSIZE];// msg recebida do client web
	char *cmd; // command enviado
	char *path; // url recebido
	char *http; // versão do HTTP
};

struct response
{
	char msg[BUFFERSIZE]; // msg a ser enviada
	char base[PATH_MAX]; // diretório base do servidor
	char path[PATH_MAX]; // path completo
	char pathindex[PATH_MAX];
	char *http; // versão do HTTP
	char *date; // Date
	char *server; // Server name
	char *lastmod; // Last Modified
	char *length; // Content Length
	char *type; // Content Type
	char *object; //arquivo a ser enviado
};


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
	struct webConfig *config;
	sem_t mutex;
};


/* Protótipos de funções */
int initDriver(struct webDriver *d, struct webConfig *c);

int composepath(char *oldpath, char *path, char *newpath);
char *getcurrentdir(char *path, int pathsize);
int parseRequest(struct request *req);
int buildResponse(struct request *req, struct response *res);

void *webserver(void *arg);


#endif /* WEB_H */
