// @flags /O1 /Oi /arch:SSE /Gr
/* SCREEN_POSTGAME_STATISTICS.CPP: the postgame statistics (the "pcr", the
   postgame carnage report; named after the debug build's source file).

   The screen (vtable 0x4596e0) is a tab bar over six tabs. Each tab is a
   screen with one list of the players' statistics: the tabs of the vtables
   0x4593c0, 0x459890, 0x4597b0, 0x459430, 0x459900 and 0x459820 share every
   slot but the one that builds them, which titles their texts, and their
   lists (vtables 0x459750, 0x459680, 0x459620, 0x4595c0, 0x459500 and
   0x4594a0) share every slot of their base (vtable 0x459560) but the one
   that fills a row. */

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>
#include "data_array.h"
#include "globals.h"
#include "screen_widgets.h"
#include "unknown_19b510.h"
#include "unknown_19b516.h"
#include "unknown_234c64.h"
#include "screen_online_y_menu.h"

bool xuid_equal(XUID const *a, XUID const *b, bool compare_guest_number);
long network_time_get(void);
long network_time_since(long time);
bool function_592f0(void);
void function_199e2e(bool close);
void function_19a942(void);
bool function_199994(void);
bool function_1999b3(void);
bool function_6c7e0();
bool function_1900a5(long player);
void function_18ff47(long player, dword *out);
long function_1480ff(long screen_id);
void unicode_string_to_ascii(const word *source, char *destination, long maximum_count);
void *__stdcall user_interface_malloc(unsigned int size);
long function_19ad9c(XUID const *xuid);
bool voice_port_flag0_only(long port);
bool function_53750(long player_index);
struct s_widget_view_2b0a;
void function_2b0a14(s_widget_view_2b0a *widget, short index);
c_screen_widget *__stdcall function_2312af(s_screen_parameters *parameters);
c_screen_widget *__stdcall function_23334f(s_screen_parameters *parameters);
/* the item's bitmap hides (unknown_250155.cpp) */
void function_251963(c_user_interface_widget *item);
word *unicode_string_append(word *destination, const word *source, long maximum_count);
bool network_session_interface_has_user(const XUID *xuid);
long function_19adca(XUID const *xuid);
bool function_19ab77(long player_index);
void function_2b01a2(long value, s_widget_item *item);
void function_22f042(s_widget_item *items, c_user_interface_widget *widget, long count);

/* what the name lookups take: a player (type 1) */
struct s_name_request
{
	long type;
	s_id_triplet id;
	char name[16];
	byte unknown20[0x78 - 0x20];
};

void __stdcall function_148893(s_name_request *request, long flag);

/* a player of the statistics (0x114 bytes) */
struct s_postgame_player
{
	word name[0x20];
	word team_name[0x10];
	long value60;
	long value64;
	long value68;
	long value6c;
	short value70;
	short value72;
	bool value74;
	byte unknown75[3];
	long value78;
	long medal_count;
	dword medals;
	long count;
	long value;
	long value2;
	/* the kills of each player */
	long values[0x10];
	dword valued0[4];
	word texte0[0x10];
	s_id_triplet id;
	long value10c;
	long value110;
};

struct s_player_ref
{
	byte unknown00[4];
	long player;
};

/* the players of the statistics, and how many there are */
s_postgame_player g_55caf0[1];
long g_51ec08;
/* the count of the first tab's rows */
long g_51ec0c;

/* the base of the lists (vtable 0x459560) */
class c_postgame_statistics_list : public c_list_widget
{
public:
	c_postgame_statistics_list(bool value8a0, long count, long value8a8, word user_flags);

	/* folded with c_widget::v2 (unknown_19b516.cpp) */
	virtual void *get_item_data() { return items; }
	/* folded with c_widget::v3 (unknown_19b516.cpp) */
	virtual long get_item_count() { return 0x10; }
	virtual void v20(c_user_interface_widget *item, long unused);
	virtual bool v21(c_user_interface_widget *item);
	/* fills the item with the row (pure in retail; the stand-ins construct
	   the class) */
	virtual void fill_row(c_user_interface_widget *item, long row) {}
	virtual void handle_item(s_controller_reference **controller, long *item);

	void show_voice_icon(long row, c_user_interface_widget *item);
	void show_row(long row, c_user_interface_widget *item);

	c_list_item_widget items[0x10];
	c_list_item_handler handler;
	bool value8a0;
	long count;
	long value8a8;
};

class c_postgame_statistics_list_459750 : public c_postgame_statistics_list
{
public:
	c_postgame_statistics_list_459750(word user_flags);

	virtual void fill_row(c_user_interface_widget *item, long row);
};

