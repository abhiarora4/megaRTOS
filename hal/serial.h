/*
 * serial.h
 *
 *  Created on: 22-Jan-2016
 *      Author: abhiarora
 */

#ifndef HAL_SERIAL_H_
#define HAL_SERIAL_H_

#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

int UARTLogLine(const char *fileName, bool eol, const char *format, ...);

#define __FILENAME__ ((strrchr(__FILE__, '/')) ? strrchr(__FILE__, '/') + 1: __FILE__)

#define serialPrintln(...) UARTLogLine(__FILENAME__, true, __VA_ARGS__)
#define serialPrint(...) UARTLogLine(__FILENAME__, false, __VA_ARGS__)

#endif /* HAL_SERIAL_H_ */
