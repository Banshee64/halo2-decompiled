// @flags /O1 /Gr
/* UNKNOWN_22C033.CPP: five flags kept in the game state, set up to an index
   at a time */

#include "cseries.h"

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

void *function_123d40(char const *name, char const *type, long size);

/* hs_library_external.cpp */
extern long *g_502248;

// @retail 0x22c033
void function_22c033()
{
	g_502248 = (long *)function_123d40("unknown 22c033", NULL, 5 * sizeof(long));
}

/* sets every flag up to and including this one */
// @retail 0x22c041
void function_22c041(long index)
{
	if (PIN(index, 0, 4) == index)
	{
		for (long i = 0; i <= index; i++)
		{
			if (!g_502248[i])
			{
				g_502248[i] = 1;
			}
		}
	}
}

// @retail 0x22c075
long function_22c075(long index)
{
	long result = 0;

	if (PIN(index, 0, 4) == index)
	{
		result = g_502248[index];
	}
	return result;
}