class c_postgame_statistics_list_459680 : public c_postgame_statistics_list
{
public:
	c_postgame_statistics_list_459680(bool value8a0, word user_flags);

	virtual void fill_row(c_user_interface_widget *item, long row);
};

class c_postgame_statistics_list_459620 : public c_postgame_statistics_list
{
public:
	c_postgame_statistics_list_459620(bool value8a0, word user_flags);

	virtual void fill_row(c_user_interface_widget *item, long row);
};

class c_postgame_statistics_list_4595c0 : public c_postgame_statistics_list
{
public:
	c_postgame_statistics_list_4595c0(bool value8ac, word user_flags);

	virtual void fill_row(c_user_interface_widget *item, long row);
	virtual void handle_item(s_controller_reference **controller, long *item);

	bool value8ac;
};

class c_postgame_statistics_list_459500 : public c_postgame_statistics_list
{
public:
	c_postgame_statistics_list_459500(bool value8a0, word user_flags);

	virtual void fill_row(c_user_interface_widget *item, long row);
};

class c_postgame_statistics_list_4594a0 : public c_postgame_statistics_list
{
public:
	c_postgame_statistics_list_4594a0(bool value8a0, word user_flags);

	virtual void fill_row(c_user_interface_widget *item, long row);
};

/* the tabs: a press is the base widget's (slot 10 is folded), and they cannot
   be created on their own (slot 26 is folded) */
class c_postgame_statistics_screen_4593c0 : public c_screen_widget
{
public:
	c_postgame_statistics_screen_4593c0(long screen_id, long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc() { return function_2312af; }

	c_postgame_statistics_list_459750 list;
};

class c_postgame_statistics_screen_459890 : public c_screen_widget
{
public:
	c_postgame_statistics_screen_459890(long screen_id, long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc() { return function_2312af; }

	c_postgame_statistics_list_459680 list;
};

class c_postgame_statistics_screen_4597b0 : public c_screen_widget
{
public:
	c_postgame_statistics_screen_4597b0(long screen_id, long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc() { return function_2312af; }

	c_postgame_statistics_list_459620 list;
};

class c_postgame_statistics_screen_459430 : public c_screen_widget
{
public:
	c_postgame_statistics_screen_459430(long screen_id, long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc() { return function_2312af; }

	c_postgame_statistics_list_4595c0 list;
};

class c_postgame_statistics_screen_459900 : public c_screen_widget
{
public:
	c_postgame_statistics_screen_459900(long screen_id, long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc() { return function_2312af; }

	c_postgame_statistics_list_459500 list;
};

class c_postgame_statistics_screen_459820 : public c_screen_widget
{
public:
	c_postgame_statistics_screen_459820(long screen_id, long a, long b, word user_flags);

	virtual bool v10(s_widget_event *event);
	virtual void v18(void *parameters);
	virtual screen_load_proc get_load_proc() { return function_2312af; }

	c_postgame_statistics_list_4594a0 list;
};

/* the postgame statistics screen (vtable 0x4596e0) */
class c_postgame_statistics_screen : public c_screen_widget
{
public:
	c_postgame_statistics_screen(long screen_id, long a, long b, word user_flags);

	virtual void v3();
	virtual bool v10(s_widget_event *event);
	virtual void v17();
	virtual void v18(void *parameters);
	virtual void v23(void *window);
	/* folded with c_screen_4596e0's (unknown_23068b.cpp) */
	virtual screen_load_proc get_load_proc() { return function_23334f; }

	bool handle_back(s_widget_event *event);

	bool tab_is_current(c_screen_widget *tab)
	{
		return tab && tab == tab_bar.focused;
	}

	c_y_menu_tab_bar tab_bar;
	c_postgame_statistics_screen_4593c0 tab1;
	c_postgame_statistics_screen_459890 tab2;
	c_postgame_statistics_screen_4597b0 tab3;
	c_postgame_statistics_screen_459430 tab4;
	c_postgame_statistics_screen_459900 tab5;
	c_postgame_statistics_screen_459820 tab6;
	/* opened from the saved film (screen 0xb9) */
	bool value5ef0;
	long start_time;
	long value5ef8;
};

/* the game engine of the game the statistics are for */
long g_50224c;

// @retail 0x23381b
void c_postgame_statistics_screen_4593c0::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	if (title)
	{
		title->set_string(0x4000278);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0x5000733);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0x5000735);
		heading2->value6e = true;
	}
}

// @retail 0x233b81
void c_postgame_statistics_screen_459430::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0xa00073a);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0xd00073b);
		heading2->value6e = true;
	}
}

// @retail 0x233c35
void c_postgame_statistics_screen_459900::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0xc00073f);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0xa000740);
		heading2->value6e = true;
	}
}

