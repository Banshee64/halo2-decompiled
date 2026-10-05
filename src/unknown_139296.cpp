// @flags /O1 /Oi /arch:SSE /Gr
/* UNKNOWN_139296.CPP: per-player interface state (built for size) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "unknown_07f720.h"
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

long function_1469f0(real seconds);

// @retail 0x13b306
void __stdcall function_13b306(real target, real seconds)
{
	if (seconds == 0.0f)
	{
		s_510c4c_fade_view *data = (s_510c4c_fade_view *)g_510c4c;
		data->current = target;
		data->target = target;
		data->rate = 0.0f;
	}
	else
	{
		long ticks = function_1469f0(seconds);
		if (ticks <= 1)
			ticks = 1;
		s_510c4c_fade_view *data = (s_510c4c_fade_view *)g_510c4c;
		data->target = target;
		data->rate = (target - data->current) / ticks;
	}
}

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

struct s_13b164_slot
{
	long object_index;
	long entry_index;
	byte field_8[0x14];
	real field_1c;
};

struct s_13b164_entity
{
	long tag_index;
	byte field_4[0xaa - 4];
	byte type;
	byte field_ab[0x138 - 0xab];
	short team;
	byte field_13a[0x1c8 - 0x13a];
	s_13b164_slot field_1c8;
};

struct s_13b164_header
{
	byte field_0[8];
	s_13b164_entity *entity;
};

struct s_13b164_view
{
	byte field_0[0xc];
	long field_c;
	long field_10;
	byte field_14[0x4c - 0x14];
	dword field_4c;
	byte field_50[0xab - 0x50];
	bool field_ab;
	byte field_ac[0xe3 - 0xac];
	bool field_e3;
};

struct s_13b164_entry
{
	byte field_0[0x14];
	dword flags;
	byte field_18[4];
};

struct s_13b164_definition
{
	byte field_0[0x68];
	long count;
	s_13b164_entry *entries;
};

struct s_object;
struct s_entry_pair;
s_object *function_badc0(long object_index, dword type_mask);
bool function_1df560(short team_a, short team_b);
bool function_106320(s_entry_pair *pair);

// @retail 0x13b164
void function_13b164(s_13b164_view *data, long object_index)
{
	s_13b164_entity *entity = ((s_13b164_header *)g_4e0300->data)[object_index & 0xffff].entity;
	s_13b164_entity *const *entity_reference = &entity;
	s_13b164_slot *slot = &(*entity_reference)->field_1c8;
	data->field_4c = 0;
	if (slot->object_index != NONE)
	{
		s_13b164_entity *other = (s_13b164_entity *)function_badc0(slot->object_index, NONE);
		if (other)
		{
			byte const *definition = g_4e3b44[other->tag_index & 0xffff].bytes;
			if (((1 << other->type) & 3) && !function_1df560(entity->team, other->team))
				data->field_4c |= 1;
			long tag_index = *(long const *)(definition + 0x38);
			if (tag_index != NONE && slot->entry_index != NONE && slot->field_1c >= 1.0f)
			{
				s_13b164_definition *entries = (s_13b164_definition *)g_4e3b44[tag_index & 0xffff].bytes;
				if (slot->entry_index < entries->count)
				{
					s_13b164_entry *entry = &entries->entries[slot->entry_index];
					if (entry->flags & 4)
						data->field_4c |= 0x10;
					if (entry->flags & 8)
						data->field_4c |= 4;
				}
			}
		}
		if (((data->field_c != NONE && data->field_ab) || (data->field_10 != NONE && data->field_e3)) &&
			function_106320((s_entry_pair *)slot) && slot->field_1c >= 1.0f)
		{
			data->field_4c |= 8;
		}
	}
}

struct s_13ad48
{
	byte field_0[0x20];
	real field_20;
	byte field_24[4];
	real field_28;
	real field_2c;
	byte field_30[4];
	short field_34;
	byte field_36[2];
	short field_38;
	short field_3a;
	byte field_3c[8];
	real field_44;
	byte field_48[0x1cc - 0x48];
	real field_1cc;
	byte field_1d0[0x218 - 0x1d0];
	real field_218;
	byte field_21c[4];
	real field_220;
};

struct s_13ad49
{
	byte field_0[6];
	short field_6;
	short field_8;
	short field_a;
	byte field_c[4];
	real field_10;
	real field_14;
	real field_18;
	byte field_1c[4];
	bool field_20;
	byte field_21[7];
	real field_28;
};

// @retail 0x13ad48
void function_13ad48(long arg_1, byte const *arg_2, real *arg_3, s_13ad48 const *arg_4, s_13ad49 const *arg_5)
{
	s_game_time_globals *local_1 = g_510c54;
	for (long local_2 = 0; local_2 < 4; local_2++)
	{
		real local_3 = 0.0f;
		switch (arg_2[local_2])
		{
		case 0: local_3 = 0.0f; break;
		case 1: local_3 = 1.0f; break;
		case 2: local_3 = local_1->game_time * local_1->rate; break;
		case 3: local_3 = function_1392a9(arg_1); break;
		case 16: local_3 = arg_4->field_2c; break;
		case 17: local_3 = arg_4->field_28; break;
		case 18: local_3 = arg_4->field_20 >= 1.0f ? 1.0f : 0.0f; break;
		case 19: local_3 = arg_4->field_34 == 0 ? 1.0f : 0.0f; break;
		case 20: local_3 = (real)arg_4->field_38; break;
		case 21: local_3 = (real)arg_4->field_3a; break;
		case 22: local_3 = arg_4->field_2c < 0.2f ? local_1->game_time * local_1->rate : 0.0f; break;
		case 24: local_3 = 1.0f - arg_4->field_44; break;
		case 32: local_3 = 0.0f; break;
		case 33: local_3 = 0.0f; break;
		case 48: local_3 = arg_5 ? (real)arg_5->field_6 : 0.0f; break;
		case 49: local_3 = arg_5 ? arg_5->field_14 : 0.0f; break;
		case 50: local_3 = arg_5 ? 100.0f - arg_5->field_10 * 100.0f : 0.0f; break;
		case 51: local_3 = arg_5 ? (real)arg_5->field_8 : 0.0f; break;
		case 52: local_3 = arg_5 ? arg_5->field_18 : 0.0f; break;
		case 53: local_3 = arg_5 && arg_5->field_20 ? 1.0f : 0.0f; break;
		case 54: local_3 = arg_5 && arg_5->field_a ? (real)arg_5->field_6 / arg_5->field_a : 0.0f; break;
		case 55: local_3 = (arg_5 ? arg_5->field_20 : false) ? local_1->game_time * local_1->rate : 0.0f; break;
		case 56: local_3 = arg_5 ? 1.0f - arg_5->field_10 : 0.0f; break;
		case 57: local_3 = arg_5 ? arg_5->field_28 : 0.0f; break;
		case 64: local_3 = arg_4->field_1cc; break;
		case 65: local_3 = arg_4->field_218; break;
		case 66: local_3 = *(real *)((byte *)g_510c4c + arg_1 * 0x6c + 0x28); break;
		case 67: local_3 = arg_4->field_220; break;
		}
		arg_3[local_2] = local_3;
	}
}

struct s_13a050
{
	byte field_0[6];
	short field_6;
	short field_8;
	short field_a;
	short field_c;
	byte field_e[2];
	real field_10;
	byte field_14[0xc];
	bool field_20;
	byte field_21[3];
	bool field_24;
	bool field_25;
	bool field_26;
	byte field_27[0x38 - 0x27];
};

struct s_13a051
{
	byte field_0[0x30];
	bool field_30;
	bool field_31;
	bool field_32;
	byte field_33[3];
	short field_36;
	byte field_38[4];
	short field_3c;
	bool field_3e;
	bool field_3f;
	bool field_40;
	byte field_41[0xb];
	dword field_4c;
	s_13a050 field_50[4];
	byte field_130;
	byte field_131;
	bool field_132;
	bool field_133;
	byte field_134[0x40];
	bool field_174;
	byte field_175[0x1d0 - 0x175];
	bool field_1d0;
	byte field_1d1[0x21c - 0x1d1];
	bool field_21c;
	byte field_21d[7];
	bool field_224;
};

PRIVATE __forceinline void function_13a051(dword &arg_1, long arg_2, bool arg_3)
{
	if (arg_3)
		arg_1 |= 1 << arg_2;
	else
		arg_1 &= 0xffff ^ (1 << arg_2);
}

bool function_22acb4(long arg_1);

// @retail 0x13a050
void function_13a050(s_13a051 const *arg_1, long arg_2, long arg_3, s_13a050 const **arg_4, word *arg_5, word *arg_6, word *arg_7, word *arg_8)
{
	s_13a050 const *local_1;
	switch (arg_3)
	{
	case 1: local_1 = &arg_1->field_50[1]; break;
	case 2: local_1 = &arg_1->field_50[2]; break;
	case 3: local_1 = &arg_1->field_50[3]; break;
	case 4: local_1 = &arg_1->field_50[0]; break;
	default: local_1 = NULL; break;
	}
	dword local_2 = 1;
	function_13a051(local_2, 1, arg_1->field_36 == NONE);
	function_13a051(local_2, 2, arg_1->field_36 == 0);
	function_13a051(local_2, 3, arg_1->field_36 == 1);
	function_13a051(local_2, 4, arg_1->field_30);
	function_13a051(local_2, 5, arg_1->field_31);
	function_13a051(local_2, 6, arg_1->field_3c == NONE);
	function_13a051(local_2, 7, arg_1->field_3c == 0);
	function_13a051(local_2, 8, arg_1->field_3c == 1);
	function_13a051(local_2, 8, arg_1->field_3c == 1);
	function_13a051(local_2, 9, arg_1->field_32);
	function_13a051(local_2, 10, arg_1->field_3e);
	function_13a051(local_2, 11, arg_1->field_3f);
	function_13a051(local_2, 12, arg_1->field_40);
	function_13a051(local_2, 13, function_22acb4(arg_2));
	dword local_7 = arg_1->field_4c;
	long local_3 = (signed char)local_7 & 1;
	if (local_7 & 8) local_3 |= 2; else local_3 &= 0xfffd;
	if (local_7 & 16) local_3 |= 4; else local_3 &= 0xfffb;
	if (local_7 & 4) local_3 |= 8; else local_3 &= 0xfff7;
	if (local_7 & 2) local_3 |= 16; else local_3 &= 0xffef;
	dword local_4;
	if (arg_3 == 1) local_4 = 1; else local_4 = 0;
	function_13a051(local_4, 1, arg_3 == 2);
	function_13a051(local_4, 2, arg_3 == 3);
	function_13a051(local_4, 6, local_1 ? local_1->field_20 : false);
	function_13a051(local_4, 7, false);
	if (local_1)
	{
		if (local_1->field_a && !local_1->field_6 && local_1->field_c && !local_1->field_8)
			local_4 |= 0x80;
		if (local_1->field_10 >= 1.0f)
			local_4 |= 0x80;
	}
	function_13a051(local_4, 8, local_1 ? local_1->field_24 : false);
	function_13a051(local_4, 9, local_1 ? local_1->field_25 : false);
	function_13a051(local_4, 10, local_1 ? local_1->field_26 : false);
	bool local_5 = *(long const *)((byte const *)g_4e6948 + 8) == 2;
	dword local_6 = arg_1->field_131 == 0;
	function_13a051(local_6, 1, arg_1->field_131 == 1);
	function_13a051(local_6, 2, local_5 && !arg_1->field_132);
	function_13a051(local_6, 3, local_5 && arg_1->field_132);
	function_13a051(local_6, 4, arg_1->field_133);
	function_13a051(local_6, 5, !arg_1->field_133);
	function_13a051(local_6, 6, arg_1->field_174);
	function_13a051(local_6, 7, !arg_1->field_174);
	function_13a051(local_6, 8, arg_1->field_1d0);
	function_13a051(local_6, 9, !arg_1->field_1d0);
	function_13a051(local_6, 10, arg_1->field_21c);
	function_13a051(local_6, 11, arg_1->field_224);
	*arg_4 = local_1;
	*arg_5 = (word)local_2;
	*arg_6 = (word)local_3;
	*arg_7 = (word)local_4;
	*arg_8 = (word)local_6;
}
