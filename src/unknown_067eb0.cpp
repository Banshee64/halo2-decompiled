// @flags /O2 /Gr
/* UNKNOWN_067EB0.CPP: a lifecycle callback (entry 23, dispose) */

#include "cseries.h"
#include "globals.h"

long g_4cf784;

// @retail 0x67eb0
void function_067eb0(void)
{
	if (g_4cf770)
	{
		g_4cf77c = 0;
		g_4cf780 = 0;
		g_4cf784 = 0;
		g_4cf770 = false;
	}
}
