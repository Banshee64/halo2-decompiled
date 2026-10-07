// @flags /O2 /arch:SSE /Gr /GL-
/* SORTED_ARRAY.CPP: the binary search of the animation graph's sorted blocks.
   Retail passes every argument on the stack (ret 0xc) to all of its LTCG
   callers, so this file is built without LTCG. */

#include "unknown_11c920.h"

/* an array of elements, each starting with its key, sorted by key */
struct s_sorted_array
{
	long count;
	byte *elements;
};

// @retail 0x1dd560
void *__stdcall function_1dd560(s_sorted_array *array, long key, long element_size)
{
	s_sorted_array *local_0 = array;
	array = NULL;
	long lower = 0;
	long upper = local_0->count - 1;
	bool last = false;

	while (!array && !last && lower <= upper)
	{
		last = lower == upper;
		long middle = (lower + upper) >> 1;
		long *element = (long *)(local_0->elements + middle * element_size);

		if (*element == key)
			array = (s_sorted_array *)element;
		else if (*element > key)
			upper = middle - 1;
		else if (*element < key)
			lower = middle + 1;
	}
	return array;
}
