#include <string.h>
#include "cseries.h"
#include "globals.h"
#include "screen_widgets.h"
#include "user_interface_lists.h"
#include "unknown_19b516.h"
#include "unknown_2b116a.h"
#include "unknown_18f576.h"

// @flags /O1 /Oi /Gr

/* UNKNOWN_2B116A.CPP: the small virtual methods of the screens and lists
   built in 0x2b0000..0x2bbfff (their load procedures and constructors sit next
   to them). Each class is named by its list's debug name where its
   constructor gives one, else by its retail vtable. */

long function_1480ff(long screen_id);
void function_236299(long sound);

/* the load procedures (some not decompiled yet: stubs in src/stubs/lane_e.cpp) */
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
	byte unknown08[4];
	short x;
	short y;
	byte unknown10[0x58 - 0x10];
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

/* a bitmap of a bitmap tag (0x74 bytes) */
struct bitmap_data
{
	byte unknown00[4];
	short width;
	short height;
	byte unknown08[0x74 - 8];
};

struct s_widget_view_2b0a
{
	byte unknown00[0x20];
	s_widget_bounds widget_bounds;
	byte unknown28[0x6e - 0x28];
	bool enabled;
	byte unknown6f;
	s_widget_item *definition;
	byte bounds[0x10];
	short value84;
	short value86;
	short value88;
	byte unknown8a[2];
	bitmap_data *bitmap;
};

