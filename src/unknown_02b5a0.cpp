// @flags /O2 /Gr
/* UNKNOWN_02B5A0.CPP: pool and caption-cache lifecycle */

#include "unknown_11c920.h"
#include "data_array.h"
#include "globals.h"
#include <string.h>

s_record_pool *g_509434;
double g_4ba040;
byte g_4c5018[0x6a8];
extern long g_4ba134;
extern long g_4b9970[12];
extern byte g_5093fc;
long g_4c1bd0;

bool object_or_parent_hidden(long object_index);

// @retail 0x3d7c0
bool function_3d7c0(long object_index, bool *out)
{
	bool result = false;
	/* Retail materializes the false fallback in a stack byte. */
	volatile bool fallback = false;
	bool current;
	if (!object_or_parent_hidden(object_index))
	{
		result = true;
		current = object_index == g_4c1bd0;
	}
	else
		current = fallback;
	if (out)
		*out = current;
	return result;
}

void function_3b950(void);

// @retail 0x2b5c0
void function_2b5c0(void)
{
	function_3b950();
	g_509434 = 0;
}

// @retail 0x2b540
void function_2b540(void)
{
	g_4ba040 = 0.0;
	g_4ba134 = 0;
	g_509434->valid = true;
	record_pool_release_all(g_509434);
	memset(g_4b9970, 0, sizeof(g_4b9970));
	memset(g_4c5018, 0, sizeof(g_4c5018));
	g_4b9970[0] = NONE;
	g_5093fc = false;
}

// @retail 0x2b5a0
void function_02b5a0(void)
{
	if (g_509434)
	{
		if (g_509434->valid)
		{
			g_509434->valid = false;
		}
	}
}


bool function_bad50(long object_index, long index, point3f *out);
dword __cdecl pack_color3f(color3f const *color);

struct s_3dd10_object_header
{
	byte unknown00[8];
	byte *object;
};

struct s_3dd10_cache
{
	byte unknown00[0xa8];
	dword colors[4];
	byte count;
	byte unknownb9[0x47];
};

// @retail 0x3dd10
void function_3dd10(long object_index, bool force)
{
	(void)&object_index;
	(void)&force;
	byte *object = ((s_3dd10_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	long cache_index = *(long *)(object + 0xcc);
	if (cache_index != NONE && (force || (g_4e3b44[*(long *)object & 0xffff].bytes[0x1c] & 1)))
	{
		s_3dd10_cache *cache = &((s_3dd10_cache *)g_509434->data)[cache_index & 0xffff];
		cache->count = 0;
		for (long i = 0; i < 4; ++i)
		{
			point3f color;
			if (function_bad50(object_index, i, &color))
			{
				cache->colors[i] = pack_color3f((color3f const *)&color);
				++cache->count;
			}
		}
	}
}
