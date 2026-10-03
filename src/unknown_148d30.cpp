// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_148D30.CPP: the window manager's value at +0x1224, set by
   function_148cfc (unknown_147f6d.cpp) */

#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_234c64.h"

/* the value, or NONE before it is set */
// @retail 0x148d30
long function_148d30()
{
	long result = NONE;

	if (g_54d598.m1220)
	{
		result = g_54d598.m1224;
	}
	return result;
}