// @retail 0x233a7e
void c_postgame_statistics_screen_4597b0::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;
	c_text_widget_45a5e0 *heading3;
	c_text_widget_45a5e0 *heading4;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	heading3 = (c_text_widget_45a5e0 *)find_child(6, 3, false);
	heading4 = (c_text_widget_45a5e0 *)find_child(6, 4, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0x5000739);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0x7000737);
		heading2->value6e = true;
	}
	if (heading3)
	{
		heading3->set_string(0x6000738);
		heading3->value6e = true;
	}
	if (heading4)
	{
		heading4->set_string(0x800073c);
		heading4->value6e = true;
	}
}

// @retail 0x233d07
void c_postgame_statistics_screen_459820::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;
	c_text_widget_45a5e0 *heading3;
	c_text_widget_45a5e0 *heading4;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	heading3 = (c_text_widget_45a5e0 *)find_child(6, 3, false);
	heading4 = (c_text_widget_45a5e0 *)find_child(6, 4, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0x900075a);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0xb00075b);
		heading2->value6e = true;
	}
	if (heading3)
	{
		heading3->set_string(0xe00075c);
		heading3->value6e = true;
	}
	if (heading4)
	{
		heading4->set_string(0xa00075d);
		heading4->value6e = true;
	}
}

// @retail 0x2338cf
void c_postgame_statistics_screen_459890::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;
	c_text_widget_45a5e0 *column1;
	c_text_widget_45a5e0 *column2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 4, false);
	column1 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	column2 = (c_text_widget_45a5e0 *)find_child(6, 3, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0x5000733);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0x5000735);
		heading2->value6e = true;
	}
	if (column1 && column2)
	{
		if (g_50224c == 2)
		{
			column1->set_string(0x8000723);
			column2->set_string(0xa000724);
		}
		else if (g_50224c == 1)
		{
			column1->set_string(0xa000725);
			column2->set_string(0xc000726);
		}
		else if (g_50224c == 3)
		{
			column1->set_string(0xd00072a);
			column2->set_string(0xf00072b);
		}
		else if (g_50224c == 4)
		{
			column1->set_string(0xd000728);
			column2->set_string(0xf000729);
		}
		else if (g_50224c == 7)
		{
			column1->set_string(0xc00072d);
			column2->set_string(0xd00072e);
		}
		else if (g_50224c == 8)
		{
			column1->set_string(0xb00072f);
			column2->set_string(0xa000730);
		}
		else if (g_50224c == 9)
		{
			column1->set_string(0xa000731);
			column2->set_string(0xa000732);
		}
		column1->value6e = true;
		column2->value6e = true;
	}
}

/* a player of the first tab (0x84 bytes) */
struct s_postgame_team
{
	word name[0x20];
	word score[0x10];
	short unknown60;
	short color_index;
	word place[0x10];
};

s_postgame_team g_55dc30[1];

real_rgb_color *function_7f720(real_rgb_color *color, short team_index);
real_hsv_color *function_1318d0(const real_rgb_color *rgb, real_hsv_color *hsv);
real_rgb_color *function_131a00(const real_hsv_color *hsv, real_rgb_color *rgb);

/* the row's player: name, emblem and colours, grey when the player has left */
// @retail 0x233f0f
void c_postgame_statistics_list::show_row(long row, c_user_interface_widget *item)
{
	s_widget_item definition;
	real_rgb_color grey;
	bool present;

	definition.flags = 0;
	present = network_session_interface_has_user((XUID const *)&g_55caf0[row].id);
	if (present)
	{
		long player_index = function_19adca((XUID const *)&g_55caf0[row].id);

		if (player_index != NONE)
		{
			function_19ab77(player_index);
		}
	}
	function_2b01a2(g_55caf0[row].value78, &definition);
	memcpy(definition.value48, g_55caf0[row].valued0, sizeof(definition.value48));
	definition.flags |= 2;
	definition.value4 = (long)g_55caf0[row].name;
	definition.flags |= 1;
	definition.value5c = g_55caf0[row].value72;
	definition.flags |= 4;
	definition.value5f = g_55caf0[row].value74;
	definition.flags |= 0x80;
	if (!present)
	{
		grey.red = 0.3f;
		grey.green = 0.3f;
		grey.blue = 0.3f;
		definition.color = grey;
		definition.flags |= 0x200;
	}
	function_22f042(&definition, item, 1);
}

