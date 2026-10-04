// @flags /O1 /Gr
/* UNKNOWN_23068B.CPP: the screens' create function getters (slot 26 of the
   screen vtables), one class per vtable until the screens are written */

#include "cseries.h"
#include <xtl.h>
#include "screen_widgets.h"
#include "user_interface_lists.h"
#include "unknown_234c64.h"
#include "globals.h"
#include "unknown_19b510.h"

void function_148a58();
void profile_edit_end();

c_screen_widget *__stdcall function_230616(s_screen_parameters *request);
c_screen_widget *__stdcall function_230691(s_screen_parameters *request);
c_screen_widget *__stdcall function_2310b7(s_screen_parameters *request);
c_screen_widget *__stdcall function_2312c2(s_screen_parameters *request);
c_screen_widget *__stdcall function_2313a8(s_screen_parameters *request);
c_screen_widget *__stdcall function_231995(s_screen_parameters *request);
c_screen_widget *__stdcall function_2320c4(s_screen_parameters *request);
c_screen_widget *__stdcall function_231db5(s_screen_parameters *request);
c_screen_widget *__stdcall function_23252e(s_screen_parameters *request);
c_screen_widget *__stdcall function_2325fb(s_screen_parameters *request);
c_screen_widget *__stdcall function_2323c3(s_screen_parameters *request);
c_screen_widget *__stdcall function_23246a(s_screen_parameters *request);
c_screen_widget *__stdcall function_23334f(s_screen_parameters *request);
c_screen_widget *__stdcall function_23764f(s_screen_parameters *request);
c_screen_widget *__stdcall function_23784f(s_screen_parameters *request);
c_screen_widget *__stdcall function_237713(s_screen_parameters *request);
c_screen_widget *__stdcall function_2312af(s_screen_parameters *request);

class c_screen_458a00 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

/* the settings screen (vtable 0x458ac8) */
class c_settings_screen : public c_screen_with_menu
{
public:
	c_settings_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_settings_list list;
};

