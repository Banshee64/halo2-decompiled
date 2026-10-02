// @flags /O2 /Gr
/* UNKNOWN_17B850.CPP: the contrails lifecycle callbacks (entry 39) */

#include "cseries.h"
#include "unknown_02b5a0.h"

s_data_header *g_4ea940;
s_data_header *g_4ea944;

// @retail 0x17b850
void contrails_dispose(void)
{
	if (g_4ea940)
	{
		g_4ea940 = 0;
	}
	if (g_4ea944)
	{
		g_4ea944 = 0;
	}
}

// @retail 0x17b8a0
void contrails_dispose_from_old_map(void)
{
	g_4ea940->valid = false;
	g_4ea944->valid = false;
}
