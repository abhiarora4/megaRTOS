#include "circularBuffer.h"
#include "strings.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

int circBufEnque(circBuf_t *circ, void *enq, bool force)
{
	if (!enq)
		return -1;

	uint16_t front = (circ->front + 1) % circ->queLen;
	if (front == circ->rear) {
		if (force)
			circ->rear = (circ->rear + 1) % circ->queLen;
		else
			return -1;
	}

	memcpy(circ->data + (circ->front * circ->elemSize), enq, circ->elemSize);
	front = circ->front;
	return 0;
}

int circBufDeque(circBuf_t *circ, void *deq, bool peak)
{
	if (circ->front == circ->rear)
		return -1;

	if (deq)
		memcpy(deque, circ->data + (circ->rear * circ->elemSize),
				circ->elemSize);
	if (!peak)
		circ->rear = (circ->rear + 1) % circ->queLen;
	return 0;
}

int circBufGetUsedSpace(circBuf_t *circBuf)
{
	int usedSpace;
	usedSpace = circBuf->front - circBuf->rear;
	if (usedSpace < 0)
		usedSpace += circBuf->queLen;
	return usedSpace;
}

int circBufGetFreeSpace(circBuf_t *circ)
{
	int freeSpace;
	freeSpace = circ->rear - circ->front;
	if (freeSpace <= 0)
		freeSpace += circ->queLen;
	return freeSpace;
}

void resetCircBuf(circBuf_t *circ)
{
	circ->front = circ->rear = 0;
}

bool isCircBuffEmpty(circBuf_t *circ)
{
	if(circ->front == circ->rear)
		return true;
	return false;
}

#ifdef __MAIN__

CIRCBUF_DEF(circ, int, 20);

int main()
{
	return 0;
}

#endif
