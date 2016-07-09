#include "queue.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

inline void reset(queue_t *q)
{
	q->rear = q->front = 0;
}

void reset(queue_t *que);

int enque(queue_t *que, void *enqData)
{
	if (que->front == que->len)
		return -1;

	if (!enqData)
		return -1;

	memcpy(que->data + (que->front * que->elemSize), enqData, que->elemSize);

	que->front++;
	return 0;
}

int deque(queue_t *que, void *deqData)
{
	if (que->front == que->rear) {
		reset(que);
		return -1;
	}

	if (deqData)
		memcpy(deqData, que->data + (que->rear * que->elemSize), que->elemSize);

	que->rear++;
	return 0;
}

bool isQueueEmpty(queue_t *que)
{
	if(que->front != que->rear)
		return false;

	reset(que);
	return true;
}

int queueGetUsedSpace(queue_t *que)
{
	return (que->front - que->rear);

}

int queueGetFreeSpace(queue_t *que)
{
	return (que->len - que->front);
}

#ifdef __MAIN__
QUEUE_DEF(queue, uint16_t, 20);

int main()
{
	uint16_t i=0;
	for(i = 0; i <25; i++) {
		if(genericQueue(&queue, (void *)&i) != 0)
			printf("Error in queuing -%d \n", i);
	}
	uint16_t data;
	for(i = 0; i <22; i++) {
		if(genericDequeue(&queue, (void *)&data) == 0)
			printf("Data - %u \n", data);
		else
			printf("Error in reading - %d\n", i);
	}
	return 0;
}

#endif
