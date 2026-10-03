// @flags /O2 /Gr
/* UNKNOWN_1E77C0.CPP: what a player's unit was last asked to do (lane I's
   outside function, called by the unit requests of 0xe6900) */

#include "cseries.h"
#include "globals.h"

/* a player (g_4e8c24, 0x21c bytes) as 0x1e77c0 reads it */
struct s_request_player
{
	byte unknown000[0x28];
	short user_index;
	byte unknown02a[0x21c - 0x2a];
};

/* the times of g_51e9c0's slots (0x1b0 bytes each, times at +0x150) */
struct s_request_time_entry
{
	long time;
	short a;
	short b;
};

struct s_request_user
{
	byte unknown000[0x150];
	s_request_time_entry times[4];
	byte unknown170[0x1b0 - 0x170];
};

struct s_request_users
{
	s_request_user users[4];
};

struct s_time_entry;

struct s_unknown_1e6a40;
extern s_unknown_1e6a40 *g_51e9c0;
void function_1e6980(s_time_entry *entries, short a, byte b);

// @retail 0x1e77c0
void function_1e77c0(long player_index, long type, bool result)
{
	s_request_player *player = (s_request_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_request_player));
	short user_index = player->user_index;

	if (user_index != NONE)
	{
		s_request_users *users = (s_request_users *)g_51e9c0;

		function_1e6980((s_time_entry *)users->users[user_index].times, (short)type, result);
	}
}
