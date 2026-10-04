/* UNKNOWN_X8D43E5.H: the player data of the online menus
   (src/unknown_1a2ca7.cpp): the
   friends list, the team (clan) members, and the tasks that fill them. All of
   it is one global structure (0x46e7b8, 0x700 bytes), so taking the address
   of the friend request makes every field reload after a call. */

#ifndef ONLINE_MENU_PLAYER_DATA_H
#define ONLINE_MENU_PLAYER_DATA_H

#include "unknown_11c920.h"
#include <xtl.h>
#include <xonline.h>

struct s_record_pool;

/* the last friend request (0x6a2 bytes, the player slot's identity) */
struct s_friend_request_data
{
	byte data[0x6a2];
};

struct s_friend_request_globals
{
	s_friend_request_data request;
	bool valid;
	bool unknown6a3;
};

struct s_online_player_data_globals
{
	long controller_index;
	s_record_pool *field_4_4;
	s_record_pool *field_8_2;
	s_record_pool *clan_member_reference_data;
	s_record_pool *field_10_3;
	s_record_pool *field_14;
	long presence_task_index;
	long friends_task_index;
	long clan_members_task_index;
	long clan_members_time;
	long task_index_e0;
	long task_index_e4;
	long task_index_e8;
	XUID xuid_ec;
	XUID xuid_f8;
	XUID recent_player_xuid;
	dword start_time;
	s_friend_request_globals friend_request;
};

extern s_online_player_data_globals g_global_4acf62;

#endif
