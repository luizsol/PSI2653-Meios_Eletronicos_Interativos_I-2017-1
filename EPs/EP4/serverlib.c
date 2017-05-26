/* -----------------------------------------------------------------------------
 * Title:       serverlib
 * File:        serverlib.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-05-25)
 * Date:        2017-05-25
 * Description: EP 4 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include "serverlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>
#include <fcntl.h>
#include <time.h>


#define min(x,y) (((x)<(y))?(x):(y))


/* Compose Path: altera PATH
 *    oldpath: path original (ex: /home/jose/test
 *    path   : path para ser alterado (ex: "..", "/tmp", "prog1" )
 *             (imagine na forma: "cd ..", "cd /tmp", "cd prog1")
 *    newpath: path resultante da composicao de oldpath e path
 *             (ex: /home/jose, /tmp, /jome/jose/test/prog1)
 */
int composepath(char *oldpath, char *path, char *newpath)
{
	char * olddir;
	char * oldbase;
	char * oldpath1;
	char * oldpath2;
	char * dir;
	char * base;
	char * path1;
	char * path2;
	int    status;

	oldpath1 = strdup(oldpath);
	oldpath2 = strdup(oldpath);
	olddir   = dirname(oldpath1);
	oldbase  = basename(oldpath2);

	path1 = strdup(path);
	path2 = strdup(path);
	dir   = dirname (path1);
	base  = basename(path2);

	strcpy(newpath,"/");

	if(path[0]=='/')
	{
		strcpy(newpath,path);
		status=0;
	}
	else if((strcmp(dir,".")==0) && (strcmp(base,".")==0))
	{
		strcpy(newpath,oldpath);
		status=0;
	}
	else if((strcmp(dir,".")==0) && (strcmp(base,"..")==0))
	{
		strcpy(newpath,olddir);
		status=0;
	}
	else if((strcmp(dir,".")==0) && (strcmp(oldbase,"/")==0))
	{
		strcpy(newpath,oldpath);
		strcat(newpath,path);
		status=0;
	}
	else if(strcmp(dir,".")==0)
	{
		strcpy(newpath,oldpath);
		strcat(newpath,"/");
		strcat(newpath,path);
		status=0;
	}
	else
	{
		strcpy(newpath,oldpath);
		status=-1;
	}
	free(oldpath1);
	free(oldpath2);
	free(path1);
	free(path2);
	return(status);
}


/* getcurrentdir()
 *      Retorna o diretorio corrente atual
 *      Entrada: buffer para string e tamanho deste buffer
 *      Saida: ponteiro para string
 */
char *getcurrentdir(char *path, int pathsize)
{
	char *p;

	p = getcwd(path,pathsize);
	if(p==NULL)
		perror("Erro na chamada getcwd");
	return(p);
}


/* append()
 */
void append(char *dest, int buffersize, char *src)
{
	int d;
	int i;

	d = strlen(dest);
	for(i=0; i<min(strlen(src),buffersize-1-d); i++)
		dest[d+i] = src[i];
	dest[d+i] = '\0';
}


/* Lista diretorio
 *    path: diretorio a ser listado
 *    buffer: buffer que contera' a string com a sequencia ASCII
 *            resultado da listagem do diretorio (finalizada por '\0'
 *    bufffersize: tamanho do buffer
 */
void lista_diretorio(char *path, char *buffer, int buffersize)
{
	DIR           * dirp;
	struct dirent * direntry;
	char            linha[80];
	int             i = 0;

	dirp = opendir(path);
	if(dirp ==NULL)
	{
		perror("ERRO: chamada opendir(): Erro na abertura do diretorio: ");
		snprintf(buffer,buffersize,"Erro na listagem diretorio!\n");
		return;
	}
	buffer[0]='\0';
	while(dirp)
	{
		// Enquanto nao chegar ao fim do diretorio, leia cada entrada
		direntry = readdir(dirp);
		if (direntry == NULL)
			// chegou ao fim
			break;
		else
		{
			// ler entrada
			append(buffer,buffersize,direntry->d_name);
			append(buffer,buffersize,"\n");
		}
	}
	closedir(dirp);
}


/* transferfile()
 *    Realiza a leitura do conteudo de um arquivo, identificado
 *    por seu caminho (path), transferindo seu conteudo para
 *    outro arquivo ou socket identificado pelo descritor "outfd"
 */
int transferfile(char *path, int output_fd)
{
	int          input_fd;     // input file descriptor
	int          status;
	int          n;
	char         buffer[BUFFERSIZE];
	char         str[10];
	struct stat  statp;

	input_fd = open(path,O_RDONLY);
	if (input_fd < 0)
	{
		perror("ERRO chamada open(): Erro na abertura do arquivo: ");
		return(-1);
	}

	// obtem tamanho do arquivo
	status = fstat(input_fd,&statp);
	if (status != 0)
	{
		perror("ERRO chamada stat(): Erro no acesso ao arquivo: ");
		status = close(input_fd);
		return(-1);
	}
	// sprintf(str,"SIZE=%d\n",statp.st_size);
	// write(output_fd,str,strlen(str));

	// le arquivo , por partes
	do
	{
		n = read(input_fd,buffer,BUFFERSIZE);
		if (n<0)
		{
			perror("ERRO: chamada read(): Erro na leitura do arquivo: ");
			status = close(input_fd);
			return(-1);
		}
		write(output_fd,buffer,n);
	}
	while(n>0);

	status = close(input_fd);
	if (status == -1)
	{
		perror("ERRO: chamada close(): Erro no fechamento do arquivo: " );
		return(-1);
	}
	return statp.st_size;
}


