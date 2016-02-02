#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"

char *strlcpy(char *dest, char *src, size_t n)
{
	if (!dest)
		return NULL;

	dest[0] = '\0';
	return strncat(dest, src, n);
}
