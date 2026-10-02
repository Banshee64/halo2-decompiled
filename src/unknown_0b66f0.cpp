#include "cseries.h"
#include <stdarg.h>
#include <stdio.h>

// @flags /O2 /Gr

// @retail 0xb66f0
char *csprintf(char *buffer, char const *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	_vsnprintf(buffer, 255, format, arguments);
	buffer[255] = 0;
	return buffer;
}
