// stubs for the game functions outside 0x230000..0x23ffff that lane G's code
// calls and that are not decompiled yet (and a few of lane G's own, until
// they are written)
#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_234c64.h"

// @stub 0x199d7c
long function_199d7c(void)
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

/* the screens' create functions (lane G, not written yet) */

// @stub 0x230616
c_screen_widget *__stdcall function_230616(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x2310b7
c_screen_widget *__stdcall function_2310b7(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x2312c2
c_screen_widget *__stdcall function_2312c2(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x2313a8
c_screen_widget *__stdcall function_2313a8(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x231995
c_screen_widget *__stdcall function_231995(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x2320c4
c_screen_widget *__stdcall function_2320c4(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x231db5
c_screen_widget *__stdcall function_231db5(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x23252e
c_screen_widget *__stdcall function_23252e(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x2323c3
c_screen_widget *__stdcall function_2323c3(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x23246a
c_screen_widget *__stdcall function_23246a(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x23334f
c_screen_widget *__stdcall function_23334f(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x23764f
c_screen_widget *__stdcall function_23764f(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x23784f
c_screen_widget *__stdcall function_23784f(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x237713
c_screen_widget *__stdcall function_237713(s_screen_parameters *request)
{
	return 0;
}

/* callees of the screen widget code */

// @stub 0x22fba9
void function_22fba9(c_screen_widget *screen)
{
}

// @stub 0x22fc08
void __stdcall function_22fc08(c_screen_widget *screen)
{
}

// @stub 0x11cae0
long function_11cae0(void)
{
	return 0;
}

// @stub 0x219070
byte __stdcall function_219070(long set_index)
{
	return 0;
}

// @stub 0x215367
void __stdcall function_215367(long player, long profile_index, void *data, long flags)
{
}
