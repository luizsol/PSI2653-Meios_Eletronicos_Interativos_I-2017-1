/* --------------------------------------------------------------------------
 * Title:       synchronized_queue
 * File:        synchronized_queue.h
 * Author:      Gabriel Crabbé, Tiago Azevedo
 * Version:     1.0 (2017-05-02)
 * Date:        2017-05-23
 * Description: EP 2 de PSI2653.
 * -------------------------------------------------------------------------- */

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
int emptyQueue(struct queue *Q);
int fullQueue(struct queue *Q);
void insertQueue(struct queue *Q, int item);
int removeQueue(struct queue *Q);
