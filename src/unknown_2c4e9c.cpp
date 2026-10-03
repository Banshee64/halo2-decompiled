#include "cseries.h"
#include "screen_widgets.h"

// @flags /O1 /Gr

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

class c_screen_45d328 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0xa9c - 0x80];
	long mode;
};

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

class c_screen_45d398 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0xa9c - 0x80];
	long index;
};

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
