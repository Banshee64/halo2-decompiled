#include <string.h>
#include "cseries.h"
#include "globals.h"
#include "screen_widgets.h"
#include "unknown_19b516.h"

// @flags /O1 /Oi /Gr

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

/* ---- the item flags of the widget definitions (read by 0x2afeae) ---- */

/* an item whose optional fields are present when their flag is set */
struct s_widget_item
{
	union
	{
		dword flags;
		struct
		{
			dword has_value4 : 1;
			dword unknown1 : 1;
			dword has_value5c : 1;
			dword has_value58 : 1;
			dword unknown4 : 1;
			dword has_value5e : 1;
			dword has_value60 : 1;
			dword has_value5f : 1;
			dword has_value64 : 1;
			dword unknown9 : 23;
		};
	};
	long value4;
	byte unknown08[0x58 - 8];
	long value58;
	short value5c;
	bool value5e;
	bool value5f;
	short value60;
	byte unknown62[2];
	long value64;
};

// @retail 0x2b014f
long function_2b014f(s_widget_item *item)
{
	return item->has_value4 ? item->value4 : 0;
}

// @retail 0x2b015b
long function_2b015b(s_widget_item *item)
{
	return TEST_FIELD_BIT(item->has_value58) ? item->value58 : NONE;
}

// @retail 0x2b016d
long function_2b016d(s_widget_item *item)
{
	return TEST_FIELD_BIT(item->has_value5c) ? item->value5c : NONE;
}

// @retail 0x2b0180
bool function_2b0180(s_widget_item *item)
{
	return TEST_FIELD_BIT(item->has_value5f) ? item->value5f : false;
}

// @retail 0x2b0191
bool function_2b0191(s_widget_item *item)
{
	return TEST_FIELD_BIT(item->has_value5e) ? item->value5e : false;
}

// @retail 0x2b01b5
void function_2b01b5(s_widget_item *item, short value)
{
	item->value60 = value;
	if (value != NONE)
		item->flags |= 0x40;
	else
		item->flags &= ~0x40;
}

// @retail 0x2b01c7
long function_2b01c7(s_widget_item *item)
{
	return TEST_FIELD_BIT(item->has_value60) ? item->value60 : NONE;
}

// @retail 0x2b01da
long function_2b01da(s_widget_item *item)
{
	return TEST_FIELD_BIT(item->has_value64) ? item->value64 : 0;
}

/* ---- helpers of the widgets whose definition (an s_widget_item) is at
   +0x70 ---- */

struct s_widget_view_2b0a
{
	byte unknown00[0x6e];
	bool enabled;
	byte unknown6f;
	s_widget_item *definition;
	byte bounds[0x10];
	short value84;
	short value86;
	short value88;
};

struct s_bitmap_tag_2b0a
{
	byte unknown00[0x44];
	long count;
};

// @retail 0x2b0a14
void function_2b0a14(s_widget_view_2b0a *widget, short index)
{
	long tag_index = *(long *)((byte *)widget->definition + 0x1c);

	if (tag_index != NONE && index >= 0)
	{
		s_bitmap_tag_2b0a *tag = (s_bitmap_tag_2b0a *)g_4e3b44[tag_index & 0xffff].bytes;

		if (index < tag->count)
			widget->value88 = index;
	}
}

// @retail 0x2b0a48
bool function_2b0a48(s_widget_view_2b0a *widget)
{
	s_widget_item *definition = widget->definition;

	return definition ? TEST_FIELD_BIT(definition->has_value4) : false;
}

// @retail 0x2b0a57
bool function_2b0a57(s_widget_view_2b0a *widget)
{
	s_widget_item *definition = widget->definition;

	return definition ? TEST_FIELD_BIT(definition->unknown1) : false;
}

// @retail 0x2b0a68
long function_2b0a68(s_widget_view_2b0a *widget)
{
	s_widget_item *definition = widget->definition;

	return definition && (definition->flags & 4);
}

