#ifndef _CARRAY_H_
#define _CARRAY_H_

#include <stdint.h>

uint16_t fmax(uint8_t *arr, uint16_t len);
uint16_t fmin(uint8_t *arr, uint16_t len);

// Refer the link below:-
//https://dustri.org/b/min-and-max-macro-considered-harmful.html

#define max(a,b) \
      ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a >= _b ? _a : _b; })

#define min(a,b) \
      ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a <= _b ? _a : _b; })


#endif /* HELPER_ARRAY_H_ */
