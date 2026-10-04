// @flags /O2 /Gr
/* UNKNOWN_199560.CPP: compressed data blocks (lane H) */

#include "unknown_11c920.h"

// @retail 0x199560
dword function_199560(void *data, dword size)
{
	dword result = 0;

	if (size >= 4)
	{
		dword value = *(dword *)data;
		result = (((value & 0xff0000) | (value >> 16)) >> 8) | (((value & 0xff00) | (value << 16)) << 8);
	}
	return result;
}