/* the xbox live menu screen (vtable 0x458ba0) */
class c_xbox_live_menu_screen : public c_screen_with_menu
{
public:
	c_xbox_live_menu_screen(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	bool name_valid;
	s_text_256 name;
	c_xbox_live_menu_list list;
};

/* the pause game screen (vtable 0x458fa8) */
class c_pause_game_screen : public c_screen_with_menu
{
public:
	c_pause_game_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_pause_game_list list;
	s_text_256 text;
};

/* the multiplayer controller settings screen (vtable 0x4590b8; 0x459148
   shares its slot 10) */
class c_mp_controller_settings_screen : public c_screen_with_menu
{
public:
	c_mp_controller_settings_screen(long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	c_mp_controller_settings_game_list list;
};

/* the multiplayer player settings screen (vtable 0x459148) */
class c_mp_player_settings_screen : public c_screen_with_menu
{
public:
	c_mp_player_settings_screen(long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	c_mp_player_settings_game_list list;
};

/* the squad privacy screen (vtable 0x4591b8) */
class c_squad_privacy_screen : public c_screen_with_menu
{
public:
	c_squad_privacy_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_squad_privacy_setting_list list;
};

/* the handicap settings screen (vtable 0x459228) */
class c_handicap_settings_screen : public c_screen_with_menu
{
public:
	c_handicap_settings_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_handicap_settings_edit_list list;
};

/* the multiplayer change teams screen (vtable 0x459338) */
class c_mp_change_teams_screen : public c_screen_with_menu
{
public:
	c_mp_change_teams_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_mp_change_teams_list list;
};

class c_screen_4596e0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

/* the variant editing screen (vtable 0x459ae8) */
class c_variant_editing_screen : public c_screen_with_menu
{
public:
	c_variant_editing_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_variant_editing_options_list list;
};

class c_screen_459ba0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

/* the player profile editing screen (vtable 0x459c10) */
class c_profile_edit_menu_screen : public c_screen_with_menu
{
public:
	c_profile_edit_menu_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_player_profile_edit_list list;
	bool value9b4;
};

class c_screen_458c98 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x23068b
screen_load_proc c_screen_458a00::get_load_proc()
{
	return function_230616;
}

// @retail 0x230691
c_screen_widget *__stdcall function_230691(s_screen_parameters *parameters)
{
	c_settings_screen *screen = new c_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2306cf
c_settings_screen::c_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x13, a, b, user_flags, &list),
	list(user_flags)
{
	function_148a58();
	g_54d598.m1224 = NONE;
	g_54d598.m1220 = false;
}

// @retail 0x230714
screen_load_proc c_settings_screen::get_load_proc()
{
	return function_230691;
}

// @retail 0x23071a deleting c_settings_screen
// @retail 0x2326a7 destructor c_settings_screen
// @retail 0x232671 destructor c_settings_list
// @retail 0x14750b destructor c_list_item_widget

// @retail 0x230c87
screen_load_proc c_xbox_live_menu_screen::get_load_proc()
{
	return function_2310b7;
}

// @retail 0x230ea2
c_xbox_live_menu_list::c_xbox_live_menu_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_xbox_live_menu_list::handle_item)
{
	data = user_interface_data_new("xbox live menu list", 4, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

/* shows the item's text */
// @retail 0x230f36
void c_xbox_live_menu_list::v20(c_user_interface_widget *item, long unused)
{
	long datum = ((c_list_item_widget *)item)->value70;

	if (datum != NONE)
	{
		c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)item->find_child(6, 0, false);
		if (text)
		{
			long string_id;

			switch (datum & 0xffff)
			{
			case 0:
				string_id = 0xa00041f;
				break;
			case 1:
				string_id = 0x9000420;
				break;
			case 2:
				string_id = 0xb000421;
				break;
			case 3:
				string_id = 0x10000445;
				break;
			default:
				string_id = 0;
				break;
			}
			text->set_string(string_id);
		}
	}
}

// @retail 0x2310b7
c_screen_widget *__stdcall function_2310b7(s_screen_parameters *parameters)
{
	c_xbox_live_menu_screen *screen = new c_xbox_live_menu_screen(parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x2310f9
c_xbox_live_menu_screen::c_xbox_live_menu_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xba, a, b, user_flags, &list),
	name_valid(false),
	list(user_flags)
{
}

// @retail 0x23113d deleting c_xbox_live_menu_screen
// @retail 0x23115b destructor c_xbox_live_menu_screen
// @retail 0x2325c5 destructor c_xbox_live_menu_list

/* the last item chosen in the xbox live menu */
long g_510a18;

extern bool g_510819;
bool g_51ec98;

struct s_game_variant_block
{
	byte data[0x15cb8];
};

long function_193f50(void);
bool function_193f70(void *value);
long function_1902de(long index);
bool game_variant_block_read(long index, s_game_variant_block *block);
void function_199e2e(bool close);
bool network_session_manager_host_session(long mode, const XNKID *kid, const XNKEY *key);
void function_199a57(void);
void function_199a03(long mode);
bool network_session_interface_set_value49a4(long value);
void function_148cfc(long value);
void function_19a0af(long value);
void function_149f1e(word user_flags, long load);
c_screen_widget *__stdcall function_230c8d(s_screen_parameters *parameters);
void function_22387b(void);

// @retail 0x230e98
bool __stdcall function_230e98(long controller_index)
{
	function_22387b();
	return true;
}

// @retail 0x230f92
void c_xbox_live_menu_list::handle_item(s_controller_reference **controller, long *item)
{
	s_game_variant_block block;

	if (*item != NONE)
	{
		long index = *item & 0xffff;

		if (index != 3)
		{
			g_510a18 = index;
			g_510819 = true;
		}
		switch (index)
		{
		case 0:
			if (function_193f50())
			{
				long player = function_1902de((*controller)->controller_index);

				if (game_variant_block_read(player, &block))
				{
					function_199e2e(true);
					if (network_session_manager_host_session(2, NULL, NULL))
					{
						function_199a57();
						function_199a03(3);
						network_session_interface_set_value49a4(2);
						function_148cfc(player);
						function_19a0af(player);
						g_51ec98 = true;
					}
				}
			}
			else
			{
				function_193f70(NULL);
			}
			break;
		case 1:
			if (function_193f50())
			{
				function_149f1e(1 << (*controller)->controller_index, (long)function_230c8d);
			}
			else
			{
				function_193f70(NULL);
			}
			break;
		case 2:
			function_199e2e(true);
			if (network_session_manager_host_session(2, NULL, NULL))
			{
				function_199a03(2);
			}
			break;
		case 3:
			dialog_choice_show(1, 0xa3, 4, 1 << (*controller)->controller_index, function_230e98, 0, 0);
			break;
		}
	}
}

word function_1901fc(void);

// @retail 0x231170
void c_xbox_live_menu_screen::v19()
{
	g_54d598.m1224 = NONE;
	g_54d598.m1220 = false;
	set_user_flags(function_1901fc());
	c_screen_widget::v19();
	if (g_510a18 >= 0 && g_510a18 < 4)
	{
		list.select_item((short)g_510a18);
	}
}

// @retail 0x2312c2
c_screen_widget *__stdcall function_2312c2(s_screen_parameters *parameters)
{
	c_clan_options_screen *screen = new c_clan_options_screen(parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x231304
c_clan_options_screen::c_clan_options_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xd9, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x23133f deleting c_clan_options_screen
// @retail 0x231393 destructor c_clan_options_screen
// @retail 0x23135d destructor c_clan_options_list

// @retail 0x231339
screen_load_proc c_clan_options_screen::get_load_proc()
{
	return function_2312c2;
}

// @retail 0x2313a8
c_screen_widget *__stdcall function_2313a8(s_screen_parameters *parameters)
{
	c_friends_options_screen *screen = new c_friends_options_screen(parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x2313ea
c_friends_options_screen::c_friends_options_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xd8, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x231425 deleting c_friends_options_screen
// @retail 0x231479 destructor c_friends_options_screen
// @retail 0x231443 destructor c_friends_options_list

// @retail 0x23141f
screen_load_proc c_friends_options_screen::get_load_proc()
{
	return function_2313a8;
}

// @retail 0x231e2e
c_pause_game_list_item::c_pause_game_list_item()
{
}

// @retail 0x231e5c
bool c_pause_game_list_item::v10(s_widget_event *event)
{
	if (event->type == 5 && event->param == 12)
	{
		get_screen()->start_animation(3);
		return true;
	}
	return c_list_item_widget::v10(event);
}

// @retail 0x231e88
c_pause_game_list::c_pause_game_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_pause_game_list::handle_item)
{
	data = user_interface_data_new("pause game list", 5, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

long game_time_get_paused(void);
void function_125a90(long value);

// @retail 0x2320c4
c_screen_widget *__stdcall function_2320c4(s_screen_parameters *parameters)
{
	c_pause_game_screen *screen = new c_pause_game_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x23203e
c_pause_game_screen::c_pause_game_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x12, a, b, user_flags, &list),
	list(user_flags)
{
	if (!game_time_get_paused())
	{
		g_510c54->unknown01 = true;
		function_125a90(0);
	}
}

// @retail 0x231daf
screen_load_proc c_pause_game_screen::get_load_proc()
{
	return function_2320c4;
}

// @retail 0x231db5
c_screen_widget *__stdcall function_231db5(s_screen_parameters *parameters)
{
	c_mp_controller_settings_screen *screen = new c_mp_controller_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x231df3
c_mp_controller_settings_screen::c_mp_controller_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xc8, a, b, user_flags, &list),
	list(user_flags)
{
}


/* leaving the screen stops editing the profile */
// @retail 0x2b51b7
bool c_mp_controller_settings_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
			if (g_54e5d0.profile_index != NONE)
				profile_edit_end();
			break;
		}
	}
	return c_screen_widget::v10(event);
}

// @retail 0x1475a9 deleting c_mp_controller_settings_screen
// @retail 0x232519 destructor c_mp_controller_settings_screen

// @retail 0x231e28
screen_load_proc c_mp_controller_settings_screen::get_load_proc()
{
	return function_231db5;
}

// @retail 0x23252e
c_screen_widget *__stdcall function_23252e(s_screen_parameters *parameters)
{
	c_mp_player_settings_screen *screen = new c_mp_player_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x23256c
c_mp_player_settings_screen::c_mp_player_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xc4, a, b, user_flags, &list),
	list(user_flags)
{
}

/* retail folded this with c_mp_controller_settings_screen::v10 (0x2b51b7) */
bool c_mp_player_settings_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
			if (g_54e5d0.profile_index != NONE)
				profile_edit_end();
			break;
		}
	}
	return c_screen_widget::v10(event);
}

// @retail 0x2325a1
screen_load_proc c_mp_player_settings_screen::get_load_proc()
{
	return function_23252e;
}

// @retail 0x2325fb
c_screen_widget *__stdcall function_2325fb(s_screen_parameters *parameters)
{
	c_squad_privacy_screen *screen = new c_squad_privacy_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x232639
c_squad_privacy_screen::c_squad_privacy_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x19, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x23266b
screen_load_proc c_squad_privacy_screen::get_load_proc()
{
	return function_2325fb;
}

// @retail 0x2323c3
c_screen_widget *__stdcall function_2323c3(s_screen_parameters *parameters)
{
	c_handicap_settings_screen *screen = new c_handicap_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x232401
c_handicap_settings_screen::c_handicap_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x2d, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2325a7 deleting c_handicap_settings_screen
// @retail 0x2bb4bd destructor c_handicap_settings_screen

// @retail 0x232433
screen_load_proc c_handicap_settings_screen::get_load_proc()
{
	return function_2323c3;
}

// @retail 0x23246a
c_screen_widget *__stdcall function_23246a(s_screen_parameters *parameters)
{
	c_mp_change_teams_screen *screen = new c_mp_change_teams_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2324a8
c_mp_change_teams_screen::c_mp_change_teams_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xc6, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2324dd
screen_load_proc c_mp_change_teams_screen::get_load_proc()
{
	return function_23246a;
}

// @retail 0x232d4e
screen_load_proc c_screen_4596e0::get_load_proc()
{
	return function_23334f;
}

// @retail 0x23764f
c_screen_widget *__stdcall function_23764f(s_screen_parameters *parameters)
{
	c_variant_editing_screen *screen = new c_variant_editing_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x23768d
c_variant_editing_screen::c_variant_editing_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xb2, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b778a deleting c_variant_editing_screen
// @retail 0x2376fe destructor c_variant_editing_screen
// @retail 0x2376c8 destructor c_variant_editing_options_list

// @retail 0x2376c2
screen_load_proc c_variant_editing_screen::get_load_proc()
{
	return function_23764f;
}

// @retail 0x237649
screen_load_proc c_screen_459ba0::get_load_proc()
{
	return function_23784f;
}

// @retail 0x237713
c_screen_widget *__stdcall function_237713(s_screen_parameters *parameters)
{
	c_profile_edit_menu_screen *screen = new c_profile_edit_menu_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->value9b4 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2ba666
c_screen_widget *__stdcall function_2ba666(s_screen_parameters *parameters)
{
	c_profile_edit_menu_screen *screen = new c_profile_edit_menu_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x237758
c_profile_edit_menu_screen::c_profile_edit_menu_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x26, a, b, user_flags, &list),
	list(user_flags)
{
	value9b4 = false;
}

// @retail 0x237791
screen_load_proc c_profile_edit_menu_screen::get_load_proc()
{
	return function_237713;
}

// @retail 0x232d48
screen_load_proc c_screen_458c98::get_load_proc()
{
	return function_2312af;
}

/* the create function of the screens that cannot be created */
// @retail 0x2312af
c_screen_widget *__stdcall function_2312af(s_screen_parameters *request)
{
	return 0;
}

/* the pause game list's item texts */
short player_slot_count_active(void);

// @retail 0x231f6c
void c_pause_game_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch ((short)widget_item(widget)->value70)
		{
		case 0:
			string_id = 0x8000144;
			break;
		case 1:
			string_id = 0x130002ea;
			break;
		case 2:
			string_id = 0xd0002eb;
			break;
		case 3:
			string_id = 0x130002ef;
			break;
		case 4:
			string_id = player_slot_count_active() > 1 ? 0x40002ed : 0xd0002ec;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}