// @retail 0x2b12ba
dword function_2b12ba(s_widget_view_2b0a *widget)
{
	s_widget_item *definition = widget->definition;

	return definition ? (definition->flags & 1) : 0;
}

// @retail 0x2b12ca
void function_2b12ca(s_widget_view_2b0a *widget, short a, short b, void const *bounds)
{
	bool disabled;

	memcpy(widget->bounds, bounds, sizeof(widget->bounds));
	widget->value84 = b;
	widget->value86 = a;
	if (a == NONE && widget->definition && (widget->definition->flags & 2))
		disabled = true;
	else
		disabled = false;
	widget->enabled = !disabled;
}

/* an iterator over a list's items (the item, then the data iterator) */
struct s_list_item_iterator
{
	byte *item;
	s_data_iterator iterator;
};

// @retail 0x2b2327
bool function_2b2327(s_list_item_iterator *iterator)
{
	iterator->item = data_iterator_next(&iterator->iterator);
	return iterator->item != 0;
}

/* whether a screen's current item (at +0x684) is the one it holds at the
   given offset */
struct s_screen_view_2b2d
{
	byte unknown00[0x684];
	void *current;
	byte item[4];
};

// @retail 0x2b2d81
long function_2b2d81(s_screen_view_2b2d *screen)
{
	void *item = screen->item;

	return item && item == screen->current;
}

struct s_screen_view_2b39
{
	byte unknown00[0x684];
	void *current;
	byte unknown688[0x29a8 - 0x688];
	byte item[4];
};

// @retail 0x2b3923
long function_2b3923(s_screen_view_2b39 *screen)
{
	void *item = screen->item;

	return item && item == screen->current;
}

struct s_screen_view_2b3e
{
	byte unknown00[0x684];
	void *current;
	byte unknown688[0x1818 - 0x688];
	byte item[4];
};

// @retail 0x2b3efc
long function_2b3efc(s_screen_view_2b3e *screen)
{
	void *item = screen->item;

	return item && item == screen->current;
}

/* a player's identifier and name, as the name lookups take it */
struct s_player_name_2b3e
{
	dword id[3];
	word name[16];
};

struct s_player_request_2b3e
{
	dword id[3];
	char name[16];
	byte unknown1c[0x70 - 0x1c];
};

void unicode_string_to_ascii(const word *source, char *destination, long maximum_count);

// @retail 0x2b3e26
void function_2b3e26(s_player_request_2b3e *request, s_player_name_2b3e const *player)
{
	memset(request, 0, sizeof(*request));
	memcpy(request->id, player->id, sizeof(request->id));
	unicode_string_to_ascii(player->name, request->name, 16);
	request->name[15] = 0;
}

/* ---- opening screens: function_149f49 builds the new screen's parameters,
   whose load procedure then builds the screen ---- */

/* load procedures outside the region (stubs in src/stubs/lane_e.cpp) */
c_screen_widget *__stdcall function_230c8d(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_230d6b(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2325fb(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2b54b2(s_screen_parameters *parameters);

/* the screen 0x2b54b2 loads */
struct s_screen_view_2b61
{
	byte unknown00[0xdb8];
	long value;
};

// @retail 0x2b6068
void function_2b6068(s_controller_reference **controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2b5406);
	parameters.load(&parameters);
}

// @retail 0x2b61ce
void function_2b61ce(word user_flags, long value)
{
	s_screen_parameters parameters;
	s_screen_view_2b61 *screen;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, user_flags, 3, 4, (long)function_2b54b2);
	screen = (s_screen_view_2b61 *)parameters.load(&parameters);
	if (screen)
		screen->value = value;
}

// @retail 0x2bb8ac
void function_2bb8ac(s_controller_reference **controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2bb2db);
	parameters.load(&parameters);
}

// @retail 0x2bb8df
void function_2bb8df(s_controller_reference **controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2b1b85);
	parameters.load(&parameters);
}

