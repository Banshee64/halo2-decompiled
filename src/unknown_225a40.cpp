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
	g_51ebf4 = (s_unknown_225a40 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_225a40));
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

struct s_sound_track_definition_entry
{
	byte unknown00[0x3c];
	long label;
	byte unknown40[0x4c - 0x40];
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

static inline void *tag_block_get_element_with_size(s_tag_block_view const *block, long index, long element_size)
{
	return (byte *)block->address + index * element_size;
}

long game_sound_find_platform_playback_by_label(long label);
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
				label = ((s_sound_track_definition_entry *)tag_block_get_element_with_size(
					&((s_sound_track_definition *)g_4e3b44[track->label & 0xffff].bytes)->entries,
					track->entry_index, sizeof(s_sound_track_definition_entry)))->label;
				break;
			case 1:
				label = track->label;
				break;
			default:
				continue;
			}

			long handle = game_sound_find_platform_playback_by_label(label);
			if (handle != NONE)
			{
				sound_track_start_impulse(i, handle);
			}
		}
	}
	function_225b60(0);
}

// @retail 0x225ef0
void function_225ef0(long label)
{
	long handle = game_sound_find_platform_playback_by_label(label);

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
