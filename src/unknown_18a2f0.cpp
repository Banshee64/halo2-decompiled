// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_18A2F0.CPP: object looping sound queries: the time left on a
   slot of g_4ed288, and the flags of the looping sound data (g_4ed28c) */

#include "cseries.h"
#include "globals.h"

struct s_looping_sound_slot
{
	long datum_index;
	long end_time;
	long c;
	long d;
};

struct s_looping_sound_globals
{
	long indices[8];
	long value20;
	long value24;
	word scales[0x80];
	s_looping_sound_slot slots[16];
};

extern s_looping_sound_globals *g_4ed288;

struct s_looping_sound
{
	byte unknown00[4];
	word flags;
	byte unknown06[0x12];
};

static __forceinline long real_to_long_round(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return result;
}

static inline s_looping_sound *looping_sound_get(long datum_index)
{
	return (s_looping_sound *)g_4ed28c->data + (datum_index & 0xffff);
}

static inline long looping_sound_slot_find(long datum_index)
{
	long i = 0;
	s_looping_sound_slot *slot = g_4ed288->slots;

	for (; i < 16; i++, slot++)
	{
		if (slot->datum_index == datum_index)
		{
			return i;
		}
	}
	return NONE;
}

// @retail 0x18a2f0
long function_18a2f0(long datum_index, long seconds)
{
	long result = 0;

	if (datum_index != NONE)
	{
		long index = looping_sound_slot_find(datum_index);
		if (index != NONE)
		{
			s_game_time_globals *game_time = g_510c54;
			real scaled = (real)seconds * (1.0f / 30.0f);
			long ticks = real_to_long_round(scaled * (real)game_time->ticks_per_second);
			long remaining = g_4ed288->slots[index].end_time - game_time->game_time + ticks;
			result = remaining > 0 ? remaining : 0;
		}
	}
	return result;
}

// @retail 0x18a5e0
word *function_18a5e0(long datum_index)
{
	word *flags = &looping_sound_get(datum_index)->flags;

	*flags |= 2;
	return flags;
}

// @retail 0x18a720
void function_18a720(long datum_index, bool set)
{
	word *flags = &looping_sound_get(datum_index)->flags;

	if (set)
	{
		*flags |= 0x10;
	}
	else
	{
		*flags &= ~0x10;
	}
}

// @retail 0x18ae50
long __stdcall function_18ae50(long a, long b, real const *values)
{
	if (values[a] > values[b])
	{
		return 1;
	}
	return 0;
}
