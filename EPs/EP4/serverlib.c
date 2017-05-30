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
#include "htmllib.h"

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

#define DEBUG 1

/* Path Splitter: divide um path em seus elementos
 *    ex: ./home/.././Downloads/diretorio/ => 
 *        {".", "home", "..", ".", "Downloads", "diretorio"}
 *    path   : o path a ser avaliado
 *    size: ponteiro para um inteiro que receberá o tamanho do retorno
 *    retorno: um array com as strings resultantes da divisão
 */
char** pathsplitter(char * path, int * size){
	if(DEBUG){
		puts("pathsplitter():start");
	}
	if(path == NULL || strlen(path) == 0){
		*size = 0;
		if(DEBUG){
			puts("pathsplitter():return");
		}
		return NULL;
	}
	if(strcmp(path, ".") == 0 || strcmp(path, "..") == 0){
		char ** result = malloc(sizeof(char*));
		result[0] = strdup(path);
		*size = 1;
		if(DEBUG){
			puts("pathsplitter():return");
		}
		return result;
	}
    char * nPath = strdup(path);
    char * buffer[strlen(path)]; //Just a placeholder
    char *token = strtok(nPath, "/");
    int i = 0;
    while(token){
    	puts(token);
    	buffer[i] = strdup(token);
    	i++;
    	token = strtok(NULL, "/");
    }
    free(nPath);
    if(i == 0){
    	*size = i;
    	if(DEBUG){
			puts("pathsplitter():return");
		}
    	return NULL;
    }
    if(DEBUG){
		printf("pathsplitter(): i = %d\n", i);
	}
    char ** result = malloc(i * sizeof(char*));

    for(int j = 0; j <= i; j++){
    	result[j] = buffer[j];
    }
    *size = i;
    if(DEBUG){
		puts("pathsplitter():return");
	}
    return result;
    
}

/* Compose Path: altera PATH
 *    oldpath: path original (ex: /home/jose/test
 *    path   : path para ser alterado (ex: "..", "/tmp", "prog1" )
 *             (imagine na forma: "cd ..", "cd /tmp", "cd prog1")
 *    newpath: path resultante da composicao de oldpath e path
 *             (ex: /home/jose, /tmp, /jome/jose/test/prog1)
 */
int composepath(char *oldpath, char *path, char *newpath){
	if(DEBUG){
		puts("composepath():start");
	}
	int oldpatharraysize;
	char ** oldpatharray = pathsplitter(oldpath, &oldpatharraysize);
	int patharraysize;
	char ** patharray = pathsplitter(path, &patharraysize);
	if(oldpatharray != NULL){
		for(int i = 0; i < oldpatharraysize; i++){
			strcat(newpath, oldpatharray[i]);
			strcat(newpath, "/");
		}
	}
	if(patharray != NULL){
		for(int i = 0; i < patharraysize; i++){
			strcat(newpath, patharray[i]);
			strcat(newpath, "/");
		}
	}
	newpath[strlen(newpath)-1] = '\0';
	if(DEBUG){
		puts("composepath():return");
	}
	return 0;
}


/* getcurrentdir()
 *      Retorna o diretorio corrente atual
 *      Entrada: buffer para string e tamanho deste buffer
 *      Saida: ponteiro para string
 */
char *getcurrentdir(char *path, int pathsize)
{
	if(DEBUG){
		puts("getcurrentdir():start");
	}
	char *p;

	p = getcwd(path,pathsize);
	if(p==NULL)
		perror("Erro na chamada getcwd");
	if(DEBUG){
		puts("getcurrentdir():return");
	}
	return(p);
}


/* append()
 */
void append(char *dest, int buffersize, char *src)
{
	if(DEBUG){
		puts("append():start");
	}
	int d;
	int i;

	d = strlen(dest);
	for(i=0; i<min((int) strlen(src),buffersize-1-d); i++)
		dest[d+i] = src[i];
	dest[d+i] = '\0';
	if(DEBUG){
		puts("append():return");
	}
}


/* Lista diretorio
 *    path: diretorio a ser listado
 *    buffer: buffer que contera' a string com a sequencia ASCII
 *            resultado da listagem do diretorio (finalizada por '\0'
 *    bufffersize: tamanho do buffer
 */
