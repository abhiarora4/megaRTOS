/*
 * priorityQueue.c
 *
 *  Created on: 09-Feb-2016
 *      Author: abhiarora
 */
#include "priorityQueue.h"
#include "queue.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * If two elements have the same priority,
 * they are served according to their order in the queue.
 */

#define extractPriority(pq) ((priorityQueueElement_t *)(pq->data))->priority
#define extractData(pq) ((priorityQueueElement_t *)(pq->data))->data

int priorityQueEnque(priorityQueue_t *pq, void *enq, uint8_t priority)
{
	uint16_t front = (pq->front + 1) % pq->len;
	if (front == pq->rear)
		return -1;
	if (!enq)
		return -1;

	int i, j;
	for (i = pq->rear; i != pq->front; (i = (i + 1) % pq->len))
		if (*((uint8_t *)extractPriority(pq) + (pq->elemSize * i)) < priority) break;

	i -= pq->front;//2 => rear data=>1,2,3,4,5,6,7,9,10,11,__FRONT__

	for (j = 1; j <= i; j++) {
		memcpy(extractData(pq) + (pq->elemSize * (front - j)),
			extractData(pq) + (pq->elemSize * (pq->front - j)), sizeof(pq->elemSize));
		memcpy(extractPriority(pq) + pq->elemSize*(front - j),
			extractPriority(pq) + pq->elemSize*(pq->front - j), sizeof(uint8_t));
	}

	memcpy(extractData(pq) + (pq->elemSize * (pq->front - i)), enq, sizeof(pq->elemSize));
	memcpy(extractPriority(pq) + (pq->elemSize * (pq->front - i)), &priority, sizeof(uint8_t));

	pq->front = front;
	return 0;
}

int priorityQueDeque(priorityQueue_t *pq, void *deq, uint8_t priority, bool peek)
{
	if (pq->front == pq->rear)
		return -1;

	if (!deq)
		return -1;

	memcpy(deq, extractData(pq) + (pq->rear * pq->elemSize), pq->elemSize);

	if (!peek)
		pq->rear = (pq->rear + 1) % pq->len;
	return 0;
}

#ifdef __MAIN__

int main()
{
	return 0;
}

#endif
