#include "serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

int UARTLogLine(const char *fileName, bool eol, const char *format,...)
{
	int numCharWrite = 0;
	static bool lastEOLState = true;
	va_list args;
	va_start(args, format) ;

	if (lastEOLState && fileName)
		numCharWrite += printf("%s: ", fileName);

	lastEOLState = eol;
	numCharWrite += vprintf(format, args);

	if (eol)
		numCharWrite += printf("\n");
	va_end(args) ;
	return numCharWrite;
}


/*
int serialPrintln(char *format, ...)
{
	int numCharWrite;
	va_list arg;

	va_start(arg, format);
	numCharWrite = vprintf(format, arg);
	numCharWrite += printf("\n");
	va_end(arg);

	return numCharWrite;
}

int serialPrint(char *format, ...)
{
	int numCharWrite;

	va_list arg;
	va_start(arg, format);
	numCharWrite = vprintf(format, arg);
	va_end(arg);

	return numCharWrite;
}
*/
