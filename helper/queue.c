#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <stdbool.h>


int enque(queue_t *que, void *enqData)
{
	if (que->front == que->queLen)
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
		que->front = que->rear = 0;
		return -1;
	}

	if (!deqData)
		return -1;

	memcpy(deqData, que->data + (que->rear * que->elemSize), que->elemSize);
	que->rear++;
	return 0;
}

bool isQueueEmpty(queue_t *que)
{
	if(que->front != que->rear)
		return false;

	que->front = que->rear = 0;
	return true;
}

int queueGetUsedSpace(queue_t *que)
{
	return (que->front - que->rear);

}

int queueGetFreeSpace(queue_t *que)
{
	return (que->queLen - que->front);
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
