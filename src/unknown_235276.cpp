// @flags /O1 /Oi /arch:SSE /Gr /GL-
/* UNKNOWN_235276.CPP: the window channel's input tests, built without
   link-time code generation: retail keeps their thiscall convention (this in
   ecx, the index on the stack) where LTCG would move this into a register */

#include "unknown_11c920.h"
#include "unknown_234c64.h"

// @retail 0x235276
bool c_window_channel::function_235276(long user_index)
{
	c_class_1473c9 *screen = focus;
	bool result = false;
	if (screen)
		result = ((1 << user_index) & (short)screen->user_flags) != 0;
	return result;
}

// @retail 0x235294
bool c_window_channel::function_235294(long controller)
{
	c_class_1473c9 *screen = focus;
	bool result = false;
	if (screen)
	{
		short mask = screen->user_flags;
		if (mask != NONE && ((1 << controller) & mask))
			result = true;
		else
			result = false;
	}
	return result;
}