/* the player's voice icon on the item */
// @retail 0x233fd5
void c_postgame_statistics_list::show_voice_icon(long row, c_user_interface_widget *item)
{
	c_user_interface_widget *bitmap = item->find_child(8, 0, false);

	if (bitmap)
	{
		long player = function_19ad9c((XUID const *)&g_55caf0[row].id);

		if (player != NONE)
		{
			short index;

			if (voice_port_flag0_only(player))
			{
				index = !function_53750(player);
			}
			else
			{
				index = 2;
			}
			function_2b0a14((s_widget_view_2b0a *)bitmap, index);
			bitmap->value6e = true;
		}
		else
		{
			function_251963(item);
		}
	}
}

// @retail 0x233dcd
c_postgame_statistics_list::c_postgame_statistics_list(bool value8a0, long count, long value8a8, word user_flags) :
	c_list_widget(user_flags),
	handler(this, (list_item_method)&c_postgame_statistics_list::handle_item),
	value8a0(value8a0),
	count(count),
	value8a8(value8a8)
{
	if (count > 0)
	{
		data = user_interface_data_new("pcr list", count, 4);
		if (data)
		{
			data_make_valid(data);
			for (long i = 0; i < this->count; i++)
			{
				long index = datum_new(data);

				if (index != NONE)
				{
					((s_list_item_datum *)data->data)[index & 0xffff].item = (short)i;
				}
			}
		}
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x233319 destructor c_postgame_statistics_list
// @retail 0x234509 deleting c_postgame_statistics_list

// @retail 0x233e97
bool c_postgame_statistics_list::v21(c_user_interface_widget *item)
{
	bool result = false;

	if (value8a0 == result)
	{
		result = c_list_widget::v21(item);
	}
	return result;
}

// @retail 0x234169
c_postgame_statistics_list_459750::c_postgame_statistics_list_459750(word user_flags) :
	c_postgame_statistics_list(true, g_51ec0c, 1, user_flags)
{
}

// @retail 0x2342ac
c_postgame_statistics_list_459680::c_postgame_statistics_list_459680(bool value8a0, word user_flags) :
	c_postgame_statistics_list(value8a0 || function_199994() || function_1999b3(), g_51ec08, 0, user_flags)
{
}

// @retail 0x2344cc
c_postgame_statistics_list_459620::c_postgame_statistics_list_459620(bool value8a0, word user_flags) :
	c_postgame_statistics_list(value8a0 || function_199994() || function_1999b3(), g_51ec08, 0, user_flags)
{
}

// @retail 0x23469d
c_postgame_statistics_list_4595c0::c_postgame_statistics_list_4595c0(bool value8ac, word user_flags) :
	c_postgame_statistics_list(false, g_51ec08, 0, user_flags),
	value8ac(value8ac)
{
}

// @retail 0x2347ef
c_postgame_statistics_list_459500::c_postgame_statistics_list_459500(bool value8a0, word user_flags) :
	c_postgame_statistics_list(value8a0 || function_199994() || function_1999b3(), g_51ec08, 0, user_flags)
{
}

// @retail 0x234a5a
c_postgame_statistics_list_4594a0::c_postgame_statistics_list_4594a0(bool value8a0, word user_flags) :
	c_postgame_statistics_list(value8a0 || function_199994() || function_1999b3(), g_51ec08, 0, user_flags)
{
}

/* a player's choice asks for the player's gamertag card */
// @retail 0x23403c
void c_postgame_statistics_list::handle_item(s_controller_reference **controller, long *item)
{
	long handle = *item;
	long index = handle & 0xffff;

	{
		dword local[0x1c];

		function_18ff47((*controller)->controller_index, local);
		if (value8a0 || handle == NONE || !function_6c7e0() || function_199994() || function_1999b3() || function_1900a5((*controller)->controller_index))
		{
			return;
		}
		if (xuid_equal((XUID const *)&g_55caf0[index].id, (XUID const *)local, false))
		{
			return;
		}
	}
	if (index >= 0 && index < g_51ec08 && !(g_55caf0[index].id.c & 3))
	{
		s_name_request request;
		s_message message;

		message.field_c = 0;
		memset(&request, 0, sizeof(request));
		request.type = 1;
		request.id = g_55caf0[index].id;
		unicode_string_to_ascii(g_55caf0[index].name, request.name, 16);
		request.name[15] = 0;
		function_148893(&request, 1);
		if (request.id.a | request.id.b)
		{
			function_149f49(&message, 0, 0, 1 << (*controller)->controller_index, 3, 4, 0x2b7223);
			message.callback(&message);
		}
	}
}

// @retail 0x2346c8
void c_postgame_statistics_list_4595c0::handle_item(s_controller_reference **controller, long *item)
{
	if (!value8ac)
	{
		c_postgame_statistics_list::handle_item(controller, item);
	}
}

/* the first tab's row: the team's name, score and place in its colour */
// @retail 0x23418a
void c_postgame_statistics_list_459750::fill_row(c_user_interface_widget *item, long row)
{
	c_user_interface_widget *name = item->find_child(6, 0, false);
	c_user_interface_widget *place = item->find_child(6, 1, false);
	c_user_interface_widget *score = item->find_child(6, 2, false);
	c_user_interface_widget *bitmap = item->find_child(8, 0, false);
	real_rgb_color bitmap_color;
	real_rgb_color color;
	real_hsv_color hsv;

	bitmap_color = *function_7f720(&color, g_55dc30[row].color_index);
	function_1318d0(&bitmap_color, &hsv);
	hsv.saturation = 0.20833333f;
	hsv.value = 0.79166669f;
	function_131a00(&hsv, &color);
	if (name)
	{
		name->get_text()->set_text(g_55dc30[row].name);
		name->color = color;
	}
	if (place)
	{
		place->get_text()->set_text(g_55dc30[row].place);
		place->color = color;
	}
	if (score)
	{
		score->get_text()->set_text(g_55dc30[row].score);
		score->color = color;
	}
	if (bitmap)
	{
		bitmap->color = bitmap_color;
		bitmap->value6e = true;
	}
}

/* the player's name, team and the game engine's two scores */
// @retail 0x2342e9
void c_postgame_statistics_list_459680::fill_row(c_user_interface_widget *item, long row)
{
	c_user_interface_widget *header = item->find_child(10, 0, false);
	c_user_interface_widget *name = header->find_child(6, 0, false);
	c_user_interface_widget *title = header->find_child(6, 1, false);
	c_user_interface_widget *team = header->find_child(6, 4, false);
	c_user_interface_widget *score1_text = header->find_child(6, 2, false);
	c_user_interface_widget *score2_text = header->find_child(6, 3, false);

	show_row(row, item);
	if (name)
	{
		name->get_text()->set_text(g_55caf0[row].name);
	}
	if (title)
	{
		title->get_text()->set_text(g_55caf0[row].texte0);
	}
	if (team)
	{
		team->get_text()->set_text(g_55caf0[row].team_name);
	}
	if (score1_text && score2_text)
	{
		word format[0x100];
		word score1[0x100];
		word score2[0x100];
		long value1 = 0;
		long value2 = 0;

		format[0] = 0;
		score1[0] = 0;
		score2[0] = 0;
		if (g_50224c == 2)
		{
			value1 = g_55caf0[row].value10c;
			value2 = g_55caf0[row].value110;
			((c_widget *)get_screen())->function_230134(0xb000736, format);
			function_1630e0(score1, format, value1 / 60, value1 % 60);
		}
		else
		{
			if (g_50224c == 1 || g_50224c == 3 || g_50224c == 4 || g_50224c == 7 || g_50224c == 8 || g_50224c == 9)
			{
				value1 = g_55caf0[row].value10c;
				value2 = g_55caf0[row].value110;
			}
			function_1630e0(score1, (const word *)L"%d", value1);
		}
		function_1630e0(score2, (const word *)L"%d", value2);
		score1_text->get_text()->set_text(score1);
		score2_text->get_text()->set_text(score2);
	}
}

/* the player's name and four counts */
// @retail 0x234527
void c_postgame_statistics_list_459620::fill_row(c_user_interface_widget *item, long row)
{
	word text[0x100];
	c_user_interface_widget *header;
	c_user_interface_widget *name;
	c_user_interface_widget *text1;
	c_user_interface_widget *text2;
	c_user_interface_widget *text3;
	c_user_interface_widget *text4;

	text[0] = 0;
	header = item->find_child(10, 0, false);
	name = header->find_child(6, 0, false);
	text1 = header->find_child(6, 1, false);
	text2 = header->find_child(6, 2, false);
	text3 = header->find_child(6, 3, false);
	text4 = header->find_child(6, 4, false);
	show_row(row, item);
	if (name)
	{
		name->get_text()->set_text(g_55caf0[row].name);
	}
	if (text1)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value60);
		text1->get_text()->set_text(text);
	}
	if (text2)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value68);
		text2->get_text()->set_text(text);
	}
	if (text3)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value64);
		text3->get_text()->set_text(text);
	}
	if (text4)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value6c);
		text4->get_text()->set_text(text);
	}
}

