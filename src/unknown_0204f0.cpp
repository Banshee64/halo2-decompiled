// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0204F0.CPP: starts a timed effect value moving toward a new value
   (an outside function lane A's script functions need) */

#include "unknown_11c920.h"
#include "globals.h"
#include "timed_effect.h"

real function_020560(long index);

/* moves timed value index from its current value to value over duration
   seconds */
// @retail 0x204f0
void function_0204f0(long index, real value, real duration)
{
	s_timed_effect_globals *globals = g_5093e0;

	if (globals && index >= 0 && index < 32)
	{
		globals->values[index][0] = duration == 0.0f ? value : function_020560(index);
		globals->times[index][0] = (real)g_4858a0;
		globals->values[index][1] = value;
		globals->times[index][1] = (real)(duration + g_4858a0);
	}
}
