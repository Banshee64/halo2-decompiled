/* UNKNOWN_1DEE50.H */

#ifndef UNKNOWN_1DEE50_H
#define UNKNOWN_1DEE50_H

#include "cseries.h"
#include "globals.h"
#include <xmmintrin.h>

/* the first object of an object list; reference_index then walks the rest */
long object_list_get_first(long list_index, long *reference_index);
long function_1dee80(long *reference_index);

/* the object references of the object lists (g_4f55d4, unknown_1dee80.cpp) */
struct s_object_reference_1dee50
{
	byte unknown00[4];
	long object_index;
	long next_reference_index;
};

extern s_data_array *g_4f55d4;

/* the inline copy of function_1dee80 that LTCG places in callers' loops */
inline long object_list_get_next(long *reference_index)
{
	long object_index;
	if (*reference_index != NONE)
	{
		long size = g_4f55d4->size;
		byte *data = g_4f55d4->data;
		s_object_reference_1dee50 *reference = (s_object_reference_1dee50 *)(data + (*reference_index & 0xffff) * size);
		long next = reference->next_reference_index;

		if (next != NONE)
			_mm_prefetch((const char *)(data + (next & 0xffff) * size), _MM_HINT_T0);

		*reference_index = next;
		object_index = reference->object_index;
	}
	else
	{
		object_index = NONE;
	}
	return object_index;
}

#endif