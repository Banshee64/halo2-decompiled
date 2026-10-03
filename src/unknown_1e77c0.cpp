// @flags /O2 /Gr
/* UNKNOWN_1E77C0.CPP: recording a unit request in its player's local state
   (the e6900 callee) */

#include "cseries.h"
#include "globals.h"

/* a player (g_4e8c24, 0x21c bytes): +0x28 is its local player slot */
struct s_player_request_view
{
	byte unknown00[0x28];
	short local_player_index;
	byte unknown2a[0x21c - 0x2a];
};

struct s_time_entry
{
	long time;
	short a;
	short b;
};

/* a local player's state in g_51e9c0 (src/unknown_1e6a40.cpp), 0x1b0 bytes:
   the four last unit requests at +0x150 */
struct s_local_player_state_view
{
	byte unknown000[0x150];
	s_time_entry requests[4];
	byte unknown170[0x1b0 - 0x170];
};

struct s_unknown_1e6a40;
extern s_unknown_1e6a40 *g_51e9c0;

void function_1e6980(s_time_entry *entries, short a, byte b);

// @retail 0x1e77c0
void function_1e77c0(long player_index, long type, byte result)
{
	s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);

	if (player->local_player_index != NONE)
		function_1e6980(((s_local_player_state_view *)g_51e9c0)[player->local_player_index].requests, (short)type, result);
}
