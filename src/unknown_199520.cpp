// @flags /O2 /Gr
/* UNKNOWN_199520.CPP: two passes over ranges of the game module table
   (g_46e320, unknown_03d380.cpp) (lane H) */

#include "cseries.h"
#include "globals.h"

// @retail 0x199520
void function_199520(dword flags)
{
	long index;

	for (index = 0; index < 5; index++)
	{
		g_46e320[5 + index](flags);
	}
}

// @retail 0x199540
void function_199540(dword flags)
{
	long index;

	for (index = 0; index < 20; index++)
	{
		g_46e320[10 + index](flags);
	}
}
