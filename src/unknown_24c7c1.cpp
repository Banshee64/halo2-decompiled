// @flags /O1 /Gr
/* UNKNOWN_24C7C1.CPP: list walkers and the state of the first-person HUD
   (g_5023f4) */

#include "cseries.h"
#include "globals.h"
#include "unknown_19b516.h"
#include "unknown_13fd90.h"

struct s_link
{
	long unknown00;
	s_link *next;
};

class c_vtable_base
{
public:
	virtual void method0(long a) = 0;
};

class c_list_item : public c_vtable_base, public s_link
{
};

class c_event_base
{
public:
	virtual void method0(s_event **event, long *key) = 0;
};

class c_event_item : public c_event_base, public s_link
{
};

struct s_item_list
{
	s_link *first;
};

struct s_hud_element
{
	byte unknown00[0x22];
	word flag_index;
	byte state;
	byte unknown25[0x1b];
};

struct s_hud_tag_data
{
	byte unknown00[0xc];
	word (*flags)[1];
	byte unknown10[4];
	s_hud_element *elements;
};

struct s_hud_state
{
	byte unknown00[0x138c];
	s_hud_element *element_138c;
	s_hud_element *element_1390;
	short value_1394;
	byte unknown1396[2];
	long time_1398;
	short ticks_139c;
	byte unknown139e[6];
	short value_13a4;
	byte flag_13a6;
	byte flag_13a7;
};

struct s_hud_player
{
	byte unknown00[0x220];
	word text[0x110];
	long value_440;
	bool flag_444;
	byte unknown445[9];
	bool flag_44e;
	bool flag_44f;
	byte unknown450[0x4e0 - 0x450];
};

struct s_object_view
{
	byte unknown00[0xe8];
	short value_e8;
	short value_ea;
};

struct s_view_globals
{
	byte unknown00;
	byte flag;
};

struct s_entry_globals_view
{
	byte unknown00[0x20c];
	long tag_index;
};


s_hud_state *g_5023f4;
s_object_view *g_510c94;
s_view_globals *g_510c98;

// @retail 0x24c7c1
void function_24c7c1(s_item_list *list, long a)
{
	for (s_link *link = list->first; link; link = link->next)
	{
		static_cast<c_list_item *>(link)->method0(a);
	}
}

// @retail 0x24c7e4
void function_24c7e4(void *list_pointer, s_event **event, long *key)
{
	s_item_list *list = (s_item_list *)list_pointer;
	for (s_link *link = list->first; link; link = link->next)
	{
		static_cast<c_event_item *>(link)->method0(event, key);
	}
}

// @retail 0x24c80b
s_object_view *function_24c80b(void)
{
	s_object_view *view = g_510c94;

	return view ? view : 0;
}

// @retail 0x24c831
void function_24c831(short index)
{
	s_entry_globals_view *globals = (s_entry_globals_view *)g_4e0350;

	if (g_510c98->flag && globals->tag_index != NONE)
	{
		long tag_index = globals->tag_index;		s_hud_tag_data *data = (s_hud_tag_data *)g_4e3b44[tag_index & 0xffff].bytes;
		g_5023f4->element_138c = data->elements + index;
	}
}

// @retail 0x24c878
void function_24c878(short index)
{
	long tag_index = ((s_entry_globals_view *)g_4e0350)->tag_index;

	if (tag_index != NONE)
	{
		s_hud_tag_data *data = (s_hud_tag_data *)g_4e3b44[tag_index & 0xffff].bytes;
		s_hud_element *element = data->elements + index;

		if (element->state == 1 && *(byte *)(data->flags + element->flag_index) == 0)
		{
			s_object_view *view = g_510c94;
			s_hud_state *hud = g_5023f4;

			hud->element_1390 = element;
			hud->value_1394 = view->value_ea + view->value_e8;
		}
	}
}

// @retail 0x24c8e2
void function_24c8e2(long a, short b)
{
	short ticks = (short)(a * 0x3c + b) * g_510c54->ticks_per_second;

	g_5023f4->ticks_139c = ticks;
	g_5023f4->flag_13a6 = 0;
	g_5023f4->flag_13a7 = 1;
	g_5023f4->time_1398 = g_510c54->game_time;

	short value = g_5023f4->value_13a4;
	g_5023f4->value_13a4 = (value < 0) ? 0 : (value > 4) ? 4 : value;
}

// @retail 0x24c93f
void function_24c93f(bool flag)
{
	g_5023f4->flag_13a6 = flag;

	short ticks = g_5023f4->ticks_139c;
	if (ticks > 0)
	{
		long delta;

		if (flag)
		{
			delta = (word)(g_5023f4->time_1398 - g_510c54->game_time);
		}
		else
		{
			delta = (short)(g_510c54->game_time - g_5023f4->time_1398);
		}
		g_5023f4->ticks_139c = delta + ticks;
	}
}

// @retail 0x24c98c
void function_24c98c(long index, bool flag)
{
	s_hud_player *player = (s_hud_player *)g_5023f4 + index;

	player->flag_44e |= (player->flag_444 != flag);
	player->flag_444 = flag;
	player->value_440 = 0;

	if (flag)
	{
		player->value_440 = 0;
		unicode_string_copy(player->text, (const word *)L"", 0xff);
	}

	player->flag_44f = flag;
}
