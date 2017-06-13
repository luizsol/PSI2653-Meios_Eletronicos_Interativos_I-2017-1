/* -----------------------------------------------------------------------------
 * Title:       queue
 * File:        queue.c
 * Author:      Gabriel Crabbé, Tiago Azevedo
 * Version:     1.0 (2017-05-24)
 * Date:        2017-06-05
 * Description: EP 2 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#include "queue.h"


void initQueue(struct queue *Q, int qsize)
{
	// Struct init
	Q->first   = 0;
	Q->last    = 0;
	Q->nItems  = 0;
	Q->maxsize = qsize;

	// Memory allocation
	Q->buffer = (int *) malloc(sizeof(int) * qsize);

	// Semaphore init
	sem_init(&Q->freeItems, 0, qsize);
	sem_init(&Q->fullItems, 0, 0);
	sem_init(&Q->mutex, 0, 1);
}


int isQueueEmpty(struct queue *Q)
{
	int res = 0;
	sem_wait(&Q->mutex);
	res = (Q->nItems ? 0 : 1);
	sem_post(&Q->mutex);
	return res;
}


int isQueueFull(struct queue *Q)
{
	int res = 0;
	sem_wait(&Q->mutex);
	res = (Q->nItems == Q->maxsize ? 1 : 0);
	sem_post(&Q->mutex);
	return res;
}


void insertQueue(struct queue *Q, int item)
{
	sem_wait(&Q->freeItems);
	sem_wait(&Q->mutex);

	Q->buffer[Q->last] = item;

	Q->nItems++;
	if(++Q->last >= Q->maxsize)
		Q->last = 0;

	sem_post(&Q->mutex);
	sem_post(&Q->fullItems);
}


int removeQueue(struct queue *Q)
{
	int item;
	sem_wait(&Q->fullItems);
	sem_wait(&Q->mutex);

	item = Q->buffer[Q->first];

	Q->nItems--;
	if(++Q->first >= Q->maxsize)
		Q->first = 0;

	sem_post(&Q->mutex);
	sem_post(&Q->freeItems);
	return item;
}


void deinitQueue(struct queue *Q)
{
	// Memory deallocation
	free((void *) Q->buffer);
}
