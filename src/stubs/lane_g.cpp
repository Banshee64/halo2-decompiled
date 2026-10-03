// stubs for the game functions outside 0x230000..0x23ffff that lane G's code
// calls and that are not decompiled yet (and a few of lane G's own, until
// they are written)
#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_234c64.h"
#include "user_interface_lists.h"

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

// @stub 0x2312c2
c_screen_widget *__stdcall function_2312c2(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x231995
c_screen_widget *__stdcall function_231995(s_screen_parameters *request)
{
	return 0;
}

// @stub 0x23252e
c_screen_widget *__stdcall function_23252e(s_screen_parameters *request)
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

// @stub 0x23784f
c_screen_widget *__stdcall function_23784f(s_screen_parameters *request)
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

struct s_bitmap_view;

// @stub 0x12360
void function_12360(s_bitmap_view *bitmap, real priority)
{
}

// @stub 0x230f92
void c_xbox_live_menu_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2b4a4d
void c_mp_controller_settings_game_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2b4c45
void c_handicap_settings_edit_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2b75e8
void c_variant_editing_options_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2b79f9
void c_player_profile_edit_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2b2b40
void c_friends_options_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x231fe3
void c_pause_game_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x146840
long function_146840(void)
{
	return 0;
}

// @stub 0x125a90
void function_125a90(long value)
{
}