/* parseini()
*/
int parseini(struct config *c)
{
	FILE *f;
	f = fopen("server.ini", "r");
	if(f == NULL)
		return -1;

	char line[82];
	char *strp;

	for(;;)
	{
		strp = fgets(line, sizeof(line), f);
		if(strp == NULL)
			break;

		line[strcspn(line, "\n")] = '\0';
		strp = strtok(line, "=");

		if(!strcmp(line, "port"))
			sscanf(strtok(NULL, " "), "%hu", &c->port);
		else if(!strcmp(line, "base"))
			strcpy(c->base, strtok(NULL, " "));
	}

	return 0;
}


/* parseRequest()
*/
int parseRequest(struct request *req)
{
	req->cmd  = strtok(req->msg, " "); // Command
	req->path = strtok(NULL, " "); // Path
	req->http = strtok(NULL, "\n"); // HTTP

	return 0;
}

/* buildResponse()
*/
int buildResponse(struct request *req, struct response *res)
{
	int rescode;
	struct stat statf;
	FILE *f;
	//if(composepath(res->base, req->path, res->path) < 0)
	//	perror("Error creating object path");
	strcpy(res->path, res->base);
	strcat(res->path, req->path); //FIXME
	printf("%s\n", res->path);

	res->http = res->msg;
	if (strncmp(req->cmd, "GET", 3) != 0)
	{
		rescode = 400;
		strcpy(res->http, "HTTP/1.0 400 Bad Request\r\n");
	}
	else 	if(strncmp(req->http, "HTTP/1.0", 8) != 0)
	{
		rescode = 505;
		strcpy(res->http, "HTTP/1.0 505 HTTP Version Not Supported\r\n");
	}
	else
	{
		f = fopen(res->path, "r");
		if(f == NULL)
		{
			rescode = 404;
			strcpy(res->http, "HTTP/1.0 404 Not Found\r\n");
		}
		else	if(f != NULL)
		{
			stat(res->path, &statf);

			// Verifica se o arquivo em res->path é diretorio (1) ou não (0)
			if(S_ISDIR(statf.st_mode))
			{
				// se sim, verifica se há um arquivo index.html no diretorio
				fclose(f);
				strcpy(res->pathindex, res->path);
				strcat(res->pathindex, "/index.html"); //FIXME

				f = fopen(res->pathindex, "r");
				if(f == NULL)
				{
					// se não houver, manda a listagem dos diretorios
					rescode = 201;
					perror("Error opening file");
				}
				else if (f != NULL)
				{
					rescode = 200;
					strcpy(res->path, res->pathindex);
				}
			}
			else if(S_ISREG(statf.st_mode))
			{
				strncpy(res->ext, &res->path[strlen(res->path) - 6], 6);
				res->endext = strtok(res->ext,".");
				res->endext = strtok(NULL,"");
				if(strncmp(res->endext, "html", 4) == 0)
					rescode = 200;
				else if(strncmp(res->endext, "txt", 3) == 0)
					rescode = 202;
				else if(strncmp(res->endext, "png", 3) == 0 || strncmp(res->endext,
					"jpg", 3) == 0)
					rescode = 203;
			}
			strcpy(res->http, "HTTP/1.0 200 OK\r\n");
		}
	}
	res->date = res->http + strlen(res->http);

	time_t rawtime;
	struct tm *servertime;
	time(&rawtime);
	servertime = localtime(&rawtime);
	strftime(res->date, 90, "Date: %a, %d %b %Y %T %g\r\n", servertime);


	res->server = res->date + strlen(res->date);

	sprintf(res->server, "Server: MEI/1.0.0 (Unix)\r\n");

	if(rescode == 400 || rescode == 505 || rescode == 404)
		return 0;
	else
	{
		res->lastmod = res->server + strlen(res->server);
		if(rescode == 201)
			{
				strftime(res->lastmod, 90, "Last-Modified: %a, %d %b %Y\r\n", servertime);
				// gerar 	aqui a listagem dos diretorios
				return 0;
			}
		else
		{
			stat(res->path, &statf);
			servertime = gmtime(&statf.st_mtime);
			strftime(res->lastmod , 90, "Last-Modified: %a, %d %b %Y\r\n",
				servertime);


			res->length = res->lastmod + strlen(res->lastmod);

			sprintf(res->length, "Content-Length: %d\n", statf.st_size);


			res->type = res->length + strlen(res->length);

			fclose(f);
			if(rescode == 200)
			{
				sprintf(res->type, "Content-Type: text/html\r\n\r\n");
				f = fopen(res->path, "r");
			}
			else if(rescode == 202)
			{
				sprintf(res->type, "Content-Type: text\r\n\r\n");
				f = fopen(res->path, "r");
			}
			else if(rescode == 203)
			{
				sprintf(res->type, "Content-Type: image\r\n\r\n");
				f = fopen(res->path, "rb");
			}

			res->object = res->type + strlen(res->type);

			if(f != NULL)
				fread(res->object, 1, BUFFERSIZE, f); // FIXME
		}
	}

	return 0;
}
