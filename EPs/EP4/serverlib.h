/* -----------------------------------------------------------------------------
 * Title:       serverlib
 * File:        serverlib.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-05-25)
 * Date:        2017-05-25
 * Description: EP 4 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef SERVERLIB_H
#define SERVERLIB_H


#include <linux/limits.h>

#define BUFFERSIZE 1000000


struct config
{

	char base[PATH_MAX]; // diretório base do servidor
	unsigned short port; // porta de conexão do servidor
};

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


int composepath(char *oldpath, char *path, char *newpath);
char *getcurrentdir(char *path, int pathsize);
void append(char *dest, int buffersize, char *src);
void lista_diretorio(char *path, char *buffer, int buffersize);
int transferfile(char *path, int output_fd);
int verifyExtension(char *path);
int parseini(struct config *c);
int parseRequest(struct request *req);
int buildResponse(struct request *req, struct response *res);

#endif
