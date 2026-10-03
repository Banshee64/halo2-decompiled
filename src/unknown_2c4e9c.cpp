#include <string.h>
#include "cseries.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"

// @flags /O1 /Oi /Gr

/* UNKNOWN_2C4E9C.CPP: the small virtual methods of the screens and lists
   built in 0x2c4e00..0x2cb8c0 (the settings, variant, and custom game
   screens). Each class is named by its list's debug name where its
   constructor gives one, else by its retail vtable. */

/* the load procedures (not decompiled yet: stubs in src/stubs/lane_e.cpp) */
c_screen_widget *__stdcall function_2b54b2(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c7dc3(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c7e0f(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c8362(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c83a4(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c8858(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c8896(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c8998(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c89a8(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c89b9(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c89ca(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c8a8f(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2c9012(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2bbacb(s_screen_parameters *parameters);

/* ---- opening screens ---- */

struct s_message;
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e);

/* the screens 0x2c7dc3/0x2c7e0f and 0x2c8362/0x2c83a4 load */
struct s_screen_view_2c83
{
	byte unknown00[0x9b4];
	long value9b4;
	byte unknown9b8[0xc9c - 0x9b8];
	long valuec9c;
};

// @retail 0x2c83e6
void function_2c83e6(long type, long a, long b, word user_flags)
{
	long types[9] = { 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e };
	s_screen_parameters parameters;
	screen_load_proc load = function_2c7dc3;
	s_screen_view_2c83 *screen;

	parameters.field_c = 0;
	for (dword i = 0; i < 9; i++)
	{
		if (type == types[i])
		{
			load = function_2c7e0f;
			break;
		}
	}
	function_149f49((s_message *)&parameters, 0, 0, user_flags, a, b, (long)load);
	screen = (s_screen_view_2c83 *)parameters.load(&parameters);
	screen->valuec9c = type;
}

// @retail 0x2c8474
void function_2c8474(long value, long a, long b, word user_flags, bool alternate)
{
	s_screen_parameters parameters;
	s_screen_view_2c83 *screen;
	screen_load_proc load = alternate ? function_2c83a4 : function_2c8362;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, user_flags, a, b, (long)load);
	screen = (s_screen_view_2c83 *)parameters.load(&parameters);
	screen->value9b4 = value;
}

/* ---- bit vectors ---- */

// @retail 0x2cb220
void function_2cb220(long count, dword *bits)
{
	memset(bits, 0xff, ((count + 31) >> 5) * 4);
}

// @retail 0x2cb200
void function_2cb200(dword *bits, long small)
{
	long count;

	memset(bits, 0, 8);
	switch (small)
	{
	case 0:
		count = 64;
		break;
	case 1:
		count = 32;
		break;
	default:
		__assume(0);
	}
	function_2cb220(count, bits);
}

/* ---- lists ---- */

class c_campaign_level_handles_list : public c_list_widget
{
public:
	virtual long get_item_count();
};

// @retail 0x2c4e9c
long c_campaign_level_handles_list::get_item_count()
{
	return 15;
}

class c_game_engine_variant_category_list : public c_list_widget
{
public:
	virtual long get_item_count();
};

// @retail 0x2c55f4
long c_game_engine_variant_category_list::get_item_count()
{
	return 9;
}

class c_list_45cf40 : public c_list_widget
{
public:
	virtual long get_item_count();
};

// @retail 0x2c6a7d
long c_list_45cf40::get_item_count()
{
	return 14;
}

class c_list_45d078 : public c_list_widget
{
public:
	c_list_45d078(word user_flags);

	virtual long get_item_count();

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[12];
	long value688;
	long value68c;
	c_list_item_handler handler;
	bool alternate;
};

// @retail 0x2c7ac7
c_list_45d078::c_list_45d078(word user_flags) :
	c_list_widget(user_flags),
	value688(NONE),
	value68c(0),
	handler(this, (list_item_method)&c_list_45d078::handle_item)
{
	alternate = false;
}

/* the item's datum holds the value the screen it opens is loaded with */
struct s_list_45d078_datum
{
	byte unknown00[4];
	long *value;
};

// @retail 0x2c7d86
void c_list_45d078::handle_item(s_controller_reference **controller, long *item)
{
	s_list_45d078_datum *datum = (s_list_45d078_datum *)datum_get(data, *item);

	if (datum && datum->value)
	{
		function_2c8474(*datum->value, 3, 4, user_flags, alternate);
	}
}

// @retail 0x2c7a9b
long c_list_45d078::get_item_count()
{
	return 12;
}

/* ---- screens ---- */

class c_screen_45cf98 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2c6a9f
screen_load_proc c_screen_45cf98::get_load_proc()
{
	return function_2b54b2;
}

class c_screen_45d140 : public c_screen_widget
{
public:
	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	byte unknown610[0xcbc - 0x610];
	bool alternate;
};

// @retail 0x2c7ec0
bool c_screen_45d140::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
			if (alternate)
			{
				s_screen_parameters parameters;

				parameters.field_c = 0;
				function_149f49((s_message *)&parameters, 0, 0, 1 << event->controller_index, 3, 4, (long)function_2bbacb);
				parameters.load(&parameters);
			}
			break;
		}
	}
	return c_screen_widget::v10(event);
}

// @retail 0x2c7a9f
screen_load_proc c_screen_45d140::get_load_proc()
{
	return alternate ? function_2c7e0f : function_2c7dc3;
}

class c_screen_45d0d0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown610[0x9bc - 0x610];
	bool alternate;
};

// @retail 0x2c7ab3
screen_load_proc c_screen_45d0d0::get_load_proc()
{
	return alternate ? function_2c83a4 : function_2c8362;
}

/* the screen at 0x45d2b8 and the ones that derive from it (0x45d328,
   0x45d398, 0x45d408): a press of B or back copies its settings out */

class c_screen_45d2b8 : public c_screen_widget
{
public:
	virtual bool v10(s_widget_event *event);

	byte unknown610[0x614 - 0x610];
	bool changed;
	byte unknown615[3];
	dword settings[0x78];
};

// @retail 0x2c87fe
bool c_screen_45d2b8::v10(s_widget_event *event)
{
	switch (event->type)
	{
	case 5:
		switch (event->param)
		{
		case 1:
		case 13:
			if (changed)
				memcpy(&g_54e5d0.settings, settings, sizeof(g_54e5d0.settings));
			break;
		}
		break;
	}
	return c_screen_widget::v10(event);
}

class c_screen_45d328 : public c_screen_45d2b8
{
public:
	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	byte unknown7f8[0xa9c - 0x7f8];
	long mode;
};

// @retail 0x2cb1d4
bool c_screen_45d328::v10(s_widget_event *event)
{
	switch (event->type)
	{
	case 5:
		switch (event->param)
		{
		case 2:
			if (mode == 0)
			{
				g_54e5d0.settings.flag ^= true;
				return true;
			}
			break;
		}
		break;
	}
	return c_screen_45d2b8::v10(event);
}

// @retail 0x2c8920
screen_load_proc c_screen_45d328::get_load_proc()
{
	screen_load_proc result;

	switch (mode)
	{
	case 0:
		result = function_2c8858;
		break;
	case 1:
		result = function_2c8896;
		break;
	default:
		__assume(0);
	}
	return result;
}

// @retail 0x2c89db
screen_load_proc function_2c89db(long index)
{
	screen_load_proc result = 0;

	switch (index)
	{
	case 0:
		result = function_2c8998;
		break;
	case 1:
		result = function_2c89a8;
		break;
	case 2:
		result = function_2c89b9;
		break;
	case 3:
		result = function_2c89ca;
		break;
	}
	return result;
}

class c_screen_45d398 : public c_screen_45d2b8
{
public:
	c_screen_45d398(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	byte unknown7f8[0xa98 - 0x7f8];
	long previous_index;
	long index;
};

// @retail 0x2c8954
c_screen_widget *function_2c8954(s_screen_parameters *parameters, long index)
{
	c_screen_45d398 *screen = new c_screen_45d398(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->index = index;
	screen->previous_index = index;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2c8998
c_screen_widget *__stdcall function_2c8998(s_screen_parameters *parameters)
{
	return function_2c8954(parameters, 0);
}

// @retail 0x2c89a8
c_screen_widget *__stdcall function_2c89a8(s_screen_parameters *parameters)
{
	return function_2c8954(parameters, 1);
}

// @retail 0x2c89b9
c_screen_widget *__stdcall function_2c89b9(s_screen_parameters *parameters)
{
	return function_2c8954(parameters, 2);
}

// @retail 0x2c89ca
c_screen_widget *__stdcall function_2c89ca(s_screen_parameters *parameters)
{
	return function_2c8954(parameters, 3);
}

// @retail 0x2c8a6f
screen_load_proc c_screen_45d398::get_load_proc()
{
	return function_2c89db(index);
}

class c_screen_45d408 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2c8aff
screen_load_proc c_screen_45d408::get_load_proc()
{
	return function_2c8a8f;
}

class c_screen_45d560 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2c8f50
screen_load_proc c_screen_45d560::get_load_proc()
{
	return function_2c9012;
}

/* ---- the settings edit lists: each item sets one of the edited profile's
   settings, then the list's window goes back ---- */

void function_53810(long voice_mask, long controller_index);
void function_54fc0(long controller_index, long voice_through_tv);
bool function_153850(byte *model);

/* "voice mask list" (vtable 0x45cbd0) */
class c_voice_mask_list : public c_list_widget
{
public:
	c_voice_mask_list(word user_flags);

	virtual void v1();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

// @retail 0x2c5dca deleting c_voice_mask_list

// @retail 0x2c5d34
c_voice_mask_list::c_voice_mask_list(word user_flags) :
	c_list_widget(user_flags),
	value188(false),
	handler(this, (list_item_method)&c_voice_mask_list::handle_item)
{
	data = user_interface_data_new("voice mask list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c5de8
void c_voice_mask_list::v1()
{
	((c_widget *)this)->c_widget::v9();
	select_item((short)g_54e5d0.settings.voice_mask);
}

// @retail 0x2c5e02
void c_voice_mask_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0x400021c;
			break;
		case 1:
			string_id = 0x9000303;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c5e42
void c_voice_mask_list::handle_item(s_controller_reference **controller, long *item)
{
	switch (*(short *)item)
	{
	case 0:
		g_54e5d0.settings.voice_mask = 0;
		break;
	case 1:
		g_54e5d0.settings.voice_mask = 1;
		break;
	}
	if (value188)
	{
		profile_edit_save();
	}
	function_53810(g_54e5d0.settings.voice_mask, (*controller)->controller_index);
	function_14800c(v11(), v12());
}

/* "voice through tv list" (vtable 0x45cc38) */
class c_voice_through_tv_list : public c_list_widget
{
public:
	c_voice_through_tv_list(word user_flags);

	virtual void v1();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

// @retail 0x2b8cb1 deleting c_voice_through_tv_list

// @retail 0x2c5ea7
c_voice_through_tv_list::c_voice_through_tv_list(word user_flags) :
	c_list_widget(user_flags),
	value288(false),
	handler(this, (list_item_method)&c_voice_through_tv_list::handle_item)
{
	data = user_interface_data_new("voice through tv list", 4, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c5f3c
void c_voice_through_tv_list::v1()
{
	((c_widget *)this)->c_widget::v9();
	select_item((short)g_54e5d0.settings.voice_through_tv);
}

// @retail 0x2c5f56
void c_voice_through_tv_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0xd000304;
			break;
		case 1:
			string_id = 0x25000305;
			break;
		case 2:
			string_id = 0x21000306;
			break;
		case 3:
			string_id = 0xe000307;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c5fab
void c_voice_through_tv_list::handle_item(s_controller_reference **controller, long *item)
{
	switch (*(short *)item)
	{
	case 0:
		g_54e5d0.settings.voice_through_tv = 0;
		break;
	case 1:
		g_54e5d0.settings.voice_through_tv = 1;
		break;
	case 2:
		g_54e5d0.settings.voice_through_tv = 2;
		break;
	case 3:
		g_54e5d0.settings.voice_through_tv = 3;
		break;
	}
	if (value288)
	{
		profile_edit_save();
	}
	function_54fc0((*controller)->controller_index, g_54e5d0.settings.voice_through_tv);
	function_14800c(v11(), v12());
}

/* "thumbstick settings edit list" (vtable 0x45cca8) */
class c_thumbstick_settings_edit_list : public c_list_widget
{
public:
	c_thumbstick_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

// @retail 0x2c602e
c_thumbstick_settings_edit_list::c_thumbstick_settings_edit_list(word user_flags) :
	c_list_widget(user_flags),
	value288(false),
	handler(this, (list_item_method)&c_thumbstick_settings_edit_list::handle_item)
{
	data = user_interface_data_new("thumbstick settings edit list", 4, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c60c3
void c_thumbstick_settings_edit_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0x7000001;
			break;
		case 1:
			string_id = 0x80002f5;
			break;
		case 2:
			string_id = 0x60002f6;
			break;
		case 3:
			string_id = 0xf0002f7;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c6118
void c_thumbstick_settings_edit_list::handle_item(s_controller_reference **controller, long *item)
{
	byte layout;

	switch (*(short *)item)
	{
	case 0:
		layout = 0;
		break;
	case 1:
		layout = 1;
		break;
	case 2:
		layout = 2;
		break;
	case 3:
		layout = 3;
		break;
	default:
		layout = 0;
		break;
	}
	g_54e5d0.settings.thumbstick_layout = layout;
	if (value288)
	{
		profile_edit_save();
	}
	function_14800c(v11(), v12());
}

/* "look sensitivity settings edit list" (vtable 0x45cd20) */
class c_look_sensitivity_settings_edit_list : public c_list_widget
{
public:
	c_look_sensitivity_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

// @retail 0x2c61ce
c_look_sensitivity_settings_edit_list::c_look_sensitivity_settings_edit_list(word user_flags) :
	c_list_widget(user_flags),
	value288(false),
	handler(this, (list_item_method)&c_look_sensitivity_settings_edit_list::handle_item)
{
	data = user_interface_data_new("look sensitivity settings edit list", 10, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c6264
void c_look_sensitivity_settings_edit_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch ((short)(widget_item(widget)->value70 + 1))
		{
		case 1:
			string_id = 0x120003cf;
			break;
		case 2:
			string_id = 0x120003d0;
			break;
		case 3:
			string_id = 0x120003d1;
			break;
		case 4:
			string_id = 0x120003d2;
			break;
		case 5:
			string_id = 0x120003d3;
			break;
		case 6:
			string_id = 0x120003d4;
			break;
		case 7:
			string_id = 0x120003d5;
			break;
		case 8:
			string_id = 0x120003d6;
			break;
		case 9:
			string_id = 0x120003d7;
			break;
		case 10:
			string_id = 0x130003d8;
			break;
		default:
			string_id = 0;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c630e
void c_look_sensitivity_settings_edit_list::handle_item(s_controller_reference **controller, long *item)
{
	long sensitivity = *(short *)item + 1;
	byte value;

	if (sensitivity < 1)
	{
		value = 1;
	}
	else if (sensitivity > 10)
	{
		value = 10;
	}
	else
	{
		value = (byte)sensitivity;
	}
	g_54e5d0.settings.look_sensitivity = value;
	if (value288)
	{
		profile_edit_save();
	}
	function_14800c(v11(), v12());
}

/* "invert look settings edit list" (vtable 0x45ce38) */
class c_invert_look_settings_edit_list : public c_list_widget
{
public:
	c_invert_look_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

// @retail 0x2c635b
c_invert_look_settings_edit_list::c_invert_look_settings_edit_list(word user_flags) :
	c_list_widget(user_flags),
	value188(false),
	handler(this, (list_item_method)&c_invert_look_settings_edit_list::handle_item)
{
	data = user_interface_data_new("invert look settings edit list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c63f1
void c_invert_look_settings_edit_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0x6000308;
			break;
		case 1:
			string_id = 0x7000309;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c6431
void c_invert_look_settings_edit_list::handle_item(s_controller_reference **controller, long *item)
{
	switch (*(short *)item)
	{
	case 0:
		g_54e5d0.settings.controller_flags.invert_look = true;
		break;
	default:
		g_54e5d0.settings.controller_flags.invert_look = false;
		break;
	}
	if (value188)
	{
		profile_edit_save();
	}
	function_14800c(v11(), v12());
}

/* "button settings edit list" (vtable 0x45cdc0) */
class c_button_settings_edit_list : public c_list_widget
{
public:
	c_button_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool value288;
	byte unknown289[0x28c - 0x289];
	c_list_item_handler handler;
};

// @retail 0x2c647d
c_button_settings_edit_list::c_button_settings_edit_list(word user_flags) :
	c_list_widget(user_flags),
	value288(false),
	handler(this, (list_item_method)&c_button_settings_edit_list::handle_item)
{
	data = user_interface_data_new("button settings edit list", 4, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c6512
void c_button_settings_edit_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0x7000001;
			break;
		case 1:
			string_id = 0x90003e4;
			break;
		case 2:
			string_id = 0x50003e5;
			break;
		case 3:
			string_id = 0xb0003e6;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c6567
void c_button_settings_edit_list::handle_item(s_controller_reference **controller, long *item)
{
	byte layout;

	switch (*(short *)item)
	{
	case 0:
		layout = 0;
		break;
	case 1:
		layout = 1;
		break;
	case 2:
		layout = 2;
		break;
	case 3:
		layout = 3;
		break;
	default:
		layout = 0;
		break;
	}
	g_54e5d0.settings.button_layout = layout;
	if (value288)
	{
		profile_edit_save();
	}
	function_14800c(v11(), v12());
}

/* "auto level settings edit list" (vtable 0x45ce38) */
class c_auto_level_settings_edit_list : public c_list_widget
{
public:
	c_auto_level_settings_edit_list(word user_flags);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

// @retail 0x2c661f
c_auto_level_settings_edit_list::c_auto_level_settings_edit_list(word user_flags) :
	c_list_widget(user_flags),
	value188(false),
	handler(this, (list_item_method)&c_auto_level_settings_edit_list::handle_item)
{
	data = user_interface_data_new("auto level settings edit list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c66b5
void c_auto_level_settings_edit_list::handle_item(s_controller_reference **controller, long *item)
{
	switch (*(short *)item)
	{
	case 0:
		g_54e5d0.settings.controller_flags.auto_level = true;
		break;
	default:
		g_54e5d0.settings.controller_flags.auto_level = false;
		break;
	}
	if (value188)
	{
		profile_edit_save();
	}
	function_14800c(v11(), v12());
}

/* "vibration settings edit list" (vtable 0x45ceb0) */
class c_vibration_settings_edit_list : public c_list_widget
{
public:
	c_vibration_settings_edit_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	bool value188;
	byte unknown189[0x18c - 0x189];
	c_list_item_handler handler;
};

// @retail 0x2c6701
c_vibration_settings_edit_list::c_vibration_settings_edit_list(word user_flags) :
	c_list_widget(user_flags),
	value188(false),
	handler(this, (list_item_method)&c_vibration_settings_edit_list::handle_item)
{
	data = user_interface_data_new("vibration settings edit list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c6797
void c_vibration_settings_edit_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0x2000181;
			break;
		case 1:
			string_id = 0x3000182;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c67d7
void c_vibration_settings_edit_list::handle_item(s_controller_reference **controller, long *item)
{
	switch (*(short *)item)
	{
	case 0:
		g_54e5d0.settings.controller_flags.vibration = false;
		break;
	case 1:
		g_54e5d0.settings.controller_flags.vibration = true;
		break;
	default:
		g_54e5d0.settings.controller_flags.vibration = true;
		break;
	}
	if (value188)
	{
		profile_edit_save();
	}
	function_14800c(v11(), v12());
}

/* "subtitle setting list" (vtable 0x45d4f0) */
class c_subtitle_setting_list : public c_list_widget
{
public:
	c_subtitle_setting_list(word user_flags);

	virtual void v1();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[3];
	c_list_item_handler handler;
};

// @retail 0x2b52b7 deleting c_subtitle_setting_list

// @retail 0x2c8ded
c_subtitle_setting_list::c_subtitle_setting_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_subtitle_setting_list::handle_item)
{
	data = user_interface_data_new("subtitle setting list", 3, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2c8e7d
void c_subtitle_setting_list::v1()
{
	short item;

	((c_widget *)this)->c_widget::v9();
	item = 0;
	switch (g_54e5d0.settings.subtitles)
	{
	case 0:
		item = 0;
		break;
	case 1:
		item = 1;
		break;
	case 2:
		item = 2;
		break;
	}
	select_item(item);
}

// @retail 0x2c8eaa
void c_subtitle_setting_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch (((short)widget_item(widget)->value70))
		{
		case 0:
			string_id = 0x7000001;
			break;
		case 1:
			string_id = 0x2000181;
			break;
		case 2:
			string_id = 0x3000182;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

// @retail 0x2c8ef5
void c_subtitle_setting_list::handle_item(s_controller_reference **controller, long *item)
{
	byte subtitles = 0;

	switch (*(short *)item)
	{
	case 0:
		subtitles = 0;
		break;
	case 1:
		subtitles = 1;
		break;
	case 2:
		subtitles = 2;
		break;
	}
	g_54e5d0.settings.subtitles = subtitles;
	function_14800c(v11(), v12());
}

/* "choose player color list" (vtable 0x45d850) */
class c_choose_player_color_list : public c_list_widget
{
public:
	c_choose_player_color_list(word user_flags);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	c_list_item_handler handler;
	long value2a0;
};

// @retail 0x2b4bd2 deleting c_choose_player_color_list

// @retail 0x2cb23f
c_choose_player_color_list::c_choose_player_color_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_choose_player_color_list::handle_item)
{
	value2a0 = 0;
	data = user_interface_data_new("choose player color list", 18, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2cb30a
void c_choose_player_color_list::handle_item(s_controller_reference **controller, long *item)
{
	short color = *(short *)item;
	long value = 0;

	if (color >= -1 && color < 18)
	{
		value = color;
	}
	g_54e5d0.settings.colors[value2a0] = (byte)value;
	function_14800c(v11(), v12());
}

/* "choose model list" (vtable 0x45d8c8) */
class c_choose_model_list : public c_list_widget
{
public:
	c_choose_model_list(word user_flags);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[2];
	c_list_item_handler handler;
};

// @retail 0x2cb3e0
c_choose_model_list::c_choose_model_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_choose_model_list::handle_item)
{
	data = user_interface_data_new("choose model list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2cb4a2
void c_choose_model_list::handle_item(s_controller_reference **controller, long *item)
{
	g_54e5d0.settings.model = *(byte *)item;
	if (!function_153850(&g_54e5d0.settings.model))
	{
		g_54e5d0.settings.model = 0;
	}
	function_14800c(v11(), v12());
}
