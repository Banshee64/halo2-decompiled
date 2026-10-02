// @flags /O2 /Gr
/* UNKNOWN_175BD0.CPP: the effects lifecycle callbacks (entry 40) */

#include "cseries.h"
#include "data_array.h"

s_data_array *g_51ec8c;
s_data_array *g_51ec88;
s_data_array *g_51ec84;
s_data_array *g_510c74;
s_data_array *g_4ea93c;
s_data_array *g_4ea938;

// @retail 0x175bd0
void effects_dispose(void)
{
	g_51ec8c = 0;
	g_51ec88 = 0;
	g_51ec84 = 0;
	g_510c74 = 0;
	if (g_4ea93c)
	{
		g_4ea93c = 0;
	}
	if (g_4ea938)
	{
		g_4ea938 = 0;
	}
}

// @retail 0x175c70
void effects_dispose_from_old_map(void)
{
	g_51ec8c->valid = false;
	g_51ec88->valid = false;
	g_51ec84->valid = false;
	g_510c74->valid = false;
	g_4ea93c->valid = false;
	g_4ea938->valid = false;
}