struct s_bitmap_tag_2b0a
{
	byte unknown00[0x44];
	long count;
	bitmap_data *bitmaps;
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

/* the widget's bitmap: the one set, else the definition's */
// @retail 0x2b0b19
bitmap_data *function_2b0b19(s_widget_view_2b0a *widget)
{
	bitmap_data *bitmap = widget->bitmap;

	if (bitmap)
	{
		return bitmap;
	}
	else
	{
		s_bitmap_tag_2b0a *tag = (s_bitmap_tag_2b0a *)g_4e3b44[*(long *)((byte *)widget->definition + 0x1c) & 0xffff].bytes;
		short index = widget->value88;

		if (index >= 0 && index < tag->count)
			return &tag->bitmaps[index];
		return tag->bitmaps;
	}
}

/* sets the widget's bitmap and sizes the widget to it */
// @retail 0x2b0a7b
void function_2b0a7b(s_widget_view_2b0a *widget, bitmap_data *bitmap)
{
	s_widget_bounds bounds;
	bitmap_data *shown;

	memset(&bounds, 0, sizeof(bounds));

	widget->bitmap = bitmap;
	shown = function_2b0b19(widget);
	if (shown)
	{
		bounds.top = widget->definition->y;
		bounds.left = widget->definition->x;
		bounds.bottom = widget->definition->y - shown->height;
		bounds.right = widget->definition->x + shown->width;
	}
	widget->widget_bounds = bounds;
}

// @retail 0x2b12ba
bool function_2b12ba(s_widget_view_2b0a *widget)
{
	s_widget_item *definition = widget->definition;

	return definition ? (definition->flags & 1) : false;
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

/* a player's profile (unknown_18f576.cpp) */
struct s_player_profile
{
	dword data[0x78];
};

void player_slot_get_profile(long index, s_player_profile *profile, long *profile_index);
c_screen_widget *__stdcall function_231db5(s_screen_parameters *parameters);

/* ---- screens ---- */

// @retail 0x2b130a
c_screen_widget *__stdcall function_2b130a(s_screen_parameters *parameters)
{
	c_level_select_screen *screen = new c_level_select_screen(parameters->a, parameters->b, parameters->user_flags, false);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x2b136d
c_screen_widget *__stdcall function_2b136d(s_screen_parameters *parameters)
{
	c_level_select_screen *screen = new c_level_select_screen(parameters->a, parameters->b, parameters->user_flags, true);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x2b13b3
c_level_select_screen::c_level_select_screen(long a, long b, word user_flags, bool alternate) :
	c_screen_with_menu(0xb, a, b, user_flags, &list),
	list(user_flags, alternate)
{
}

// @retail 0x2b13fe deleting c_level_select_screen
// @retail 0x2b1452 destructor c_level_select_screen
// @retail 0x2b141c destructor c_campaign_level_handles_list

// @retail 0x2b13ea
screen_load_proc c_level_select_screen::get_load_proc()
{
	return list.alternate ? function_2b136d : function_2b130a;
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

// @retail 0x2b1ab3
c_variant_game_engine_type_screen::c_variant_game_engine_type_screen(long a, long b, word user_flags, long screen_id, bool edit_settings, bool create, bool edit_alternate) :
	c_screen_with_menu(screen_id, a, b, user_flags, &list),
	list(user_flags)
{
	this->edit_settings = edit_settings;
	this->create = create;
	this->edit_alternate = edit_alternate;
}

// @retail 0x2b1b01
c_screen_widget *__stdcall function_2b1b01(s_screen_parameters *parameters)
{
	c_variant_game_engine_type_screen *screen = new c_variant_game_engine_type_screen(parameters->a, parameters->b, parameters->user_flags, 0x3c, true, false, false);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b1b43
c_screen_widget *__stdcall function_2b1b43(s_screen_parameters *parameters)
{
	c_variant_game_engine_type_screen *screen = new c_variant_game_engine_type_screen(parameters->a, parameters->b, parameters->user_flags, 0x3c, false, true, false);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b1b85
c_screen_widget *__stdcall function_2b1b85(s_screen_parameters *parameters)
{
	c_variant_game_engine_type_screen *screen = new c_variant_game_engine_type_screen(parameters->a, parameters->b, parameters->user_flags, 0xcf, false, false, false);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b1bc9
c_screen_widget *__stdcall function_2b1bc9(s_screen_parameters *parameters)
{
	c_variant_game_engine_type_screen *screen = new c_variant_game_engine_type_screen(parameters->a, parameters->b, parameters->user_flags, 0x3c, false, false, true);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b1c61 deleting c_variant_game_engine_type_screen
// @retail 0x2b1cb5 destructor c_variant_game_engine_type_screen
// @retail 0x2b1c7f destructor c_game_engine_variant_category_list

// @retail 0x2b1c0b
void c_variant_game_engine_type_screen::v17()
{
	list.edit_settings = edit_settings;
	list.create = create;
	list.edit_alternate = edit_alternate;
}

// @retail 0x2b1c30
screen_load_proc c_variant_game_engine_type_screen::get_load_proc()
{
	if (create)
		return function_2b1b43;
	if (edit_settings)
		return function_2b1b01;
	return edit_alternate ? function_2b1bc9 : function_2b1b85;
}

/* the xbox live appear offline screen (vtable 0x45b220) */
class c_xbox_live_appear_offline_screen : public c_screen_with_menu
{
public:
	c_xbox_live_appear_offline_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_xbox_live_appear_offline_list list;
};

// @retail 0x2b2801
c_screen_widget *__stdcall function_2b2801(s_screen_parameters *parameters)
{
	c_xbox_live_appear_offline_screen *screen = new c_xbox_live_appear_offline_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b283f
c_xbox_live_appear_offline_screen::c_xbox_live_appear_offline_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x3a, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b2871
screen_load_proc c_xbox_live_appear_offline_screen::get_load_proc()
{
	return function_2b2801;
}

// @retail 0x147494 deleting c_xbox_live_appear_offline_screen
// @retail 0x2b2877 destructor c_xbox_live_appear_offline_screen

/* the voice mask screen (vtable 0x45b290) */
class c_voice_mask_screen : public c_screen_with_menu
{
public:
	c_voice_mask_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_voice_mask_list list;
};

// @retail 0x2b288c
c_screen_widget *__stdcall function_2b288c(s_screen_parameters *parameters)
{
	c_voice_mask_screen *screen = new c_voice_mask_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b4ed2
c_screen_widget *__stdcall function_2b4ed2(s_screen_parameters *parameters)
{
	c_voice_mask_screen *screen = new c_voice_mask_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value188 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b28ca
c_voice_mask_screen::c_voice_mask_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x36, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b28fc
screen_load_proc c_voice_mask_screen::get_load_proc()
{
	return function_2b288c;
}

/* the voice through tv screen (vtable 0x45b300) */
class c_voice_through_tv_screen : public c_screen_with_menu
{
public:
	c_voice_through_tv_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_voice_through_tv_list list;
};

// @retail 0x2b2917
c_screen_widget *__stdcall function_2b2917(s_screen_parameters *parameters)
{
	c_voice_through_tv_screen *screen = new c_voice_through_tv_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b4f17
c_screen_widget *__stdcall function_2b4f17(s_screen_parameters *parameters)
{
	c_voice_through_tv_screen *screen = new c_voice_through_tv_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value288 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b2955
c_voice_through_tv_screen::c_voice_through_tv_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x37, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b2987
screen_load_proc c_voice_through_tv_screen::get_load_proc()
{
	return function_2b2917;
}

/* the thumbstick settings screen (vtable 0x45b628) */
class c_thumbstick_settings_screen : public c_screen_with_menu
{
public:
	c_thumbstick_settings_screen(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_thumbstick_settings_edit_list list;
	long value8b8;
};

// @retail 0x2b4274
c_screen_widget *__stdcall function_2b4274(s_screen_parameters *parameters)
{
	c_thumbstick_settings_screen *screen = new c_thumbstick_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b42b6
c_screen_widget *__stdcall function_2b42b6(s_screen_parameters *parameters)
{
	c_thumbstick_settings_screen *screen = new c_thumbstick_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value288 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b42ff
c_thumbstick_settings_screen::c_thumbstick_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x29, a, b, user_flags, &list),
	list(user_flags),
	value8b8(NONE)
{
	if (b != 4)
	{
		set_screen_id(0xeb);
	}
}

// @retail 0x2b434c
void c_thumbstick_settings_screen::v19()
{
	switch (g_54e5d0.settings.thumbstick_layout)
	{
	case 0:
		list.select_item(0);
		break;
	case 1:
		list.select_item(1);
		break;
	case 2:
		list.select_item(2);
		break;
	case 3:
		list.select_item(3);
		break;
	default:
		list.select_item(0);
		break;
	}
	c_screen_widget::v19();
}

// @retail 0x2b4391
screen_load_proc c_thumbstick_settings_screen::get_load_proc()
{
	return function_2b4274;
}

// @retail 0x2b4688 deleting c_thumbstick_settings_screen
// @retail 0x2b8bf0 destructor c_thumbstick_settings_screen
// @retail 0x2b83a5 destructor c_thumbstick_settings_edit_list

/* the look sensitivity settings screen (vtable 0x45b7d0) */
class c_look_sensitivity_settings_screen : public c_screen_with_menu
{
public:
	c_look_sensitivity_settings_screen(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_look_sensitivity_settings_edit_list list;
};

// @retail 0x2b4397
c_screen_widget *__stdcall function_2b4397(s_screen_parameters *parameters)
{
	c_look_sensitivity_settings_screen *screen = new c_look_sensitivity_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b43d5
c_screen_widget *__stdcall function_2b43d5(s_screen_parameters *parameters)
{
	c_look_sensitivity_settings_screen *screen = new c_look_sensitivity_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value288 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b441a
c_look_sensitivity_settings_screen::c_look_sensitivity_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x2a, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b444c
void c_look_sensitivity_settings_screen::v19()
{
	byte sensitivity = g_54e5d0.settings.look_sensitivity;
	short item = sensitivity < 1 ? 1 : sensitivity > 10 ? 10 : sensitivity;

	list.select_item(item - 1);
	c_screen_widget::v19();
}

// @retail 0x2b447f
screen_load_proc c_look_sensitivity_settings_screen::get_load_proc()
{
	return function_2b4397;
}

/* the invert look settings screen (vtable 0x45b708) */
class c_invert_look_settings_screen : public c_screen_with_menu
{
public:
	c_invert_look_settings_screen(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_invert_look_settings_edit_list list;
};

// @retail 0x2b4485
c_screen_widget *__stdcall function_2b4485(s_screen_parameters *parameters)
{
	c_invert_look_settings_screen *screen = new c_invert_look_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b44c3
c_screen_widget *__stdcall function_2b44c3(s_screen_parameters *parameters)
{
	c_invert_look_settings_screen *screen = new c_invert_look_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value188 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b4508
c_invert_look_settings_screen::c_invert_look_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x2b, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b453a
void c_invert_look_settings_screen::v19()
{
	if (TEST_FIELD_BIT(g_54e5d0.settings.controller_flags.invert_look))
	{
		list.select_item(0);
	}
	else
	{
		list.select_item(1);
	}
	c_screen_widget::v19();
}

// @retail 0x2b455f
screen_load_proc c_invert_look_settings_screen::get_load_proc()
{
	return function_2b4485;
}

// @retail 0x2b4789 deleting c_invert_look_settings_screen
// @retail 0x2b2902 destructor c_invert_look_settings_screen
// @retail 0x2b488c destructor c_invert_look_settings_edit_list

/* the button settings screen (vtable 0x45b840) */
class c_button_settings_screen : public c_screen_with_menu
{
public:
	c_button_settings_screen(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_button_settings_edit_list list;
	long value8b8;
};

// @retail 0x2b4565
c_screen_widget *__stdcall function_2b4565(s_screen_parameters *parameters)
{
	c_button_settings_screen *screen = new c_button_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b45a7
c_screen_widget *__stdcall function_2b45a7(s_screen_parameters *parameters)
{
	c_button_settings_screen *screen = new c_button_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value288 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b45f0
c_button_settings_screen::c_button_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x28, a, b, user_flags, &list),
	list(user_flags),
	value8b8(NONE)
{
	if (b != 4)
	{
		set_screen_id(0xea);
	}
}

// @retail 0x2b463d
void c_button_settings_screen::v19()
{
	switch (g_54e5d0.settings.button_layout)
	{
	case 0:
		list.select_item(0);
		break;
	case 1:
		list.select_item(1);
		break;
	case 2:
		list.select_item(2);
		break;
	case 3:
		list.select_item(3);
		break;
	default:
		list.select_item(0);
		break;
	}
	c_screen_widget::v19();
}

// @retail 0x2b4682
screen_load_proc c_button_settings_screen::get_load_proc()
{
	return function_2b4565;
}

/* the auto level settings screen (vtable 0x45b5b8) */
class c_auto_level_settings_screen : public c_screen_with_menu
{
public:
	c_auto_level_settings_screen(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_auto_level_settings_edit_list list;
};

// @retail 0x2b46a6
c_screen_widget *__stdcall function_2b46a6(s_screen_parameters *parameters)
{
	c_auto_level_settings_screen *screen = new c_auto_level_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b46e4
c_screen_widget *__stdcall function_2b46e4(s_screen_parameters *parameters)
{
	c_auto_level_settings_screen *screen = new c_auto_level_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value188 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b4729
c_auto_level_settings_screen::c_auto_level_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x2c, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b475b
void c_auto_level_settings_screen::v19()
{
	if (TEST_FIELD_BIT(g_54e5d0.settings.controller_flags.auto_level))
	{
		list.select_item(0);
	}
	else
	{
		list.select_item(1);
	}
	c_screen_widget::v19();
}

// @retail 0x2b4783
screen_load_proc c_auto_level_settings_screen::get_load_proc()
{
	return function_2b46a6;
}

/* the vibration settings screen (vtable 0x45b698) */
class c_vibration_settings_screen : public c_screen_with_menu
{
public:
	c_vibration_settings_screen(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_vibration_settings_edit_list list;
};

// @retail 0x2b47a7
c_screen_widget *__stdcall function_2b47a7(s_screen_parameters *parameters)
{
	c_vibration_settings_screen *screen = new c_vibration_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b47e5
c_screen_widget *__stdcall function_2b47e5(s_screen_parameters *parameters)
{
	c_vibration_settings_screen *screen = new c_vibration_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.value188 = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b482a
c_vibration_settings_screen::c_vibration_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xb7, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b485f
void c_vibration_settings_screen::v19()
{
	if (!TEST_FIELD_BIT(g_54e5d0.settings.controller_flags.vibration))
	{
		list.select_item(0);
	}
	else
	{
		list.select_item(1);
	}
	c_screen_widget::v19();
}

// @retail 0x2b4886
screen_load_proc c_vibration_settings_screen::get_load_proc()
{
	return function_2b47a7;
}

// @retail 0x2b5406
c_screen_widget *__stdcall function_2b5406(s_screen_parameters *parameters)
{
	c_clan_member_privileges_screen *screen = new c_clan_member_privileges_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b5444
c_clan_member_privileges_screen::c_clan_member_privileges_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xec, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b547f deleting c_clan_member_privileges_screen
// @retail 0x2b549d destructor c_clan_member_privileges_screen
// @retail 0x2b53ea deleting c_clan_member_privileges_list

// @retail 0x2b5479
screen_load_proc c_clan_member_privileges_screen::get_load_proc()
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
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();
};

/* marks the widget and its children as animated in (bit 3 of the
   animation's flags) */
// @retail 0x2b730e
void __stdcall function_2b730e(c_user_interface_widget *widget)
{
	widget->animation.flags.flag3 = true;
	for (c_user_interface_widget *child = widget->child; child; child = child->next)
	{
		function_2b730e(child);
	}
}

// @retail 0x2b7289
void c_screen_45bc60::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, 0, 0 }
		}
	};

	build(&layout);
	c_user_interface_widget::v1();
	start_animation(4);
	function_2b730e(this);
}

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
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();
};

// @retail 0x2b740a
void c_screen_45bd40::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, 0, 0 }
		}
	};

	build(&layout);
	c_user_interface_widget::v1();
	function_236299(8);
}

// @retail 0x2b7394
screen_load_proc c_screen_45bd40::get_load_proc()
{
	return function_2b739a;
}

/* the controller settings screen (vtable 0x45be30) */
class c_controller_settings_screen : public c_screen_with_menu
{
public:
	c_controller_settings_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_controller_settings_edit_list list;
};

// @retail 0x2b7714
c_screen_widget *__stdcall function_2b7714(s_screen_parameters *parameters)
{
	c_controller_settings_screen *screen = new c_controller_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b7752
c_controller_settings_screen::c_controller_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x27, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b7784
screen_load_proc c_controller_settings_screen::get_load_proc()
{
	return function_2b7714;
}

/* the multiplayer settings screen (vtable 0x45bef8) */
class c_multiplayer_settings_screen : public c_screen_with_menu
{
public:
	c_multiplayer_settings_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_multiplayer_settings_edit_list list;
};

// @retail 0x2b77a8
c_screen_widget *__stdcall function_2b77a8(s_screen_parameters *parameters)
{
	c_multiplayer_settings_screen *screen = new c_multiplayer_settings_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b77e6
c_multiplayer_settings_screen::c_multiplayer_settings_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0x2f, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b7818
screen_load_proc c_multiplayer_settings_screen::get_load_proc()
{
	return function_2b77a8;
}

// @retail 0x2b781e deleting c_multiplayer_settings_screen
// @retail 0x2b7872 destructor c_multiplayer_settings_screen

/* the subtitle setting screen (vtable 0x45bf68; its deleting destructor is
   folded with c_settings_screen's) */
class c_subtitle_setting_screen : public c_screen_with_menu
{
public:
	c_subtitle_setting_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_subtitle_setting_list list;
};

// @retail 0x2b7887
c_screen_widget *__stdcall function_2b7887(s_screen_parameters *parameters)
{
	c_subtitle_setting_screen *screen = new c_subtitle_setting_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b78c5
c_subtitle_setting_screen::c_subtitle_setting_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xf2, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2b78fa
screen_load_proc c_subtitle_setting_screen::get_load_proc()
{
	return function_2b7887;
}

bool function_19a951(long player_index);
bool function_19ab77(long player_index);

/* an item of a list whose data holds a value per item */
struct s_list_item_value
{
	short salt;
	short item;
	long value;
};

/* "potential squad leader player list" (vtable 0x45c150) */
class c_potential_squad_leader_player_list : public c_list_widget
{
public:
	c_potential_squad_leader_player_list(word user_flags);

	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);
	void fill();

	c_list_item_widget items[4];
	bool value288;
	c_list_item_handler handler;
};

/* the squad leader screen (vtable 0x45c0e0) */
class c_screen_45c0e0 : public c_screen_widget
{
public:
	c_screen_45c0e0(long a, long b, word user_flags);

	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	c_potential_squad_leader_player_list list;
};

// @retail 0x2b85d9
void c_screen_45c0e0::v18(void *parameters)
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

	build(&layout);
	c_user_interface_widget::v1();
}

// @retail 0x2b8536
c_screen_widget *__stdcall function_2b8536(s_screen_parameters *parameters)
{
	c_screen_45c0e0 *screen = new c_screen_45c0e0(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b8572
c_screen_45c0e0::c_screen_45c0e0(long a, long b, word user_flags) :
	c_screen_widget(0xb1, a, b, user_flags),
	list(user_flags)
{
}

// @retail 0x2b831d
screen_load_proc c_screen_45c0e0::get_load_proc()
{
	return function_2b8536;
}

// @retail 0x2b85a6 deleting c_screen_45c0e0
// @retail 0x2b85c4 destructor c_screen_45c0e0

// @retail 0x2b8323
c_potential_squad_leader_player_list::c_potential_squad_leader_player_list(word user_flags) :
	c_list_widget(user_flags),
	value288(false),
	handler(this, (list_item_method)&c_potential_squad_leader_player_list::handle_item)
{
	data = user_interface_data_new("potential squad leader player list", 16, 8);
	data_make_valid(data);
	delegate_register(&item_handlers, &handler);
	fill();
}

// @retail 0x2b8446
void c_potential_squad_leader_player_list::fill()
{
	long players[16];
	long count = 0;

	for (long i = 0; i < 16; i++)
	{
		if (function_19a951(i) && !function_19ab77(i))
		{
			players[count++] = i;
		}
	}
	while (--count >= 0)
	{
		((s_list_item_value *)data->data)[datum_new(data) & 0xffff].value = players[count];
	}
}

/* ---- the live feedback dialog: feedback on a player (mode 1) or a clan
   (mode 2) ---- */

/* "feedback list" (vtable 0x45c1d0) */
class c_feedback_list : public c_list_widget
{
public:
	c_feedback_list(word user_flags, long mode);
	~c_feedback_list();

	/* folded with c_widget's v2 */
	virtual void *get_item_data() { return items; }
	virtual long get_item_count() { return 4; }
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	c_list_item_handler handler;
	long kind;
	long mode;
};

/* the live feedback dialog (vtable 0x45c228) */
class c_live_feedback_dialog_screen : public c_screen_with_menu
{
public:
	c_live_feedback_dialog_screen(long a, long b, word user_flags, long mode);

	virtual screen_load_proc get_load_proc();

	c_feedback_list list;
	long mode;
};

/* the player or clan the feedback is about (0x78 bytes, from
   function_14887e) */
#pragma pack(push, 4)
struct s_feedback_target
{
	long type;
	union
	{
		unsigned __int64 player_xuid;
		unsigned __int64 clan_id;
	};
	byte unknown0c[0x78 - 0x0c];
};
#pragma pack(pop)

/* the target's player or clan, when it is one */
static __forceinline unsigned __int64 *feedback_target_xuid(s_feedback_target *target)
{
	unsigned __int64 *xuid = 0;

	switch (target->type)
	{
	case 1:
		xuid = &target->player_xuid;
		break;
	case 2:
		xuid = &target->clan_id;
		break;
	}
	return xuid;
}

struct s_screen_settings_54dc6c;
void function_14887e(s_screen_settings_54dc6c *settings);
void __stdcall function_19b5af(long a, long message, long b, dword controller_flags, void *callback0, void *callback1, long c);
bool __stdcall function_8c150(void *cache, long a, unsigned __int64 *xuid, void *data, unsigned __int64 *clan_id);
struct _XUID;
void online_feedback_send(const _XUID *xuid, unsigned long controller_index, long kind);
void function_14800c(long channel, long index);

extern byte g_4771c8[0x2580];

/* the list waiting for the dialog's answer */
c_feedback_list *g_51ecbc;

// @retail 0x2b86fa
c_feedback_list::c_feedback_list(word user_flags, long mode) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_feedback_list::handle_item)
{
	s_feedback_target target;
	byte data_buffer[0x20];
	unsigned __int64 clan_id;
	bool in_clan;
	unsigned __int64 *xuid;

	this->mode = mode;
	function_14887e((s_screen_settings_54dc6c *)&target);
	xuid = feedback_target_xuid(&target);
	if (function_8c150(g_4771c8, 0, xuid, data_buffer, &clan_id) && clan_id)
	{
		in_clan = true;
	}
	else
	{
		in_clan = false;
	}
	data = user_interface_data_new("feedback list", 10, 4);
	data_make_valid(data);
	if (this->mode == 1)
	{
		list_item_add(this, 0);
		list_item_add(this, 1);
		list_item_add(this, 2);
		list_item_add(this, 3);
		list_item_add(this, 4);
		list_item_add(this, 5);
		list_item_add(this, 6);
		if (in_clan)
		{
			list_item_add(this, 7);
		}
	}
	else if (this->mode == 2)
	{
		list_item_add(this, 8);
		list_item_add(this, 9);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b862d
c_feedback_list::~c_feedback_list()
{
	g_51ecbc = 0;
}

// @retail 0x2b8670 deleting c_feedback_list

// @retail 0x2b88d1
void c_feedback_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);
	s_list_item_datum *datum = (s_list_item_datum *)datum_get(data, widget_item(widget)->value70);

	if (text && datum)
	{
		long string_id;

		switch (datum->item)
		{
		case 0:
			string_id = 0xd000291;
			break;
		case 1:
			string_id = 0xb000292;
			break;
		case 2:
			string_id = 0xc000293;
			break;
		case 3:
			string_id = 0x8000294;
			break;
		case 4:
			string_id = 0x10000295;
			break;
		case 5:
			string_id = 0xa000296;
			break;
		case 6:
			string_id = 0x8000297;
			break;
		case 7:
			string_id = 0xd000298;
			break;
		case 9:
			string_id = 0x1500029a;
			break;
		case 8:
			string_id = 0xc000299;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

/* the dialog's answer: sends the feedback */
// @retail 0x2b8989
bool __stdcall function_2b8989(long controller)
{
	if (g_51ecbc)
	{
		s_feedback_target target;
		unsigned __int64 *xuid;

		function_14887e((s_screen_settings_54dc6c *)&target);
		xuid = feedback_target_xuid(&target);
		if (xuid && *xuid)
		{
			online_feedback_send((const _XUID *)xuid, controller, g_51ecbc->kind);
		}
		function_14800c(g_51ecbc->v11(), g_51ecbc->v12());
	}
	return true;
}

// @retail 0x2b89ee
void c_feedback_list::handle_item(s_controller_reference **controller, long *item)
{
	s_list_item_datum *datum = (s_list_item_datum *)datum_get(data, *item);

	if (datum)
	{
		s_feedback_target target;
		unsigned __int64 *xuid;
		long feedback = datum->item;

		kind = feedback;
		function_14887e((s_screen_settings_54dc6c *)&target);
		xuid = feedback_target_xuid(&target);
		if (xuid && *xuid)
		{
			if (feedback > 1)
			{
				g_51ecbc = this;
				function_19b5af(1, 0xa5, 4, 1 << (*controller)->controller_index, function_2b8989, 0, 0);
				return;
			}
			online_feedback_send((const _XUID *)xuid, (*controller)->controller_index, feedback);
		}
		function_14800c(v11(), v12());
	}
}

// @retail 0x2b868e
c_live_feedback_dialog_screen::c_live_feedback_dialog_screen(long a, long b, word user_flags, long mode) :
	c_screen_with_menu(0x22, a, b, user_flags, &list),
	list(user_flags, mode)
{
	this->mode = mode;
}

// @retail 0x2b86c7 deleting c_live_feedback_dialog_screen
// @retail 0x2b86e5 destructor c_live_feedback_dialog_screen

// @retail 0x2b8a89
c_screen_widget *live_feedback_dialog_load(s_screen_parameters *parameters, long mode)
{
	c_live_feedback_dialog_screen *screen = new c_live_feedback_dialog_screen(parameters->a, parameters->b, parameters->user_flags, mode);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x2b8acd
c_screen_widget *__stdcall function_2b8acd(s_screen_parameters *parameters)
{
	return live_feedback_dialog_load(parameters, 0);
}

// @retail 0x2b8add
c_screen_widget *__stdcall function_2b8add(s_screen_parameters *parameters)
{
	return live_feedback_dialog_load(parameters, 1);
}

// @retail 0x2b8aed
c_screen_widget *__stdcall function_2b8aed(s_screen_parameters *parameters)
{
	return live_feedback_dialog_load(parameters, 2);
}

// @retail 0x2b8afd
screen_load_proc c_live_feedback_dialog_screen::get_load_proc()
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

/* a list of up to four items that calls back with the chosen item
   (vtable 0x45c318) */
class c_list_45c318 : public c_list_widget
{
public:
	c_list_45c318(word user_flags);

	virtual long get_item_count();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	c_list_item_widget items[4];
	bool (__stdcall *callback)(long controller_index, long item);
	c_list_item_handler handler;
};

/* the screen of the list of 0x45c318 (vtable 0x45c2a8; its deleting
   destructor is folded with c_thumbstick_settings_screen's) */
class c_screen_45c2a8 : public c_screen_with_menu
{
public:
	c_screen_45c2a8(long a, long b, word user_flags);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_list_45c318 list;
	/* the string of the title text */
	long value8b8;
};

// @retail 0x2b8b2d
c_screen_widget *__stdcall function_2b8b2d(s_screen_parameters *parameters)
{
	c_screen_45c2a8 *screen = new c_screen_45c2a8(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2b8b71
c_screen_45c2a8::c_screen_45c2a8(long a, long b, word user_flags) :
	c_screen_with_menu(0xe6, a, b, user_flags, &list),
	list(user_flags),
	value8b8(0)
{
}

// @retail 0x2b8bad
void c_screen_45c2a8::v19()
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)find_child(6, 2, false);

	if (text)
	{
		text->set_string(value8b8);
	}
	c_screen_widget::v19();
}

// @retail 0x2b8b6b
screen_load_proc c_screen_45c2a8::get_load_proc()
{
	return function_2b8b2d;
}

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

// @retail 0x2b8c51
c_list_45c318::c_list_45c318(word user_flags) :
	c_list_widget(user_flags),
	callback(0),
	handler(this, (list_item_method)&c_list_45c318::handle_item)
{
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b8d07
void c_list_45c318::handle_item(s_controller_reference **controller, long *item)
{
	if (callback)
	{
		s_list_item_value *entry = &((s_list_item_value *)data->data)[*item & 0xffff];

		if (callback((*controller)->controller_index, entry->item))
		{
			get_screen()->start_animation(3);
		}
	}
}

class c_screen_45c388 : public c_screen_widget
{
public:
	c_screen_45c388(long a, long b, word user_flags);

	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	byte unknown610[4];
	long value614;
	c_user_interface_widget *bitmaps[4];
};

/* builds the screen and finds its four bitmaps */
// @retail 0x2ba4c0
void c_screen_45c388::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, 0, 0 }
		}
	};
	word i;

	build(&layout);
	for (i = 0; i < 4; i++)
	{
		bitmaps[i] = find_child(8, i + 1, false);
	}
	c_user_interface_widget::v1();
}

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

/* "actions list" (vtable 0x45c468): copy, edit or delete the player profile
   (mode 0) or the game variant (mode 1) being edited */
class c_actions_list : public c_list_widget
{
public:
	c_actions_list(word user_flags);

	virtual void *get_item_data();
	/* folded with c_squad_privacy_setting_list's */
	virtual long get_item_count() { return 3; }
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);

	long mode;
	c_list_item_widget items[3];
	c_list_item_handler handler;
};

/* the actions screen (vtable 0x45c3f8) */
class c_actions_screen : public c_screen_with_menu
{
public:
	c_actions_screen(long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual screen_load_proc get_load_proc();

	long mode;
	c_actions_list list;
};

// @retail 0x2baa9b
c_screen_widget *__stdcall function_2baa9b(s_screen_parameters *parameters)
{
	c_actions_screen *screen = new c_actions_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->mode = 0;
	screen->list.mode = 0;
	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2baae7
c_screen_widget *__stdcall function_2baae7(s_screen_parameters *parameters)
{
	c_actions_screen *screen = new c_actions_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->mode = 1;
	screen->list.mode = 1;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2bab33
c_actions_screen::c_actions_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xc2, a, b, user_flags, &list),
	mode(0),
	list(user_flags)
{
}

// @retail 0x2bab6f deleting c_actions_screen
// @retail 0x2bab8d destructor c_actions_screen

// @retail 0x2bac18
bool c_actions_screen::v10(s_widget_event *event)
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
screen_load_proc c_actions_screen::get_load_proc()
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
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc();

	/* the screen's list (not written yet) */
	byte list[4];
};

bool function_1999b3(void);

/* builds the screen around its list; the text says whether the user leads
   the squad */
// @retail 0x2bbb6d
void c_screen_45c518::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		0,
		1,
		{
			{ 0, 0, (c_list_widget *)list, 0 }
		}
	};
	c_text_widget_45a5e0 *text;

	build(&layout);
	v7((c_user_interface_widget *)list);
	c_user_interface_widget::v1();
	text = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	if (text)
	{
		text->set_string(function_1999b3() ? 0xb0005f9 : 0xa0005f8);
	}
}

// @retail 0x2bb299
screen_load_proc c_screen_45c518::get_load_proc()
{
	return function_2bbacb;
}

/* the difficulty screen (vtable 0x45c588; its deleting destructor is folded
   with c_handicap_settings_screen's) */
class c_difficulty_screen : public c_screen_with_menu
{
public:
	c_difficulty_screen(long a, long b, word user_flags);

	virtual screen_load_proc get_load_proc();

	c_difficulty_list list;
};

// @retail 0x2bb3ed
c_screen_widget *__stdcall function_2bb3ed(s_screen_parameters *parameters)
{
	c_difficulty_screen *screen = new c_difficulty_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.alternate = false;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2bb432
c_screen_widget *__stdcall function_2bb432(s_screen_parameters *parameters)
{
	c_difficulty_screen *screen = new c_difficulty_screen(parameters->a, parameters->b, parameters->user_flags);

	screen->m6c = true;
	screen->list.alternate = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2bb477
c_difficulty_screen::c_difficulty_screen(long a, long b, word user_flags) :
	c_screen_with_menu(0xc, a, b, user_flags, &list),
	list(user_flags)
{
}

// @retail 0x2bb4a9
screen_load_proc c_difficulty_screen::get_load_proc()
{
	return list.alternate ? function_2bb432 : function_2bb3ed;
}

/* the custom game maps screen (vtable 0x45c650): a custom game's maps, or a
   coop game's (alternate) */
class c_custom_game_maps_screen : public c_screen_with_menu
{
public:
	c_custom_game_maps_screen(long a, long b, word user_flags, long screen_id, bool alternate);

	virtual void v19();
	virtual screen_load_proc get_load_proc();

	c_custom_game_maps_list list;
	bool alternate;
};

// @retail 0x2bb2db
c_screen_widget *__stdcall function_2bb2db(s_screen_parameters *parameters)
{
	c_custom_game_maps_screen *screen = new c_custom_game_maps_screen(parameters->a, parameters->b, parameters->user_flags, 0xce, false);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2bb31d
c_screen_widget *__stdcall function_2bb31d(s_screen_parameters *parameters)
{
	c_custom_game_maps_screen *screen = new c_custom_game_maps_screen(parameters->a, parameters->b, parameters->user_flags, 0x11, true);

	screen->m6c = true;
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x2bb29f
c_custom_game_maps_screen::c_custom_game_maps_screen(long a, long b, word user_flags, long screen_id, bool alternate) :
	c_screen_with_menu(screen_id, a, b, user_flags, &list),
	list(user_flags)
{
	this->alternate = alternate;
}

// @retail 0x2bb384 deleting c_custom_game_maps_screen
// @retail 0x2bb3d8 destructor c_custom_game_maps_screen

// @retail 0x2bb373
void c_custom_game_maps_screen::v19()
{
	list.coop = alternate;
	c_screen_widget::v19();
}

// @retail 0x2bb35f
screen_load_proc c_custom_game_maps_screen::get_load_proc()
{
	return alternate ? function_2bb31d : function_2bb2db;
}

/* ---- lists ---- */

class c_campaign_options_list : public c_list_widget
{
public:
	virtual long get_item_count();
	virtual void v20(c_user_interface_widget *widget, long index);
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

/* a playlist the list knows: its saved game file type and index */
struct s_playlist_entry
{
	s_playlist_entry();

	byte type;
	byte unknown01[3];
	long index;
	/* the playlist itself: its name follows a long */
	long unknown08;
	word name[(0x2d9c - 0x0c) / 2];
};

/* an item of the list: a playlist, or a game variant (variant), or the
   item that makes a new one (create) */
struct s_playlist_item
{
	dword salt : 16;
	dword variant : 1;
	dword create : 1;
	dword unused : 14;
	long index;
};

/* "playlist saved game file list" (vtable 0x45b138) */
class c_playlist_saved_game_file_list : public c_list_widget
{
public:
	c_playlist_saved_game_file_list(word user_flags);

	virtual void v3();
	virtual void v20(c_user_interface_widget *widget, long index);

	void handle_item(s_controller_reference **controller, long *item);
	long *find_playlist(byte type, long index);

	c_list_item_widget items[16];
	s_playlist_entry playlists[16];
	bool value2e248;
	bool value2e249;
	bool value2e24a;
	long value2e24c;
	c_list_item_handler handler;
};

/* the playlist listing screen (vtable 0x458c10) */
class c_playlist_listing_screen : public c_screen_with_menu
{
public:
	c_playlist_listing_screen(long a, long b, word user_flags, long mode);

	virtual screen_load_proc get_load_proc();

	c_playlist_saved_game_file_list list;
	byte unknown2e87c[0x2e97c - 0x2e87c];
	long mode;
};

// @retail 0x2b1ece
s_playlist_entry::s_playlist_entry()
{
	index = NONE;
}

// @retail 0x2b1e08
c_playlist_saved_game_file_list::c_playlist_saved_game_file_list(word user_flags) :
	c_list_widget(user_flags),
	value2e248(false),
	value2e249(false),
	value2e24a(false),
	value2e24c(NONE),
	handler(this, (list_item_method)&c_playlist_saved_game_file_list::handle_item)
{
	data = user_interface_data_new("playlist saved game file list", 0x1001, 8);
	data_make_valid(data);
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b1eb0 deleting c_playlist_saved_game_file_list
// @retail 0x230e4d destructor c_playlist_saved_game_file_list

// @retail 0x2b221d
long *c_playlist_saved_game_file_list::find_playlist(byte type, long index)
{
	dword i;
	long *result = 0;

	for (i = 0; i < 16; i++)
	{
		if (playlists[i].type == type && playlists[i].index == index)
		{
			result = &playlists[i].unknown08;
			break;
		}
	}
	return result;
}

bool game_variant_get_name(long index, word *name);

/* shows the playlist's or the variant's name */
// @retail 0x2b1f5f
void c_playlist_saved_game_file_list::v20(c_user_interface_widget *widget, long index)
{
	long datum = widget_item(widget)->value70;

	if (datum != NONE)
	{
		c_user_interface_widget *text = widget->find_child(6, 0, false);

		if (text)
		{
			s_playlist_item *item = &((s_playlist_item *)data->data)[datum & 0xffff];

			if (TEST_FIELD_BIT(item->create))
			{
				((c_text_widget_45a5e0 *)text)->set_string(0x130001a1);
			}
			else
			{
				word *name = (word *)L"";

				if (item->variant)
				{
					word variant_name[0x10];

					if (game_variant_get_name(item->index, variant_name))
					{
						name = variant_name;
					}
				}
				else
				{
					long *playlist = find_playlist(0, item->index);

					if (playlist)
					{
						name = (word *)(playlist + 1);
					}
				}
				text->get_text()->set_text(name);
			}
		}
	}
}

void __stdcall function_2b2181(void *list, long controller_index);

// @retail 0x2b2149
void c_playlist_saved_game_file_list::handle_item(s_controller_reference **controller, long *item)
{
	if (*item != NONE)
	{
		s_playlist_item *datum = (s_playlist_item *)datum_get(data, *item);

		if (datum && datum->variant)
		{
			function_2b2181(this, (*controller)->controller_index);
		}
	}
}

// @retail 0x230dce
c_playlist_listing_screen::c_playlist_listing_screen(long a, long b, word user_flags, long mode) :
	c_screen_with_menu(NONE, a, b, user_flags, &list),
	list(user_flags)
{
	this->mode = mode;
}

// @retail 0x230e2f deleting c_playlist_listing_screen
// @retail 0x230e83 destructor c_playlist_listing_screen

// @retail 0x230c8d
c_screen_widget *__stdcall function_230c8d(s_screen_parameters *parameters)
{
	long screen_id = parameters->a == 3 ? 0xed : 0xdb;
	c_playlist_listing_screen *screen = new c_playlist_listing_screen(parameters->a, parameters->b, parameters->user_flags, 0);

	screen->m6c = true;
	screen->list.value2e248 = true;
	screen->list.value2e249 = false;
	screen->list.value2e24a = false;
	screen->set_screen_id(screen_id);
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x230d08
c_screen_widget *__stdcall function_230d08(s_screen_parameters *parameters)
{
	c_playlist_listing_screen *screen = new c_playlist_listing_screen(parameters->a, parameters->b, parameters->user_flags, 1);

	screen->m6c = true;
	screen->list.value2e248 = false;
	screen->list.value2e249 = true;
	screen->list.value2e24a = true;
	screen->set_screen_id(0xb3);
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x230d6b
c_screen_widget *__stdcall function_230d6b(s_screen_parameters *parameters)
{
	c_playlist_listing_screen *screen = new c_playlist_listing_screen(parameters->a, parameters->b, parameters->user_flags, 2);

	screen->m6c = true;
	screen->list.value2e248 = false;
	screen->list.value2e249 = true;
	screen->list.value2e24a = false;
	screen->set_screen_id(0xdc);
	screen->function_147f6d(parameters);
	return screen;
}

// @retail 0x230e09
screen_load_proc c_playlist_listing_screen::get_load_proc()
{
	screen_load_proc result;

	switch (mode)
	{
	case 0:
		result = function_230c8d;
		break;
	case 1:
		result = function_230d08;
		break;
	case 2:
		result = function_230d6b;
		break;
	default:
		result = 0;
		break;
	}
	return result;
}

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

bool function_6c7e0();
bool function_19a148(long privacy);

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

/* opens the screen that edits the chosen setting of the controller's
   profile */
// @retail 0x2b50d9
void c_mp_player_settings_game_list::handle_item(s_controller_reference **controller, long *item)
{
	if (*item != NONE)
	{
		s_list_item_datum *datum = &((s_list_item_datum *)data->data)[*item & 0xffff];
		s_screen_parameters parameters;

		long window;
		long channel;

		parameters.field_c = 0;
		window = v12();
		channel = v11();
		function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, channel, window, 0);
		switch (datum->item)
		{
		case 0:
			parameters.load = function_231db5;
			break;
		case 1:
			parameters.load = function_2b4f17;
			break;
		case 2:
			parameters.load = function_2b4ed2;
			break;
		case 3:
			parameters.load = function_2b2801;
			break;
		}
		if (parameters.load)
		{
			s_player_profile profile;
			long profile_index;

			if (g_54e5d0.profile_index != NONE)
			{
				profile_edit_end();
			}
			player_slot_get_profile(get_controller_index(), &profile, &profile_index);
			profile_edit_begin(get_controller_index(), (s_player_profile_settings *)&profile, profile_index);
			parameters.load(&parameters);
		}
	}
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

// @retail 0x2ba65f
void *c_actions_list::get_item_data()
{
	return items;
}

// @retail 0x2ba6ae
c_actions_list::c_actions_list(word user_flags) :
	c_list_widget(user_flags),
	mode(0),
	handler(this, (list_item_method)&c_actions_list::handle_item)
{
	data = user_interface_data_new("actions list", 3, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2ba744 deleting c_actions_list
// @retail 0x2ba762 destructor c_actions_list

// @retail 0x2ba798
void c_actions_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch ((short)widget_item(widget)->value70)
		{
		case 0:
			string_id = 0x400019e;
			break;
		case 1:
			string_id = 0x600019f;
			break;
		case 2:
			string_id = 0x60001a0;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

/* whether a saved game file is one of the built-in ones (bit 21 of its
   index) */
static inline bool saved_game_file_is_builtin(long file_index)
{
	return (bool)(((dword)file_index >> 21) & 1);
}

void function_236299(long sound);
void function_148a2c();
void function_148afb();
bool player_slot_profile_in_use(long profile_index);
bool saved_game_storage_has_free_blocks(long blocks);
long saved_game_file_type_size_in_blocks(long type);
long saved_game_file_type_from_variant(s_game_variant *variant);
void function_238c21(long controller, long type, word *name, long maximum_count);
void function_238c69(long mode, long type, word *name, long maximum_count, long controller);
void __stdcall function_19b527(long a, dword b, long c, word d, long e, long f);
void __stdcall function_19b5af(long a, long message, long b, dword controller_flags, void *callback0, void *callback1, long c);
void __stdcall function_19b590(long a, long b, dword controller_flags, void *callback, long c);
c_screen_widget *__stdcall function_23764f(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_2ba666(s_screen_parameters *parameters);
bool __stdcall function_2523b7(long controller);
bool __stdcall function_236973(long controller);
bool __stdcall function_236989(long controller);

// @retail 0x2ba6a4
bool __stdcall function_2ba6a4(long controller)
{
	function_148a2c();
	return true;
}

// @retail 0x2ba949
bool __stdcall function_2ba949(long controller)
{
	function_148afb();
	return true;
}

/* edits the player profile */
// @retail 0x2ba8a0
void function_2ba8a0(long controller)
{
	if (saved_game_file_is_builtin(g_54e5d0.profile_index))
	{
		function_236299(2);
	}
	else
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		function_149f49((s_message *)&parameters, 0, 0, 1 << controller, 5, 4, (long)function_2ba666);
		parameters.load(&parameters);
	}
}

/* copies the player profile */
// @retail 0x2ba8e9
void function_2ba8e9(long controller)
{
	long profile_index = g_54e5d0.profile_index;
	long blocks = saved_game_file_type_size_in_blocks(0);

	if (saved_game_file_is_builtin(profile_index))
	{
		function_236299(2);
	}
	else if (saved_game_storage_has_free_blocks(blocks))
	{
		function_238c21(controller, 4, (word *)&g_54e5d0.settings.unknown000[8], 0x20);
	}
	else
	{
		function_19b5af(3, 0x4e, 4, 1 << controller, function_236973, function_2523b7, 0);
	}
}

/* deletes the player profile */
// @retail 0x2ba953
void function_2ba953(long controller)
{
	long profile_index = g_54e5d0.profile_index;

	if (saved_game_file_is_builtin(profile_index))
	{
		function_19b527(1, 0x53, 4, 1 << controller, 0, 0);
	}
	else if (player_slot_profile_in_use(profile_index))
	{
		function_19b527(1, 0x57, 4, 1 << controller, 0, 0);
	}
	else
	{
		function_19b5af(1, 0x70, 4, 1 << controller, function_2ba949, 0, 0);
	}
}

/* edits the game variant */
// @retail 0x2ba9ab
void function_2ba9ab(long controller)
{
	if (saved_game_file_is_builtin(g_54e49c))
	{
		function_236299(2);
	}
	else
	{
		s_screen_parameters parameters;

		parameters.field_c = 0;
		function_149f49((s_message *)&parameters, 0, 0, 1 << controller, 5, 4, (long)function_23764f);
		parameters.load(&parameters);
	}
}

/* copies the game variant */
// @retail 0x2ba9f4
void function_2ba9f4(long controller)
{
	long blocks = saved_game_file_type_size_in_blocks(1);

	if (saved_game_file_is_builtin(g_54e49c))
	{
		function_236299(2);
	}
	else if (saved_game_storage_has_free_blocks(blocks))
	{
		function_238c69(6, saved_game_file_type_from_variant(&g_54e4a0), (word *)g_54e4a0.name, 0x20, controller);
	}
	else
	{
		function_19b5af(3, 0x4f, 4, 1 << controller, function_236989, function_2523b7, 0);
	}
}

/* deletes the game variant */
// @retail 0x2baa60
void function_2baa60(c_actions_list *list, long controller)
{
	if (saved_game_file_is_builtin(g_54e49c))
	{
		function_236299(2);
	}
	else
	{
		function_19b590(3, list->v12(), 1 << controller, function_2ba6a4, 0xb);
	}
}

// @retail 0x2ba7e3
void c_actions_list::handle_item(s_controller_reference **controller, long *item)
{
	if (*item != NONE)
	{
		switch ((short)*item)
		{
		case 0:
			switch (mode)
			{
			case 0:
				function_2ba8a0((*controller)->controller_index);
				break;
			case 1:
				function_2ba9ab((*controller)->controller_index);
				break;
			}
			break;
		case 1:
			switch (mode)
			{
			case 0:
				function_2ba8e9((*controller)->controller_index);
				break;
			case 1:
				function_2ba9f4((*controller)->controller_index);
				break;
			}
			break;
		case 2:
			switch (mode)
			{
			case 0:
				function_2ba953((*controller)->controller_index);
				break;
			case 1:
				function_2baa60(this, (*controller)->controller_index);
				break;
			}
			break;
		}
	}
	get_screen()->start_animation(3);
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

// @retail 0x2b48c2
c_mp_controller_settings_game_list::c_mp_controller_settings_game_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_mp_controller_settings_game_list::handle_item)
{
	data = user_interface_data_new("mp controller settings game list", 6, 4);
	data_make_valid(data);
	list_item_add(this, 0);
	list_item_add(this, 1);
	list_item_add(this, 2);
	list_item_add(this, 3);
	list_item_add(this, 4);
	list_item_add(this, 5);
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b49e6
void c_mp_controller_settings_game_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[6];

	table[0].item = 0;
	table[0].string_id = 0x130003dd;
	table[1].item = 1;
	table[1].string_id = 0xf0003de;
	table[2].item = 2;
	table[2].string_id = 0x100003df;
	table[3].item = 3;
	table[3].string_id = 0xb0003e0;
	table[4].item = 4;
	table[4].string_id = 0xb0003e1;
	table[5].item = 5;
	table[5].string_id = 0x90003e2;
	function_24c75c(this, item, table, 0, 6);
}

// @retail 0x2b4b43
c_handicap_settings_edit_list::c_handicap_settings_edit_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_handicap_settings_edit_list::handle_item)
{
	data = user_interface_data_new("handicap settings edit list", 4, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b4bf0
void c_handicap_settings_edit_list::v20(c_user_interface_widget *item, long unused)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)item->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch ((short)((c_list_item_widget *)item)->value70)
		{
		case 0:
			string_id = 0xa0003d9;
			break;
		case 1:
			string_id = 0xa0003da;
			break;
		case 2:
			string_id = 0xa0003db;
			break;
		case 3:
			string_id = 0xa0003dc;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

/* opens the screen that edits the chosen setting of the controller's
   profile */
// @retail 0x2b4a4d
void c_mp_controller_settings_game_list::handle_item(s_controller_reference **controller, long *item)
{
	if (*item != NONE)
	{
		s_list_item_datum *datum = &((s_list_item_datum *)data->data)[*item & 0xffff];
		s_screen_parameters parameters;

		long window;
		long channel;

		parameters.field_c = 0;
		window = v12();
		channel = v11();
		function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, channel, window, 0);
		switch (datum->item)
		{
		case 0:
			parameters.load = function_2b42b6;
			break;
		case 1:
			parameters.load = function_2b45a7;
			break;
		case 2:
			parameters.load = function_2b43d5;
			break;
		case 3:
			parameters.load = function_2b44c3;
			break;
		case 4:
			parameters.load = function_2b46e4;
			break;
		case 5:
			parameters.load = function_2b47e5;
			break;
		}
		if (parameters.load)
		{
			s_player_profile profile;
			long profile_index;

			if (g_54e5d0.profile_index != NONE)
			{
				profile_edit_end();
			}
			player_slot_get_profile(get_controller_index(), &profile, &profile_index);
			profile_edit_begin(get_controller_index(), (s_player_profile_settings *)&profile, profile_index);
			parameters.load(&parameters);
		}
	}
}

// @retail 0x2b49c8 deleting c_mp_controller_settings_game_list
// @retail 0x2324e3 destructor c_mp_controller_settings_game_list

// @retail 0x2b74c3
c_variant_editing_options_list::c_variant_editing_options_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_variant_editing_options_list::handle_item)
{
	data = user_interface_data_new("variant editing options list", 6, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		((s_list_item_datum *)data->data)[datum_new(data) & 0xffff].item = (short)i;
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b7581
void c_variant_editing_options_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[6];

	table[0].item = 0;
	table[0].string_id = 0xd000434;
	table[1].item = 1;
	table[1].string_id = 0xe000430;
	table[2].item = 2;
	table[2].string_id = 0xc000435;
	table[3].item = 3;
	table[3].string_id = 0x1100042e;
	table[4].item = 4;
	table[4].string_id = 0xf000433;
	table[5].item = 5;
	table[5].string_id = 0x11000436;
	function_24c75c(this, item, table, 0, 6);
}

// @retail 0x2b7563 deleting c_variant_editing_options_list

// @retail 0x2b7900
c_player_profile_edit_list::c_player_profile_edit_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_player_profile_edit_list::handle_item)
{
	data = user_interface_data_new("player profile edit list", 6, 4);
	data_make_valid(data);
	for (long i = 0; i < data->maximum_count; i++)
	{
		datum_new(data);
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b7990
void c_player_profile_edit_list::v20(c_user_interface_widget *item, long unused)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)item->find_child(6, 0, false);

	if (text)
	{
		long string_id;

		switch ((short)((c_list_item_widget *)item)->value70)
		{
		case 0:
			string_id = 0x130002ef;
			break;
		case 1:
			string_id = 0x130002f0;
			break;
		case 2:
			string_id = 0xc000302;
			break;
		case 3:
			string_id = 0xd000301;
			break;
		case 4:
			string_id = 0x120002f2;
			break;
		case 5:
			string_id = 0x120002f1;
			break;
		default:
			string_id = NONE;
			break;
		}
		text->set_string(string_id);
	}
}

bool function_19a935(void);

// @retail 0x2b298d
c_friends_options_list::c_friends_options_list(word user_flags) :
	c_list_widget(user_flags),
	name(0),
	entries(0),
	entry_count(0),
	source(0),
	handler(this, (list_item_method)&c_friends_options_list::handle_item)
{
	bool online = function_19a935();

	data = user_interface_data_new("friends options list", 6, 4);
	data_make_valid(data);
	list_item_add(this, 0);
	list_item_add(this, 1);
	if (online)
	{
		list_item_add(this, 2);
	}
	list_item_add(this, 3);
	list_item_add(this, 4);
	list_item_add(this, 5);
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b2ad9
void c_friends_options_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[6];

	table[0].item = 0;
	table[0].string_id = 0x1600029e;
	table[1].item = 1;
	table[1].string_id = 0x180002a0;
	table[2].item = 2;
	table[2].string_id = 0x170002a1;
	table[3].item = 3;
	table[3].string_id = 0xc000302;
	table[4].item = 4;
	table[4].string_id = 0xd000301;
	table[5].item = 5;
	table[5].string_id = 0x120002f2;
	function_24c75c(this, item, table, 0, 6);
}

// @retail 0x2b2abb deleting c_friends_options_list

bool function_1900a5(long index);
bool function_1906da(long index);

// @retail 0x2b4f5c
c_mp_player_settings_game_list::c_mp_player_settings_game_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_mp_player_settings_game_list::handle_item)
{
	data = user_interface_data_new("mp player settings game list", 4, 4);
	data_make_valid(data);
	list_item_add(this, 0);
	if (!function_1900a5(get_controller_index()))
	{
		if (function_1906da(get_controller_index()))
		{
			list_item_add(this, 1);
			list_item_add(this, 2);
		}
		if (function_6c7e0())
		{
			list_item_add(this, 3);
		}
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b508c
void c_mp_player_settings_game_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[4];

	table[0].item = 0;
	table[0].string_id = 0x130002ef;
	table[1].item = 1;
	table[1].string_id = 0xc000302;
	table[2].item = 2;
	table[2].string_id = 0xd000301;
	table[3].item = 3;
	table[3].string_id = 0x120002f2;
	function_24c75c(this, item, table, 0, 4);
}

void function_190728(long index);

/* a player's clan membership as function_18ffc3 fills it in */
struct s_clan_membership
{
	byte unknown00[0x1c];
	long rank;
	byte unknown20[0x94 - 0x20];
};

// @retail 0x2b233d
c_clan_options_list::c_clan_options_list(word user_flags) :
	c_list_widget(user_flags),
	name(0),
	entries(0),
	entry_count(0),
	source(0),
	handler(this, (list_item_method)&c_clan_options_list::handle_item)
{
	long controller = get_controller_index();
	bool online = function_19a935();
	s_clan_membership membership;

	data = user_interface_data_new("clan options list", 4, 4);
	data_make_valid(data);
	if (function_18ffc3(controller, (s_player_slot_blockb82 *)&membership))
	{
		bool member = membership.rank >= 1;

		online = online && membership.rank >= 1;
		if (member)
		{
			list_item_add(this, 0);
		}
		if (member)
		{
			list_item_add(this, 1);
		}
		if (online)
		{
			list_item_add(this, 2);
		}
	}
	list_item_add(this, 3);
	delegate_register(&item_handlers, &handler);
	function_190728(get_controller_index());
}

// @retail 0x2b24b2
void c_clan_options_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[4];

	table[0].item = 0;
	table[0].string_id = 0x180002a2;
	table[1].item = 1;
	table[1].string_id = 0x1a0002a3;
	table[2].item = 2;
	table[2].string_id = 0x190002a4;
	table[3].item = 3;
	table[3].string_id = 0xa0002a5;
	function_24c75c(this, item, table, 0, 4);
}

// @retail 0x2b2494 deleting c_clan_options_list

struct s_network_session_interface_view
{
	byte unknown00[0x48];
	dword flags;
};

byte *network_session_interface_get_data_4db0(void);
word function_157a40(void);

// @retail 0x2b4ce1
c_mp_change_teams_list::c_mp_change_teams_list(word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_mp_change_teams_list::handle_item)
{
	s_network_session_interface_view *session = (s_network_session_interface_view *)network_session_interface_get_data_4db0();

	data = user_interface_data_new("mp change teams list", 9, 4);
	data_make_valid(data);
	if (session->flags & 1)
	{
		dword teams = function_157a40();
		for (long team = 0; team < 8; team++)
		{
			if (teams & (1 << team))
			{
				long datum = datum_new(data);
				if (datum != NONE)
				{
					((s_list_item_datum *)data->data)[datum & 0xffff].item = (short)team;
				}
			}
		}
	}
	if (TEST_FIELD_BIT((session->flags >> 5) & 1))
	{
		long datum = datum_new(data);
		if (datum != NONE)
		{
			((s_list_item_datum *)data->data)[datum & 0xffff].item = NONE;
		}
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b4dcd
void c_mp_change_teams_list::v20(c_user_interface_widget *item, long unused)
{
	s_list_item_text table[9];

	table[0].item = 0;
	table[0].string_id = 0x30001be;
	table[1].item = 1;
	table[1].string_id = 0x40001bf;
	table[2].item = 2;
	table[2].string_id = 0x50001c0;
	table[3].item = 3;
	table[3].string_id = 0x60001c1;
	table[4].item = 4;
	table[4].string_id = 0x60001c2;
	table[5].item = 5;
	table[5].string_id = 0x60001c3;
	table[6].item = 6;
	table[6].string_id = 0x50001c4;
	table[7].item = 7;
	table[7].string_id = 0x40001c5;
	table[8].item = NONE;
	table[8].string_id = 0x90001c6;
	function_24c75c(this, item, table, 0, 9);
}

/* ---- the lists' item texts ---- */

// @retail 0x2b1d6e
void c_settings_list::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		switch ((short)widget_item(widget)->value70)
		{
		case 0:
			text->set_string(0x17000196);
			break;
		case 1:
			text->set_string(0x10000197);
			break;
		}
	}
}

// @retail 0x2b52e4
void c_squad_privacy_setting_list::v20(c_user_interface_widget *widget, long index)
{
	s_list_item_text table[3];

	table[0].item = 0;
	table[0].string_id = 0x4000201;
	table[1].item = 1;
	table[1].string_id = 0xf000202;
	table[2].item = 2;
	table[2].string_id = 0x6000203;
	function_24c75c(this, widget, table, 0, 3);
}

/* the xbox live message send list (vtable 0x45c068) */
class c_list_45c068 : public c_list_widget
{
public:
	virtual void v20(c_user_interface_widget *widget, long index);
};

// @retail 0x2b7d00
void c_list_45c068::v20(c_user_interface_widget *widget, long index)
{
	s_list_item_text table[3];

	table[0].item = 0;
	table[0].string_id = 0xc0002a6;
	table[1].item = 1;
	table[1].string_id = 0x13000601;
	table[2].item = 2;
	table[2].string_id = 0x14000602;
	function_24c75c(this, widget, table, 0, 3);
}

/* an item of the list of 0x45c318: its string */
struct s_list_45c318_datum
{
	short salt;
	short item;
	long string_id;
};

// @retail 0x2b8ccf
void c_list_45c318::v20(c_user_interface_widget *widget, long index)
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)widget->find_child(6, 0, false);

	if (text)
	{
		text->set_string(((s_list_45c318_datum *)data->data)[widget_item(widget)->value70 & 0xffff].string_id);
	}
}

// @retail 0x2b1613
void c_campaign_options_list::v20(c_user_interface_widget *widget, long index)
{
	s_list_item_text table[5];

	table[0].item = 0;
	table[0].string_id = 0x14000425;
	table[1].item = 1;
	table[1].string_id = 0x15000426;
	table[2].item = 2;
	table[2].string_id = 0x15000427;
	table[3].item = 3;
	table[3].string_id = 0x4000424;
	table[4].item = 4;
	table[4].string_id = 0x4000423;
	function_24c75c(this, widget, table, 0, 5);
}

byte *function_19aaa5(long player_index);

/* an item of the potential squad leader list: its player */
struct s_squad_leader_datum
{
	short salt;
	short unknown02;
	long player_index;
};

/* shows the player's name */
// @retail 0x2b83db
void c_potential_squad_leader_player_list::v20(c_user_interface_widget *widget, long index)
{
	c_user_interface_widget *text = widget->find_child(6, 0, false);

	if (text && widget_item(widget)->value70 != NONE)
	{
		s_squad_leader_datum *datum = &((s_squad_leader_datum *)data->data)[widget_item(widget)->value70 & 0xffff];

		if (function_19a951(datum->player_index))
		{
			word *name = (word *)function_19aaa5(datum->player_index);

			text->get_text()->set_text(name);
		}
		else
		{
			text->get_text()->set_text((word *)L"");
		}
	}
}

/* the gamertag select list (vtable 0x459f58): the gamertags to choose from */
class c_list_459f58 : public c_list_widget
{
public:
	virtual void v20(c_user_interface_widget *widget, long index);

	byte unknown88[0x288 - 0x88];
	word gamertags[4][0x40];
};

// @retail 0x24b2ac
void c_list_459f58::v20(c_user_interface_widget *widget, long index)
{
	c_user_interface_widget *text = widget->find_child(6, 0, false);

	if (text)
	{
		short gamertag = (short)widget_item(widget)->value70;

		text->get_text()->set_text(gamertags[gamertag]);
	}
}
