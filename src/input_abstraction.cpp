// @flags /O2 /arch:SSE /Gr
/* INPUT_ABSTRACTION.CPP: the end of retail's input_abstraction.cpp (its
   start, 0x217bc0 and 0x217c40, is not decompiled yet): the default gamepad
   preferences, the reaction to gamepad changes during a movie, and whether a
   local player sits in a seat that takes over its controls */

#include "cseries.h"
#include <xtl.h>
#include <string.h>
#include "globals.h"
#include "data_array.h"

/* the gamepad preferences of a player (0x1c bytes) */
struct s_gamepad_preferences
{
	real look_sensitivity_horizontal;
	real look_sensitivity_vertical;
	char button_map[16];
	short unknown18;
	bool unknown1a;
	bool unknown1b;
};

// @retail 0x218420
void input_preferences_set_defaults(s_gamepad_preferences *preferences)
{
	preferences->look_sensitivity_horizontal = 120.0f;
	preferences->look_sensitivity_vertical = 60.0f;
	memset(preferences->button_map, NONE, sizeof(preferences->button_map));
	preferences->button_map[0] = 0;
	preferences->button_map[1] = 4;
	preferences->button_map[2] = 2;
	preferences->button_map[3] = 3;
	preferences->button_map[4] = 1;
	preferences->button_map[5] = 5;
	preferences->button_map[6] = 6;
	preferences->button_map[7] = 7;
	preferences->button_map[8] = 12;
	preferences->button_map[9] = 13;
	preferences->button_map[10] = 14;
	preferences->button_map[11] = 15;
	preferences->button_map[12] = 10;
	preferences->button_map[13] = 11;
	preferences->button_map[14] = 0;
	preferences->button_map[15] = 1;
	preferences->unknown18 = 0;
	preferences->unknown1a = false;
	preferences->unknown1b = false;
}

/* whether the input system runs, the time of the last input and the time
   gamepads changed while nothing was played */
bool g_51ebd0;
dword g_51ebc8;
dword g_55e77c;

void bink_playback_end(void);

/* told about gamepad insertions and removals: ends a movie that plays
   while the gamepads change, after two seconds without input */
// @retail 0x2184a0
void function_2184a0(dword device_changes)
{
	if (g_51ebd0)
	{
		if (device_changes)
		{
			if (GetTickCount() - g_51ebc8 >= 2000 || (g_55e77c && GetTickCount() - g_55e77c >= 2000))
			{
				if (g_4e9188.initialized && !(g_4e9188.flags & 0x500))
				{
					bink_playback_end();
				}
			}
		}
		if ((device_changes & 0xfff000) && !g_55e77c)
		{
			g_55e77c = GetTickCount();
		}
	}
}

/* the local players' player indices */
struct s_local_player_table_view
{
	byte unknown00[0x1c];
	long player_indices[4];
};

/* a player (0x21c bytes) */
struct s_input_player_view
{
	byte unknown00[0x2c];
	long unit_index;
	byte unknown30[0x21c - 0x30];
};

/* an object header (12 bytes) */
struct s_input_object_header
{
	byte unknown00[8];
	byte *object;
};

/* a seat of a unit definition (0xb0 bytes) */
struct s_input_unit_seat
{
	dword flag0 : 1;
	dword flag1 : 1;
	dword controls_player : 1;
	dword unknown03 : 29;
	byte unknown04[0xb0 - 4];
};

static inline byte *input_object_get(long object_index)
{
	return ((s_input_object_header *)g_4e0300->data)[object_index & 0xffff].object;
}

/* whether a local player's unit sits in a seat with its third flag set */
// @retail 0x218510
bool function_218510(long local_player_index)
{
	long player_index;
	long unit_index;
	bool result;

	if (local_player_index == NONE)
	{
		player_index = NONE;
	}
	else
	{
		player_index = ((s_local_player_table_view *)g_4e8c20)->player_indices[local_player_index];
	}
	if (player_index == NONE)
	{
		unit_index = NONE;
	}
	else
	{
		unit_index = ((s_input_player_view *)g_4e8c24->data)[player_index & 0xffff].unit_index;
	}

	result = false;
	if (unit_index != NONE)
	{
		byte *unit = input_object_get(unit_index);
		long parent_index = *(long *)(unit + 0x14);

		if (parent_index != NONE)
		{
			short seat_index = *(short *)(unit + 0x1fc);

			if (seat_index != NONE)
			{
				byte *parent = input_object_get(parent_index);
				byte *definition = g_4e3b44[*(long *)parent & 0xffff].bytes;
				short type = *(short *)(definition + 0x1f0);

				if (type == 3 || type == 5)
				{
					s_input_unit_seat *seats = *(s_input_unit_seat **)(definition + 0x1cc);

					if (TEST_FIELD_BIT(seats[seat_index].controls_player))
					{
						result = true;
					}
				}
			}
		}
	}
	return result;
}
