#include "strings.h"

size_t strlcpy(char *dest, const char *src, size_t n)
{
	if (!dest)
		return 0;

	int i;
	for (i=0; i<(n-1) && src[i]; i++)
		dest[i] = src[i];
	dest[i]='\0';

	return i;
}
