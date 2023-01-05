/*
 * print.c
 *
 *  Created on: Sep 26, 2022
 *      Author: Andre
 */

#include "print.h"

#include <stdio.h>
#include <stdarg.h>
#include "UART.h"

void prints(char *format, ...){
	va_list aptr;
	char buffer[80];
	va_start(aptr, format);

	int ret = vsprintf(buffer, format, aptr);
	va_end(aptr);

	USART_Write(USART2, (uint8_t*)buffer, ret);
}
