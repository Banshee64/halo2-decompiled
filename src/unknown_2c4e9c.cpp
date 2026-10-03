#include <string.h>
#include "cseries.h"
#include "screen_widgets.h"

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

/* ---- opening screens ---- */

struct s_message;
void function_149f49(word a, s_message *message, dword *id, word b, long c, long d, long e);

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
	function_149f49(0, (s_message *)&parameters, 0, user_flags, a, b, (long)load);
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
	function_149f49(0, (s_message *)&parameters, 0, user_flags, a, b, (long)load);
	screen = (s_screen_view_2c83 *)parameters.load(&parameters);
	screen->value9b4 = value;
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
	virtual long get_item_count();
};

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
	virtual screen_load_proc get_load_proc();

	byte unknown80[0xcbc - 0x80];
	bool alternate;
};

// @retail 0x2c7a9f
screen_load_proc c_screen_45d140::get_load_proc()
{
	return alternate ? function_2c7e0f : function_2c7dc3;
}

class c_screen_45d0d0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0x9bc - 0x80];
	bool alternate;
};

// @retail 0x2c7ab3
screen_load_proc c_screen_45d0d0::get_load_proc()
{
	return alternate ? function_2c83a4 : function_2c8362;
}

/* the screen at 0x45d2b8 and the ones that derive from it (0x45d328,
   0x45d398, 0x45d408): a press of B or back copies its settings out */
dword g_54e5d8[0x78];
bool g_54e6f7;

class c_screen_45d2b8 : public c_screen_widget
{
public:
	virtual bool v10(s_widget_event *event);

	byte unknown80[0x614 - 0x80];
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
				memcpy(g_54e5d8, settings, sizeof(g_54e5d8));
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
				g_54e6f7 ^= true;
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
	screen->function_147f6d();
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

class c_screen_45d6f8 : public c_screen_widget
{
public:
	virtual void v17();

	byte unknown80[0x4fb0 - 0x80];
	long value;
	byte unknown4fb4[0x4fbc - 0x4fb4];
	bool flag_a;
	bool flag_b;
	byte unknown4fbe[0x4fd8 - 0x4fbe];
	long new_value;
	bool new_flag_a;
	bool new_flag_b;
};

// @retail 0x2c9e26
void c_screen_45d6f8::v17()
{
	value = new_value;
	flag_a = new_flag_a;
	flag_b = new_flag_b;
}
