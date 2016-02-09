/*
 * array.c
 *
 *  Created on: 09-Feb-2016
 *      Author: abhiarora
 */

#include "array.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


uint16_t fmax(uint8_t *arr, uint16_t len)
{
	uint8_t max_ele = arr[0];
	uint16_t maxNdx = 0;
	uint16_t i;
	for (i = 0; i < len; i++) {
		if (arr[i] > max_ele) {
			max_ele = arr[i];
			maxNdx = i;
		}
	}
	return maxNdx;
}

uint16_t fmin(uint8_t *arr, uint16_t len)
{
	uint8_t min_ele = arr[0];
	uint16_t minNdx = 0;

	uint16_t i;

	for (i = 0; i < len; i++) {
		if (arr[i] < min_ele) {
			min_ele = arr[i];
			minNdx = i;
		}
	}
	return minNdx;
}

