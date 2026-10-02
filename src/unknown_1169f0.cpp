// @flags /O2 /Gr
#include <string.h>
#include "cseries.h"
#include "globals.h"

/* The callbacks of the table at 0x4674d8: a data array of 0x1258-byte
   elements (g_4e0338) and its accessors. */

struct s_cloth_tag_data
{
	byte unknown00[0xc];
	long value;
};

struct s_cloth_element
{
	byte unknown00[4];
	long tag_index;
};

s_data_array *g_4e0338;
c_data_allocator *g_510c2c;

static inline s_data_array *cloth_data_new(const char *name, long maximum_count, long size, long alignment_bits, c_data_allocator *allocator)
{
	long bitmap_size = ((maximum_count + 31) >> 5) * 4;
	s_data_array *data = (s_data_array *)allocator->allocate(sizeof(s_data_array) + maximum_count * size + bitmap_size + (1 << alignment_bits) - 1);

	if (data)
	{
		data_initialize(data, name, maximum_count, size, alignment_bits, allocator);
		data->allocated = 1;
	}
	return data;
}

// @retail 0x116a10
void __stdcall function_116a10()
{
	g_4e0338 = cloth_data_new("cloth", 8, 0x1258, 0, g_510c2c);
}

// @retail 0x116a50
void __stdcall function_116a50()
{
	g_4e0338->valid = 1;
	data_delete_all(g_4e0338);
}

// @retail 0x116a70
void __stdcall function_116a70()
{
	g_4e0338->valid = 0;
}

// @retail 0x116a80
void __stdcall function_116a80()
{
	if (g_4e0338)
	{
		s_data_array *data = g_4e0338;
		c_data_allocator *allocator = data->allocator;

		memset(data, 0, sizeof(s_data_array));
		if (allocator)
			allocator->deallocate(data);
		g_4e0338 = 0;
	}
}

// @retail 0x1169f0
void __stdcall function_1169f0(long index)
{
	datum_delete(g_4e0338, index);
}

// @retail 0x116ac0
long __stdcall function_116ac0(long index)
{
	s_cloth_element *element = (s_cloth_element *)(g_4e0338->data + (index & 0xffff) * 0x1258);

	return ((s_cloth_tag_data *)g_4e3b44[element->tag_index & 0xffff].bytes)->value;
}

/* the callbacks of the table at 0x4674d8 that are decompiled */
void *g_4674d8[12] =
{
	(void *)function_116a10, (void *)function_116a50, (void *)function_116a70, (void *)function_116a80, 0,
	(void *)function_1169f0, 0, 0, 0, 0,
	(void *)function_116ac0, 0
};
