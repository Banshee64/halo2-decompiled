// @flags /O2 /Gr
/* DATA_COMPRESS.CPP: compressed data blocks (lane H) */

#include "cseries.h"

// @retail 0x199560
dword data_decompressed_size(void *data, dword size)
{
	dword result = 0;

	if (size >= 4)
	{
		dword value = *(dword *)data;
		result = (((value & 0xff0000) | (value >> 16)) >> 8) | (((value & 0xff00) | (value << 16)) << 8);
	}
	return result;
}
