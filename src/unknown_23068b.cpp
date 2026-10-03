// @flags /O1 /Gr
/* UNKNOWN_23068B.CPP: the screens' create function getters (slot 26 of the
   screen vtables), one class per vtable until the screens are written */

#include "cseries.h"
#include "screen_widgets.h"
#include "user_interface_lists.h"
#include "unknown_234c64.h"

void function_148a58();

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

/* a text buffer of 0x100 characters, empty when constructed */
struct s_text_256
{
	s_text_256()
	{
		text[0] = 0;
	}

	word text[0x100];
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

class c_screen_458d08 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458de8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458e58 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

class c_screen_458fa8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
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

class c_screen_459148 : public c_screen_with_menu
{
public:
	virtual screen_load_proc get_load_proc();
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

class c_screen_459338 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
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

// @retail 0x231339
screen_load_proc c_screen_458d08::get_load_proc()
{
	return function_2312c2;
}

// @retail 0x23141f
screen_load_proc c_screen_458de8::get_load_proc()
{
	return function_2313a8;
}

// @retail 0x2312bc
screen_load_proc c_screen_458e58::get_load_proc()
{
	return function_231995;
}

// @retail 0x231daf
screen_load_proc c_screen_458fa8::get_load_proc()
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

void profile_edit_end();

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

// @retail 0x231e28
screen_load_proc c_mp_controller_settings_screen::get_load_proc()
{
	return function_231db5;
}

// @retail 0x2325a1
screen_load_proc c_screen_459148::get_load_proc()
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

// @retail 0x232433
screen_load_proc c_handicap_settings_screen::get_load_proc()
{
	return function_2323c3;
}

// @retail 0x2324dd
screen_load_proc c_screen_459338::get_load_proc()
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
