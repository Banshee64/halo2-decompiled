// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0359E0.CPP: how many mipmap levels a bitmap can have. Decompiled by
   lane L for the texture cache (xbox_texture_cache.cpp). */

#include "cseries.h"

#define MAXIMUM(a, b) ((a) > (b) ? (a) : (b))
#define IS_POWER_OF_TWO(x) (!((x) & ((x) - 1)))

long log2_floor(dword value);

static inline bool bitmap_format_is_compressed(long format)
{
	return format >= 14 && format <= 16;
}

/* the levels down to 1x1 (compressed formats stop at 4x4), none for a bitmap
   whose sides are not powers of two, at most maximum_levels */
// @retail 0x359e0
short bitmap_get_mipmap_count(short width, short height, short depth, short format, bool linear, short maximum_levels)
{
	short result = 0;

	if (IS_POWER_OF_TWO(width) && IS_POWER_OF_TWO(height) && IS_POWER_OF_TWO(depth))
	{
		if (linear)
		{
			result = 0;
		}
		else if (bitmap_format_is_compressed(format))
		{
			result = (short)log2_floor(MAXIMUM(width / 4, MAXIMUM(height / 4, depth)));
		}
		else
		{
			result = (short)log2_floor(MAXIMUM(width, MAXIMUM(height, depth)));
		}
	}
	return maximum_levels > result ? result : maximum_levels;
}
