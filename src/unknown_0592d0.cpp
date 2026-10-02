// @flags /O2 /Gr
/* UNKNOWN_0592D0.CPP: a global getter */

#include "cseries.h"

byte g_527330;
dword g_527334;

// @retail 0x592d0
dword function_0592d0(void)
{
	dword result = 0;
	if (g_527330)
	{
		result = g_527334;
	}
	return result;
}
