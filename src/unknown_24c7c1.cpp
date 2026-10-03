// @flags /O1 /Ob1 /arch:SSE /Gr
/* UNKNOWN_24C7C1.CPP: list walkers and the state of the first-person HUD
   (g_5023f4) */

#include <string.h>
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

/* a message of the HUD (four for each local player) */
struct s_hud_message
{
	long time;
	word text[0x3f];
	bool active;
	byte sequence;
	bool counted;
	bool expired;
	short count;
};

struct s_hud_player
{
	s_hud_message messages[4];
	word text[0x110];
	long value_440;
	bool flag_444;
	byte unknown445;
	short ticks_446;
	long sound_448;
	byte unknown44c[2];
	bool flag_44e;
	bool flag_44f;
	long time_450;
	word text_454[0x3f];
	bool flag_4d2;
	byte sequence_4d3;
	byte unknown4d4[4];
	long time_4d8;
	long value_4dc;
};

struct s_hud_state
{
	s_hud_player players[4];
	byte unknown1380[5];
	byte next_sequence;
	byte unknown1386[6];
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


struct s_object_view
{
	byte unknown00[0x58];
	real fade_time;
	real display_time;
	byte unknown60[0xe8 - 0x60];
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

	return view ? (s_object_view *)view->unknown00 : 0;
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
void function_24c8e2(short a, short b)
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

/* ---- the messages of the HUD (lane O) ---- */

void function_13925f(long string_id, word *buffer);
bool function_13cb40();
long function_14de70(long user_index);
bool function_161b60(long player_index);
long function_1896c0(real scale, long tag_index);

static inline s_hud_player *hud_player_get(long index)
{
	return &g_5023f4->players[index];
}

// @retail 0x24ca1d
void function_24ca1d(long player_index, word const *text, long sound, long tag_index)
{
	s_hud_player *player = hud_player_get(player_index);

	unicode_string_copy(player->text, text, 0xff);
	player->text[0xff] = 0;
	player->flag_444 = true;
	player->value_440 = 0;

	if (player->sound_448 != sound)
	{
		if (player->sound_448 == NONE && player->ticks_446 > g_510c54->ticks_per_second / 4 && tag_index != NONE)
			function_1896c0(1.0f, tag_index);
		player->sound_448 = sound;
		player->ticks_446 = 0;
	}
}

// @retail 0x24c9e9
void function_24c9e9(long player_index, long string_id, long sound, long tag_index)
{
	word text[0x100];

	text[0] = 0;
	function_13925f(string_id, text);
	function_24ca1d(player_index, text, sound, tag_index);
}

/* the message to reuse: a free one, the one showing the same text, or the
   oldest */
// @retail 0x24ccdb
s_hud_message *function_24ccdb(s_hud_player *player, word const *text, word const *plural_text)
{
	long oldest_time = 0x7fffffff;
	short oldest_index = 0;

	for (short i = 0; i < sizeof(player->messages) / sizeof(player->messages[0]); i++)
	{
		s_hud_message *message = &player->messages[i];

		if (!message->active)
			return message;
		if (message->counted && !message->expired && text && plural_text &&
			!wcsncmp((wchar_t const *)message->text, (wchar_t const *)(message->count > 1 ? plural_text : text), 0x3f))
			return message;
		if (!message->active)
			return message;
		if (oldest_time > message->time)
		{
			oldest_time = message->time;
			oldest_index = i;
		}
	}

	s_hud_message *oldest = &player->messages[oldest_index];

	oldest->active = false;
	return oldest;
}

// @retail 0x24caac
void function_24caac(long player_index, word const *text, word const *plural_text, short count)
{
	if (player_index != NONE)
	{
		s_hud_player *player = hud_player_get(player_index);
		s_hud_message *message = function_24ccdb(player, text, plural_text);

		if (!message->active)
			message->count = 0;
		message->count += count;
		unicode_string_copy(message->text, message->count > 1 ? plural_text : text, 0x3f);
		message->counted = true;
		message->expired = false;
		message->time = g_510c54->game_time;
		message->active = true;
		message->sequence = g_5023f4->next_sequence++;
		player->flag_44e = false;
	}
}

// @retail 0x24cb54
void function_24cb54(long player_index, word const *text, word const *plural_text)
{
	if (player_index != NONE)
	{
		s_hud_player *player = hud_player_get(player_index);

		for (long i = 0; i < 4; i++)
		{
			s_hud_message *message = &player->messages[i];

			if (message->active && message->counted && !message->expired && text && plural_text &&
				!wcsncmp((wchar_t const *)message->text, (wchar_t const *)(message->count > 1 ? plural_text : text), 0x3f))
				message->expired = true;
		}
	}
}

// @retail 0x24cbee
void function_24cbee(long player_index, word const *text)
{
	if (player_index != NONE && text && *text)
	{
		s_hud_player *player = hud_player_get(player_index);
		s_hud_message *message = function_24ccdb(player, NULL, NULL);

		unicode_string_copy(message->text, text, 0x3f);
		message->time = g_510c54->game_time;
		message->active = true;
		message->counted = false;
		message->expired = false;
		message->sequence = g_5023f4->next_sequence++;
		player->flag_44e = false;
	}
}

// @retail 0x24cbbf
void function_24cbbf(long player_index, long string_id)
{
	word text[0x100];

	text[0] = 0;
	function_13925f(string_id, text);
	function_24cbee(player_index, text);
}

// @retail 0x24cc73
void function_24cc73(long player_index, word const *text, long value)
{
	if (player_index != NONE)
	{
		s_hud_player *player = hud_player_get(player_index);

		unicode_string_copy(player->text_454, text, 0x3f);
		player->time_450 = g_510c54->game_time;
		player->flag_4d2 = true;
		player->sequence_4d3 = g_5023f4->next_sequence++;
		player->time_4d8 = g_510c54->game_time;
		player->value_4dc = value;
	}
}

/* sorts the messages: active ones first, then the newest */
// @retail 0x24cd7a
int __cdecl hud_message_compare(void const *a, void const *b)
{
	s_hud_message const *message_a = (s_hud_message const *)a;
	s_hud_message const *message_b = (s_hud_message const *)b;
	int result;

	if (message_b->active && !message_a->active)
		return 1;

	result = message_b->time - message_a->time;
	if (!result)
		result = message_b->sequence - message_a->sequence;

	return result;
}

// @retail 0x24cdaf
void __fastcall scripted_hud_messages_clear(void)
{
	for (long i = 0; i < 4; i++)
	{
		for (long j = 0; j < 4; j++)
			g_5023f4->players[i].messages[j].active = false;
	}
}

// @retail 0x24cdd8
void function_24cdd8(long player_index)
{
	s_object_view *view = function_24c80b();

	if (view && player_index != NONE && !function_13cb40() && function_161b60(function_14de70(player_index)))
	{
		s_hud_state *hud = g_5023f4;
		s_hud_player *player = &hud->players[player_index];
		short *ticks;
		long value;

		if (hud->element_1390 && hud->value_1394 > 0)
			hud->value_1394--;

		ticks = &player->ticks_446;
		value = *ticks + 1;
		*ticks = (short)(value > 0x7fff ? 0x7fff : value);

		for (long i = 0; i < 4; i++)
		{
			s_hud_message *message = &player->messages[i];

			if (message->active && (real)(g_510c54->game_time - message->time) * g_510c54->rate >= view->display_time + view->fade_time)
			{
				message->time = NONE;
				message->active = false;
			}
		}
	}
}

/* scales the four vertices of a quad about its center */
// @retail 0x24cedf
bool __stdcall function_24cedf(real *vertices, real scale)
{
	if (scale > 0.0f)
	{
		real center_x = (vertices[0] + vertices[10]) * 0.5f;
		real center_y = (vertices[1] + vertices[11]) * 0.5f;

		for (long i = 0; i < 4; i++)
		{
			real *vertex = vertices + i * 5;

			vertex[0] = vertex[0] + (vertex[0] - center_x) * scale * 0.5f;
			vertex[1] = vertex[1] + (vertex[1] - center_y) * scale * 0.5f;
		}
	}

	return true;
}
