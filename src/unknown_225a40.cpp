// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_225A40.CPP: the lifecycle callbacks of entry 47, and the two
   sound tracks it holds (each plays a sound effect and an impulse) */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include "unknown_222930.h"
#include <string.h>

#define PIN(n, floor, ceiling) ((n) < (floor) ? (floor) : ((n) > (ceiling) ? (ceiling) : (n)))

struct s_unknown_225a40
{
	long values[10];
};

s_unknown_225a40 *g_51ebf4;

// @retail 0x225a40
void function_225a40(void)
{
	g_51ebf4 = (s_unknown_225a40 *)function_123d40("unknown", "unknown", sizeof(s_unknown_225a40));
}

// @retail 0x225a80
void function_225a80(void)
{
	memset(g_51ebf4, 0, sizeof(*g_51ebf4));
}

/* a sound track (0x14 bytes) */
struct s_sound_track
{
	bool active;
	byte unknown01[3];
	long effect_index;
	real unknown08;
	long label;	/* track 0: its definition's tag index */
	union
	{
		long entry_index;	/* track 0 */
		real unknown10;	/* track 1 */
	};
};

#define SOUND_TRACKS ((s_sound_track *)g_51ebf4)

/* a tag function's data, as the evaluator reads it */
struct s_sound_track_function
{
	long size;
	byte *address;
};

struct s_sound_track_definition_entry
{
	byte unknown00[0x3c];
	long label;
	byte unknown40[4];
	s_sound_track_function scale;
};

struct s_tag_block_view
{
	long count;
	void *address;
	long unused;
};

struct s_sound_track_definition
{
	byte unknown00[0x6c];
	s_tag_block_view entries;
};

static inline void *function_x332d0d(s_tag_block_view const *block, long index, long element_size)
{
	return (byte *)block->address + index * element_size;
}

long function_18d5b0(long label);
long function_21d2c0(long platform_playback, real scale, short priority);
void sound_effect_stop(long effect_index);
void __stdcall function_225b60(long unused);

/* plays the impulse of a sound effect in one of the driver's impulse buffers */
static inline void sound_track_start_impulse(long index, long handle)
{
	if (PIN(index, 0, 1) == index)
	{
		s_looping_impulse_parameters parameters;

		function_222930(NONE, handle, &parameters);
		parameters.index = (short)index;
		function_21f720(&parameters);
	}
}

// @retail 0x225ab0
void function_225ab0(void)
{
	for (long i = 0; i < 2; i++)
	{
		s_sound_track *track = &SOUND_TRACKS[i];

		if (track->active)
		{
			long label;

			switch (i)
			{
			case 0:
				label = ((s_sound_track_definition_entry *)function_x332d0d(
					&((s_sound_track_definition *)g_4e3b44[track->label & 0xffff].bytes)->entries,
					track->entry_index, sizeof(s_sound_track_definition_entry)))->label;
				break;
			case 1:
				label = track->label;
				break;
			default:
				continue;
			}

			long handle = function_18d5b0(label);
			if (handle != NONE)
			{
				sound_track_start_impulse(i, handle);
			}
		}
	}
	function_225b60(0);
}

real function_13b390(void const *function, real input, real range);

/* a tag function's value at 0, mapped to its range */
static inline real sound_track_function_evaluate(s_sound_track_function const *function)
{
	real value;

	if (function->address && function->size > 0)
	{
		value = function_13b390(function, 0.0f, 0.0f);
		byte const *data = function->address;

		if (!(data[1] & 0xf0))
		{
			real lower = *(real const *)(data + 4);
			real upper = *(real const *)(data + 8);

			value = lower + (upper - lower) * PIN(value, 0.0f, 1.0f);
		}
	}
	else
	{
		value = 0.0f;
	}
	return value;
}

/* starts track 0 with an entry of a definition */
// @retail 0x225df0
void function_225df0(long tag_index, long entry_index)
{
	s_sound_track_definition_entry *entry = (s_sound_track_definition_entry *)function_x332d0d(
		&((s_sound_track_definition *)g_4e3b44[tag_index & 0xffff].bytes)->entries, entry_index, sizeof(s_sound_track_definition_entry));
	long handle = function_18d5b0(entry->label);

	if (handle != NONE)
	{
		s_sound_track *track = &SOUND_TRACKS[0];

		if (track->active && track->effect_index != NONE)
		{
			sound_effect_stop(track->effect_index);
		}
		real scale = sound_track_function_evaluate(&entry->scale);

		track->active = true;
		track->effect_index = function_21d2c0(handle, scale, 8);
		track->unknown08 = 0.0f;
		track->label = tag_index;
		track->entry_index = entry_index;
		sound_track_start_impulse(0, handle);
	}
}

// @retail 0x225ef0
void function_225ef0(long label)
{
	long handle = function_18d5b0(label);

	if (handle != NONE)
	{
		s_sound_track *track = &SOUND_TRACKS[1];

		if (track->active && track->effect_index != NONE)
		{
			sound_effect_stop(track->effect_index);
		}
		track->active = true;
		track->effect_index = function_21d2c0(handle, 1.0f, 8);
		track->unknown08 = 0.0f;
		track->label = label;
		track->unknown10 = 1.0f;
		sound_track_start_impulse(1, handle);
	}
}
