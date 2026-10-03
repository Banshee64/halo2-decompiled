/* ONLINE_FRIENDS.H: a friend as the game keeps it (src/online_friends.cpp) */

#ifndef ONLINE_FRIENDS_H
#define ONLINE_FRIENDS_H

#include "cseries.h"
#include <xtl.h>
#include <xonline.h>

/* 0x40 bytes: online_friend_copy fills the first 0x1d; retail's frame in
   0x1a3728 (unknown_1a2ca7.cpp) reserves the full size */
#pragma pack(push, 1)
struct s_online_friend
{
	XUID xuid;
	dword flags;
	XNKID session_id;
	DWORD title_id;
	bool unknown1c;
	byte unknown1d[0x40 - 0x1d];
};
#pragma pack(pop)

void online_friend_copy(const XONLINE_FRIEND *friend_, s_online_friend *result);

#endif
