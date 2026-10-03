// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_18A2F0.CPP: object looping sound queries: the time left on a
   slot of g_4ed288, and the flags of the looping sound data (g_4ed28c) */

#include "cseries.h"
#include "globals.h"

struct s_looping_sound
{
	short salt;
	byte value2;
	char type;
	union
	{
		word flags;
		struct
		{
			word bit0 : 1;
			word bit1 : 1;
			word bit2 : 1;
			word bit3 : 1;
			word bit4 : 1;
			word bit5 : 1;
			word bit6 : 1;
			word bit7 : 1;
			word bit8 : 1;
		} flag_bits;
	};
	short value6;
	real value8;
	long tag_index;
	long value10;
	union
	{
		long value14;
		struct
		{
			char byte14;
			char byte15;
			short short16;
		};
	};
};

struct s_looping_sound_definition
{
	byte unknown00[4];
	real duration;
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

// @retail 0x188f90
long looping_sound_new_attached(long tag_index, long object_index, short marker_index, long value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = datum_new(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->tag_index = tag_index;
			sound->value2 = 3;
			sound->flags = 0;
			sound->flags |= 0x100;
			if (object_index != NONE)
			{
				sound->type = 1;
				sound->value14 = value;
				sound->value10 = object_index;
				sound->value6 = marker_index;
			}
			else
			{
				sound->value6 = NONE;
				sound->type = 0;
				sound->value10 = 0;
				sound->byte14 = NONE;
				sound->short16 = NONE;
			}
		}
	}
	return datum_index;
}

// @retail 0x18a5a0
long function_18a5a0(long tag_index, long object_index, real value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = looping_sound_new_attached(tag_index, object_index, NONE, 0);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);
			sound->flag_bits.bit0 = true;
			sound->value8 = value;
		}
	}
	return datum_index;
}

// @retail 0x18a600
long function_18a600(long tag_index, real value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = datum_new(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound_definition *definition = (s_looping_sound_definition *)g_4e3b44[tag_index & 0xffff].bytes;
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->type = 4;
			sound->value2 = 3;
			sound->flags = 0;
			sound->flags |= 0x100;
			sound->value8 = value;
			sound->tag_index = tag_index;
			sound->value6 = NONE;
			sound->flags |= 1;
			if (definition->duration > 0.0f)
			{
				sound->value10 = g_510c54->game_time + real_to_long_round((real)g_510c54->ticks_per_second * definition->duration);
			}
			else
			{
				sound->value10 = NONE;
			}
		}
	}
	return datum_index;
}

// @retail 0x18a6c0
long function_18a6c0(long tag_index, long value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = datum_new(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->type = 2;
			sound->value2 = 3;
			sound->value10 = value;
			sound->flags = 0;
			sound->flags |= 0x100;
			sound->value8 = 0.0f;
			sound->tag_index = tag_index;
			sound->value6 = NONE;
			sound->flags |= 1;
		}
	}
	return datum_index;
}

// @retail 0x18a750
long function_18a750(long tag_index, long value)
{
	long datum_index = NONE;

	if (tag_index != NONE)
	{
		datum_index = datum_new(g_4ed28c);
		if (datum_index != NONE)
		{
			s_looping_sound *sound = looping_sound_get(datum_index);

			sound->type = 3;
			sound->value2 = 3;
			sound->value10 = value;
			sound->flags = 0;
			sound->flags |= 0x100;
			sound->value8 = 0.0f;
			sound->tag_index = tag_index;
			sound->value6 = NONE;
			sound->flags |= 1;
		}
	}
	return datum_index;
}