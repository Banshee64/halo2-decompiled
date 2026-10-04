// @flags /O2 /Gr
/* UNKNOWN_11C9A0.CPP: the bounded string length of cseries (the callers in
   other files, such as src/unknown_0a4ab0.cpp, have it inlined) */

#include "cseries.h"

// @retail 0x11c9a0
unsigned long function_11c9a0(char const *string, unsigned long size)
{
	unsigned long length;
	for (length = 0; length < size && *string++ != 0; length++)
		;
	return length;
}
