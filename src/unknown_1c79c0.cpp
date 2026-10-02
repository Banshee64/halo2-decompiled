// @flags /O2 /Gr
/* UNKNOWN_1C79C0.CPP: a lifecycle callback (entry 58, dispose) */

#include "cseries.h"
#include "globals.h"

// @retail 0x1c79c0
void function_1c79c0(void)
{
	g_557c6c->allocator->deallocate(g_557c6c);
	g_557c6c = 0;
}
