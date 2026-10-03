// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_214ED0.CPP: the largest cache file of each map type. Decompiled by
   lane L for the cache header check (cache_files.cpp). */

#include "cseries.h"

// @retail 0x214ed0
long cache_file_get_maximum_size(long type)
{
	long result = 0;

	switch (type)
	{
	case 1:
		result = 0x5000000;
		break;
	case 2:
		result = 0x5000000;
		break;
	case 0:
		result = 0x11800000;
		break;
	case 3:
		result = 0xb400000;
		break;
	case 4:
		result = 0x20800000;
		break;
	}
	return result;
}
