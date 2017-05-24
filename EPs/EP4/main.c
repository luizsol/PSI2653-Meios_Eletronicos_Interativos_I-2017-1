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
#include "synchronized_queue.h"


#define DEFAULT_PORT 8080
#define DEFAULT_PATH "./"
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
		char rxbuffer[80];

		// Receive
		status = read(E, rxbuffer, sizeof(rxbuffer));
		if(status < 0)
			perror("Error reading from TCP stream");
		else if(status > 0)
			printf("%s\n", rxbuffer);
		else
		{
			printf("Connection closed\n");
			break;
		}

		// HTTP 1.0 response
		// status = write(E, rxbuffer, strlen(rxbuffer) + 1);

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
	FILE *ini;
	ini = fopen("server.ini", "a+");
	if(ini == NULL)
	{
		perror("Error opening server.ini");
		exit(1);
	}

	int s_port = getPort(ini);
	if(s_port < 0)
	{
		s_port = DEFAULT_PORT;
		fprintf(ini, "port=%d\n", s_port);
	}

	char path[256];
	if(getPath(ini, path) < 0)
	{
		path = "./";
		fprintf(ini, "path=%s\n", path);
	}

	// Bind
	int status;
	struct sockaddr_in serveraddr;

	serveraddr.sin_family      = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port        = htons(s_port);

	status = bind(sd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
	if(status < 0)
		perror("Error binding socket");

	// Listen
	status = listen(sd, QLEN);
	if(status)
	{
		perror("Error listening to socket");
		exit(1);
	}

	// Queue
	initQueue(&requestQueue);

	// Create threads
	pthread_t myworkers[NUM_WORKERS];
	printf("Launching worker threads\n");
	for(int i = 0; i < NUM_WORKERS; i++)
		pthread_create(&myworkers[i], NULL, (void *) worker, i + 1);

	// Accept
	for(;;)
	{
		struct sockaddr_in clientaddr;
		int size;
		int newsd = accept(sd, (struct sockaddr *) &clientaddr,
			(socklen_t *) &size);
		if(newsd < 0)
		{
			perror("Error accepting connection");
		}

		insertQueue(&requestQueue, newsd);
	}

	// Close
	status = close(sd);
	if(status)
		perror("Error closing socket");

}
