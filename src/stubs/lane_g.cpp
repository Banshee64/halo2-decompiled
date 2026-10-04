// stubs for the game functions outside 0x230000..0x23ffff that lane G's code
// calls and that are not decompiled yet (and a few of lane G's own, until
// they are written)
#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_234c64.h"
#include "user_interface_lists.h"
#include "screen_online_y_menu.h"

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

// @stub 0x2c6ecf
void c_xbox_live_message_list::handle_item(s_controller_reference **controller, long *item)
{
}

// @stub 0x252ed8
void __stdcall function_252ed8(void *list)
{
}

// @stub 0x2b2181
void __stdcall function_2b2181(void *list, long controller_index)
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

/* lane H's 0x193f70 (it takes its argument in ecx) and lane K's 0x22387b */
// @stub 0x193f70
bool function_193f70(void *value)
{
	return false;
}

// @stub 0x22387b
void function_22387b(void)
{
}

// @stub 0x125a90
void function_125a90(long value)
{
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

// @stub 0x2305d0
void c_legalese_acceptance_list::handle_item(s_controller_reference **controller, long *item)
{
}

/* unknown_2b116a.cpp's list (only the member the stub defines) */
class c_potential_squad_leader_player_list
{
public:
	void handle_item(s_controller_reference **controller, long *item);
};

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

// @stub 0x15ea80
void function_15ea80(long string_id, long maximum_count, word *buffer)
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
// @stub 0x148523
void function_148523()
{
}

// @stub 0x805e0
bool function_805e0(s_recent_player *player, long *iterator)
{
	return false;
}

// @stub 0x1a31ff
void function_1a31ff()
{
}

// @stub 0x1a303b
void function_1a303b(long controller_index)
{
}

/* UI lane round 5: callees of the press start screen */

// @stub 0x23699f
bool __stdcall function_23699f(void *data)
{
	return false;
}

// @stub 0x19a02d
void __stdcall function_19a02d(long *string_id, real *progress)
{
}

/* lane M */
struct s_player_profile_settings;
// @stub 0x1a0540
bool function_1a0540(s_player_profile_settings *settings, long profile_index)
{
	return false;
}

/* the open region 0x180000..0x18ffff (lane F, paused) */
// @stub 0x18fb34
void __stdcall function_18fb34(long player, s_player_profile_settings *settings, long profile_index)
{
}

/* lane D */
struct _XONLINE_USER;
// @stub 0x6c8b0
long function_6c8b0(_XONLINE_USER *user, long player)
{
	return 0;
}

/* my own, not written yet */
// @stub 0x24b869
void __stdcall function_24b869(c_screen_widget *screen)
{
}

// @stub 0x24b407
bool __stdcall function_24b407(long controller_index)
{
	return false;
}

/* my own, the main menu's dialog callbacks, not written yet */
// @stub 0x236877
bool __stdcall function_236877(long controller_index)
{
	return false;
}

// @stub 0x2368c1
bool __stdcall function_2368c1(long controller_index)
{
	return false;
}

// @stub 0x236917
bool __stdcall function_236917(long controller_index)
{
	return false;
}

/* lane D */
// @stub 0x6cc10
long __stdcall function_6cc10(long controller_index)
{
	return 0;
}

// @stub 0x236937
bool __stdcall function_236937(long controller_index)
{
	return false;
}

/* UI lane round 7: my own, not written yet */
struct _XONLINE_FRIEND;
// @stub 0x2395dc
void __stdcall function_2395dc(_XONLINE_FRIEND *friend_, long controller_index, long mode)
{
}

/* lane D: the message blocks (screen_xbox_live_message_send.cpp) */
struct s_state_block;
struct _XUID;

// @stub 0x8fa30
void function_08fa30(s_state_block *block)
{
}

// @stub 0x8eff0
long function_08eff0(s_state_block *block, long controller_index, _XUID const *recipients, long recipient_count)
{
	return 0;
}

// @stub 0x8ef90
long function_08ef90(s_state_block *block, long controller_index, const char *gamertag)
{
	return 0;
}

