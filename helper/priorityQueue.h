#ifndef HELPER_PRIORITYQUEUE_H_
#define HELPER_PRIORITYQUEUE_H_

/*
 * More advanced implementations may support more complicated operations,
 * such as pull_lowest_priority_element, inspecting the first
 * few highest- or lowest-priority elements, clearing the queue,
 * clearing subsets of the queue, performing a batch insert,
 * merging two or more queues into one,
 * incrementing priority of any element, etc.
 */

#include "queue.h"

#include <stdint.h>

typedef queue_t priorityQueue_t;

typedef struct {
	void *data;
	void *priority;
}priorityQueueElement_t;

#define PRIORITY_QUEUE_DEF(n,t,s) uint8_t __pr_##n[s]; t __elem_##n[s]; \
	priorityQueueElement_t pqElement_##n = {.element = priority_##n, \
	.element = element_##n}; \
	priorityQueue_t n = {.front = 0, .rear = 0, \
	.queueSize = s, .elementSize = sizeof(t), \
	.data = &pqElement_##n};


#endif /* HELPER_PRIORITYQUEUE_H_ */