// @retail 0x2bb912
void function_2bb912(s_controller_reference **controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2b136d);
	parameters.load(&parameters);
}

// @retail 0x2bb945
void function_2bb945(s_controller_reference **controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2bb432);
	parameters.load(&parameters);
}

// @retail 0x2bb9ce
void function_2bb9ce(s_controller_reference **controller)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2325fb);
	parameters.load(&parameters);
}

// @retail 0x2bbf06
void function_2bbf06(long controller_index, bool alternate)
{
	s_screen_parameters parameters;

	parameters.field_c = 0;
	function_149f49((s_message *)&parameters, 0, 0, 1 << controller_index, 3, 4, (long)(alternate ? function_230c8d : function_230d6b));
	parameters.load(&parameters);
}

/* ---- widgets ---- */

/* the widgets at 0x45ad60, 0x45ada8 and 0x45adf0 share slot 6 */
class c_widget_45ad60 : public c_user_interface_widget
{
public:
	virtual long v6();

	void *data;
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

	byte unknown610[0xe34 - 0x610];
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
	c_campaign_options_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	long value610;
	bool value614;
};

// @retail 0x2b14a3
c_campaign_options_screen::c_campaign_options_screen(long a, long b, word user_flags) :
	c_screen_widget(0xd1, a, b, user_flags),
	value610(0),
	value614(false)
{
}

// @retail 0x2b1467
c_screen_widget *__stdcall function_2b1467(s_screen_parameters *parameters)
{
	c_campaign_options_screen *screen = new c_campaign_options_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

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

	byte unknown610[0xb1c - 0x610];
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

	byte unknown610[0x10e8 - 0x610];
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

	byte unknown610[0x8bc - 0x610];
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
	c_screen_45c388(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	byte unknown610[4];
	long value614;
	byte unknown618[0x628 - 0x618];
};

// @retail 0x2ba497
c_screen_45c388::c_screen_45c388(long a, long b, word user_flags) :
	c_screen_widget(0x15, a, b, user_flags)
{
	value614 = 0;
}

// @retail 0x2ba45b
c_screen_widget *__stdcall function_2ba45b(s_screen_parameters *parameters)
{
	c_screen_45c388 *screen = new c_screen_45c388(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2ba455
screen_load_proc c_screen_45c388::get_load_proc()
{
	return function_2ba45b;
}

void function_148a8d();
void function_148c21();

/* the screen transition state (cleared by 0x148bff and 0x148c21; defined by
   unknown_249fa3.cpp) */

class c_screen_45c3f8 : public c_screen_widget
{
public:
	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	byte unknown610[0x614 - 0x610];
	long mode;
};

// @retail 0x2bac18
bool c_screen_45c3f8::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		switch (event->param)
		{
		case 1:
		case 13:
			switch (mode)
			{
			case 0:
				function_148c21();
				break;
			case 1:
				function_148a8d();
				break;
			default:
				__assume(0);
			}
			break;
		}
	}
	return c_screen_widget::v10(event);
}

/* the screens at 0x4590b8 and 0x459148 */
class c_screen_4590b8 : public c_screen_widget
{
public:
	virtual bool v10(s_widget_event *event);
};

// @retail 0x2b51b7
bool c_screen_4590b8::v10(s_widget_event *event)
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

class c_screen_45bff8 : public c_screen_widget
{
public:
	virtual bool v10(s_widget_event *event);

	byte unknown610[0xa4c - 0x610];
	bool busy;
};

// @retail 0x2b81b9
bool c_screen_45bff8::v10(s_widget_event *event)
{
	if (busy)
		return busy;
	return c_screen_widget::v10(event);
}

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

	byte unknown610[0x8b4 - 0x610];
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

	byte unknown610[0xdb8 - 0x610];
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

	byte unknown88[0x3a8 - 0x88];
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
	c_settings_list(word user_flags);

	virtual long get_item_count();

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[3];
	c_list_item_handler handler;
	bool extended;
};

