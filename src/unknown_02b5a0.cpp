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

extern dword g_4ba034;
long g_4ba050;
struct s_interface_function_context;
void function_2c3b0(s_interface_function_context *context);
real __stdcall function_be8b0(long object_index, long name);
struct s_object_list;
extern s_object_list *g_4de2f4;
long function_baf80(long object_index);

// @retail 0x3ddd0
long __stdcall function_3ddd0(long object_index)
{
    (void)&object_index;
    byte *object = ((s_3dd10_object_header *)g_4e0300->data)[object_index & 0xffff].object;
    long result = *(long *)(object + 0xcc);
    bool initialize = false;
    if (result == NONE || *(long *)(g_509434->data + (result & 0xffff) * 0x100 + 4) != object_index)
    {
        result = record_pool_allocate(g_509434);
        if (result == NONE)
        {
            real oldest = -3.402823466e+38f;
            long absolute = function_16bc00(g_509434, 0);
            long index = data_datum_index(g_509434, absolute);
            while (index != NONE)
            {
                byte *record = g_509434->data + (index & 0xffff) * 0x100;
                real age = (real)((long)g_4ba034 - *(long *)(record + 0xc));
                if (age < 0.0f) age = 1000.0f;
                if (age > oldest)
                {
                    oldest = age;
                    result = index;
                }
                absolute = function_16bc00(g_509434, (index & 0xffff) + 1);
                index = data_datum_index(g_509434, absolute);
            }
        }
        if (result == NONE) return result;
        initialize = true;
    }
    byte *record = g_509434->data + (result & 0xffff) * 0x100;
    if (initialize)
    {
        *(long *)(record + 4) = object_index;
        *(long *)(record + 8) = NONE;
        *(long *)(record + 0xc) = NONE;
        record[2] = 0;
        record[3] = 0;
        function_2c3b0((s_interface_function_context *)(record + 0x44));
        *(long *)(record + 0x44) = object_index;
        *(long *)(record + 0x48) = (long)function_be8b0;
        *(long *)(record + 0xbc) = NONE;
        object = ((s_3dd10_object_header *)g_4e0300->data)[object_index & 0xffff].object;
        byte *data = object + *(short *)(object + 0x11a);
        long bytes = *(short *)(object + 0x118) / 10;
        for (long i = 0; i < 4; ++i)
            memcpy(record + 0xc0 + i * 16, data, bytes);
        *(long *)(object + 0xcc) = result;
        function_3dd10(object_index, true);
    }
    return result;
}

// @retail 0x3d430
signed char *function_3d430(long object_index)
{
    (void)&object_index;
    signed char *result = NULL;
    if (g_4ba050 >= 0 && g_4ba050 < 4)
    {
        long cache = function_3ddd0(object_index);
        if (cache != NONE)
            result = (signed char *)(g_509434->data + (cache & 0xffff) * 0x100 + 0xc0 + g_4ba050 * 16);
    }
    return result;
}

// @retail 0x4c640
void function_4c640(long object_index, signed char value)
{
    (void)&object_index;
    (void)&value;
    if (g_4ba050 >= 0 && g_4ba050 < 4)
    {
        byte *slot = NULL;
        long cache = function_3ddd0(object_index);
        if (cache != NONE)
            slot = g_509434->data + (cache & 0xffff) * 0x100 + 0xbc;
        slot[g_4ba050] = (byte)value;
    }
}

// @retail 0x3dc90
byte *function_3dc90(long object_index)
{
    (void)&object_index;
    byte *result = NULL;
    byte *object = ((s_3dd10_object_header *)g_4e0300->data)[object_index & 0xffff].object;
    if (object[7] & 1)
        result = (byte *)g_4de2f4 + 0x1c;
    else if (*(long *)(object + 0x14) != NONE)
        result = function_3dc90(function_baf80(object_index));
    else
    {
        long cache = function_3ddd0(object_index);
        if (cache != NONE)
        {
            byte *record = g_509434->data + (cache & 0xffff) * 0x100;
            if (record[3]) result = record + 0x54;
        }
    }
    return result;
}

// @retail 0x3e420
void function_3e420(long tag, long object_index, signed char *output, byte level)
{
    (void)&level;
    byte *definition = g_4e3b44[tag & 0xffff].bytes;
    signed char *current = object_index != NONE ? function_3d430(object_index) : NULL;
    for (long i = 0; i < *(long *)(definition + 0x1c); ++i)
    {
        byte *group = *(byte **)(definition + 0x20) + i * 16;
        long index = current ? current[i] : 0;
        if (index != NONE)
        {
            if (index < 0) index = 0;
            else if (index > *(long *)(group + 8) - 1) index = *(long *)(group + 8) - 1;
            byte *variant = *(byte **)(group + 0xc) + index * 16;
            output[i] = variant[4 + level * 2];
        }
        else output[i] = (signed char)0xff;
    }
}
