#include "array.h"

#include <stdint.h>

uint16_t fmax(uint8_t *arr, uint16_t len)
{
	uint8_t max = arr[0];
	uint16_t max_ndx = 0;

	unsigned int i;
	for (i = 0; i < len; i++) {
		if (arr[i] > max) {
			max = arr[i];
			max_ndx = i;
		}
	}
	return max_ndx;
}

uint16_t fmin(uint8_t *arr, uint16_t len)
{
	uint8_t min = arr[0];
	uint16_t min_ndx = 0;

	unsigned int i;
	for (i = 0; i < len; i++) {
		if (arr[i] < min) {
			min = arr[i];
			min_ndx = i;
		}
	}
	return min_ndx;
}

