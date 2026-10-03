// stubs for the game functions outside 0x230000..0x23ffff that lane G's code
// calls and that are not decompiled yet (and a few of lane G's own, until
// they are written)
#include "cseries.h"
#include "screen_widget.h"
#include "unknown_234c64.h"

// @stub 0x22e957
void c_user_interface_widget::function_22e957(long a)
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

/* the screens' create functions (lane G, not written yet) */

// @stub 0x230616
c_screen_widget *__stdcall function_230616(s_screen_request *request)
{
	return 0;
}

// @stub 0x230691
c_screen_widget *__stdcall function_230691(s_screen_request *request)
{
	return 0;
}

// @stub 0x2310b7
c_screen_widget *__stdcall function_2310b7(s_screen_request *request)
{
	return 0;
}

// @stub 0x2312c2
c_screen_widget *__stdcall function_2312c2(s_screen_request *request)
{
	return 0;
}

// @stub 0x2313a8
c_screen_widget *__stdcall function_2313a8(s_screen_request *request)
{
	return 0;
}

// @stub 0x231995
c_screen_widget *__stdcall function_231995(s_screen_request *request)
{
	return 0;
}

// @stub 0x2320c4
c_screen_widget *__stdcall function_2320c4(s_screen_request *request)
{
	return 0;
}

// @stub 0x231db5
c_screen_widget *__stdcall function_231db5(s_screen_request *request)
{
	return 0;
}

// @stub 0x23252e
c_screen_widget *__stdcall function_23252e(s_screen_request *request)
{
	return 0;
}

// @stub 0x2325fb
c_screen_widget *__stdcall function_2325fb(s_screen_request *request)
{
	return 0;
}

// @stub 0x2323c3
c_screen_widget *__stdcall function_2323c3(s_screen_request *request)
{
	return 0;
}

// @stub 0x23246a
c_screen_widget *__stdcall function_23246a(s_screen_request *request)
{
	return 0;
}

// @stub 0x23334f
c_screen_widget *__stdcall function_23334f(s_screen_request *request)
{
	return 0;
}

// @stub 0x23764f
c_screen_widget *__stdcall function_23764f(s_screen_request *request)
{
	return 0;
}

// @stub 0x23784f
c_screen_widget *__stdcall function_23784f(s_screen_request *request)
{
	return 0;
}

// @stub 0x237713
c_screen_widget *__stdcall function_237713(s_screen_request *request)
{
	return 0;
}

/* callees of the screen widget code */

// @stub 0x22f583
c_screen_widget_member::c_screen_widget_member(long a)
{
}

struct s_screen_definition;

// @stub 0x22f871
s_screen_definition *function_22f871(c_widget *widget)
{
	return 0;
}

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

struct s_sound_play;

// @stub 0x189760
void function_189760(s_sound_play *play)
{
}

// @stub 0x1896c0
void function_1896c0(long tag_index, real scale)
{
}