/* the kills between the player of the row and the focused player */
// @retail 0x2346d9
void c_postgame_statistics_list_4595c0::fill_row(c_user_interface_widget *item, long row)
{
	word text[0x100];
	c_user_interface_widget *header;
	c_user_interface_widget *name;
	c_user_interface_widget *killed;
	c_user_interface_widget *killed_by;
	long column;

	text[0] = 0;
	header = item->find_child(10, 0, false);
	name = header->find_child(6, 0, false);
	killed = header->find_child(6, 1, false);
	killed_by = header->find_child(6, 2, false);
	if (get_focused_datum() != NONE)
	{
		column = get_focused_datum() & 0xffff;
	}
	else
	{
		column = 0;
	}
	show_row(row, item);
	if (name)
	{
		name->get_text()->set_text(g_55caf0[row].name);
	}
	if (killed)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[column].values[row]);
		killed->get_text()->set_text(text);
	}
	if (killed_by)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].values[column]);
		killed_by->get_text()->set_text(text);
	}
}

/* the player's name, medal count and up to eight of the medals' names */
// @retail 0x23482c
void c_postgame_statistics_list_459500::fill_row(c_user_interface_widget *item, long row)
{
	c_user_interface_widget *header = item->find_child(10, 0, false);
	c_user_interface_widget *name = header->find_child(6, 0, false);
	c_user_interface_widget *count_text = header->find_child(6, 1, false);
	c_user_interface_widget *medals_text = header->find_child(6, 2, false);
	word text[0x100];

	show_row(row, item);
	if (name)
	{
		name->get_text()->set_text(g_55caf0[row].name);
	}
	if (count_text)
	{
		text[0] = 0;
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].medal_count);
		count_text->get_text()->set_text(text);
	}
	if (medals_text)
	{
		if (g_55caf0[row].medals)
		{
			word medal_name[0x100];
			long shown;
			long i;

			medal_name[0] = 0;
			text[0] = 0;
			shown = 0;
			long string_ids[0x18] =
			{
				0xf000741, 0xf000742, 0xf000743, 0xf000744, 0xf000745, 0xf000746,
				0xb000747, 0xe000748, 0x9000749, 0xc00074a, 0xe00074b, 0xf00074c,
				0xb00074d, 0x1000074e, 0x1100074f, 0x11000750, 0x11000751, 0x11000752,
				0x9000753, 0x11000754, 0xb000755, 0xa000756, 0x11000757, 0xd000758
			};
			for (i = 0x17; i >= 0 && shown < 8; i--)
			{
				if (g_55caf0[row].medals & (1 << i))
				{
					if (shown)
					{
						unicode_string_append(text, (const word *)L", ", 0x100);
						text[0xff] = 0;
					}
					((c_widget *)get_screen())->function_230134(string_ids[i], medal_name);
					unicode_string_append(text, medal_name, 0x100);
					shown++;
					text[0xff] = 0;
				}
			}
			if (shown > 0)
			{
				medals_text->get_text()->set_text(text);
				medals_text->value6e = true;
				return;
			}
		}
		medals_text->value6e = false;
	}
}

