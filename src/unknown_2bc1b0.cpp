#include "cseries.h"
#include "globals.h"
#include "game_engine.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_2BC1B0.CPP: the game engine whose vtable is at 0x45c7a8 (the second
   engine object at 0x47fc84): its slots 30..50, which game_engine.h numbers
   v0..v20 (its slots 0..28 are c_game_engine_derived's v22..v50 in
   unknown_072c70.cpp), and the helpers they use. The engine keeps up to three
   marker indices in its state (g_51ecc4, defined by unknown_072c70.cpp). */

struct s_game_engine_data;
extern s_game_engine_data *g_51ecc4;

/* the engine's state, as these functions read it */
struct s_state_2bc1
{
	byte unknown00[0xc];
	long markers[3];
};

long function_19f3c0(long player_index, long type);
bool function_15eaf0();
void function_15b930(long player_index, bool by_team, long counter, long delta);

class c_game_engine_45c7a8 : public c_game_engine
{
public:
	virtual void v0(long, long, bool, long);
	virtual bool v5(long, long);
	virtual long v7(long, byte *);
};

// @retail 0x2bc1b0
bool function_2bc1b0(real_point3d *position, long index)
{
	long marker_index = ((s_state_2bc1 *)g_51ecc4)->markers[index];

	if (marker_index == NONE)
		return false;

	s_marker_entry *marker = &g_4e0350->marker_entries[marker_index];

	if (position)
		*position = marker->position;
	return true;
}

// @retail 0x2bcc10
void c_game_engine_45c7a8::v0(long player_index, long other_player_index, bool flag, long)
{
	if (function_19f3c0(other_player_index, 2) != NONE && !flag && player_index != NONE && player_index != other_player_index)
	{
		function_15b930(player_index, function_15eaf0(), 0x1a, 1);
	}
	if (player_index != NONE && !flag && player_index != other_player_index && function_19f3c0(player_index, 2) != NONE)
	{
		function_15b930(player_index, function_15eaf0(), 0x19, 1);
	}
}

// @retail 0x2bcc90
bool c_game_engine_45c7a8::v5(long player_index, long type)
{
	bool result = false;

	if (function_19f3c0(player_index, 2) != NONE)
	{
		if (type == 2)
			result = g_4e6948->s232 == 0;
		else if (type == 1)
			result = g_4e6948->flags22c_bits.bit1;
		else if (type == 3)
			result = g_4e6948->flags22c_bits.bit2;
	}
	else
		result = c_game_engine::v5(player_index, type);
	return result;
}

// @retail 0x2bcd20
long c_game_engine_45c7a8::v7(long player_index, byte *b)
{
	long result = NONE;

	*b = 1;
	if (function_19f3c0(player_index, 2) != NONE)
		result = 0xe;
	return result;
}
