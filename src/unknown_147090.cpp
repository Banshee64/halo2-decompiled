// @flags /O2 /Gr
/* UNKNOWN_147090.CPP */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_146a20.h"

/* Havok's fixed buffer in physical memory (unknown_146a20.cpp makes it) */
c_havok_fixed_memory *g_4798a0;

/* the bytes in use: the allocation table (8 bytes an entry) grows down from
   the end of the buffer, and each entry's second word is its size */
static long list_total(c_havok_fixed_memory *list)
{
	long total = 0;
	long count = list->m_count;

	if (count > 0)
	{
		long *p = (long *)((byte *)list->m_buffer + list->m_size) - 1;
		do
		{
			total += *p;
			p -= 2;
			count--;
		}
		while (count);
	}
	return total;
}

// @retail 0x147090
long function_147090(void)
{
	long a = 0;

	if (g_47989c)
		a = list_total(g_47989c);

	return (g_4798a0 ? list_total(g_4798a0) : 0) + a;
}
