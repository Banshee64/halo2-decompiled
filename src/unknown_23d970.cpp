// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_23D970.CPP: a view state and its default direction, player and
   object searches, and the vertex shader constant table / full screen quad
   push buffer writers (batch 55-1) */

#include "cseries.h"
#include <xtl.h>
#include <math.h>
#include <string.h>
#include "globals.h"
#include "data_array.h"
#include "object_iterator.h"

/* ---- types ---- */

struct s_view_state
{
	real x;
	real y;
	real z;
	real yaw;
	real pitch;
	real unknown14;
	real unknown18;
};

struct s_view_globals
{
	s_view_state state;
	dword unknown1c;
	s_view_state output;
	byte valid;
};

struct s_player
{
	short identifier;
	byte unknown02[0x2a];
	long value2c;
	byte unknown30[0x90];
	char team;
	byte unknownc1[0x21c - 0xc1];
};

struct s_object
{
	byte unknown00[0x14];
	long parent_index;
	byte unknown18[0x14];
	short value2c;
};

struct s_input_state
{
	byte unknown00[0x10];
	byte values[0x38];
};

struct s_recent_entry
{
	long id;
	dword unknown04;
	byte flag08;
	byte unknown09;
	short value;
};

struct s_hash_entry
{
	dword hash;
	long values[4];
	long time;
};

struct s_recent_globals
{
	s_recent_entry recent[5];
	s_hash_entry hashes[4];
	long count;
};

struct s_hash_key
{
	dword v0;
	dword v1;
	dword unknown08;
	dword v3;
	dword v4;
	dword v5;
	dword v6;
};

void __stdcall function_23d970(s_view_state *state);
void __stdcall function_23f120(long a, long b, long c);
bool function_015d00(long count, dword **out);

/* ---- globals ---- */

s_view_globals g_51ec40;
real_vector3d g_502318;
byte g_4e61b9;
s_input_state g_4e61dc[3];
s_input_state g_4e630c;
byte g_485af0;
real g_4856c4[9];
long g_470a3c[8];
byte g_470a38;
s_recent_globals g_502350;

/* the callback 23d970 is stored in the .rdata definition at 0x44ab70 (slot
   0x44ab90); 23f120 is slot 4 of the sound source table g_444b7c
   (unknown_18c810.cpp) */
void (__stdcall *g_44ab90)(s_view_state *) = function_23d970;

#define PLAYER(array, index) ((s_player *)((array)->data + sizeof(s_player) * (index)))

/* ---- functions ---- */

// @retail 0x23d970
void __stdcall function_23d970(s_view_state *state)
{
	g_51ec40.state = *state;
	if (g_51ec40.valid)
	{
		*state = g_51ec40.output;
		return;
	}

	state->x = 0.f;
	state->y = 1.f;
	state->z = 0.f;
	state->yaw = (real)atan2(g_502318.j, g_502318.i);
	state->pitch = (real)atan2(g_502318.k, sqrt(g_502318.j * g_502318.j + g_502318.i * g_502318.i));
}

// @retail 0x23dba0
bool function_23dba0(long player_index)
{
	s_data_array *players = g_4e8c24;
	long team = PLAYER(players, player_index & 0xffff)->team;
	long index = NONE;

	for (;;)
	{
		index = data_find_index(players, index + 1);
		if (index == NONE)
			break;
		s_player *player = (s_player *)(players->data + players->size * index);
		long datum = (player->identifier << 16) | index;
		if (datum != player_index && player->team == team)
			return true;
	}
	return false;
}

// @retail 0x23dc40
long function_23dc40(long player_index, long last_index, bool same_team)
{
	s_data_array *players = g_4e8c24;
	long team = NONE;
	long result = NONE;
	long index = NONE;

	if (same_team)
		team = PLAYER(players, player_index & 0xffff)->team;

	for (;;)
	{
		index = data_find_index(players, index + 1);
		if (index == NONE)
			break;
		s_player *player = (s_player *)(players->data + players->size * index);
		long datum = (player->identifier << 16) | index;
		if (datum != player_index && player->value2c != NONE && (!same_team || player->team == team))
		{
			if (result == NONE)
				result = datum;
			else if ((datum & 0xffff) > (last_index & 0xffff))
			{
				result = datum;
				break;
			}
		}
	}

	if (result == NONE)
		result = last_index;
	return result;
}
// @retail 0x23dd30
long function_23dd30(void)
{
	s_object_iterator iterator;
	long result = NONE;

	iterator.signature = 0x86868686;
	iterator.type_mask = 0x40;
	iterator.flags = 0;
	iterator.index = 0;
	iterator.object_index = NONE;
	s_object *object;
	while ((object = function_baeb0(&iterator)) != 0)
	{
		if (object->parent_index == NONE && object->value2c != NONE)
			result = iterator.object_index;
	}
	return result;
}

