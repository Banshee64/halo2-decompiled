// @flags /O2 /Gr
/* NETWORK_UTILITIES.CPP: a player's colours from their appearance and team */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "network_utilities.h"

real_rgb_color *function_1a06a0(long index, real_rgb_color *color);

/* the default team colour */
real_rgb_color const *g_468734;

struct s_team_colors
{
	byte unknown00[0x10];
	long team_color_count;
	real_rgb_color *team_colors;
};

/* the multiplayer globals tag (index at +0x16c of the globals) */
struct s_multiplayer_globals
{
	long universal_count;
	s_team_colors *universal;
};

// @retail 0x7f720
real_rgb_color *function_7f720(real_rgb_color *color, short team_index)
{
	real_rgb_color result = *g_468734;
	long tag_index = g_4e034c->index;

	if (tag_index != NONE)
	{
		s_team_colors *universal = ((s_multiplayer_globals *)g_4e3b44[tag_index & 0xffff].bytes)->universal;

		if (team_index >= 0 && team_index < universal->team_color_count)
		{
			result = universal->team_colors[team_index];
		}
	}

	*color = result;
	return color;
}

// @retail 0x7f790
void function_7f790(short team_index, bool use_default, s_player_appearance const *appearance, real_rgb_color *colors)
{
	real_rgb_color color;

	colors[0] = *function_1a06a0(appearance->colors[0], &color);
	colors[1] = *function_1a06a0(appearance->colors[1], &color);
	colors[2] = *function_1a06a0(appearance->colors[2], &color);
	colors[3] = *function_1a06a0(appearance->colors[3], &color);

	if (use_default)
	{
		colors[0] = *(real_rgb_color const *)g_468710;
		colors[1] = *(real_rgb_color const *)g_468710;
	}
	else if (team_index != NONE)
	{
		colors[0] = *function_7f720(&color, team_index);
		colors[1] = *function_1a06a0(appearance->colors[0], &color);
	}
}
