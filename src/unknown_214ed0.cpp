// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_214ED0.CPP: the largest cache file of each map type. Decompiled by
   lane L for the cache header check (cache_files.cpp). */

#include "cseries.h"

// @retail 0x214ed0
long cache_file_get_maximum_size(long type)
{
	switch (type)
	{
	case 0:
		return 0x11800000;
	case 1:
		return 0x5000000;
	case 2:
		return 0x5000000;
	case 3:
		return 0xb400000;
	case 4:
		return 0x20800000;
	}
	return 0;
}