/* the row's name, kills, deaths, accuracy and assists */
// @retail 0x234a97
void c_postgame_statistics_list_4594a0::fill_row(c_user_interface_widget *item, long row)
{
	word text[256];
	word percent_text[256];

	text[0] = 0;
	c_user_interface_widget *header = item->find_child(10, 0, false);
	c_user_interface_widget *name = header->find_child(6, 0, false);
	c_user_interface_widget *score = header->find_child(6, 1, false);
	c_user_interface_widget *total = header->find_child(6, 2, false);
	c_user_interface_widget *percent = header->find_child(6, 3, false);
	c_user_interface_widget *extra = header->find_child(6, 4, false);
	show_row(row, item);
	if (name)
	{
		name->get_text()->set_text(g_55caf0[row].name);
	}
	if (score)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value);
		score->get_text()->set_text(text);
	}
	if (total)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].count);
		total->get_text()->set_text(text);
	}
	if (percent)
	{
		long percentage;

		percent_text[0] = 0;
		percentage = 0;
		if (g_55caf0[row].count > 0)
		{
			percentage = (long)((real)g_55caf0[row].value * 100.0f / (real)g_55caf0[row].count);
		}
		((c_widget *)get_screen())->function_230134(0x11000759, text);
		function_1630e0(percent_text, text, percentage);
		percent->get_text()->set_text(percent_text);
	}
	if (extra)
	{
		function_1630e0(text, (const word *)L"%d", g_55caf0[row].value2);
		extra->get_text()->set_text(text);
	}
}

/* ---- the tabs ---- */

