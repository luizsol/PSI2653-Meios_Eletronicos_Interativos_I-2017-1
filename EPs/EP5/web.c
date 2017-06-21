/* -----------------------------------------------------------------------------
 * Title:       web
 * File:        web.c
 * Author:      Gabriel Crabbé, Tiago Amano
 * Version:     0.0 (2017-06-18)
 * Date:        2017-06-18
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

#include "lumiar.h"
#include "queue.h"


struct queue requestQueue;


/**
 * Gera path de tamanho até PATH_MAX (incluindo o '\0' final) completo a partir
 * de um path base e de um path relativo.
 *
 * @param  basepath path base.
 * @param  relpath  path relativo.
 * @param  fullpath ponteiro para o path. Deve ter tamanho mínimo PATH_MAX.
 * @return          0 em caso de sucesso, não-nulo caso contrário.
 */
int composePath(const char *basepath, const char *relpath, char *fullpath)
{
	if(basepath == NULL || relpath == NULL || fullpath == NULL)
		return -1;
	strcpy(fullpath, basepath);

	/* Multiple slashes in sequence are not a problem */
	strcat(fullpath, relpath);
	printf("full2 %s\n", fullpath);
	printf("full3 %d\n", strlen(fullpath));
	for(int i = 0; i < strlen(fullpath); i++)
		printf("full4 %d\n", fullpath[i]);
	return 0;
}


/**
 * Decodifica a mensagem HTTP recebida e identifica os diferentes trechos desta.
 *
 * @param  req a struct request da mensagem recebida.
 * @return     0 em caso de sucesso, não-nulo caso contrário.
 */
int parseRequest(struct request *req)
{
	req->cmd  = strtok(req->msg, " ");
	req->path = strtok(NULL, " ");
	req->http = strtok(NULL, "\n");
	return 0;
}


/**
 * Constrói a resposta do servidor a partir dos parâmetros da request.
 *
 * @param  req a struct request da mensagem recebida.
 * @param  res a struct response da mensagem a enviar.
 * @return     0 em caso de sucesso, não-nulo caso contrário.
 */
int buildResponse(struct webDriver *d, struct request *req, struct response *res)
{
	sem_wait(&d->mutex);
	int i, rescode;
	struct stat statf;
	FILE *f;
	char state[11];
	char mode[11];
	char value[11];
	char luminosity[11];

	composePath(res->basePath, "/index.html", res->fullPath);
	res->http = res->hdr;

	/* Parse request */
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
		sem_post(&d->mutex);
		return strlen(res->hdr);
	}
	else
	{
		//Last Modified:
		res->lastmod = res->server + strlen(res->server);
		// Chamada de sistema para obter
		//as informações sobre o arquivo
		stat(res->fullPath, &statf);
		printf("%s ----\n %s ----\n %s\n", res->basePath, res->fullPath, req->path);

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
				d->current->state = LUMIAR_STATE_ON;
			else if(strcmp(aux, "Standby") == 0)
				d->current->state = LUMIAR_STATE_STANDBY;

			// Verifica e seta o modo de operação:
			strtok(NULL, "=");
			aux = strtok(NULL, "&");
			if(strcmp(aux, "Manual") == 0)
				d->current->mode = LUMIAR_MODE_MANUAL;
			else if(strcmp(aux, "Auto") == 0)
				d->current->mode = LUMIAR_MODE_AUTO;

			// Verifica e seta o valor de Intensidade:
			strtok(NULL, "=");
			aux = strtok(NULL, "\0");
			d->current->userValue = atoi(aux);

		}
		sem_post(&d->mutex);
		sem_wait(&d->mutex);
		// mandar o index.html atualizado
		f = fopen(res->fullPath, "r");
		i = strlen(res->hdr);

		if(d->current->state == LUMIAR_STATE_ON)
			sprintf(state, "     ON");
		else
			sprintf(state, "Standby");

		if(d->current->mode == LUMIAR_MODE_MANUAL)
			sprintf(mode, "     Manual");
		else
			sprintf(mode, "Automatico");

		sprintf(value, "%d", d->current->pwmValue);
		sprintf(luminosity, "%d", d->current->luminosity);

		if(f != NULL)
		{
			int j = fread(res->object, 1, statf.st_size,f);
			memcpy(&res->object[189], state, strlen(state));
			memcpy(&res->object[555], mode, strlen(mode));
			memcpy(&res->object[921], value, strlen(value));
			memcpy(&res->object[942], value, strlen(value));
			memcpy(&res->object[1039], value, strlen(value));
			memcpy(&res->object[1060], value, strlen(value));
			memcpy(&res->object[1211], luminosity, strlen(luminosity));
			memcpy(&res->object[1232], luminosity, strlen(luminosity));
			res->object[j] = '\0';
			i = strlen(res->hdr);
		}
	}
	if(f != NULL)
		fclose(f);
	sem_post(&d->mutex);
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

	strcpy(res.basePath, sdriver->config->base);
	for(;;)
	{
		// Receive
		E = removeQueue(&requestQueue);
		status = read(E, req.msg, sizeof(req.msg));
		printf("%s\n", req.msg);
		if(status < 0)
			perror("Error reading from TCP stream");
		else if(status > 0)
		{
			// Parse request
			parseRequest(&req);

			// Build response
			len = buildResponse(sdriver, &req, &res);

			printf("%s\n", res.hdr);
			status = write(E, res.hdr, len);
			if(status <= 0)
				perror("Error writing to TCP stream");

			// Close
			status = close(E);
			if(status)
				perror("Error closing socket");
		}
		else
			printf("Connection closed\n");
	}
	return NULL;
}


/**
 * Thread principal de servidor.
 */
void *webService(void *arg)
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
	saddr.sin_port        = htons(sdriver->config->port);
	status = bind(sd, (struct sockaddr *) &saddr, sizeof(saddr));
	if(status < 0)
	{
		perror("Error binding socket");
		exit(1);
	}

	// Listen
	status = listen(sd, 10);
	if(status)
	{
		perror("Error listening to socket");
		exit(1);
	}

	// Queue
	initQueue(&requestQueue, 10);
	// Create threads
	pthread_t myworkers[WORKER_THREADS];
	printf("Launching TCP worker threads\n");
	for(int i = 1; i <= WORKER_THREADS; i++)
		pthread_create(&myworkers[i], NULL, worker, (void *) sdriver);

	// Accept
	for(;;)
	{
		int size, newsd;
		struct sockaddr_in caddr;

		newsd = accept(sd,(struct sockaddr*) &caddr,(socklen_t *) &size);
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
int initDriver(struct webDriver *d, struct lumiarState *s, struct webConfig *c)
{
	d->config = c;
	sem_init(&d->mutex, 0, 1);

	d->current = s;
	d->current->state = LUMIAR_STATE_DEFAULT;
	d->current->mode = LUMIAR_MODE_DEFAULT;
	d->current->userValue = LUMIAR_VALUE_DEFAULT;
	d->current->luminosity = 0;

	return 0;
}
