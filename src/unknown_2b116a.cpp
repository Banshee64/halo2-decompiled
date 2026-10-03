#include "cseries.h"
#include "screen_widgets.h"

// @flags /O1 /Gr

/* UNKNOWN_2B116A.CPP: the small virtual methods of the screens and lists
   built in 0x2b0000..0x2bbfff (their load procedures and constructors sit next
   to them). Each class is named by its list's debug name where its
   constructor gives one, else by its retail vtable. */

/* the load procedures (not decompiled yet: stubs in src/stubs/lane_e.cpp) */
c_screen_widget *__stdcall function_2b130a(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b136d(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b1467(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b19dc(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b1b01(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b1b43(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b1b85(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b1bc9(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b2801(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b288c(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b2917(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b4274(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b4397(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b4485(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b4565(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b46a6(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b47a7(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b5406(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b71f0(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b7152(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b7201(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b7212(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b7223(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b72e6(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b7333(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b739a(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b7714(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b77a8(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b7887(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b8536(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b8acd(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b8add(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b8aed(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b8b2d(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2ba45b(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2baa9b(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2baae7(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2bb2db(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2bb31d(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2bb3ed(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2bb432(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2bbacb(s_screen_parameters *parameters);

/* ---- widgets ---- */

/* the widgets at 0x45ad60, 0x45ada8 and 0x45adf0 share slot 6 */
class c_widget_45ad60 : public c_user_interface_widget
{
public:
	virtual long v6();
};

struct s_widget_data_45ad60
{
	byte unknown00[6];
	short value;
};

// @retail 0x2b116a
long c_widget_45ad60::v6()
{
	s_widget_data_45ad60 *widget_data = (s_widget_data_45ad60 *)data;

	if (widget_data)
		return widget_data->value;
	return 0;
}

/* ---- screens ---- */

class c_screen_45ae38 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0xe34 - 0x80];
	bool alternate;
};

// @retail 0x2b13ea
screen_load_proc c_screen_45ae38::get_load_proc()
{
	return alternate ? function_2b136d : function_2b130a;
}

class c_campaign_options_screen : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b14d6
screen_load_proc c_campaign_options_screen::get_load_proc()
{
	return function_2b1467;
}

class c_screen_45b0b8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b1a7c
screen_load_proc c_screen_45b0b8::get_load_proc()
{
	return function_2b19dc;
}

class c_screen_45aff0 : public c_screen_widget
{
public:
	virtual void v17();
	virtual screen_load_proc get_load_proc();

	byte unknown80[0xb1c - 0x80];
	bool flags[3];
	byte unknownb1f[0xb38 - 0xb1f];
	bool new_flags[3];
};

// @retail 0x2b1c0b
void c_screen_45aff0::v17()
{
	flags[0] = new_flags[0];
	flags[1] = new_flags[1];
	flags[2] = new_flags[2];
}

// @retail 0x2b1c30
screen_load_proc c_screen_45aff0::get_load_proc()
{
	if (new_flags[1])
		return function_2b1b43;
	if (new_flags[0])
		return function_2b1b01;
	return new_flags[2] ? function_2b1bc9 : function_2b1b85;
}

class c_screen_45b220 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b2871
screen_load_proc c_screen_45b220::get_load_proc()
{
	return function_2b2801;
}

class c_screen_45b290 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b28fc
screen_load_proc c_screen_45b290::get_load_proc()
{
	return function_2b288c;
}

class c_screen_45b300 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b2987
screen_load_proc c_screen_45b300::get_load_proc()
{
	return function_2b2917;
}

class c_screen_45b628 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b4391
screen_load_proc c_screen_45b628::get_load_proc()
{
	return function_2b4274;
}

class c_screen_45b7d0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b447f
screen_load_proc c_screen_45b7d0::get_load_proc()
{
	return function_2b4397;
}

class c_screen_45b708 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b455f
screen_load_proc c_screen_45b708::get_load_proc()
{
	return function_2b4485;
}

class c_screen_45b840 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b4682
screen_load_proc c_screen_45b840::get_load_proc()
{
	return function_2b4565;
}

class c_screen_45b5b8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b4783
screen_load_proc c_screen_45b5b8::get_load_proc()
{
	return function_2b46a6;
}

class c_screen_45b698 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b4886
screen_load_proc c_screen_45b698::get_load_proc()
{
	return function_2b47a7;
}

class c_screen_45bb08 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b5479
screen_load_proc c_screen_45bb08::get_load_proc()
{
	return function_2b5406;
}

class c_screen_45bbd0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0x10e8 - 0x80];
	long mode;
};

// @retail 0x2b7234
screen_load_proc c_screen_45bbd0::get_load_proc()
{
	screen_load_proc result = function_2b7152;

	switch (mode)
	{
	case 1:
		result = function_2b71f0;
		break;
	case 2:
		result = function_2b7201;
		break;
	case 3:
		result = function_2b7212;
		break;
	case 4:
		result = function_2b7223;
		break;
	}
	return result;
}

class c_screen_45bc60 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b7263
screen_load_proc c_screen_45bc60::get_load_proc()
{
	return function_2b72e6;
}

class c_screen_45bcd0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b732d
screen_load_proc c_screen_45bcd0::get_load_proc()
{
	return function_2b7333;
}

class c_screen_45bd40 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b7394
screen_load_proc c_screen_45bd40::get_load_proc()
{
	return function_2b739a;
}

class c_screen_45be30 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b7784
screen_load_proc c_screen_45be30::get_load_proc()
{
	return function_2b7714;
}

class c_screen_45bef8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b7818
screen_load_proc c_screen_45bef8::get_load_proc()
{
	return function_2b77a8;
}

class c_player_profile_edit_screen : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b78fa
screen_load_proc c_player_profile_edit_screen::get_load_proc()
{
	return function_2b7887;
}

class c_screen_45c0e0 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b831d
screen_load_proc c_screen_45c0e0::get_load_proc()
{
	return function_2b8536;
}

class c_screen_45c228 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0x8bc - 0x80];
	long mode;
};

// @retail 0x2b8afd
screen_load_proc c_screen_45c228::get_load_proc()
{
	screen_load_proc result = function_2b8acd;

	switch (mode)
	{
	case 1:
		result = function_2b8add;
		break;
	case 2:
		result = function_2b8aed;
		break;
	}
	return result;
}

class c_screen_45c2a8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b8b6b
screen_load_proc c_screen_45c2a8::get_load_proc()
{
	return function_2b8b2d;
}

class c_screen_45c388 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2ba455
screen_load_proc c_screen_45c388::get_load_proc()
{
	return function_2ba45b;
}

class c_screen_45c3f8 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0x614 - 0x80];
	long mode;
};

// @retail 0x2bac02
screen_load_proc c_screen_45c3f8::get_load_proc()
{
	screen_load_proc result;

	switch (mode)
	{
	case 0:
		result = function_2baa9b;
		break;
	case 1:
		result = function_2baae7;
		break;
	default:
		__assume(0);
	}
	return result;
}

class c_screen_45c518 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2bb299
screen_load_proc c_screen_45c518::get_load_proc()
{
	return function_2bbacb;
}

class c_screen_45c588 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0x8b4 - 0x80];
	bool alternate;
};

// @retail 0x2bb4a9
screen_load_proc c_screen_45c588::get_load_proc()
{
	return alternate ? function_2bb432 : function_2bb3ed;
}

class c_screen_45c650 : public c_screen_widget
{
public:
	virtual screen_load_proc get_load_proc();

	byte unknown80[0xdb8 - 0x80];
	bool alternate;
};

// @retail 0x2bb35f
screen_load_proc c_screen_45c650::get_load_proc()
{
	return alternate ? function_2bb31d : function_2bb2db;
}

/* ---- lists ---- */

class c_campaign_options_list : public c_list_widget
{
public:
	virtual long get_item_count();
};

// @retail 0x2b160c
long c_campaign_options_list::get_item_count()
{
	return data->actual_count;
}

/* the 26-slot list at 0x45af88; its item count (6) is shared by the lists at
   0x45b370, 0x45bb78, 0x45bdb8, 0x45bea0, 0x45d020 and 0x45d240 */
class c_list_45af88 : public c_list_widget_26
{
public:
	virtual long get_item_count();
	virtual void *get_items(long *count);

	byte unknown80[0x3a8 - 0x80];
	byte items[6][0x10];
};

// @retail 0x2b198a
long c_list_45af88::get_item_count()
{
	return 6;
}

// @retail 0x2b198e
void *c_list_45af88::get_items(long *count)
{
	*count = 6;
	return items;
}

class c_settings_list : public c_list_widget
{
public:
	virtual long get_item_count();

	byte unknown80[0x220 - 0x80];
	bool extended;
};

// @retail 0x2b1d60
long c_settings_list::get_item_count()
{
	return extended ? 3 : 2;
}

/* the lists at 0x45b3e0 and 0x45b510 read their items from the screen three
   levels up */
struct s_screen_items_2b41
{
	byte unknown00[0x3668];
	byte items[0x55a8 - 0x3668];
	long count;
};

class c_list_45b3e0 : public c_list_widget_with_items
{
public:
	virtual void *get_item_data();
	virtual long get_item_count();
	virtual void *get_items(long *count);

	byte unknown80[0xac - 0x80];
	byte item_data[4];
};

// @retail 0x2b3ef5
void *c_list_45b3e0::get_item_data()
{
	return item_data;
}

// @retail 0x2b2d7d
long c_list_45b3e0::get_item_count()
{
	return 8;
}

// @retail 0x2b419f
void *c_list_45b3e0::get_items(long *count)
{
	s_screen_items_2b41 *screen = (s_screen_items_2b41 *)parent->parent->parent;

	*count = screen->count;
	return screen->items;
}

class c_squad_privacy_setting_list : public c_list_widget
{
public:
	virtual long get_item_count();
};

// @retail 0x2b51ea
long c_squad_privacy_setting_list::get_item_count()
{
	return 3;
}

class c_list_45c318 : public c_list_widget
{
public:
	virtual long get_item_count();
};

// @retail 0x2b8b1a
long c_list_45c318::get_item_count()
{
	if (data)
	{
		dword count = data->actual_count;

		if (count <= 4)
			return count;
	}
	return 4;
}

class c_actions_list : public c_list_widget
{
public:
	virtual void *get_item_data();

	byte unknown80[0x8c - 0x80];
	byte item_data[4];
};

// @retail 0x2ba65f
void *c_actions_list::get_item_data()
{
	return item_data;
}

class c_squad_setting_list : public c_list_widget
{
public:
	virtual long get_item_count();
};

// @retail 0x2bb295
long c_squad_setting_list::get_item_count()
{
	return 7;
}
