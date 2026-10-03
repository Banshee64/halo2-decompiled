#include <string.h>
#include "cseries.h"
#include "globals.h"
#include "game_engine.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_2BBF50.CPP: the first slots of the game engine whose vtable is at
   0x45c6d8 (the first engine object at 0x47fc80). game_engine.h numbers the
   slots from 0x45c750, so this engine's slots 0..28 are v22..v50 there; its
   slots from 30 on are in unknown_072c70.cpp. The engine keeps a player index
   in each player (+0x1b8) and four longs of state in the multiplayer
   globals at +0xfc. */

/* a game event (0x24 bytes), as 072c70 sends it */
struct s_event
{
	long type;
	long subtype;
	long a;
	long b;
	long c;
	long d;
	long e;
	long f;
	short g;
};

struct s_player_2bbf
{
	byte unknown00[0x1b8];
	long target_player_index;
	byte unknown1bc[0x21c - 0x1bc];
};

/* the engine's state in the multiplayer globals (+0xfc) */
struct s_state_2bbf
{
	long l0;
	long l4;
	long l8;
	long lc;
};

s_state_2bbf *g_51ecc0;

/* callees not decompiled yet (stubs in src/stubs/game_engine.cpp) */
void function_a7c50(s_event *);
void function_19eb30(s_event *);

class c_game_engine_45c6d8 : public c_game_engine
{
public:
	virtual bool v23();
	virtual void v27(long);
	virtual void v28(long);
	virtual void v31(long, long);
};

static inline s_player_2bbf *player_get_2bbf(long player_index)
{
	return (s_player_2bbf *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_player_2bbf));
}

/* data_iterator_next (0x16bb70), which retail inlines here */
static __forceinline byte *data_iterator_next_inlined(s_data_iterator *iterator)
{
	s_data_array *data = iterator->data;
	long index = iterator->index + 1;
	long next = NONE;
	byte *result;

	if (index >= 0)
	{
		for (; index < data->high_water_index; index++)
		{
			if (data->bitmap[index >> 5] & (1 << (index & 0x1f)))
			{
				next = index;
				break;
			}
		}
	}
	if (next != NONE)
	{
		result = data->data + data->size * next;
		iterator->index = next;
		iterator->datum_index = (*(short *)result << 16) | next;
	}
	else
	{
		iterator->index = data->maximum_count;
		iterator->datum_index = NONE;
		result = 0;
	}

	return result;
}

// @retail 0x2bbf50
bool c_game_engine_45c6d8::v23()
{
	s_state_2bbf *state = (s_state_2bbf *)&g_4e9ae8->stats;

	state->l0 = 0;
	state->l4 = 0;
	state->l8 = 0;
	g_51ecc0 = state;
	state->lc = 0;
	return true;
}

// @retail 0x2bbf70
void c_game_engine_45c6d8::v27(long player_index)
{
	player_get_2bbf(player_index)->target_player_index = NONE;
}

// @retail 0x2bbfa0
void c_game_engine_45c6d8::v28(long a)
{
	if (g_4e6948->mode != 4)
	{
		s_event event;

		event.type = 2;
		event.subtype = 0;
		event.a = a;
		event.b = NONE;
		event.c = NONE;
		event.d = NONE;
		event.e = NONE;
		event.f = 0;
		event.g = NONE;
		function_a7c50(&event);
		function_19eb30(&event);
	}
}

// @retail 0x2bc110
void c_game_engine_45c6d8::v31(long old_player_index, long new_player_index)
{
	s_data_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.datum_index = NONE;
	iterator.index = NONE;
	while (data_iterator_next_inlined(&iterator))
	{
		s_player_2bbf *player = player_get_2bbf(iterator.datum_index);

		if (player->target_player_index == old_player_index)
			player->target_player_index = new_player_index;
	}
}
