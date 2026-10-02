// @flags /O2 /Gr
/* UNKNOWN_02B5A0.CPP: a lifecycle callback (entry 32, dispose_from_old_map) */

#include "cseries.h"
#include "unknown_02b5a0.h"

s_data_header *g_509434;

// @retail 0x2b5a0
void function_02b5a0(void)
{
	if (g_509434)
	{
		if (g_509434->valid)
		{
			g_509434->valid = false;
		}
	}
}
