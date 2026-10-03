#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "unknown_19ec40.h"
#include "data_array.h"
#include <string.h>

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_19CA10.CPP: the multiplayer globals' marker pairs: for each of the
   16 keys, the first markers of types 7 and 8 that carry it, and a message to
   a player */

/* the players (0x21c bytes each) as the marker pairs see them */
struct s_marker_player
{
	short identifier;
	word flags0 : 7;
	word near_marker : 1;
	word flag8 : 1;
	word flags9 : 7;
	byte unknown04[0x28 - 4];
	short local_index;
	byte unknown2a[0x2c - 0x2a];
	long unit_index;
	byte unknown30[0x21c - 0x30];
};

struct s_message_view
{
	byte unknown00[0xf8];
	long string_id;
};

struct s_marker_runtime_view
{
	byte unknown00[0x534];
	s_message_view *message;
};

struct s_marker_globals_definition
{
	byte unknown00[8];
	long runtime_count;
	s_marker_runtime_view *runtime;
};

void unicode_string_list_get_string(long tag_index, long string_id, word *buffer);
void function_24cbee(long player_index, word const *text);

/* shows a player the multiplayer globals' message */
// @retail 0x19ca10
void function_19ca10(long player_index)
{
	s_marker_player *player;
	s_marker_globals_definition *definition;
	word text[0x100];

	text[0] = 0;
	player = (s_marker_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_marker_player));
	wcsncpy(text, L"", 0xff);
	text[0xff] = 0;
	definition = (s_marker_globals_definition *)g_4e3b44[g_4e034c->index & 0xffff].bytes;
	if (definition->runtime_count)
	{
		long string_id = definition->runtime->message->string_id;

		text[0] = 0;
		if (g_510c94 && g_510c94->string_list != NONE)
			unicode_string_list_get_string(g_510c94->string_list, string_id, text);
	}
	function_24cbee(player->local_index, text);
}

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