void lista_diretorio(char *path, char *buffer, int buffersize)
{
	if(DEBUG){
		puts("lista_diretorio():start");
	}
	DIR           * dirp;
	struct dirent * direntry;

	dirp = opendir(path);
	if(dirp ==NULL)
	{
		perror("ERRO: chamada opendir(): Erro na abertura do diretorio: ");
		snprintf(buffer,buffersize,"Erro na listagem diretorio!\n");
		if(DEBUG){
			puts("lista_diretorio():return");
		}
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
	if(DEBUG){
		puts("lista_diretorio():return");
	}
}

/* diretorioarquivo()
 *    Define se um determinado path existe e, em caso de
 *    existência, se ele aponta para um diretório (0), 
 *    arquivo (1) ou outro (2). Caso não exista retorna
 *    -1.
 *    Fonte: https://stackoverflow.com/questions/3543231/how-to-find-out-if-a-file-or-directory-exists
 */
int diretorioarquivo(char * path){
	if(DEBUG){
		printf("diretorioarquivo(%s):start\n", path);
	}
	struct stat info;

	if(lstat(path,&info) != 0) {
		if(DEBUG){
			printf("diretorioarquivo(%s):return\n (nao existe)\n", path);
		}
		return -1;
	}
	//so, it exists.

	if(S_ISDIR(info.st_mode)) {
		//it's a directory
		if(DEBUG){
			printf("diretorioarquivo(%s):return\n (existe: diretorio)\n", path);
		}
		return 0;
	} else if(S_ISREG(info.st_mode)) {
		//it's a file
		if(DEBUG){
			printf("diretorioarquivo(%s):return\n (existe: arquivo)\n", path);
		}
		return 1;
	}
	if(DEBUG){
		printf("diretorioarquivo(%s):return\n (existe: outro)\n", path);
	}
	return 2;

}


/* transferfile()
 *    Realiza a leitura do conteudo de um arquivo, identificado
 *    por seu caminho (path), transferindo seu conteudo para
 *    outro arquivo ou socket identificado pelo descritor "outfd"
 */
int transferfile(char *path, int output_fd)
{
	if(DEBUG){
		puts("transferfile():start");
	}
	int          input_fd;     // input file descriptor
	int          status;
	int          n;
	char         buffer[BUFFERSIZE];
	struct stat  statp;

	input_fd = open(path,O_RDONLY);
	if (input_fd < 0)
	{
		perror("ERRO chamada open(): Erro na abertura do arquivo: ");
		if(DEBUG){
			puts("transferfile():return");
		}
		return(-1);
	}

	// obtem tamanho do arquivo
	status = fstat(input_fd,&statp);
	if (status != 0)
	{
		perror("ERRO chamada stat(): Erro no acesso ao arquivo: ");
		status = close(input_fd);
		if(DEBUG){
			puts("transferfile():return");
		}
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
			if(DEBUG){
				puts("transferfile():return");
			}
			return(-1);
		}
		write(output_fd,buffer,n);
	}
	while(n>0);

	status = close(input_fd);
	if (status == -1)
	{
		perror("ERRO: chamada close(): Erro no fechamento do arquivo: " );
		if(DEBUG){
			puts("transferfile():return");
		}
		return(-1);
	}
	if(DEBUG){
		puts("transferfile():return");
	}
	return statp.st_size;
}


/* parseini()
*/
int parseini(struct config *c) // lê as configurações relacionadas a porta e endereço base do servidor salvas no arquivo server.ini
{
	if(DEBUG){
		puts("parseini():start");
	}
	FILE *f;
	f = fopen("./server.ini", "r");
	if(f == NULL){
		if(DEBUG){
			puts("parseini():return");
		}
		return -1;
	}

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
	if(DEBUG){
		puts("parseini():return");
	}
	return 0;
}


/* parseRequest()
*/
int parseRequest(struct request *req) //decodifica a mensagem HTTP recebida e salva nos parâmetros respectivo do request
{
	if(DEBUG){
		puts("parseRequest():start");
	}
	req->cmd  = strtok(req->msg, " "); // Command (GET)
	req->path = strtok(NULL, " "); // Path
	req->http = strtok(NULL, "\n"); // HTTP
	if(DEBUG){
		puts("parseRequest():return");
	}
	return 0;
}

