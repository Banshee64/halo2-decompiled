// @flags /O1 /Oi /Gr
/* UNKNOWN_14741B.CPP: the legal notice, main menu and multiplayer pause
   screens, which the window manager loads itself */

#include "cseries.h"
#include "screen_widgets.h"
#include "user_interface_lists.h"

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
	accepted = false;
	data = user_interface_data_new("legalese acceptance list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
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

// @retail 0x232708
c_mp_pause_game_list::c_mp_pause_game_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_mp_pause_game_list::handle_item)
{
	data = user_interface_data_new("mp pause game list", 6, 4);
	data_make_valid(data);
	list_item_add(this, 0);
	list_item_add(this, 1);
	list_item_add(this, 2);
	list_item_add(this, 3);
	list_item_add(this, 4);
	list_item_add(this, 5);
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
