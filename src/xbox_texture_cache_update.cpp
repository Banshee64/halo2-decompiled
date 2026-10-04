// @flags /O2 /Ob1 /Gr
/* the texture cache's per-frame update (unknown_12c0d0.cpp's state).
   Retail calls it out of line from its callers (bink playback, havok memory,
   the simulation world's buffers), so it lives in a file of its own built
   /Ob1, where LTCG doesn't inline it. */

#include "unknown_11c920.h"
#include <xtl.h>

extern dword g_4e6480;
extern long g_4e6484;

void texture_cache_update_locks(void);
void function_12c450(void);
void function_12c5b0(void);
void texture_cache_update_scale(void);

/* updates the locks and the loads, and every 200 milliseconds the scale */
// @retail 0x12c600
void function_12c600(void)
{
	texture_cache_update_locks();
	function_12c450();
	function_12c5b0();
	if (GetTickCount() > g_4e6480)
	{
		g_4e6480 = GetTickCount() + 200;
		texture_cache_update_scale();
	}
	g_4e6484 = 0;
}
