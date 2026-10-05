// @flags /O2 /Gr
/* UNKNOWN_215720.CPP: a lifecycle callback (entry 9, field_10_2) */

#include "unknown_11c920.h"

long g_55c158;
extern void *g_51ea14;
void function_216760(void);

// @retail 0x215720
void function_215720(void)
{
	g_55c158 = NONE;
}

// @retail 0x2157c0
void function_2157c0(long selection)
{
	if (selection != NONE)
		g_55c158 = selection;
	if (g_51ea14)
	{
		*(long *)g_51ea14 = 0;
		function_216760();
	}
}
