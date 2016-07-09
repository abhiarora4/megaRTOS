/*
 * stack.c
 *
 *  Created on: 23-Jan-2016
 *  Author: abhiarora
 */
#ifndef _CSTACK_H_
#define _CSTACK_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	void *const data;

	uint16_t top;
	uint16_t elemSize;
	uint16_t length;
}stack_t;

#define STACK_DEF(n,t,s) t __stack_buf_##n[s]; \
	stack_t n = {.top = 0, \
		.data = (void *)__stack_buf_##n, \
		.elemSize = sizeof(t), \
		.length = s }


int stackPush(stack_t *, void *);
int stackPop(stack_t *, void *, bool);

#endif /* HELPER_STACK_H_ */
