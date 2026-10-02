// @flags /O2 /Gr
/* UNKNOWN_14B560.CPP: players_dispose (entry 19, dispose) */

#include "cseries.h"

void *g_4e8c24;
void *g_4e8c20;

// @retail 0x14b560
void players_dispose(void)
{
	if (g_4e8c24)
	{
		g_4e8c24 = 0;
	}
	if (g_4e8c20)
	{
		g_4e8c20 = 0;
	}
}
