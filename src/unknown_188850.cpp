// @flags /O2 /Gr
/* UNKNOWN_188850.CPP: an object looping sounds lifecycle callback (entry 46, dispose) */

#include "cseries.h"

void *g_4ed28c;

// @retail 0x188850
void object_looping_sounds_dispose(void)
{
	if (g_4ed28c)
	{
		g_4ed28c = 0;
	}
}
