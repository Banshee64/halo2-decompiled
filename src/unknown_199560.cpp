// @flags /O2 /Gr
/* UNKNOWN_199560.CPP: compressed data blocks (lane H) */

#include "unknown_11c920.h"

#define BYTE_SWAP_LONG(v) ((((v) & 0xff0000) | ((v) >> 16)) >> 8 | ((((v) << 16) | ((v) & 0xff00)) << 8))

// @retail 0x199560
dword function_199560(void *data, dword size)
{
	dword result = 0;

	if (size >= 4)
	{
		result = BYTE_SWAP_LONG(*(dword *)data);
	}
	return result;
}
