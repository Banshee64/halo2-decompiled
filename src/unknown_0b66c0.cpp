// @flags /O2 /Gr
#include "cseries.h"
#include <stdarg.h>
#include <stdio.h>

struct s_index_pair
{
	long index;
	byte unknown04[2];
	short index2;
};

struct s_index_triple
{
	long index;
	byte unknown04[2];
	short index2;
	byte unknown08[0x60];
	long index3;
};

struct s_small_index
{
	byte unknown00[0x18];
	signed char index;
	byte unknown19[0x5b];
	long count;
};

// @retail 0x000b66c0
char *csprintf_1024(char *buffer, const char *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	_vsnprintf(buffer, 0x3ff, format, arguments);
	buffer[0x3ff] = 0;
	return buffer;
}

// @retail 0x000b66f0
char *csprintf_256(char *buffer, const char *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	_vsnprintf(buffer, 0xff, format, arguments);
	buffer[0xff] = 0;
	return buffer;
}

// @retail 0x000b6720
unsigned long function_0b6720(const char *string)
{
	unsigned long length = 0;
	for (; length < 0xff; length++)
	{
		if (!*string++)
			break;
	}
	return length;
}

// @retail 0x000b6760
bool function_0b6760(const s_index_pair *pair)
{
	return pair->index != NONE && pair->index2 != NONE;
}

// @retail 0x000b6780
bool function_0b6780(const s_index_triple *triple)
{
	return triple->index3 != NONE && triple->index != NONE && triple->index2 != NONE;
}

// @retail 0x000b67a0
short function_0b67a0(const s_small_index *data)
{
	signed char index = data->index;
	if (index >= 0 && index < data->count)
		return index;
	return NONE;
}
