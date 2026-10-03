#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "unknown_19ec40.h"
#include "data_array.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_19CAD0.CPP: the multiplayer globals' marker pairs: for each of the
   16 keys, the first markers of types 7 and 8 that carry it */

// @retail 0x19cb20
bool function_19cb20(s_marker_pair *pair, long key)
{
	long index;

	index = NONE;
	function_19ec40(NULL, 0.0f, 7, NONE, (short)key, 1, &index, 0.0f);
	pair->first = index;
	index = NONE;
	function_19ec40(NULL, 0.0f, 8, NONE, (short)key, 1, &index, 0.0f);
	pair->second = index;
	pair->unknown08 = 0;
	pair->unknown0c = 0;

	return pair->first != NONE && pair->second != NONE;
}

// @retail 0x19cad0
void function_19cad0()
{
	s_mp_globals *globals = g_4e9ae8;
	long key;

	globals->marker_pair_count = 0;
	for (key = 0; key < 16; key++)
	{
		if (function_19cb20(&globals->marker_pairs[globals->marker_pair_count], key))
			globals->marker_pair_count++;
	}
}

/* the players (0x21c bytes each) as the marker pairs see them */
struct s_marker_player
{
	short identifier;
	word flags0 : 7;
	word near_marker : 1;
	word flag8 : 1;
	word flags9 : 7;
	byte unknown04[0x2c - 4];
	long unit_index;
};

/* the iterator of 0x19f300 (the players with a unit) */
struct s_marker_player_iterator
{
	s_marker_player *player;
	s_data_array *data;
	long datum_index;
	long index;
};

struct s_marker_object
{
	byte unknown00[0x14];
	long parent_index;
};

struct s_marker_object_header
{
	byte unknown00[8];
	s_marker_object *object;
};

bool function_19f300(long *iterator);
real distance_squared3d(real_point3d const *a, real_point3d const *b);

real const g_45dc20 = 0.25f;

// @retail 0x19ce10
void function_19ce10(s_marker_pair *pair)
{
	if (!pair->unknown08)
	{
		real_point3d position = g_4e0350->marker_entries[pair->first].position;
		s_marker_player_iterator iterator;

		iterator.data = g_4e8c24;
		iterator.index = NONE;
		iterator.datum_index = NONE;
		while (function_19f300((long *)&iterator))
		{
			s_marker_player *player = iterator.player;
			long unit_index = player->unit_index;
			real_point3d unit_position;
			real_vector3d delta;

			function_b9dd0(unit_index, &unit_position);
			vector3d_from_points3d(&position, &unit_position, &delta);
			if (((s_marker_object_header *)g_4e0300->data)[unit_index & 0xffff].object->parent_index == NONE &&
				delta.j * delta.j + delta.i * delta.i + delta.k * delta.k < g_45dc20)
			{
				player->near_marker = true;
			}
		}
	}
}
