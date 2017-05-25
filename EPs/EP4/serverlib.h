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

#define BUFFERSIZE 4096


struct config
{
	char base[PATH_MAX];
	unsigned short port;
};

struct request
{
	char msg[BUFFERSIZE];
	unsigned short port;
	char *cmd;
	char *path;
	char *http;
};

struct response
{
	char msg[BUFFERSIZE]; //msg a ser enviada
	unsigned short port;
	char *http; //parte da http da resposta
};


int composepath(char *oldpath, char *path, char *newpath);
char *getcurrentdir(char *path, int pathsize);
void append(char *dest, int buffersize, char *src);
void lista_diretorio(char *path, char *buffer, int buffersize);
int transferfile(char *path, int output_fd);
int parseini(struct config *c);
int parseRequest(struct request *req);
int buildResponse(struct request *req, struct response *res);

#endif
