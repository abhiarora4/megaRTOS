#include "circularBuffer.h"
#include "strings.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

inline void resetCircBuf(circBuf_t *circ)
{
	circ->front = circ->rear = 0;
}

int circBufEnque(circBuf_t *circ, void *enq, bool force)
{
	if (!enq)
		return -1;

	uint16_t front = (circ->front + 1) % circ->len;
	if (front == circ->rear) {
		if (force)
			circ->rear = (circ->rear + 1) % circ->len;
		else
			return -1;
	}
	memcpy(circ->data + (circ->front * circ->elemSize), enq, circ->elemSize);
	circ->front = front;
	return 0;
}

int circBufDeque(circBuf_t *circ, void *deq, bool peak)
{
	if (circ->front == circ->rear)
		return -1;

	if (deq)
		memcpy(deq, circ->data + (circ->rear * circ->elemSize),
				circ->elemSize);
	if (!peak)
		circ->rear = (circ->rear + 1) % circ->len;
	return 0;
}

int circBufGetUsedSpace(circBuf_t *circBuf)
{
	int usedSpace;
	usedSpace = circBuf->front - circBuf->rear;
	if (usedSpace < 0)
		usedSpace += circBuf->len;
	return usedSpace;
}

int circBufGetFreeSpace(circBuf_t *circ)
{
	int freeSpace;
	freeSpace = circ->rear - circ->front - 1;
	if (freeSpace <= 0)
		freeSpace += circ->len;
	return freeSpace;
}

bool isCircBuffEmpty(circBuf_t *circ)
{
	if(circ->front == circ->rear)
		return true;
	return false;
}

//#define __MAIN__

#ifdef __MAIN__
#include <stdio.h>

CIRCBUF_DEF(int, circ, 20);

int main()
{
	int i;
	for (i=0; i<20; i++) {
		circBufEnque(&circ, &i, false);
	printf("Used Space - %d\n", circBufGetUsedSpace(&circ));
	printf("Free Space - %d\n", circBufGetFreeSpace(&circ));
	}

	for (i=0; i<20; i++) {
		int a;
		circBufDeque(&circ, &a, false);
		printf("i - %-2d, %d\n", i, a);
	}

	return 0;
}

#endif
