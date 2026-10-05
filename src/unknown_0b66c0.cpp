// @flags /O2 /Ob1 /Gr
#include "unknown_11c920.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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

// @retail 0xb6190
byte function_b6190(char const *a, char const *b)
{
	return strcmp(a, b) == 0;
}

static __forceinline unsigned long string_length_512_ab(char const *string)
{
	unsigned long length = 0;
	for (; length < 0x1ff; length++)
	{
		if (!*string++)
			break;
	}
	return length;
}

// @retail 0xb61d0
long function_b61d0(char const *string, long start, char const *substring)
{
	long result = NONE;
	if (start < (long)string_length_512_ab(string))
	{
		char const *found = strstr(string + start, substring);
		if (found)
			result = found - string;
	}
	return result;
}

// @retail 0xb6220
bool function_b6220(char const *string, long start, long count, char *destination)
{
	char const *const *string_reference = &string;
	bool result = false;
	if (start >= 0 && count > 0 && start + count <= (long)string_length_512_ab(*string_reference))
	{
		long size = count + 1;
		if (size > 0x200)
			size = 0x200;
		strncpy(destination, string + start, size);
		destination[size - 1] = 0;
		result = true;
	}
	return result;
}

// @retail 0xb6290
char *function_b6290(char *buffer, char const *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	_vsnprintf(buffer, 0xf, format, arguments);
	buffer[0xf] = 0;
	return buffer;
}
