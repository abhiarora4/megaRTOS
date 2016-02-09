#include "stack.h"

#include "../hal/serial.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

int stackPush(stack_t *s, void *pushData)
{
	uint16_t new_top = s->top + 1;
	if (new_top == s->stackSize)
		return -1;

	if (!pushData)
		return -1;

	memcpy(s->data + (s->top * s->elementSize), pushData, s->elementSize);
	s->top = new_top;
	return 0;
}

int stackPop(stack_t *s, void *popData, bool readOnly)
{
	if (s->top == 0)
		return -1;

	if (popData)
		memcpy(popData, s->data + ((s->top - 1) * s->elementSize), s->elementSize);
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
