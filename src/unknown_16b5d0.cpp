// @flags /O2 /Gr
/* UNKNOWN_16B5D0.CPP: data_dispose, apart from the rest of the data array code
   (unknown_16b570.cpp is built /Ob1) because retail inlines it into its
   callers, and LTCG does not inline a function out of an /Ob1 file */

#include "cseries.h"
#include "data_array.h"
#include <string.h>

// @retail 0x16b5d0
void data_dispose(s_data_array *data)
{
	c_data_allocator *allocator = data->allocator;

	memset(data, 0, sizeof(s_data_array));
	if (allocator)
	{
		allocator->deallocate(data);
	}
}
