// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_13BF00.CPP: the lifecycle callbacks of entry 53 */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include <string.h>

struct s_unknown_ids
{
	long values[4];
};

struct s_unknown_13bf00
{
	real unknown0;
	bool flag4;
	bool flag5;
	byte unknown06[2];
	s_unknown_ids ids;
	byte unknown18[0xa];
	bool flag22;
	byte unknown23;
};

s_unknown_13bf00 *g_510c50;

PRIVATE void ids_clear(s_unknown_ids *ids)
{
	for (long i = 0; i < 4; i++)
	{
		ids->values[i] = NONE;
	}
}

// @retail 0x13bf00
void function_13bf00(void)
{
	g_510c50 = (s_unknown_13bf00 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_13bf00));
	memset(g_510c50, 0, sizeof(*g_510c50));
}

// @retail 0x13bf60
void function_13bf60(void)
{
	s_unknown_13bf00 *data = g_510c50;

	memset(data, 0, sizeof(*data));
	ids_clear(&data->ids);
	if (g_4e6948->state == 1)
	{
		data->flag4 = true;
		data->unknown0 = 1.0f;
	}
}

// @retail 0x13bfc0
void function_13bfc0(void)
{
	if (g_4e6948->state != 1)
	{
		g_510c50->flag4 = false;
	}
	g_510c50->flag5 = false;
	g_510c50->flag22 = false;
}

/* the titles showing: a title index and the ticks it has shown */
struct s_title_state
{
	short index;
	short ticks;
};

struct s_13bf00_view
{
	real fade;
	bool fading_in;
	byte unknown05[3];
	s_title_state titles[4];
	long timer_index;
	real timer;
};

struct s_title_definition
{
	byte unknown00[0x1c];
	real fade_in_time;
	real up_time;
};

struct s_4e0350_titles_view
{
	byte unknown000[0x1f0];
	long title_count;
	s_title_definition *titles;
};

// @retail 0x13c1e0
void __stdcall function_13c1e0(short title_index, real seconds)
{
	s_13bf00_view *data = (s_13bf00_view *)g_510c50;
	short i;
	for (i = 0; i < 4; i++)
	{
		if (data->titles[i].index == NONE)
		{
			break;
		}
	}
	if (i < 4)
	{
		data->titles[i].index = title_index;
		real ticks = g_510c54->ticks_per_second * seconds;
		long rounded;
		__asm
		{
			fld ticks
			fistp rounded
		}
		data->titles[i].ticks = (short)-rounded;
	}
}

static inline void timer_update()
{
	s_13bf00_view *data = (s_13bf00_view *)g_510c50;
	if (data->timer_index)
	{
		data->timer -= g_510c54->rate;
		if (0.0f >= data->timer)
		{
			data->timer_index = 0;
			*(long *)&data->timer = 0;
		}
	}
}

// @retail 0x13c680
void function_13c680(void)
{
	s_game_time_globals *game_time = g_510c54;
	timer_update();

	s_13bf00_view *data = (s_13bf00_view *)g_510c50;
	if (data)
	{
		s_4e0350_titles_view *globals = (s_4e0350_titles_view *)g_4e0350;
		if (data->fading_in || data->fade > 0.0f)
		{
			real fade;
			if (data->fading_in)
			{
				fade = game_time->rate + data->fade;
				data->fade = fade;
				if (fade > 1.0f)
				{
					fade = 1.0f;
				}
			}
			else
			{
				fade = data->fade - game_time->rate;
				data->fade = fade;
				if (!(fade > 0.0f))
				{
					fade = 0.0f;
				}
			}
			data->fade = fade;
		}

		s_title_state *title = data->titles;
		long count = 4;
		do
		{
			short index = title->index;
			long pinned = index < 0 ? 0 : (index > globals->title_count - 1 ? globals->title_count - 1 : index);
			if (pinned == index)
			{
				title->ticks++;
				s_title_definition *definition = &globals->titles[index];
				real duration = definition->up_time + definition->fade_in_time;
				if (title->ticks * game_time->rate >= duration)
				{
					title->index = NONE;
					title->ticks = NONE;
				}
			}
			title++;
		} while (--count);
	}
}

// @retail 0x13cb40
bool function_13cb40(void)
{
	bool result = false;
	if (g_510c50)
	{
		result = g_510c50->flag5;
	}
	return result;
}

void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);

struct s_4e0350_strings_view
{
	byte unknown000[0x35c];
	long strings_tag_index;
};

// @retail 0x13cb50
void function_13cb50(long string_id, real seconds)
{
	if (g_4e0350)
	{
		word string[256];
		string[0] = 0;
		unicode_string_list_get_string(((s_4e0350_strings_view *)g_4e0350)->strings_tag_index, string_id, string);
		if (string[0])
		{
			s_13bf00_view *data = (s_13bf00_view *)g_510c50;
			data->timer_index = string_id;
			data->timer = seconds + 1.5f;
		}
	}
}