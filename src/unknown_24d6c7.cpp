// @flags /O1 /Ob1 /arch:SSE /Gr
/* UNKNOWN_24D6C7.CPP: the navigation points of the HUD (g_5023f8): four
   points for each local user, each naming an object, a type and a team */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_123b30.h"

struct s_nav_point
{
	short type;
	short team : 4;
	short field_2_4 : 4;
	short unknown : 8;
	real value;
	long object_index;
};

struct s_nav_points
{
	s_nav_point points[4];
};

struct s_nav_point_player
{
	byte unknown00[0x28];
	short user_index;
	byte unknown2a[0xc0 - 0x2a];
	char team;
	byte unknownc1[0x21c - 0xc1];
};

s_nav_points *g_5023f8;

static inline s_nav_point_player *nav_point_player_get(long player_index)
{
	return (s_nav_point_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_nav_point_player));
}

// @retail 0x24d6c7
void hud_nav_points_initialize()
{
	g_5023f8 = (s_nav_points *)function_123d40("unknown", "unknown", 4 * sizeof(s_nav_points));
}

// @retail 0x24d6d7
void function_24d6d7(long player_index, short type, short team, long object_index, real value)
{
	if (player_index != NONE)
	{
		long user_index = nav_point_player_get(player_index)->user_index;

		if (user_index != NONE && object_index != NONE && type != NONE)
		{
			s_nav_points *points = &g_5023f8[user_index];
			short free_index = NONE;
			short i;

			for (i = 0; i < 4; i++)
			{
				s_nav_point *point = &points->points[i];

				if (point->team == team && point->object_index == object_index)
				{
					point->type = type;
					point->value = value;
					return;
				}
				if (point->team == NONE)
					free_index = i;
			}

			if (free_index != NONE)
			{
				s_nav_point *point = &points->points[free_index];

				point->value = value;
				point->team = team;
				point->object_index = object_index;
				point->type = type;
			}
		}
	}
}

// @retail 0x24d7ac
void __stdcall function_24d7ac(short type, short player_team, short is_object, long object_index, real value)
{
	s_data_datum_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (data_datum_iterator_next(&iterator))
	{
		s_nav_point_player *player = (s_nav_point_player *)iterator.datum;

		if (player->user_index != NONE && player_team == player->team)
			function_24d6d7(iterator.datum_index, type, is_object, object_index, value);
	}
}

// @retail 0x24d806
void function_24d806(long player_index, short team, long object_index)
{
	if (player_index != NONE)
	{
		long user_index = nav_point_player_get(player_index)->user_index;

		if (user_index != NONE && object_index != NONE)
		{
			s_nav_points *points = &g_5023f8[user_index];

			for (short i = 0; i < 4; i++)
			{
				s_nav_point *point = &points->points[i];

				if (point->team == team && point->object_index == object_index)
				{
					point->team = NONE;
					point->object_index = NONE;
					point->type = NONE;
					break;
				}
			}
		}
	}
}

// @retail 0x24d877
void __stdcall function_24d877(short player_team, short is_object, long object_index)
{
	s_data_datum_iterator iterator;

	iterator.data = g_4e8c24;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (data_datum_iterator_next(&iterator))
	{
		s_nav_point_player *player = (s_nav_point_player *)iterator.datum;

		if (player->user_index != NONE && player_team == player->team)
			function_24d806(iterator.datum_index, is_object, object_index);
	}
}

bool function_14ddc0(long local_player_index);
long function_14de70(long local_player_index);
void function_caf60(long unit_index, point3f *position);
struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
void function_30c60(dword handle, point3f *position, long *out);
long function_24da18(point3f const *point0, point3f const *point1,
	long ignore_unit_index, long local_player_index);

struct s_object_24d8d3
{
	byte field_00[0x10a];
	word field_10a_0 : 2;
	word field_10a_2 : 1;
	word field_10a_3 : 13;
};

struct s_point_24d8d3
{
	byte field_00[0x24];
	point3f field_24;
	byte field_30[8];
};

struct s_scenario_24d8d3
{
	byte field_00[0x1e4];
	s_point_24d8d3 *field_1e4;
};

// @retail 0x24d8d3
void __stdcall function_24d8d3(long local_player_index)
{
	s_nav_points *points = &g_5023f8[local_player_index];
	long unit_index;
	if (function_14ddc0(local_player_index))
	{
		long player_index = function_14de70(local_player_index);
		unit_index = *(long *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c + 0x2c);
	}
	else
		unit_index = NONE;
	for (long i = 0; i < 4; ++i)
	{
		s_nav_point *point = &points->points[i];
		if (point->type != NONE && point->object_index != NONE && point->team != NONE)
		{
			if (unit_index != NONE)
			{
				point3f origin;
				point3f destination;
				long ignore_index = NONE;
				function_caf60(unit_index, &origin);
				switch (point->team)
				{
				case 0:
					destination = ((s_scenario_24d8d3 *)g_4e0350)->field_1e4[point->object_index].field_24;
					break;
				case 1:
				{
					long object_index = point->object_index;
					s_object_24d8d3 *object = (s_object_24d8d3 *)function_badc0(object_index, NONE);
					ignore_index = object_index;
					if (!object || TEST_FIELD_BIT(object->field_10a_2))
					{
						point->object_index = NONE;
						point->team = NONE;
						point->type = NONE;
						continue;
					}
					long local_10;
					function_30c60(object_index, &destination, &local_10);
					break;
				}
				}
				destination.z += point->value;
				point->field_2_4 = function_24da18(&origin, &destination, ignore_index, local_player_index);
			}
		}
		else
			point->team = NONE;
	}
}