/* buildResponse()
*/
int buildResponse(struct request *req, struct response *res)
{
	if(DEBUG){
		puts("buildResponse():start");
	}
	int i;
	int rescode;
	struct stat statf;
	FILE *f = NULL;
	res->path[0] = '\0';
	composepath(res->base, req->path, res->path);
	res->http = res->msg;

	//análise de msgs de erro

	//HTTP Header:
	if (strncmp(req->cmd, "GET", 3) != 0)
	{
		rescode = 400;
		strcpy(res->http, "HTTP/1.0 400 Bad Request\r\n");
		if(DEBUG){
			puts("buildResponse():Bad request");
		}
	}
	else 	if(strncmp(req->http, "HTTP/1.0", 8) != 0)// && strncmp(req->http, "HTTP/1.1", 8) != 0)
	{
		rescode = 505;
		strcpy(res->http, "HTTP/1.0 505 HTTP Version Not Supported\r\n");
		if(DEBUG){
			puts("buildResponse():Not Supported");
		}
	}
	else
	{
		//TODO: primeiro ver se é diretório:
		int tipo = diretorioarquivo(res->path);
		
		if(tipo == -1) {
			
			rescode = 404;
			strcpy(res->http, "HTTP/1.0 404 Not Found\r\n");

		} else {
			
			if(tipo == 0){ // res->path é diretorio:
				// Se sim, verificação se há um arquivo index.html no diretorio
				composepath(res->path, "/index.html", res->pathindex);
				int existe = diretorioarquivo(res->pathindex);

				if(existe != 1) {
					// Se não houver, manda a listagem dos diretorios
					rescode = 201;
					perror("Error opening file");
				} else if(existe == 1){
					// Se houver, manda o index.html
					f = fopen(res->pathindex, "r");
					rescode = 200;
					strcpy(res->path, res->pathindex);
				}
			} else if(tipo == 1) { //res->path é arquivo:
				// Verificação do tipo (extensão) do arquivo:
				strncpy(res->ext, &res->path[strlen(res->path) - 6], 6);
				res->endext = strtok(res->ext,".");
				res->endext = strtok(NULL,"");

				if(strncmp(res->endext, "html", 4) == 0){
					rescode = 200;
					f = fopen(res->path, "r"); 
				}
				else if(strncmp(res->endext, "txt", 3) == 0){
					rescode = 202;
					f = fopen(res->path, "r"); 
				}
				else if(strncmp(res->endext, "jpg", 3) == 0){
					rescode = 203;
					f = fopen(res->path, "r"); 
				}
				else if(strncmp(res->endext, "png", 3) == 0){
					rescode = 204;
					f = fopen(res->path, "r"); 
				}
			}
			strcpy(res->http, "HTTP/1.0 200 OK\r\n");
		}
	}

	
	//Date:
	res->date = res->http + strlen(res->http);

	time_t rawtime;
	struct tm *servertime;
	time(&rawtime);
	servertime = localtime(&rawtime);
	strftime(res->date, 90, "Date: %a, %d %b %Y %T %g\r\n", servertime);

	//Server:
	res->server = res->date + strlen(res->date);

	sprintf(res->server, "Server: MEI/1.0.0 (Unix)\r\n");

	if(rescode == 400 || rescode == 505 || rescode == 404){
		puts("Entrou no if");

		//fclose(f); //FIXME: segmentation fault nessa linha
		if(DEBUG){
			puts("buildResponse():return");
		}
		return strlen(res->msg);
	} else {		//Last Modified:
		puts("Entrou no else");
		res->lastmod = res->server + strlen(res->server);
		if(rescode == 201){		
			// Se precisar mandar a listagem dos diretorios, lastmod = localtime
			strftime(res->lastmod, 90, "Last-Modified: %a, %d %b %Y\r\n", servertime);
			res->type = res->lastmod + strlen(res->lastmod);
			sprintf(res->type, "Content-Type: text/html\r\n\r\n");
			generatedirhtml(res->path, res->http);
			if(DEBUG){
				puts("buildResponse():return");
			}
			return strlen(res->msg);
		} else {		// Se não, normal:
			stat(res->path, &statf);
			servertime = gmtime(&statf.st_mtime);
			strftime(res->lastmod , 90, "Last-Modified: %a, %d %b %Y\r\n",
				servertime);

				//Content Length:
			res->length = res->lastmod + strlen(res->lastmod);

			sprintf(res->length, "Content-Length: %d\r\n", (int) statf.st_size);


			res->type = res->length + strlen(res->length);

			//Content Type:
			if(rescode == 200)
				sprintf(res->type, "Content-Type: text/html\r\n\r\n");
			else if(rescode == 202)
				sprintf(res->type, "Content-Type: text\r\n\r\n");
			else if(rescode == 203)
			{
				sprintf(res->type, "Content-Type: image/jpeg\r\nContent-Transfer-Encoding: binary\r\n\r\n");
				f = fopen(res->path, "rb");
			}
			else if(rescode == 204)
			{
				sprintf(res->type, "Content-Type: image/png\r\nContent-Transfer-Encoding: binary\r\n\r\n");
				f = fopen(res->path, "rb");
			}

			res->object = res->type + strlen(res->type);
				//Lê arquivo a ser enviado
			i = strlen(res->msg);
			if(f != NULL){
				int j = fread(res->object, 1, statf.st_size, f);
				if(rescode == 200 || rescode == 202)
				{
					res->object[j] = '\0';
					i = strlen(res->msg);
				}
				else
					i += j;
			}
		}
	}
	if(DEBUG){
		puts("buildResponse():return");
	}
	if(f != NULL){
		fclose(f);
	}
	return i;
}
