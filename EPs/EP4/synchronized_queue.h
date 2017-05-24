/* -----------------------------------------------------------------------------
 * Title:       queue
 * File:        queue.h
 * Author:      Gabriel Crabbé, Tiago Azevedo
 * Version:     1.0 (2017-05-24)
 * Date:        2017-05-24
 * Description: EP 2 de PSI2653.
 * -----------------------------------------------------------------------------
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <semaphore.h>


#define QUEUE_LENGTH 10


struct queue
{
	int buffer[QUEUE_LENGTH];
	int first, last;
	int nItems, maxsize;
	sem_t freeItems, fullItems, mutex;
};


void initQueue(struct queue *Q);
int isQueueEmpty(struct queue *Q);
int isQueueFull(struct queue *Q);
void insertQueue(struct queue *Q, int item);
int removeQueue(struct queue *Q);

#endif
