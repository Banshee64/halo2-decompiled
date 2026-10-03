// @flags /O2 /Gr
/* UNKNOWN_2172A0.CPP: removes an entry from the table of unknown_2157e0.cpp
   (g_55c164, g_55c160 entries of 0x44 bytes, keyed by the handle at +0) by
   moving the last entry into its place (lane H) */

#include "cseries.h"
#include <string.h>

struct s_55c164
{
	void *field0;
	byte unknown04[0x40];
};

extern long g_55c160;
extern s_55c164 g_55c164[16];

// @retail 0x2172a0
void function_2172a0(long handle)
{
	long count = g_55c160;
	long i;

	for (i = 0; i < count; i++)
	{
		long entry = (long)g_55c164[i].field0;

		if (entry != NONE && entry == handle)
		{
			long last = count - 1;

			if (i != last)
				g_55c164[i] = g_55c164[last];
			g_55c160--;
			return;
		}
	}
}
