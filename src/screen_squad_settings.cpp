#include <string.h>
#include "cseries.h"
#include "globals.h"
#include "screen_widgets.h"
#include "unknown_19b510.h"
#include "user_interface_lists.h"
#include "unknown_19b516.h"

// @flags /O1 /Oi /Gr

/* SCREEN_SQUAD_SETTINGS.CPP: the squad settings screen (vtable 0x45c518)
   and its list of what the squad's leader can change (named after the debug
   build's source file) */

struct s_message;
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e);
byte *network_session_interface_get_data_4db0(void);
long function_19989d(void);
long function_1480ff(long screen_id);
bool function_1999b3(void);
void function_199a57(void);
void function_199a03(long mode);
void function_19a864(void);
bool function_19a76d(short index);
void function_121100(long *value);
void function_148d42(long value);
void dialog_ok_show(long a, long dialog_id, long b, word user_flags, dialog_choice_callback chosen, dialog_closed_callback closed);
void function_2c83e6(long type, long a, long b, word user_flags);

/* the squad settings that open a screen (unknown_2b116a.cpp) */
void function_2bb8ac(s_controller_reference **controller);
void function_2bb8df(s_controller_reference **controller);
void function_2bb912(s_controller_reference **controller);
void function_2bb945(s_controller_reference **controller);
void function_2bb9ce(s_controller_reference **controller);
void function_2bbf06(long controller_index, bool alternate);

extern bool g_54d5a0;
extern bool g_54e7cc;

/* "squad setting list" (vtable 0x45c5f8) */
class c_squad_setting_list : public c_list_widget
{
public:
	c_squad_setting_list(word user_flags);

	/* a press of X opens the variant's settings */
	virtual bool v10(s_widget_event *event);
	virtual long get_item_count();
	virtual void v20(c_user_interface_widget *item, long unused);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[7];
	c_list_item_handler handler;
};

/* the squad settings screen (vtable 0x45c518) */
class c_squad_settings_screen : public c_screen_widget
{
public:
	c_squad_settings_screen(long a, long b, word user_flags);

	/* a press of Y opens the squad privacy setting */
	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	c_squad_setting_list list;
};

// @retail 0x2bbacb
c_screen_widget *__stdcall function_2bbacb(s_screen_parameters *parameters)
{
	c_squad_settings_screen *screen = new c_squad_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2bbb07
c_squad_settings_screen::c_squad_settings_screen(long a, long b, word user_flags) :
	c_screen_widget(0x17, a, b, user_flags),
	list(user_flags)
{
}

// @retail 0x2bbb3a deleting c_squad_settings_screen
// @retail 0x2bbb58 destructor c_squad_settings_screen

/* builds the screen around its list; the text says whether the user leads
   the squad */
// @retail 0x2bbb6d
void c_squad_settings_screen::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, &list, 0 }
		}
	};
	c_text_widget_45a5e0 *text;

	build(&layout);
	v7(&list);
	c_user_interface_widget::v1();
	text = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	if (text)
	{
		if (function_1999b3())
		{
			text->set_string(0xb0005f9);
		}
		else
		{
			text->set_string(0xa0005f8);
		}
	}
}

// @retail 0x2bb299
screen_load_proc c_squad_settings_screen::get_load_proc()
{
	return function_2bbacb;
}

// @retail 0x2bbed0
bool c_squad_settings_screen::v10(s_widget_event *event)
{
	if (event->type == 5 && event->param == 2 && !function_1999b3())
	{
		function_2bb9ce((s_controller_reference **)&event);
		return true;
	}
	return c_screen_widget::v10(event);
}

/* the setting the list's focused item stands for, or NONE */
// @retail 0x2bbaad
long function_2bbaad(c_squad_setting_list *list)
{
	s_list_item_datum *datum = (s_list_item_datum *)datum_get(list->data, list->get_focused_datum());

	if (datum)
	{
		return datum->item;
	}
	return NONE;
}