// @retail 0x2337e9
c_postgame_statistics_screen_4593c0::c_postgame_statistics_screen_4593c0(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(user_flags)
{
}

// @retail 0x233892
c_postgame_statistics_screen_459890::c_postgame_statistics_screen_459890(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(screen_id == 0xb9, user_flags)
{
}

// @retail 0x233a41
c_postgame_statistics_screen_4597b0::c_postgame_statistics_screen_4597b0(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(screen_id == 0xb9, user_flags)
{
}

// @retail 0x233b44
c_postgame_statistics_screen_459430::c_postgame_statistics_screen_459430(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(screen_id == 0xb9, user_flags)
{
}

// @retail 0x233bf8
c_postgame_statistics_screen_459900::c_postgame_statistics_screen_459900(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(screen_id == 0xb9, user_flags)
{
}

// @retail 0x233cac
c_postgame_statistics_screen_459820::c_postgame_statistics_screen_459820(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	list(screen_id == 0xb9, user_flags)
{
}

// @retail 0x2332a7 destructor c_postgame_statistics_screen_4593c0
// @retail 0x233ce9 deleting c_postgame_statistics_screen_4593c0

// @retail 0x232d43
bool c_postgame_statistics_screen_4593c0::v10(s_widget_event *event)
{
	return c_user_interface_widget::v10(event);
}

bool c_postgame_statistics_screen_459890::v10(s_widget_event *event)
{
	return c_user_interface_widget::v10(event);
}

bool c_postgame_statistics_screen_4597b0::v10(s_widget_event *event)
{
	return c_user_interface_widget::v10(event);
}

bool c_postgame_statistics_screen_459430::v10(s_widget_event *event)
{
	return c_user_interface_widget::v10(event);
}

bool c_postgame_statistics_screen_459900::v10(s_widget_event *event)
{
	return c_user_interface_widget::v10(event);
}

bool c_postgame_statistics_screen_459820::v10(s_widget_event *event)
{
	return c_user_interface_widget::v10(event);
}

/* the item of the row the list shows (the list's slot 20): filled, and its
   player's voice icon shown */
// @retail 0x233ea9
void c_postgame_statistics_list::v20(c_user_interface_widget *item, long unused)
{
	c_screen_widget *tab = (c_screen_widget *)parent;
	c_postgame_statistics_screen *screen = (c_postgame_statistics_screen *)parent->parent->parent;

	{
		if (tab != 0 && tab == screen->tab_bar.focused)
		{
			long row = widget_item(item)->value70 & 0xffff;

			if (row >= 0 && row < count)
			{
				fill_row(item, row);
				if (!value8a8)
				{
					if (screen->value5ef0)
					{
						function_251963(item);
					}
					else
					{
						show_voice_icon(row, item);
					}
				}
			}
		}
	}
}

/* ---- the screen ---- */

/* a random value in [lower, upper) from the second seed */
__forceinline short postgame_random_range(short lower, short upper)
{
	dword *seed = &g_4e7408->seed;
	*seed = *seed * 0x19660d + 0x3c6ef35f;
	return lower + (short)(((upper - lower) * (*seed >> 16)) >> 16);
}

// @retail 0x2331a2
c_postgame_statistics_screen::c_postgame_statistics_screen(long screen_id, long a, long b, word user_flags) :
	c_screen_widget(screen_id, a, b, user_flags),
	tab_bar(user_flags),
	tab1(screen_id, a, b, user_flags),
	tab2(screen_id, a, b, user_flags),
	tab3(screen_id, a, b, user_flags),
	tab4(screen_id, a, b, user_flags),
	tab5(screen_id, a, b, user_flags),
	tab6(screen_id, a, b, user_flags),
	value5ef0(false),
	start_time(0),
	value5ef8(postgame_random_range(0, 15000))
{
	value5ef0 = screen_id == 0xb9;
}

// @retail 0x2332bc destructor c_postgame_statistics_screen
// @retail 0x233289 deleting c_postgame_statistics_screen

// @retail 0x23334f
c_screen_widget *__stdcall function_23334f(s_screen_parameters *parameters)
{
	c_postgame_statistics_screen *screen = new c_postgame_statistics_screen(0x10, parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x233395
c_screen_widget *__stdcall function_233395(s_screen_parameters *parameters)
{
	c_postgame_statistics_screen *screen = new c_postgame_statistics_screen(0xb9, parameters->a, parameters->b, parameters->user_flags);

	if (screen)
	{
		screen->m6c = true;
		screen->function_147f6d(parameters);
	}
	return screen;
}

// @retail 0x2333dd
void c_postgame_statistics_screen::v17()
{
	c_screen_widget *tab;

	tab = &tab1;
	tab->v17();
	tab = &tab2;
	tab->v17();
	tab = &tab3;
	tab->v17();
	tab = &tab4;
	tab->v17();
	tab = &tab5;
	tab->v17();
	tab = &tab6;
	tab->v17();
}

/* the tabs and their lists; the first tab shows unless it has no rows */
// @retail 0x233423
void c_postgame_statistics_screen::v18(void *parameters)
{
	volatile long definition_index = function_1480ff(screen_id);
	s_screen_layout layout =
	{
		&tab_bar,
		6,
		{
			{ 0, 0, &tab1.list, 0 },
			{ 0, 0, &tab2.list, 0 },
			{ 0, 0, &tab3.list, 0 },
			{ 0, 0, &tab4.list, 0 },
			{ 0, 0, &tab5.list, 0 },
			{ 0, 0, &tab6.list, 0 }
		}
	};
	c_screen_widget *tab;

	tab_bar.add_child(&tab1);
	tab_bar.add_child(&tab2);
	tab_bar.add_child(&tab3);
	tab_bar.add_child(&tab4);
	tab_bar.add_child(&tab5);
	tab_bar.add_child(&tab6);
	build(&layout);
	c_user_interface_widget::v1();
	tab = &tab1;
	tab->v18(0);
	tab = &tab2;
	tab->v18(0);
	tab = &tab3;
	tab->v18(0);
	tab = &tab4;
	tab->v18(0);
	tab = &tab5;
	tab->v18(0);
	tab = &tab6;
	tab->v18(0);
	start_time = network_time_get();
	if (tab1.parent)
	{
		if (g_51ec0c > 0)
		{
			tab = &tab1;
			tab->v7(&tab1.list);
			tab_bar.focused = tab;
		}
		else
		{
			tab = &tab2;
			tab->v7(&tab2.list);
			tab_bar.focused = tab;
			tab1.value6e = false;
		}
	}
	else
	{
		start_animation(3);
	}
}

/* the subtitle tells how to leave and whether a player's card can be shown */
// @retail 0x2335b7
void c_postgame_statistics_screen::v3()
{
	if (tab_bar.focused)
	{
		bool can_leave = !function_592f0() && !value5ef0;
		long index = 0;
		c_user_interface_widget *text = 0;
		long string_ids[4] = { 0x1d000764, 0x16000763, 0xd000766, 0x6000765 };

		if (tab_is_current(&tab1))
		{
			index = can_leave ? 2 : 3;
			text = tab1.find_child(6, 4, false);
		}
		else if (tab_is_current(&tab2))
		{
			index = !can_leave;
			text = tab2.find_child(6, 6, false);
		}
		else if (tab_is_current(&tab3))
		{
			index = !can_leave;
			text = tab3.find_child(6, 6, false);
		}
		else if (tab_is_current(&tab4))
		{
			index = !can_leave;
			text = tab4.find_child(6, 4, false);
		}
		else if (tab_is_current(&tab5))
		{
			index = !can_leave;
			text = tab5.find_child(6, 4, false);
		}
		else if (tab_is_current(&tab6))
		{
			index = !can_leave;
			text = tab6.find_child(6, 6, false);
		}
		if (!function_6c7e0() || function_199994() || function_1999b3())
		{
			if (index < 2)
			{
				index += 2;
			}
		}
		subtitle.set_string(string_ids[index]);
		if (text)
		{
			text->value6e = g_51ec08 <= 0;
		}
	}
	c_user_interface_widget::v3();
}

/* the dialog's choice leaves the game */
// @retail 0x233786
bool __stdcall function_233786(long controller_index)
{
	function_199e2e(false);
	return true;
}

/* B or back: the saved film's statistics close, a host's are left by the
   dialog */
// @retail 0x233792
bool c_postgame_statistics_screen::handle_back(s_widget_event *event)
{
	bool result = false;
	bool leaving = function_592f0();

	switch (event->param)
	{
	case 1:
	case 13:
		if (value5ef0)
		{
			start_animation(3);
		}
		else if (leaving)
		{
			function_19a942();
		}
		else
		{
			dialog_choice_show(3, 0x98, 4, 1 << event->controller_index, function_233786, 0, 0);
		}
		result = true;
		break;
	}
	return result;
}

// @retail 0x2336ef
bool c_postgame_statistics_screen::v10(s_widget_event *event)
{
	if (event->type == 5)
	{
		handle_back(event);
	}
	return c_screen_widget::v10(event);
}

/* the screen fades in over the game's last view two seconds after it opens */
// @retail 0x23370e
void c_postgame_statistics_screen::v23(void *window)
{
	if (!value5ef0)
	{
		long elapsed = network_time_since(start_time);
		dword color;

		if (elapsed <= 2000)
		{
			color = 0xff000000;
		}
		else
		{
			long fade = elapsed - 2000;
			real alpha;
			long value;

			if (fade < 0)
			{
				fade = 0;
			}
			else if (fade > 2000)
			{
				fade = 2000;
			}
			alpha = (1.0f - (real)fade * 0.0005f) * 255.0f;
			__asm
			{
				fld alpha
				fistp value
			}
			color = value << 24;
		}
		((c_render_window *)window)->function_147cdb(color);
	}
}
