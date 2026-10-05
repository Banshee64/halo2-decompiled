// @flags /O1 /Gr
/* UNKNOWN_22ACB4.CPP: whether a local player's player is of the kind that
   shows the second marker color */

#include "unknown_11c920.h"
#include "globals.h"

long function_14de70(long local_player_index);

/* a player, as the test reads it */
struct s_player_22a
{
	byte unknown00[0x88];
	byte type;
	byte unknown89[0x21c - 0x89];
};

// @retail 0x22acb4
bool function_22acb4(long local_player_index)
{
	long player_index = function_14de70(local_player_index);
	bool result = false;

	if (player_index != NONE)
	{
		s_player_22a *player = (s_player_22a *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);

		if (player->type == 1 || player->type == 3)
		{
			result = true;
		}
	}
	return result;
}
