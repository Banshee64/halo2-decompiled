// @flags /O2 /Gr
/* UNKNOWN_20A810.CPP: script_nodes_dispose (entry 54, dispose) */

#include "unknown_11c920.h"
#include "data_array.h"

s_record_pool *g_4f9394;
s_record_pool *g_4f9380;

// @retail 0x20a810
void script_nodes_dispose(void)
{
	data_dispose(g_4f9394);
	g_4f9380->valid = false;
}
