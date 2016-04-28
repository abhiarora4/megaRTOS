#include "array.h"

#include <stdint.h>

uint16_t fmax(uint8_t *arr, uint16_t len)
{
	uint16_t max_ndx = 0;

	unsigned int i;
	for (i = 0; i < len; i++)
		if (arr[i] > arr[max_ndx])
			max_ndx = i;

	return max_ndx;
}

uint16_t fmin(uint8_t *arr, uint16_t len)
{
	uint16_t min_ndx = 0;

	unsigned int i;
	for (i = 0; i < len; i++)
		if (arr[i] < arr[min_ndx])
			min_ndx = i;

	return min_ndx;
}

