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
    char ** result = malloc(i * sizeof(char*));

    for(int j = 0; j < i; j++){
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
		perror("ERRO: opendir(): Erro na abertura do diretorio: ");
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
 *    Fonte: 
goo.gl/K5A3xG
 */
int diretorioarquivo(char * path){
	if(DEBUG){
		printf("diretorioarquivo(%s):start\n", path);
	}
	struct stat info;

	if(lstat(path,&info) != 0) {
		if(DEBUG){
		printf("diretorioarquivo(%s):return\n(nao existe)\n",path);
		}
		return -1;
	}
	//so, it exists.

	if(S_ISDIR(info.st_mode)) {
		//it's a directory
		if(DEBUG){
	     printf("diretorioarquivo(%s):return\n(existe:diretorio)\n",path);
		}
		return 0;
	} else if(S_ISREG(info.st_mode)) {
		//it's a file
		if(DEBUG){
		printf("diretorioarquivo(%s):return\n(existe:arquivo)\n",path);
		}
		return 1;
	}
	if(DEBUG){
		printf("diretorioarquivo(%s):return\n(existe:outro)\n", path);
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
	//FIX provisorio para tamanhos maiores de arquivos
	char         *buffer = malloc(BUFFERSIZE*sizeof(char)); 
	struct stat  statp;

	input_fd = open(path,O_RDONLY);
	if (input_fd < 0)
	{
		perror("ERRO: open(): Erro na abertura do arquivo: ");
		if(DEBUG){
			puts("transferfile():return");
		}
		return(-1);
	}

	// obtem tamanho do arquivo
	status = fstat(input_fd,&statp);
	if (status != 0)
	{
		perror("ERRO: stat(): Erro no acesso ao arquivo: ");
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
			perror("ERRO: read(): Erro na leitura do arquivo: ");
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
		perror("ERRO: close(): Erro no fechamento do arquivo: " );
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
lê as configurações relacionadas a porta e
 endereço base do servidor salvas no arquivo server.ini
*/
int parseini(struct config *c) 
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
decodifica a mensagem HTTP recebida 
e salva nos parâmetros respectivos do request
*/
int parseRequest(struct request *req) 
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
Constrói a resposta do servidor
a partir dos parâmetros do request 

*/
int buildResponse(struct request *req, struct response *res)
{
	if(DEBUG){
		puts("buildResponse():start");
	}
	int i;
	int rescode;
	struct stat statf = { 0 };
	FILE *f = NULL;
	res->path[0] = '\0';
	composepath(res->base, req->path, res->path);
	res->http = res->msg;

	//Avaliando HTTP Header:
	if (strncmp(req->cmd, "GET", 3) != 0)
	{
		rescode = 400;
		strcpy(res->http, "HTTP/1.0 400 Bad Request\r\n");
		if(DEBUG){
			puts("buildResponse():Bad request");
		}
	}
	else if(strncmp(req->http, "HTTP/1.0", 8) != 0)
	{
		rescode = 505;
		strcpy(res->http, 
		"HTTP/1.0 505 HTTP Version Not Supported\r\n");

		if(DEBUG){
			puts("buildResponse():Not Supported");
		}
	}
	else //HTTP compatível
	{
		if(DEBUG){
			puts("buildResponse():Request Valido, prosseguindo");
		}
		//Determinando se o path é válido e seu tipo
		int tipo = diretorioarquivo(res->path); 

		if(tipo == -1) { //Path inexistente
			if(DEBUG){
				puts("buildResponse():Path inexistente");
			}
			rescode = 404;
			strcpy(res->http, "HTTP/1.0 404 Not Found\r\n");

		} else { //Path existe

			if(tipo == 0){ // res->path é diretorio:
			//Verificar se existe arquivo index.html no diretorio
				memset(res->pathindex,0,
				strlen(res->pathindex));
				
				puts("huehue");
				composepath(res->path,
				"/index.html",res->pathindex);

				int existe = diretorioarquivo(res->pathindex);

				if(existe != 1) { 
			//Não existe index.html, enviar a listagem do diretório
					rescode = 201;
					if(DEBUG){
				   puts("buildResponse():dir sem index.html");
					}
				} else if(existe == 1){ 
				//Existe index.html. Envia-lo
					rescode = 200;
					strcpy(res->path, res->pathindex);
					if(DEBUG){
				puts("buildResponse():index.html encontrado");
					}
				}
			} else if(tipo == 1) { //res->path é um arquivo:
				// Verificação do tipo (extensão) do arquivo:
				strncpy(res->ext,
				&res->path[strlen(res->path)-6],6);

				res->endext = strtok(res->ext,".");
				res->endext = strtok(NULL,"");

				if(strncmp(res->endext, "html", 4) == 0){
					rescode = 200;
					if(DEBUG){
					puts("buildResponse():Path para html");
					}
				}
				else if(strncmp(res->endext, "txt", 3) == 0){
					rescode = 202;
					if(DEBUG){
					puts("buildResponse():Path para txt");
					}
				}
				else if(strncmp(res->endext, "jpg", 3) == 0){
					rescode = 203;
					if(DEBUG){
					puts("buildResponse():Path para jpg");
					}
				}
				else if(strncmp(res->endext, "png", 3) == 0){
					rescode = 204;
					if(DEBUG){
					puts("buildResponse():Path para png");
					}
				}
			}
			strcpy(res->http, "HTTP/1.0 200 OK\r\n");
		}
	}



	//Date:
	res->date = res->http + strlen(res->http);
	puts("1");
	time_t rawtime;
	puts("2");
	struct tm *servertime;
	puts("3");
	rawtime = time(NULL);
	puts("4");
	servertime = gmtime(&rawtime);
	puts("5");
	strftime(res->date, 90, "Date: %a, %d %b %Y %T %Z\r\n", servertime);
	puts("6");
	//Server:
	res->server = res->date + strlen(res->date);
	puts("7");
	sprintf(res->server, "Server: MEI/1.0.0 (Unix)\r\n");

	if(DEBUG){
		puts("buildResponse(): Valores da resposta definidos");
	}

	if(rescode == 400 || rescode == 505 || rescode == 404){
		if(f != NULL){
			fclose(f);
		}
		if(DEBUG){
			puts("buildResponse():return");
		}
		return strlen(res->msg);
	} else { //Last Modified:
		res->lastmod = res->server + strlen(res->server);
		if(rescode == 201){	//Listando o diretório
		// Se precisar mandar a listagem dos diretorios,lastmod=gmtime
			strftime(res->lastmod, 90, 
			"Last-Modified: %a, %d %b %Y\r\n", servertime);
			res->type = res->lastmod + strlen(res->lastmod);
			sprintf(res->type, "Content-Type: text/html\r\n\r\n");
			generatedirhtml(res->path, res->http);
			if(DEBUG){
				puts("buildResponse():return");
			}
			return strlen(res->msg);
		} else { //Enviando algum arquivo
			if(DEBUG){
				puts("buildResponse():Preparo para envio");
			}
			// Se não, normal:

			// Chamada de sistema para obter 
			//as informações sobre o arquivo
			stat(res->path, &statf);
			//apontado por res->path e 
			//armazena as informações em statf
			servertime = gmtime(&statf.st_mtime);
			strftime(res->lastmod , 90,
			 "Last-Modified: %a, %d %b %Y\r\n",
				servertime);

			//Content Length:
			res->length = res->lastmod + strlen(res->lastmod);

			sprintf(res->length,
			"Content-Length: %d\r\n",(int) statf.st_size);


			res->type = res->length + strlen(res->length);

			//Content Type:
			if(rescode == 200){ //HTML
				sprintf(res->type, 
				"Content-Type: text/html\r\n\r\n");
				if(f != NULL){
					fclose(f);
				}
				if(DEBUG){
					puts("buildResponse():abrindo HTML");
				}
				f = fopen(res->path, "r");
			} else if(rescode == 202){ //TXT
				sprintf(res->type, "Content-Type: text\r\n\r\n");
				if(f != NULL){
					fclose(f);
				}
				if(DEBUG){
					puts("buildResponse():abrindo TXT");
				}
				f = fopen(res->path, "r");
			} else if(rescode == 203){ //JPEG
				sprintf(res->type, 
      "Content-Type: image/jpeg\r\nContent-Transfer-Encoding: binary\r\n\r\n");
				if(f != NULL){
					fclose(f);
				}
				if(DEBUG){
					puts("buildResponse():abrindo JPEG");
				}
				f = fopen(res->path, "rb");
			} else if(rescode == 204){ //PNG
				sprintf(res->type, 
      "Content-Type: image/png\r\nContent-Transfer-Encoding: binary\r\n\r\n");
				if(f != NULL){
					fclose(f);
				}
				if(DEBUG){
					puts("buildResponse():abrindo PNG");
				}
				f = fopen(res->path, "rb");
			}

			res->object = res->type + strlen(res->type);

			//Lê arquivo a ser enviado
			i = strlen(res->msg);
			if(DEBUG){
				puts("buildResponse():enviando dados");
			}
			if(f != NULL){
				int j = fread(res->object, 1,statf.st_size,f);
				//int j = fread(res->object,statf.st_size,1,f);
				if(rescode == 200 || rescode == 202){
					res->object[j] = '\0';
					i = strlen(res->msg);
				} else {
					i += j;
				}
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
