// @flags /O1 /Ob1 /arch:SSE /Gr
/* HUD_NAV_POINTS.CPP: the navigation points of the HUD (g_5023f8): four
   points for each local user, each naming an object, a type and a team */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "game_state.h"

struct s_nav_point
{
	short type;
	short team : 4;
	short unknown : 12;
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
	g_5023f8 = (s_nav_points *)game_state_malloc("unknown", "unknown", 4 * sizeof(s_nav_points));
}

// @retail 0x24d6d7
void hud_activate_nav_point(long player_index, short type, short team, long object_index, real value)
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
			hud_activate_nav_point(iterator.datum_index, type, is_object, object_index, value);
	}
}

// @retail 0x24d806
void hud_deactivate_nav_point(long player_index, short team, long object_index)
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
			hud_deactivate_nav_point(iterator.datum_index, is_object, object_index);
	}
}
