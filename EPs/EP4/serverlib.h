/* --------------------------------------------------------------------------
 * Title:       serverlib
 * File:        serverlib.h
 * Author:      Gabriel Crabbé
 * Version:     0.0
 * Date:        2017-05-23
 * Description: EP 4 de PSI2653.
 * -------------------------------------------------------------------------- */

#ifndef SERVERLIB_H
#define SERVERLIB_H

int composepath(char *oldpath, char *path, char *newpath);
char * getcurrentdir(char *path, int pathsize);
void append(char *dest, int buffersize, char *src);
void lista_diretorio(char *path, char *buffer, int buffersize);
int transferfile(char *path, int output_fd);

#endif
