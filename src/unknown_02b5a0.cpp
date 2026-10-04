// @flags /O2 /Gr
/* UNKNOWN_02B5A0.CPP: a lifecycle callback (entry 32, field_10_2) */

#include "cseries.h"
#include "data_array.h"

s_record_pool *g_509434;

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
