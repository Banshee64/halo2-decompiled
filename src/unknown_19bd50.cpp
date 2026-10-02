// @flags /O2 /Gr
/* UNKNOWN_19BD50.CPP: level_handle_tables_dispose (entry 13, dispose) */

#include "cseries.h"
#include "unknown_02b5a0.h"

s_data_header *g_4ee4e4;
s_data_header *g_4ee4e8;

// @retail 0x19bd50
void level_handle_tables_dispose(void)
{
	if (g_4ee4e4)
	{
		data_dispose(g_4ee4e4);
		g_4ee4e4 = 0;
	}
	if (g_4ee4e8)
	{
		data_dispose(g_4ee4e8);
		g_4ee4e8 = 0;
	}
}
