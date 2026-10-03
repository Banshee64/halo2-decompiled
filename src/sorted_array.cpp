// @flags /O2 /arch:SSE /Gr /GL-
/* SORTED_ARRAY.CPP: the binary search of the animation graph's sorted blocks.
   Retail passes every argument on the stack (ret 0xc) to all of its LTCG
   callers, so this file is built without LTCG. */

#include "cseries.h"

/* an array of elements, each starting with its key, sorted by key */
struct s_sorted_array
{
	long count;
	byte *elements;
};

// @retail 0x1dd560
void *__stdcall function_1dd560(s_sorted_array *array, long key, long element_size)
{
	bool last = false;
	long lower = 0;
	long upper = array->count - 1;
	void *result = NULL;

	while (!result && !last && lower <= upper)
	{
		long middle = (lower + upper) >> 1;
		long *element = (long *)(array->elements + middle * element_size);

		last = lower == upper;
		if (*element == key)
			result = element;
		else if (*element > key)
			upper = middle - 1;
		else if (*element < key)
			lower = middle + 1;
	}
	return result;
}
