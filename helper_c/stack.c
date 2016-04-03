#include "stack.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

int stackPush(stack_t *s, void *pushData)
{
	if (!pushData)
		return -1;

	if (s->top == s->length)
		return -1;

	memcpy(s->data + (s->top * s->elemSize), pushData, s->elemSize);
	s->top++;
	return 0;
}

int stackPop(stack_t *s, void *popData, bool readOnly)
{
	if (!s->top)
		return -1;

	if (popData)
		memcpy(popData, s->data + ((s->top - 1) * s->elemSize), s->elemSize);
	if (readOnly)
			s->top--;
	return 0;
}

//#define __MAIN__

#ifdef __MAIN__

STACK_DEF(abhi, int, 10);

int main()
{
	int a = 5;
	stackPush(&abhi, &a);
	a = 20;
	stackPush(&abhi, &a);

	a= 0;
	stackPop(&abhi, &a, true);
	serialPrintln("Poped is - %d", a);
	stackPop(&abhi, &a, true);
		serialPrintln("Poped is - %d", a);
	return 0;
}

#endif
