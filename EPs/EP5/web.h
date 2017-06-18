/* -----------------------------------------------------------------------------
 * Title:       web
 * File:        web.h
 * Author:      Gabriel Crabbé, Tiago Amano
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
 * Description: Eexercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef WEB_H
#define WEB_H


#include <semaphore.h>

#include "lumiarlib.h"


#define WORKER_THREADS 1
#define BUFFERSIZE 4096


/**
 * Struct para mensagem recebida pelo webserver.
 *
 * Consiste num buffer de 4 KiB para a mensagem em si, e de ponteiros para os
 * diferentes segmentos desta.
 */
struct request
{
	char msg[BUFFERSIZE];     /* Request message */
	char *cmd;                /* Command */
	char *path;               /* Path */
	char *http;               /* HTTP version */
};


/**
 * Struct para mensagem enviada pelo webserver.
 *
 * Consiste num buffer de 4 KiB para o header da mensagem, e de ponteiros para
 * os diferentes segmentos deste.
 * Um ponteiro adicional aponta para o conteúdo a transmitir.
 */
struct response
{
	char hdr[BUFFERSIZE];     /* Response header */
	char basePath[PATH_MAX];  /* Base path */
	char fullPath[PATH_MAX];  /* Full path */
	char *http;               /* HTTP version */
	char *date;               /* Server date */
	char *server;             /* Server ID */
	char *lastmod;            /* Last modified */
	char *length;             /* Content length */
	char *type;               /* Content type */
	char *object;             /* Content pointer */
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

void *webService(void *arg);


#endif /* WEB_H */
