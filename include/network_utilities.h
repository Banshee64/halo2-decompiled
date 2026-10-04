#ifndef NETWORK_UTILITIES_H
#define NETWORK_UTILITIES_H
/* NETWORK_UTILITIES.H: a player's colours (src/network_utilities.cpp) */

#include "cseries.h"
#include "real_math.h"

/* the appearance of a player (at +0x84 of a player): four colour indices
   first */
struct s_player_appearance
{
	char colors[4];
	byte unknown04[0x10 - 0x04];
};

real_rgb_color *function_7f720(real_rgb_color *color, short team_index);
void function_7f790(short team_index, bool use_default, s_player_appearance const *appearance, real_rgb_color *colors);

#endif