// @retail 0x2bb4d2
c_squad_setting_list::c_squad_setting_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_squad_setting_list::handle_item)
{
	long state = function_19989d();

	data = user_interface_data_new("squad setting list", 10, 4);
	data_make_valid(data);
	switch (state)
	{
	case 0:
	case 2:
		list_item_add(this, 2);
		list_item_add(this, 3);
		list_item_add(this, 7);
		break;
	case 1:
	case 3:
		list_item_add(this, 0);
		list_item_add(this, 1);
		list_item_add(this, 4);
		list_item_add(this, 6);
		break;
	case 4:
		list_item_add(this, 2);
		list_item_add(this, 3);
		list_item_add(this, 7);
		list_item_add(this, 8);
		break;
	case 5:
		list_item_add(this, 0);
		list_item_add(this, 1);
		list_item_add(this, 4);
		list_item_add(this, 6);
		list_item_add(this, 8);
		break;
	case 6:
		list_item_add(this, 9);
		list_item_add(this, 6);
		list_item_add(this, 7);
		break;
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2bb295
long c_squad_setting_list::get_item_count()
{
	return 7;
}

// @retail 0x2bb74f
void c_squad_setting_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[9];

	table[0].item = 0;
	table[0].string_id = 0xa0005d3;
	table[1].item = 1;
	table[1].string_id = 0xe0005d4;
	table[2].item = 2;
	table[2].string_id = 0xc0005d5;
	table[3].item = 3;
	table[3].string_id = 0x110005d6;
	table[4].item = 4;
	table[4].string_id = 0xd00042a;
	table[5].item = 6;
	table[5].string_id = 0xe0005db;
	table[6].item = 7;
	table[6].string_id = 0x120005dd;
	table[7].item = 8;
	table[7].string_id = 0x130005dc;
	table[8].item = 9;
	table[8].string_id = 0xd0005d8;
	function_24c75c(this, item, table, 0, 9);
}

/* opens the settings of the session's variant's game engine */
// @retail 0x2bb978
void function_2bb978(s_controller_reference **controller)
{
	s_game_variant *variant = (s_game_variant *)network_session_interface_get_data_4db0();
	long type;

	switch (variant->game_engine_index)
	{
	case 1:
		type = 0x16;
		break;
	case 2:
		type = 0x17;
		break;
	case 3:
		type = 0x18;
		break;
	case 4:
		type = 0x19;
		break;
	case 7:
		type = 0x1c;
		break;
	case 8:
		type = 0x1d;
		break;
	case 9:
		type = 0x1e;
		break;
	default:
		return;
	}
	function_2c83e6(type, 3, 4, 1 << (*controller)->controller_index);
}

/* starts the game */
// @retail 0x2bba01
void __stdcall function_2bba01(c_squad_setting_list *list, s_controller_reference **controller)
{
	if (g_54d5a0)
	{
		dialog_ok_show(1, 0x32, 4, 1 << (*controller)->controller_index, 0, 0);
	}
	else
	{
		long value;

		function_199a57();
		function_199a03(0);
		function_121100(&value);
		function_148d42(value);
		g_54e7cc = false;
		function_19a76d((short)value);
		list->get_screen()->start_animation(3);
	}
}

// @retail 0x2bba66
void __stdcall function_2bba66(c_squad_setting_list *list)
{
	function_199a57();
	function_199a03(2);
	function_19a864();
	list->get_screen()->start_animation(3);
}

// @retail 0x2bba8c
void __stdcall function_2bba8c(c_squad_setting_list *list, s_controller_reference **controller)
{
	function_2bbf06((*controller)->controller_index, true);
	list->get_screen()->start_animation(3);
}

/* the same as function_2bba8c (retail folded the two) */
void __stdcall function_2bba8c_alternate(c_squad_setting_list *list, s_controller_reference **controller)
{
	function_2bbf06((*controller)->controller_index, true);
	list->get_screen()->start_animation(3);
}

// @retail 0x2bb7dd
bool c_squad_setting_list::v10(s_widget_event *event)
{
	if (event->type == 6)
	{
		function_2bba8c(this, (s_controller_reference **)&event);
	}
	return ((c_widget *)this)->c_widget::v18((s_event *)event);
}

// @retail 0x2bb801
void c_squad_setting_list::handle_item(s_controller_reference **controller, long *item)
{
	if (*item != NONE)
	{
		s_list_item_datum *datum = &((s_list_item_datum *)data->data)[*item & 0xffff];

		switch (datum->item)
		{
		case 0:
			function_2bb8ac(controller);
			break;
		case 1:
			function_2bb8df(controller);
			break;
		case 2:
			function_2bb912(controller);
			break;
		case 3:
			function_2bb945(controller);
			break;
		case 4:
			function_2bb978(controller);
			break;
		case 6:
			function_2bba01(this, controller);
			break;
		case 7:
			function_2bba66(this);
			break;
		case 8:
			function_2bba8c(this, controller);
			break;
		case 9:
			function_2bba8c_alternate(this, controller);
			break;
		default:
			__assume(0);
		}
	}
}
