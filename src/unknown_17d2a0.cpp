// @flags /O2 /Gr
/* UNKNOWN_17D2A0.CPP: decals_dispose (entry 37, dispose) */

#include "cseries.h"
#include "globals.h"
#include <xtl.h>

D3DResource *g_509444;

// @retail 0x17d2a0
void decals_dispose(void)
{
	g_4ea950 = 0;
	if (g_509444)
	{
		D3DResource_Release(g_509444);
		g_509444 = 0;
	}
	g_509448->allocator->deallocate(g_509448);
}
