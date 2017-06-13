/* -----------------------------------------------------------------------------
 * Title:       web
 * File:        web.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-06-12)
 * Date:        2017-06-12
 * Description: Exercício 5 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>

#include "web.h"
#include "queue.h"


struct queue requestQueue;


#define min(x,y) (((x)<(y))?(x):(y))

#define DEBUG 1

/* Path Splitter: divide um path em seus elementos
 *    ex: ./home/.././Downloads/diretorio/ =>
 *        {".", "home", "..", ".", "Downloads", "diretorio"}
 *    path   : o path a ser avaliado
 *    size: ponteiro para um inteiro que receberá o tamanho do retorno
 *    retorno: um array com as strings resultantes da divisão
 */
char** pathsplitter(char * path, int * size)
{
	if(path == NULL || strlen(path) == 0)
	{
		*size = 0;
		return NULL;
	}
	if(strcmp(path, ".") == 0 || strcmp(path, "..") == 0)
	{
		char ** result = malloc(sizeof(char*));
		result[0] = strdup(path);
		*size = 1;
		return result;
	}
  char * nPath = strdup(path);
  char * buffer[strlen(path)]; //Just a placeholder
  char *token = strtok(nPath, "/");
  int i = 0;
  while(token)
	{
  	buffer[i] = strdup(token);
  	i++;
  	token = strtok(NULL, "/");
  }
  free(nPath);
  if(i == 0)
	{
  	*size = i;
  	return NULL;
  }
  char ** result = malloc(i * sizeof(char*));

  for(int j = 0; j < i; j++)
	{
  	result[j] = buffer[j];
  }
  *size = i;
  return result;
}

/* Compose Path: altera PATH
 *    oldpath: path original (ex: /home/jose/test
 *    path   : path para ser alterado (ex: "..", "/tmp", "prog1" )
 *             (imagine na forma: "cd ..", "cd /tmp", "cd prog1")
 *    newpath: path resultante da composicao de oldpath e path
 *             (ex: /home/jose, /tmp, /jome/jose/test/prog1)
 */
int composepath(char *oldpath, char *path, char *newpath)
{
	int oldpatharraysize;
	char ** oldpatharray = pathsplitter(oldpath, &oldpatharraysize);
	int patharraysize;
	char ** patharray = pathsplitter(path, &patharraysize);
	if(oldpatharray != NULL)
	{
		for(int i = 0; i < oldpatharraysize; i++)
		{
			strcat(newpath, oldpatharray[i]);
			strcat(newpath, "/");
		}
	}
	if(patharray != NULL)
	{
		for(int i = 0; i < patharraysize; i++)
		{
			strcat(newpath, patharray[i]);
			strcat(newpath, "/");
		}
	}
	newpath[strlen(newpath)-1] = '\0';
	return 0;
}


/* parseRequest()
decodifica a mensagem HTTP recebida
e salva nos parâmetros respectivos do request
*/
int parseRequest(struct request *req)
{
	req->cmd  = strtok(req->msg, " "); // Command (GET)
	req->path = strtok(NULL, " "); // Path
	req->http = strtok(NULL, "\n"); // HTTP
	return 0;
}