/* the state of the observer camera that follows another player */
struct s_observer_state
{
	byte unknown00[0x28];
	real timer;
	long player_index;
	long target_player_index;
	long target_unit_index;
	real delay;
	byte reset;
};

struct s_game_options_flags_view
{
	byte unknown00[0x184];
	byte flag0 : 1;
};

// @retail 0x23dda0
void function_23dda0(s_observer_state *observer)
{
	s_game_options_view *options = g_4e6948;
	long unit_index = NONE;
	bool same_team;

	if (options->state == 2)
	{
		same_team = false;
		if (g_55e4d0[g_4e9ae8->engine_index])
			same_team = TEST_FIELD_BIT(((s_game_options_flags_view *)options)->flag0);
	}
	else
	{
		same_team = function_23dba0(observer->player_index);
	}

	observer->target_player_index = function_23dc40(observer->player_index, observer->target_player_index, same_team);
	if (observer->target_player_index != NONE)
		unit_index = PLAYER(g_4e8c24, observer->target_player_index & 0xffff)->value2c;

	real delay = 3.f;
	if (unit_index != observer->target_unit_index && unit_index != NONE)
	{
		observer->timer = delay;
		observer->target_unit_index = unit_index;
		observer->reset = false;
	}
	if (options->state == 2)
		delay = 15.f;
	observer->delay = delay;
}

static inline long next_index(long index, long maximum)
{
	long result = NONE;
	if (index >= 0 && index < maximum)
		result = index + 1;
	return result;
}

// @retail 0x23e400
bool function_23e400(long index)
{
	bool result = false;

	for (long i = 0; i != NONE; i = next_index(i, 3))
	{
		short slot = (short)i;
		if (g_4e61cc[slot])
		{
			s_input_state *state;
			if (g_4e61b9)
				state = &g_4e630c;
			else
				state = &g_4e61dc[slot];
			if (state && state->values[index])
				result = true;
		}
	}
	return result;
}

// @retail 0x23e460
void function_23e460(void)
{
	if (g_485af0)
	{
		real constants[22][4] =
		{
			{ 53.f, 15.f, 0.f, 0.15915494f },
			{ (real)sin(0.f), (real)cos(0.f), 0.f, 0.f },
			{ (real)sin(0.41887903f), (real)cos(0.41887903f), 0.f, 0.f },
			{ (real)sin(0.83775806f), (real)cos(0.83775806f), 0.f, 0.f },
			{ (real)sin(1.2566371f), (real)cos(1.2566371f), 0.f, 0.f },
			{ (real)sin(1.6755161f), (real)cos(1.6755161f), 0.f, 0.f },
			{ (real)sin(2.0943952f), (real)cos(2.0943952f), 0.f, 0.f },
			{ (real)sin(2.5132742f), (real)cos(2.5132742f), 0.f, 0.f },
			{ (real)sin(2.9321532f), (real)cos(2.9321532f), 0.f, 0.f },
			{ (real)sin(3.3510323f), (real)cos(3.3510323f), 0.f, 0.f },
			{ (real)sin(3.7699113f), (real)cos(3.7699113f), 0.f, 0.f },
			{ (real)sin(4.1887903f), (real)cos(4.1887903f), 0.f, 0.f },
			{ (real)sin(4.6076694f), (real)cos(4.6076694f), 0.f, 0.f },
			{ (real)sin(5.0265484f), (real)cos(5.0265484f), 0.f, 0.f },
			{ (real)sin(5.4454274f), (real)cos(5.4454274f), 0.f, 0.f },
			{ (real)sin(5.8643064f), (real)cos(5.8643064f), 0.f, 0.f },
			{ (real)sin(6.2831855f), (real)cos(6.2831855f), 0.f, 0.f },
			{ g_4856c4[0], g_4856c4[3], g_4856c4[6], 0.f },
			{ g_4856c4[1], g_4856c4[4], g_4856c4[7], 0.f },
			{ g_4856c4[2], g_4856c4[5], g_4856c4[8], 0.f },
			{ 1.f, 0.f, 0.99f, 42.f },
			{ 2.f, 3.051851e-05f, 1.f, 42.f }
		};

		for (long i = 1; i < 17; i++)
		{
			long next = (i == 16) ? 1 : i + 1;
			constants[i][2] = constants[next][0] - constants[i][0];
			constants[i][3] = constants[next][1] - constants[i][1];
		}

		D3DDevice_SetVertexShaderConstant(52, constants, 22);
		g_485af0 = 0;
	}
}

// @retail 0x23ecd0
void function_23ecd0(void)
{
	g_470a3c[0] = 1;
	g_470a3c[1] = 2;
	g_470a3c[2] = 3;
	g_470a3c[3] = 4;
	g_470a3c[4] = 5;
	g_470a3c[5] = 6;
	g_470a3c[6] = 7;
	g_470a3c[7] = 8;
}

