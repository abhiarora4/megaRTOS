/*
 * stack.c
 *
 *  Created on: 23-Jan-2016
 *  Author: abhiarora
 */
#ifndef HELPER_STACK_H_
#define HELPER_STACK_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t top;
	void * const data;
	uint16_t elementSize;
	uint16_t stackSize;
}stack_t;

#define STACK_DEF(n,t,s) t stackBuf_##n[s]; \
	stack_t n = {.top = 0, \
		.data = (void *)stackBuf_##n, \
		.elementSize = sizeof(t), \
		.stackSize = s }


int stackPush(stack_t *, void *);
int stackPop(stack_t *, void *, bool);

#endif /* HELPER_STACK_H_ */