// @retail 0x2b27f9
void c_list_item_handler::invoke(s_controller_reference **controller, long *item)
{
	(owner->*method)(controller, item);
}

// @retail 0x2b1cca
c_settings_list::c_settings_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_settings_list::handle_item)
{
	extended = false;
	data = user_interface_data_new("settings list", 2, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

/* the item handler the list's constructor (0x2b1cca) registers */
// @retail 0x2b1da9
void c_settings_list::handle_item(s_controller_reference **controller, long *item)
{
	s_data_array *items = data;

	if (datum_get(items, *item))
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		switch (*(short *)item)
		{
		case 0:
			function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 5, 4, (long)function_2b19dc);
			parameters.load(&parameters);
			break;
		case 1:
			function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 5, 4, (long)function_2b1b01);
			parameters.load(&parameters);
			break;
		}
	}
}

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

/* the item sources of the lists at 0x45b3e0 and 0x45b510 */
s_data_array *g_46e7bc;
s_data_array *g_46e7c0;

class c_list_45b3e0 : public c_list_widget_with_items
{
public:
	virtual void v1();
	virtual void *get_item_data();
	virtual long get_item_count();
	virtual void *get_items(long *count);

	byte unknown88[0xac - 0x88];
	byte item_data[4];
	byte unknownb0[0x4ac - 0xb0];
	long item_count;
};

/* slot 1 of the list (c_widget::v9 in unknown_19b516.h's view) */
// @retail 0x2b2e19
void c_list_45b3e0::v1()
{
	data = g_46e7bc;
	item_count = NONE;
	((c_widget *)this)->m7f = 0;
	((c_widget *)this)->c_widget::v9();
}

class c_list_45b510 : public c_list_widget_with_items
{
public:
	virtual void v1();

	byte unknown88[0x4ac - 0x88];
	long item_count;
};

// @retail 0x2b3f78
void c_list_45b510::v1()
{
	data = g_46e7c0;
	item_count = NONE;
	((c_widget *)this)->m7f = 0;
	((c_widget *)this)->c_widget::v9();
}

class c_playlist_saved_game_file_list : public c_list_widget
{
public:
	virtual void v3();
};

// @retail 0x2b1f5a
void c_playlist_saved_game_file_list::v3()
{
	((c_widget *)this)->c_widget::v11();
}

class c_y_menu_player_selected_list : public c_list_widget
{
public:
	virtual void v1();
};

// @retail 0x2b5575
void c_y_menu_player_selected_list::v1()
{
	((c_widget *)this)->c_widget::v9();
}

class c_widget_45b570 : public c_user_interface_widget
{
public:
	virtual void v1();

	c_user_interface_widget *focused;
};

/* slot 1 of the widget: remembers its first child, then runs the base slot
   (0x22e315, stubbed as c_widget::function_22e315) */
// @retail 0x2b41db
void c_widget_45b570::v1()
{
	if (child)
		focused = child;
	((c_widget *)this)->function_22e315();
}

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
	c_squad_privacy_setting_list(word user_flags);

	virtual long get_item_count();

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[3];
	c_list_item_handler handler;
};

bool function_6c7e0();
void function_19a148(long privacy);

// @retail 0x2b51ee
c_squad_privacy_setting_list::c_squad_privacy_setting_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_squad_privacy_setting_list::handle_item)
{
	data = user_interface_data_new("squad privacy setting list", 3, 4);
	data_make_valid(data);
	list_item_add(this, 0);
	if (function_6c7e0())
	{
		list_item_add(this, 1);
	}
	list_item_add(this, 2);
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b5324
void c_squad_privacy_setting_list::handle_item(s_controller_reference **controller, long *item)
{
	s_list_item_datum *datum = (s_list_item_datum *)datum_get(data, *item);

	if (datum)
	{
		function_19a148(datum->item);
		function_14800c(v11(), v12());
	}
}

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

	byte unknown88[0x8c - 0x88];
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
