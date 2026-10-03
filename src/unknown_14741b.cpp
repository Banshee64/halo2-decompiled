// @flags /O1 /Oi /Gr
/* UNKNOWN_14741B.CPP: the legal notice, main menu and multiplayer pause
   screens, which the window manager loads itself */

#include "cseries.h"
#include "screen_widgets.h"
#include "user_interface_lists.h"
#include "user_interface_controller_sign_in.h"
#include "unknown_19b510.h"
#include "unknown_19b516.h"

/* the legal notice screen (vtable 0x4537b8; its deleting destructor is
   folded with the appear offline screen's) */
class c_legalese_screen : public c_screen_with_menu
{
public:
	c_legalese_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_legalese_acceptance_list list;
};

/* the main menu screen (vtable 0x453828; its deleting destructor is folded
   with the multiplayer controller settings screen's) */
class c_main_menu_screen : public c_screen_with_menu
{
public:
	c_main_menu_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_main_menu_list list;
	long value934;
};

/* the multiplayer pause screen (vtable 0x453898) */
class c_mp_pause_game_screen : public c_screen_with_menu
{
public:
	c_mp_pause_game_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_mp_pause_game_list list;
};

// @retail 0x14741b
c_screen_widget *__stdcall function_14741b(s_screen_parameters *parameters)
{
	c_legalese_screen *screen = new c_legalese_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x147459
c_legalese_screen::c_legalese_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xcb, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x14748e
screen_load_proc c_legalese_screen::get_load_proc()
{
	return function_14741b;
}

// @retail 0x14752c
c_screen_widget *__stdcall function_14752c(s_screen_parameters *parameters)
{
	c_main_menu_screen *screen = new c_main_menu_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x14756a
c_main_menu_screen::c_main_menu_screen(long a, long b, word user_flags) :
	c_screen_with_menu(6, a, b, user_flags, &list),
	list(user_flags),
	value934(NONE)
{
}

// @retail 0x1475a3
screen_load_proc c_main_menu_screen::get_load_proc()
{
	return function_14752c;
}

// @retail 0x1475c7
c_screen_widget *__stdcall function_1475c7(s_screen_parameters *parameters)
{
	c_mp_pause_game_screen *screen = new c_mp_pause_game_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x147605
c_mp_pause_game_screen::c_mp_pause_game_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xc3, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x14763a
screen_load_proc c_mp_pause_game_screen::get_load_proc()
{
	return function_1475c7;
}

// @retail 0x232091 deleting c_mp_pause_game_screen
// @retail 0x2320af destructor c_mp_pause_game_screen
// @retail 0x231f36 destructor c_mp_pause_game_list

/* the lists (in 0x2304d2..0x232708 in retail) */

// @retail 0x2304d2
c_legalese_acceptance_list::c_legalese_acceptance_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_legalese_acceptance_list::handle_item)
{
	data = user_interface_data_new("legalese acceptance list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
	accepted = false;
}

// @retail 0x230738
c_main_menu_list::c_main_menu_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_main_menu_list::handle_item)
{
	data = user_interface_data_new("main menu list", 5, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x231f18 deleting c_mp_pause_game_list

// @retail 0x230591
void c_legalese_acceptance_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0xe000223;
			break;
		case 1:
			string_id = 0x7000224;
			break;
		default:
			string_id = 0;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2307c8
void c_main_menu_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0x800010b;
			break;
		case 1:
			string_id = 0x9000283;
			break;
		case 2:
			string_id = 0xb000284;
			break;
		case 3:
			string_id = 0xa000285;
			break;
		case 4:
			string_id = 0x8000286;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

struct s_player_profile
{
	dword data[0x78];
};

bool function_19028d(void);
bool function_8d7c0(void);
bool function_6c7e0();
word function_1901fc(void);
void function_1906b4(void);
void __stdcall function_18f1c0(long a);
void player_slot_get_profile(long index, s_player_profile *profile, long *profile_index);
c_screen_widget *__stdcall function_230616(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_230691(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_24b4a9(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_25240c(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_252433(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_25245a(s_screen_parameters *parameters);
bool __stdcall function_236877(long controller_index);
bool __stdcall function_2368c1(long controller_index);
bool __stdcall function_236917(long controller_index);

extern bool g_54d5a0;

/* the main menu's last choice */
long g_510a14;
bool g_510819;
bool g_54e7cd;

/* the campaign */
// @retail 0x230888
void function_230888(s_controller_reference **controller)
{
	if (g_54d5a0)
	{
		dialog_ok_show(1, 0x32, 4, 1 << (*controller)->controller_index, 0, 0);
	}
	else
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_230616);
		parameters.load(&parameters);
		g_54e7cd = true;
	}
}

/* Xbox Live: signs the controller's player in first when it has a profile */
// @retail 0x2308e0
void __stdcall function_2308e0(c_main_menu_list *list, s_controller_reference **controller)
{
	s_player_profile profile;
	s_screen_parameters parameters;

	parameters.field_c = 0;
	if (function_19028d())
	{
		function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 5, 4, (long)function_25245a);
		parameters.load(&parameters);
	}
	else if (function_8d7c0())
	{
		c_main_menu_screen *screen = (c_main_menu_screen *)list->get_screen();
		s_controller_reference *reference = *controller;
		s_player_slot_profile *slot_profile = player_slot_profile_get(reference->controller_index);
		long profile_index;

		player_slot_get_profile(reference->controller_index, &profile, &profile_index);
		if (profile_index != NONE)
		{
			slot_profile->initialize(reference->controller_index);
			slot_profile->set_profile_index(profile_index);
			function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_24b4a9);
			parameters.load(&parameters);
			screen->value934 = (*controller)->controller_index;
		}
		else
		{
			function_1906b4();
			function_18f1c0(0);
		}
	}
	else
	{
		dialog_choice_show(1, 0x23, 4, 1 << (*controller)->controller_index, function_236917, 0, 0);
	}
}

/* split screen */
// @retail 0x2309ec
void function_2309ec(s_controller_reference **controller)
{
	if (g_54d5a0)
	{
		dialog_ok_show(1, 0x32, 4, 1 << (*controller)->controller_index, 0, 0);
	}
	else
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		if (function_6c7e0())
		{
			dialog_choice_show(3, 0x78, 4, 1 << (*controller)->controller_index, function_236877, 0, 0);
		}
		else
		{
			function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 5, 4, (long)function_25240c);
			parameters.load(&parameters);
		}
	}
}

/* system link */
// @retail 0x230a64
void function_230a64(s_controller_reference **controller)
{
	bool connected = function_8d7c0();

	if (g_54d5a0)
	{
		dialog_ok_show(1, 0x32, 4, 1 << (*controller)->controller_index, 0, 0);
	}
	else
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		if (function_6c7e0())
		{
			dialog_choice_show(3, 0x78, 4, 1 << (*controller)->controller_index, function_2368c1, 0, 0);
		}
		else if (connected)
		{
			function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 5, 4, (long)function_252433);
			parameters.load(&parameters);
		}
		else
		{
			dialog_ok_show(1, 0x3b, 4, function_1901fc(), 0, 0);
		}
	}
}

/* the settings */
// @retail 0x230aff
void function_230aff(s_controller_reference **controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 5, 4, (long)function_230691);
	parameters.load(&parameters);
}

// @retail 0x230827
void c_main_menu_list::handle_item(s_controller_reference **controller, long *item)
{
	long index = *item & 0xffff;

	g_510a14 = index;
	g_510819 = true;
	switch (index)
	{
	case 0:
		function_230888(controller);
		break;
	case 1:
		function_2308e0(this, controller);
		break;
	case 2:
		function_2309ec(controller);
		break;
	case 3:
		function_230a64(controller);
		break;
	case 4:
		function_230aff(controller);
		break;
	}
}

// @retail 0x23284e
void c_mp_pause_game_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[6];

	table[0].item = 0;
	table[0].string_id = 0xa0001b7;
	table[1].item = 1;
	table[1].string_id = 0x8000286;
	table[2].item = 2;
	table[2].string_id = 0xc0001b9;
	table[3].item = 3;
	table[3].string_id = 0xc0001ba;
	table[4].item = 4;
	table[4].string_id = 0x1c0001bb;
	table[5].item = 5;
	table[5].string_id = 0x80001bd;
	function_24c75c(this, item, table, 0, 6);
}
