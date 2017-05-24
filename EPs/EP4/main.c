/* -----------------------------------------------------------------------------
 * Title:       main
 * File:        main.c
 * Author:      Gabriel Crabbé
 * Version:     0.0 (2017-05-24)
 * Date:        2017-05-24
 * Description: Exercício 4 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "serverlib.h"
#include "queue.h"


#define DEFAULT_PORT 8080
#define DEFAULT_BASE "./"
#define WORKER_THREADS 3


struct queue requestQueue;


/* worker thread
*/
void worker(int id)
{
	for(;;)
	{
		int status;
		int E = removeQueue(&requestQueue);
		char rxbuffer[4096];

		// Receive
		status = read(E, rxbuffer, sizeof(rxbuffer));
		if(status < 0)
			perror("Error reading from TCP stream");
		else if(status > 0)
			printf("%s\n", rxbuffer);
		else
			printf("Connection closed\n");

		// decodificação Get - host - user - acce - http
		// separa o texto
		char *get=NULL;
		char *host=NULL;
		char *user=NULL;
		char *acce=NULL;
		char *http=NULL;
		// char *token=NULL;

		get=strtok(rxbuffer,"\n");
		printf("%s\n",get);
		host=strtok(NULL,"\n");
		printf("%s\n",host);
		user=strtok(NULL,"\n");
		printf("%s\n",user);
		acce=strtok(NULL,"\n");
		printf("%s\n",acce);

		http=strtok(get," ");
		http=strtok(NULL," ");
		http=strtok(NULL,"\n");
		printf("%s\n",http);

		host=strtok(host," ");
		host=strtok(NULL,"\n");
		printf("%s\n",host);

		user=strtok(user," ");
		user=strtok(NULL,"\n");
		printf("%s\n",user);

		acce=strtok(acce," ");
		acce=strtok(NULL,"\n");
		printf("%s\n",acce);

		// HTTP 1.0 response
		status = transferfile("index.html", E);
		if(status <= 0)
			perror("Error writing to TCP stream");

		// Close
		status = close(E);
		if(status)
			perror("Error closing socket");
	}
}


/* main
*/
int main()
{
	// Socket descriptor
	int sd = socket(PF_INET, SOCK_STREAM, 0);
	if(sd == -1)
	{
		perror("Error getting socket descriptor");
		exit(1);
	}

	// Parse .ini
	int status;
	struct config sconf;
	struct sockaddr_in saddr;

	if(parseini(&sconf) < 0)
	{
		sconf.port = DEFAULT_PORT;
		sconf.base = DEFAULT_BASE;
	}

	// Bind
	saddr.sin_family      = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port        = htons(sconf.port);

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
	initQueue(&requestQueue);

	// Create threads
	pthread_t myworkers[WORKER_THREADS];
	printf("Launching worker threads\n");
	for(int i = 0; i < WORKER_THREADS; i++)
		pthread_create(&myworkers[i], NULL, (void *) worker, i + 1);

	// Accept
	for(;;)
	{
		int size, newsd;
		struct sockaddr_in caddr;

		newsd = accept(sd, (struct sockaddr *) &caddr, (socklen_t *) &size);
		if(newsd < 0)
			perror("Error accepting connection");

		insertQueue(&requestQueue, newsd);
	}

	// Close
	status = close(sd);
	if(status)
		perror("Error closing socket");

}
