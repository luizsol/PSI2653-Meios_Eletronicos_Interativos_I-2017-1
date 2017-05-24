/* -----------------------------------------------------------------------------
 * Title:       serverlib
 * File:        serverlib.h
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-05-24)
 * Date:        2017-05-24
 * Description: EP 4 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef SERVERLIB_H
#define SERVERLIB_H

#include <linux/limits.h>


struct config
{
	char base[PATH_MAX];
	uint16_t port;
};


int composepath(char *oldpath, char *path, char *newpath);
char *getcurrentdir(char *path, int pathsize);
void append(char *dest, int buffersize, char *src);
void lista_diretorio(char *path, char *buffer, int buffersize);
int transferfile(char *path, int output_fd);
int parseini(struct config *c);

#endif
