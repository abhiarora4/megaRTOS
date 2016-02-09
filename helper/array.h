/*
 * array.h
 *
 *  Created on: 09-Feb-2016
 *      Author: abhiarora
 */

#ifndef HELPER_ARRAY_H_
#define HELPER_ARRAY_H_

#include <stdint.h>

uint16_t fmax(uint8_t *arr, uint16_t len);
uint16_t fmin(uint8_t *arr, uint16_t len);

#define max(a,b) \
      ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a >= _b ? _a : _b; })

#define min(a,b) \
      ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a <= _b ? _a : _b; })


#endif /* HELPER_ARRAY_H_ */
