#include "unknown_11c920.h"
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

// @flags /O2 /Gr

// @retail 0x1630e0
word *function_1630e0(word *buffer, const word *format, ...)
{
	va_list arguments;

	va_start(arguments, format);
	_vsnwprintf((wchar_t *)buffer, 0xff, (const wchar_t *)format, arguments);
	va_end(arguments);

	buffer[0xff] = 0;
	return buffer;
}
