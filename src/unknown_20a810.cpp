// @flags /O2 /Gr
/* UNKNOWN_20A810.CPP: script_nodes_dispose (entry 54, dispose) */

#include "cseries.h"
#include "unknown_02b5a0.h"

s_data_header *g_4f9394;
s_data_header *g_4f9380;

// @retail 0x20a810
void script_nodes_dispose(void)
{
	data_dispose(g_4f9394);
	g_4f9380->valid = false;
}