/* buildResponse()
Constrói a resposta do servidor
a partir dos parâmetros do request

*/
int buildResponse(struct request *req, struct response *res)
{
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
	}
	else if(strncmp(req->http, "HTTP/1.0", 8) != 0)
	{
		rescode = 505;
		strcpy(res->http,
		"HTTP/1.0 505 HTTP Version Not Supported\r\n");
	}
	else if(strncmp(req->path, "/", 1) != 0 && strncmp(req->path,
		"/index.html", 11) != 0)
	{
		rescode = 404;
		strcpy(res->http,	"HTTP/1.0 404 Not Found\r\n");
	}
	else
	{
		if(strpbrk(req->path, "?") == NULL)
		{
			rescode = 200;
			strcpy(res->http,	"HTTP/1.0 200 OK\r\n");
		}
		else
		{
			rescode = 201;
			strcpy(res->http,	"HTTP/1.0 200 OK\r\n");
		}
	}

	//Date:
	res->date = res->http + strlen(res->http);
	time_t rawtime;
	struct tm *servertime;
	rawtime = time(NULL);
	servertime = gmtime(&rawtime);
	strftime(res->date, 90, "Date: %a, %d %b %Y %T %Z\r\n", servertime);
	//Server:
	res->server = res->date + strlen(res->date);
	sprintf(res->server, "Server: MEI/1.0.0 (Unix)\r\n");


	if(rescode == 400 || rescode == 505 || rescode == 404)
	{
		return strlen(res->msg);
	}
	else
	{
		//Last Modified:
		res->lastmod = res->server + strlen(res->server);
		composepath(res->path, "index.html",res->pathindex);
		// Chamada de sistema para obter
		//as informações sobre o arquivo
		stat(res->path, &statf);
		//apontado por res->path e
		//armazena as informações em statf
		servertime = gmtime(&statf.st_mtime);
		strftime(res->lastmod , 90, "Last-Modified: %a, %d %b %Y\r\n",
			servertime);

		//Content Length:
		res->length = res->lastmod + strlen(res->lastmod);
		sprintf(res->length, "Content-Length: %d\r\n",(int) statf.st_size);

		//Content Type:
		res->type = res->length + strlen(res->length);
		sprintf(res->type, "Content-Type: text/html\r\n\r\n");

		res->object = res->type + strlen(res->type);
		if(rescode == 201)
		{	// pegar as info
			char *aux;

			// Verifica e seta o estado:
			strtok(req->path, "=");
			aux = strtok(NULL, "&");
			if(strcmp(aux, "ON") == 0)
			  sdriver.current.state = ON;
			else if(strcmp(aux, "Standby") == 0)
			  sdriver.current.state = STANDBY;

			// Verifica e seta o modo de operação:
			strtok(NULL, "=");
			aux = strtok(NULL, "&");
			if(strcmp(aux, "Manual") == 0)
			  sdriver.current.mode = MANUAL;
			else if(strcmp(aux, "Auto") == 0)
			  sdriver.current.mode = AUTO;

			// Verifica e seta o valor de luminosidade:
			strtok(NULL, "=");
			aux = strtok(NULL, "\0");
			sdriver.current.value = atoi(aux);
		}
		if(rescode == 200)
		{
			if(strncmp(req->path, "/", 1) == 0)
				composepath(res->path, "/index.html", res->path);
		}
		// mandar o index.html atualizado
		f = fopen(res->path, "r");
		i = strlen(res->msg);
		if(f != NULL)
		{
			int j = fread(res->object, 1, statf.st_size,f);
			res->object[j] = '\0';
			i = strlen(res->msg);
		}
	}
	if(f != NULL)
		fclose(f);
	return i;
}


/**
 * Thread para processamento de request TCP ao servidor.
 */
void *worker(void *arg)
{
	struct webDriver *sdriver = (struct webDriver *) arg;
	int E, len, status;
	struct request  req;
	struct response res;

	strcpy(res.base, sdriver->config.base);

	for(;;)
	{
		// Receive
		E = removeQueue(&requestQueue);
		status = read(E, req.msg, sizeof(req.msg));
		if(status < 0)
			perror("Error reading from TCP stream");
		else if(status > 0)
		{
			sem_wait(sdriver.mutex);
			printf("%s\n", req.msg);
			// Parse request
			parseRequest(&req);

			// Build response
			len = buildResponse(&req, &res);

			printf("%s\n", res.msg);
			status = write(E, res.msg, len);
			if(status <= 0)
				perror("Error writing to TCP stream");

			// Close
			status = close(E);
			if(status)
				perror("Error closing socket");
			sem_post(sdriver.mutex);
		}
		else
			printf("Connection closed\n");
	}
	return NULL;
}


/**
 * Thread principal de servidor.
 */
void *webserver(void *arg)
{
	// Socket descriptor
	int sd = socket(PF_INET, SOCK_STREAM, 0);
	if(sd == -1)
	{
		perror("Error getting socket descriptor");
		exit(1);
	}

	// Get config
	int status;
	struct webDriver *sdriver = (struct webDriver *) arg;
	struct sockaddr_in saddr;

	// Bind
	saddr.sin_family      = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port        = htons(svdriver->config.port);

	status = bind(sd, (struct sockaddr *) &saddr, sizeof(saddr));
	if(status < 0)
	{
		perror("Error binding socket");
		exit(1);
	}

	// Listen
	status = listen(sd, QUEUE_LENGTH);
	if(status)
	{
		perror("Error listening to socket");
		exit(1);
	}

	// Queue
	initQueue(&requestQueue, 10);

	// Create threads
	pthread_t myworkers[WORKER_THREADS];
	printf("Launching worker threads\n");
	for(int i = 1; i <= WORKER_THREADS; i++)
		pthread_create(&myworkers[i], NULL, worker, (void *) sdriver);

	// Accept
	for(;;)
	{
		int size, newsd;
		struct sockaddr_in caddr;

		newsd=accept(sd,(struct sockaddr*) &caddr,(socklen_t *) &size);
		if(newsd < 0)
			perror("Error accepting connection");

		insertQueue(&requestQueue, newsd);
	}

	// Close
	status = close(sd);
	if(status)
		perror("Error closing socket");
}

/**
 * Inicializa a struct de controle do webserver.
 */
int initDriver(struct webDriver *d, struct webConfig *c)
{
	d->config = c;
	d->current.state = DEFAULT_LUMIAR_STATE;
	d->current.mode = DEFAULT_LUMIAR_MODE;
	d->current.value = DEFAULT_LUMIAR_VALUE;
	sem_init(&d->mutex, 0, 1);

	return 0;
}