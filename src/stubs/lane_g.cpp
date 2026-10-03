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

/* UI lane round 4: callees of the campaign level select list and the game
   engine variant category list */

// @stub 0x215f40
bool __stdcall function_215f40(long game_engine, byte *buffer)
{
	return false;
}

// @stub 0x212380
long function_212380(long game_engine, long controller_index, byte *buffer)
{
	return 0;
}

// @stub 0x212bc0
bool function_212bc0(long file_index, s_game_variant *variant)
{
	return false;
}

// @stub 0x8c150
bool __stdcall function_8c150(void *cache, long a, unsigned __int64 *xuid, void *data, unsigned __int64 *clan_id)
{
	return false;
}

// @stub 0x2393ae
void __stdcall function_2393ae(long controller, long privilege)
{
}

// @stub 0x236964
bool __stdcall function_236964(long controller)
{
	return false;
}

// @stub 0x124770
bool function_124770(long profile_index)
{
	return false;
}

// @stub 0x163890
void __stdcall function_163890(char const *scenario_path, long a)
{
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

// @stub 0x231995
c_screen_widget *__stdcall function_231995(s_screen_parameters *request)
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

// @stub 0x125a90
void function_125a90(long value)
{
}

// @stub 0x2b24ff
void c_clan_options_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2b4e5a
void c_mp_change_teams_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x157a40
word function_157a40(void)
{
	return 0;
}

/* UI lane round 2: the custom game profile list (unknown_2c9ddb.cpp) */

// @stub 0x2ca284
void c_custom_game_profile_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2ca0d9
void c_custom_game_profile_list::fill()
{
}

// @stub 0x120e70
long __stdcall function_120e70(byte *buffer)
{
	return 0;
}

// @stub 0x215b50
word *function_215b50(long variant, word *buffer)
{
	return 0;
}
// @stub 0x149ef3
void __stdcall function_149ef3(long user_flags, screen_load_proc load)
{
}


// @stub 0x2305d0
void c_legalese_acceptance_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x230827
void c_main_menu_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2328b5
void c_mp_pause_game_list::handle_item(s_controller_reference **controller, long *item)
{
}
/* unknown_2b116a.cpp's list (only the member the stub defines) */
class c_potential_squad_leader_player_list
{
public:
	void handle_item(s_controller_reference **controller, long *item);
};

// @stub 0x2c9d38
void c_difficulty_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x2b8497
void c_potential_squad_leader_player_list::handle_item(s_controller_reference **controller, long *item)
{
}

/* UI lane round 3: callees of user_interface_text_parser.cpp */

// @stub 0x122dd0
real __stdcall function_122dd0(byte *map_name, long unknown)
{
	return 0.f;
}

// @stub 0x13934d
void function_13934d(long string_id, word *buffer)
{
}

// @stub 0x15ea80
void function_15ea80(long string_id, long maximum_count, word *buffer)
{
}

// @stub 0x1a33c4
void __stdcall function_1a33c4(dword *xuid, bool *a, bool *b, long *c, long *d, bool *e, long f)
{
}

// @stub 0x19a902
bool function_19a902(void)
{
	return false;
}

// @stub 0x19a8d0
long function_19a8d0(void)
{
	return 0;
}
/* UI lane round 3: callees of the actions list (unknown_2b116a.cpp) */

// @stub 0x19b5af
void __stdcall function_19b5af(long a, long message, long b, dword controller_flags, void *callback0, void *callback1, long c)
{
}

// @stub 0x19b590
void __stdcall function_19b590(long a, long b, dword controller_flags, void *callback, long c)
{
}

// @stub 0x236973
bool __stdcall function_236973(long controller)
{
	return true;
}

// @stub 0x236989
bool __stdcall function_236989(long controller)
{
	return true;
}
// @stub 0x190565
long function_190565()
{
	return 0;
}