static inline dword *push_vertex_data4f(dword *push, long slot, real a, real b, real c, real d)
{
	push[0] = D3DPUSH_ENCODE(0x1a00 + slot * 16, 4);
	((real *)push)[1] = a;
	((real *)push)[2] = b;
	((real *)push)[3] = c;
	((real *)push)[4] = d;
	return push + 5;
}

static inline dword *push_vertex_data2f(dword *push, long slot, real a, real b)
{
	push[0] = D3DPUSH_ENCODE(0x1880 + slot * 8, 2);
	((real *)push)[1] = a;
	((real *)push)[2] = b;
	return push + 3;
}

static inline dword *push_vertex_data4ub(dword *push, long slot, dword value)
{
	push[0] = D3DPUSH_ENCODE(0x1940 + slot * 4, 1);
	push[1] = value;
	return push + 2;
}
// @retail 0x23ed30
void function_23ed30(real_point3d *a, real_point3d *b, real c, real d, dword color, real e, real *f, real *g, real *h)
{
	dword *push;

	if (g_470a38 && function_015d00(0x2e, &push))
	{
		push = push_vertex_data4f(push, g_470a3c[0], b->x, b->y, b->z, 1.f);
		push = push_vertex_data2f(push, g_470a3c[1], e, 0.f);
		push = push_vertex_data4f(push, g_470a3c[3], a->x, a->y, a->z, 1.f);
		push = push_vertex_data2f(push, g_470a3c[4], c, d);
		push = push_vertex_data4ub(push, g_470a3c[7], color);
		push = push_vertex_data4f(push, g_470a3c[2], f[0], f[1], f[2], f[3]);
		push = push_vertex_data4f(push, g_470a3c[5], g[0], g[1], g[2], g[3]);
		push = push_vertex_data4f(push, g_470a3c[6], h[0], h[1], h[2], h[3]);
		*push++ = D3DPUSH_ENCODE(D3DPUSH_SET_BEGIN_END, 1);
		*push++ = 8;
		*push++ = D3DPUSH_ENCODE(D3DPUSH_INLINE_ARRAY, 8) | D3DPUSH_NOINCREMENT_FLAG;
		*push++ = 0;
		*push++ = 0;
		*push++ = 1;
		*push++ = 0;
		*push++ = 0x10001;
		*push++ = 0;
		*push++ = 0x10000;
		*push++ = 0;
		*push++ = D3DPUSH_ENCODE(D3DPUSH_SET_BEGIN_END, 1);
		*push++ = 0;
		D3DDevice_EndPush(push);
	}
}

// @retail 0x23f0a0
void function_23f0a0(void)
{
	memset(&g_502350, 0, sizeof(g_502350));
	g_502350.recent[0].flag08 = 0;
	g_502350.count = 1;
	g_502350.recent[0].id = NONE;

	real seconds = g_510c54->ticks_per_second * 2.f;
	long ticks;
	__asm
	{
		fld seconds
		fistp ticks
	}
	g_502350.recent[0].value = (short)ticks;

	for (long i = 0; i < 4; i++)
		g_502350.hashes[i].time = -1000;
}

// @retail 0x23f120
void __stdcall function_23f120(long a, long b, long c)
{
	if (g_502350.recent[0].unknown04 == b)
		g_502350.recent[0].flag08 = 0;
}

// @retail 0x23f140
void function_23f140(long id, short value)
{
	if (g_502350.count < 5)
	{
		g_502350.recent[g_502350.count].id = id;
		g_502350.recent[g_502350.count].value = value;
		g_502350.count++;
	}
}

// @retail 0x23f180
s_hash_entry *function_23f180(s_hash_key *key)
{
	s_game_time_globals *game_time = g_510c54;
	real seconds = game_time->ticks_per_second * 0.9f;
	long threshold;
	__asm
	{
		fld seconds
		fistp threshold
	}

	dword hash = ((((key->v1 << 12) ^ key->v4) << 4) ^ ~(key->v6 << 8)) ^ ~key->v5 ^ key->v3 ^ key->v0;
	long now = game_time->game_time;
	long best = NONE;
	long best_age = NONE;
	bool is_new = true;

	for (long i = 0; i < 4; i++)
	{
		s_hash_entry *entry = &g_502350.hashes[i];
		if (entry->hash == hash && now - entry->time < threshold)
		{
			best = i;
			is_new = false;
			break;
		}
		long age = now - entry->time;
		if (age > best_age)
		{
			best_age = age;
			best = i;
		}
	}

	g_502350.hashes[best].time = now;
	if (is_new)
	{
		g_502350.hashes[best].hash = hash;
		memset(g_502350.hashes[best].values, 0xff, sizeof(g_502350.hashes[best].values));
	}
	return &g_502350.hashes[best];
}
