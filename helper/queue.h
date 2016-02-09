#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	void * const data;
	uint16_t front;
	uint16_t rear;
	const uint16_t queLen;
	const uint16_t elemSize;
}queue_t;

#define QUEUE_DEF(n,t,s) t _que_##n[s]; \
	queue_t n = {.data = (void *)_que_##n, \
		.front = 0, \
		.rear = 0, \
		.elemSize = sizeof(t), \
		.qSize = s }


int enque(queue_t *, void *);
int deque(queue_t *, void *);

bool isQueueEmpty(queue_t *);
int queueGetFreeSpace(queue_t *);
int queueGetUsedSpace(queue_t *);


#endif
