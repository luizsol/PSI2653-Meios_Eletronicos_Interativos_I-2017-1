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
void worker(struct config *sconf)
{
	int E, status;
	struct request  req;
	struct response res;

	printf("W0");
	strcpy(res.base, sconf->base);
	printf("W0b");

	for(;;)
	{
		// Receive
		E = removeQueue(&requestQueue);
		status = read(E, req.msg, sizeof(req.msg));
		if(status < 0)
			perror("Error reading from TCP stream");
		else if(status > 0)
			printf("%s\n", req.msg);
		else
			printf("Connection closed\n");

		// Parse request
		printf("W1");
		parseRequest(&req);
		printf("W2");

		// Build response
		buildResponse(&req, &res);
		printf("W3");

		// HTTP 1.0 response
		// status = transferfile("index.html", E);
		// if(status < 0)
		//	perror("Error writing to TCP stream");

		status = write(E, res.msg, strlen(res.msg) + 1);
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
		strcpy(sconf.base, DEFAULT_BASE);
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
	printf("M0");
	for(int i = 1; i <= WORKER_THREADS; i++)
		pthread_create(&myworkers[i], NULL, (void *) worker, &sconf);
	printf("M1");

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
