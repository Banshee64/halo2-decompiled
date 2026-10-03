// @flags /O2 /Gr
/* UNKNOWN_020670.CPP: the compiler's vector constructor iterator, and a
   bitmap slot release */

#include "cseries.h"
#include "visibility_slot.h"
#include <string.h>

typedef void *(__fastcall *constructor_proc)(void *);


// @retail 0x20670
void __stdcall vector_constructor_iterator(void *array, unsigned size, int count, constructor_proc constructor)
{
	while (--count >= 0)
	{
		constructor(array);
		array = (char *)array + size;
	}
}

s_slot g_51f40c[511];
dword g_5233f0[3][16];

// @retail 0x20e50
void function_20e50(long index)
{
	s_slot *slot = &g_51f40c[index];
	long i = 0;

	do
	{
		g_5233f0[i][index >> 5] &= ~(1 << (index & 0x1f));
		i++;
	}
	while (i < 3);

	memset(slot, 0, sizeof(s_slot));
	((dword *)slot)[1] |= 0xff80;
}