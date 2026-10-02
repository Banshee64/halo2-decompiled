// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "globals.h"

/* an array of elements, each starting with its key, sorted by key */
struct s_sorted_array
{
	long count;
	byte *elements;
};

// @retail 0x1dd560
void *function_1dd560(s_sorted_array *array, long key, long element_size)
{
	void *result = 0;
	long lower = 0;
	long upper = array->count - 1;
	bool last = false;

	while (!last && lower <= upper)
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
		if (result)
			break;
	}
	return result;
}

/* the globals of g_4e034c, as 1e96a0 reads them: the table of reals at +0xfc
   has four columns per row */
struct s_1e96a0_globals
{
	byte unknown00[0xf8];
	void *table_valid;
	real *table;
};

// @retail 0x1e96a0
real function_1e96a0(short column, short row)
{
	s_1e96a0_globals *globals = (s_1e96a0_globals *)g_4e034c;
	real result = 1.0f;

	if (globals && globals->table_valid && globals->table)
	{
		if (column < 0)
			column = 0;
		else if (column > 3)
			column = 3;
		result = globals->table[row * 4 + column];
	}
	return result;
}
