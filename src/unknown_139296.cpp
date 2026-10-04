// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_139296.CPP: per-player interface state (built for size) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "network_utilities.h"
#include <string.h>
#include <math.h>

struct s_player_view
{
	byte unknown00[0x28];
	short user_index;
};

struct s_user_interface_state
{
	real value00;
	byte unknown04[0x6c - 0x04];
};

struct s_510c4c;
extern s_510c4c *g_510c4c;

/* the interface state cleared each game: draw state, then a value per user */
struct s_4e6950
{
	byte unknown00[0x70];
	real user_values[4];
};

s_4e6950 g_4e6950;

struct s_4e69d0
{
	long indices[7];
	byte unknown1c[0x270 - 0x1c];
};

s_4e69d0 g_4e69d0[4];

// @retail 0x139296
void function_139296(long user_index, real value)
{
	if (user_index >= 0 && user_index < 4)
	{
		g_4e6950.user_values[user_index] = value;
	}
}

// @retail 0x1392a9
real function_1392a9(long user_index)
{
	if (user_index >= 0 && user_index < 4)
	{
		return g_4e6950.user_values[user_index];
	}
	return 1.0f;
}

// @retail 0x13a6e8
void function_13a6e8(long player_index, real amount)
{
	s_player_view *player = (s_player_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long user_index = player->user_index;
	if (user_index != NONE)
	{
		s_user_interface_state *state = (s_user_interface_state *)(user_index * sizeof(s_user_interface_state) + (byte *)g_510c4c);
		state->value00 = state->value00 - amount;
	}
}

// @retail 0x13ac42
void function_13ac42(long index)
{
	memset(&g_4e69d0[index], 0, sizeof(s_4e69d0));
	g_4e69d0[index].indices[0] = NONE;
	g_4e69d0[index].indices[1] = NONE;
	g_4e69d0[index].indices[2] = NONE;
	g_4e69d0[index].indices[3] = NONE;
	g_4e69d0[index].indices[4] = NONE;
	g_4e69d0[index].indices[5] = NONE;
	g_4e69d0[index].indices[6] = NONE;
}

// @retail 0x13ac30
void function_13ac30(void)
{
	for (long index = 0; index < 4; index++)
	{
		function_13ac42(index);
	}
}

void *function_123d40(char const *name, char const *type, long size);

/* the new hud's game state (g_510c4c, 0x1e4 bytes) */
struct s_new_hud_user
{
	real value00;
	real value04;
	long index08;
	byte unknown0c[0x50 - 0x0c];
	long unknown50[6];
	long unknown68;
};

struct s_new_hud_globals
{
	s_new_hud_user users[4];
	byte unknown1b0[4];
	long player_index;
	byte unknown1b8[0x1c0 - 0x1b8];
	s_player_appearance appearance;
	short unknown1d0;
	bool unknown1d2;
	bool unknown1d3;
	bool unknown1d4;
	byte unknown1d5[0x1d8 - 0x1d5];
	real current;
	real target;
	real rate;
};

void function_22a648(void);

// @retail 0x139152
void new_hud_initialize_for_new_map(void)
{
	s_new_hud_globals *globals = (s_new_hud_globals *)g_510c4c;

	memset(globals, 0, sizeof(s_new_hud_globals));
	globals->current = 1.0f;
	globals->target = 1.0f;
	globals->rate = 0.0f;
	globals->player_index = NONE;
	globals->unknown1d0 = NONE;
	globals->unknown1d2 = true;
	globals->unknown1d4 = true;
	globals->unknown1d3 = true;
	for (long i = 0; i < 4; i++)
	{
		memset(globals->users[i].unknown50, 0xff, sizeof(globals->users[i].unknown50));
		globals->users[i].value00 = -1.0f;
		globals->users[i].value04 = -1.0f;
		globals->users[i].index08 = NONE;
	}
	memset(&globals->appearance, 0, sizeof(globals->appearance));
	function_22a648();
	function_13ac30();
}

dword __cdecl pack_color3f(const color3f *color);

dword g_502234[4];
byte g_502244;

// @retail 0x1392f8
void function_1392f8(s_player_appearance const *appearance)
{
	s_player_appearance const *const *appearance_reference = &appearance;
	color3f colors[4];

	function_7f790(NONE, false, appearance, colors);
	for (long i = 0; i < 4; i++)
	{
		g_502234[i] = pack_color3f(&colors[i]);
	}
	((s_new_hud_globals *)g_510c4c)->appearance = **appearance_reference;
	g_502244 = 4;
}

struct s_new_hud_player
{
	byte unknown00[0x84];
	s_player_appearance appearance;
};

// @retail 0x1392c5
void function_1392c5(long player_index)
{
	((s_new_hud_globals *)g_510c4c)->player_index = player_index;
	if (player_index != NONE)
	{
		s_new_hud_player *player = (s_new_hud_player *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
		function_1392f8(&player->appearance);
	}
}

/* the per-user interface state in the game state (called by function_19170f) */
// @retail 0x139130
void function_139130(void)
{
	g_510c4c = (s_510c4c *)function_123d40("new hud", NULL, 0x1e4);
	memset(&g_4e6950, 0, sizeof(g_4e6950));
	function_13ac30();
}

struct s_name_buffer
{
	wchar_t name[256];
};

void function_08cc20(s_name_buffer *buffer, const wchar_t *name);

struct s_510c4c_view
{
	byte unknown000[0x1bc];
	byte *strings;
	byte unknown1c0[0x1d2 - 0x1c0];
	bool flag1d2;
};

// @retail 0x13934d
void function_13934d(s_name_buffer *buffer, long string_handle)
{
	byte *strings = ((s_510c4c_view *)g_510c4c)->strings;
	if (strings)
	{
		wchar_t const *name;
		switch (string_handle)
		{
		case 0xe42d:
			name = (wchar_t const *)(strings + 0x1bc);
			break;
		case 0xe42e:
			name = (wchar_t const *)(strings + 0x208);
			break;
		case 0xe42f:
			name = (wchar_t const *)(strings + 0x134);
			break;
		case 0xe430:
			name = (wchar_t const *)(strings + 0x176);
			break;
		default:
			name = NULL;
			break;
		}
		if (name)
		{
			function_08cc20(buffer, name);
			return;
		}
	}
	buffer->name[0] = 0;
}

long function_155760(long index);
bool function_155d60(long index);
extern long g_4b9ed8;
bool g_4f55e2;

// @retail 0x13939b
bool function_13939b()
{
	long index = g_4b9ed8;
	return (function_155760(index) != 3 || function_155d60(index)) &&
		function_155760(index) != 2 &&
		((s_510c4c_view *)g_510c4c)->flag1d2 &&
		!(g_4e6948->state == 1 ? g_4f55e2 : false);
}

short g_4b9dd4;
short g_4b9dd6;
extern short g_4b9dd0;
extern short g_4b9dd2;
byte function_016a90();

// @retail 0x13a690
long function_13a690(long mode)
{
	long result = 0;
	short width = g_4b9dd6 - g_4b9dd2;
	short top = g_4b9dd0;
	short bottom = g_4b9dd4;

	if (width < 640 || (short)(bottom - top) < 480)
	{
		if (width < 640 && (short)(bottom - top) < 480)
		{
			result = 2;
		}
		else
		{
			result = 1;
			if (function_016a90() && mode == 3)
			{
				result = 2;
			}
		}
	}

	return result;
}
struct s_510c4c_fade_view
{
	byte unknown000[0x1d8];
	real current;
	real target;
	real rate;
};

/* moves the value towards its target at its rate, stopping there */
// @retail 0x13b285
void function_13b285()
{
	s_510c4c_fade_view *data = (s_510c4c_fade_view *)g_510c4c;
	if (data->target > data->current)
	{
		data->current += (real)fabs(data->rate);
		if (data->current > data->target)
		{
			data->current = data->target;
			data->rate = 0.0f;
		}
	}
	else if (data->current > data->target)
	{
		data->current -= (real)fabs(data->rate);
		if (data->target > data->current)
		{
			data->current = data->target;
			data->rate = 0.0f;
		}
	}
}

/* the conditions of an interface element: masks of which one must match
   and none of the other may */
struct s_condition_masks
{
	word required[4];
	word excluded[4];
	byte minimum_value;
	byte minimum_a;
	byte minimum_b;
};

struct s_condition_subject
{
	byte unknown00[6];
	short a;
	short b;
	byte unknown0a[6];
	real value;
};

// @retail 0x13ac87
bool function_13ac87(s_condition_masks const *masks, word first, word second, word fourth, word third, s_condition_subject const *subject)
{
	if (subject)
	{
		if (masks->minimum_value > subject->value)
		{
			third |= 8;
		}
		else
		{
			third &= ~8;
		}
		if (subject->a < masks->minimum_a)
		{
			third |= 0x10;
		}
		else
		{
			third &= ~0x10;
		}
		if (subject->b < masks->minimum_b)
		{
			third |= 0x20;
		}
		else
		{
			third &= ~0x20;
		}
	}

	if ((masks->required[0] & first) || (masks->required[1] & second) || (masks->required[2] & third) || (masks->required[3] & fourth))
	{
		if (!(masks->excluded[0] & first) && !(masks->excluded[1] & second) && !(masks->excluded[2] & third) && !(masks->excluded[3] & fourth))
		{
			return true;
		}
	}
	return false;
}

#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

struct s_ammunition_state
{
	byte unknown00[8];
	short rounds;
	byte unknown0a[2];
	short magazine;
	byte unknown0e[2];
	real charge;
	byte unknown14[0xd];
	bool flag21;
};

struct s_ammunition_definition
{
	byte unknown00[0x1a];
	short maximum_rounds;
	real minimum_charge;
};

/* the state an ammunition counter shows */
// @retail 0x13b083
long function_13b083(s_ammunition_state const *state, long definition_index)
{
	long result = NONE;
	if (definition_index != NONE)
	{
		s_ammunition_definition *definition = (s_ammunition_definition *)g_4e3b44[definition_index & 0xffff].bytes;
		if (state->magazine == 0 && 100 - PIN((long)(state->charge * 100.0f), 0, 100) == 0)
		{
			return 4;
		}
		if (state->magazine == 0 && definition->minimum_charge >= (1.0f - state->charge) * 100.0f)
		{
			return 3;
		}
		if (state->rounds == 0)
		{
			return 2;
		}
		if (state->rounds > definition->maximum_rounds || state->flag21)
		{
			return 7;
		}
		return 1;
	}
	return result;
}

void function_1a0180(long tag_index, long string_handle, word *buffer);

/* copies one of the HUD's message strings into a buffer of 0x100 characters */
// @retail 0x13925f
void function_13925f(long string_handle, word *buffer)
{
	s_hud_globals_definition *definition = g_510c94;

	buffer[0] = 0;
	if (definition && definition->string_list != NONE)
	{
		function_1a0180(definition->string_list, string_handle, buffer);
	}
}
