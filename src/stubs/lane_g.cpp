// stubs for the game functions outside 0x230000..0x23ffff that lane G's code
// calls and that are not decompiled yet (and a few of lane G's own, until
// they are written)
#include "cseries.h"
#include "screen_widget.h"
#include "unknown_234c64.h"

// @stub 0x22e957
void c_screen_widget::function_22e957(long a)
{
}

// @stub 0x1a4826
void __stdcall function_1a4826(void *pointer)
{
}

// @stub 0x1a47fd
void *__stdcall function_1a47fd(long size)
{
	return 0;
}

// @stub 0x148148
void function_148148(c_screen_widget *screen)
{
}

// @stub 0x147f4f
long function_147f4f(void)
{
	return 0;
}

// @stub 0x148350
s_user_interface_globals *function_148350(void)
{
	return 0;
}

// @stub 0x199d7c
long function_199d7c(void)
{
	return 0;
}

// @stub 0x19022f
word function_19022f(void)
{
	return 0;
}

// @stub 0x2b72e6
c_screen_widget *__stdcall function_2b72e6(s_screen_request *request)
{
	return 0;
}

// @stub 0x2b7333
c_screen_widget *__stdcall function_2b7333(s_screen_request *request)
{
	return 0;
}

// @stub 0x147cdb
void c_render_window::function_147cdb(dword color)
{
}

/* lane G's own, not written yet */

// @stub 0x235756
void function_235756(real fade)
{
}

// @stub 0x2359ce
void function_2359ce(c_window_channel_459a34 *channel)
{
}

// @stub 0x235abc
void function_235abc(c_window_channel_459a34 *channel)
{
}

// @stub 0x23029a
bool function_23029a(c_screen_widget *screen)
{
	return false;
}

// @stub 0x230374
real function_230374(c_screen_widget *screen)
{
	return 0.f;
}
