/*
 * circularBuffer.h
 *
 *  Created on: 15-Jan-2016
 *      Author: abhiarora
 */

#ifndef _CCIRCULARBUFFER_H_
#define _CCIRCULARBUFFER_H_

#include "queue.h"

#include <stdbool.h>

typedef queue_t circBuf_t;

#define CIRCBUF_DEF(t,n,s) QUEUE_DEF(t,n,s+1)

int circBufEnque(circBuf_t *circ, void *enq, bool force);
int circBufDeque(circBuf_t *circ, void *deq, bool peak);

int circBufGetUsedSpace(circBuf_t *circBuf);
int circBufGetFreeSpace(circBuf_t *circ);

bool isCircBuffEmpty(circBuf_t *circ);


#endif /* HELPER_CIRCULARBUFFER_H_ */
