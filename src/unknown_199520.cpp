// @flags /O2 /Gr
/* UNKNOWN_199520.CPP: two passes over ranges of the game module table
   (g_46e320, unknown_03d380.cpp) (lane H) */

#include "cseries.h"
#include "globals.h"

// @retail 0x199520
void function_199520(dword flags)
{
	game_module_proc *proc = &g_46e320[5];
	long count;

	for (count = 5; count != 0; count--)
	{
		(*proc)(flags);
		proc++;
	}
}

// @retail 0x199540
void function_199540(dword flags)
{
	game_module_proc *proc = &g_46e320[10];
	long count;

	for (count = 20; count != 0; count--)
	{
		(*proc)(flags);
		proc++;
	}
}
