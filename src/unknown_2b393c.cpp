// @flags /O1 /Oi /Gr
/* UNKNOWN_2B393C.CPP: the online Y menu's list of
   the players of the recent games, copied from the player configuration
   cache when the list is built */

#include "unknown_11c920.h"
#include <string.h>
#include "data_array.h"
#include "screen_widgets.h"
#include "unknown_2312b4.h"

struct s_message;
void function_149f49(s_message *message, word a, dword *id, word b, long c, long d, long e);
c_class_1473c9 *__stdcall function_2b7212(s_screen_parameters *parameters);
void unicode_string_to_ascii(const word *source, char *destination, long maximum_count);

/* the player configuration cache's first used entry (not decompiled yet) */
long g_4cf984;

/* the next player of the player configuration cache after the iterator's */
bool player_configuration_cache_next_recent_player(s_recent_player *player, long *iterator);

/* a recent player's datum */
struct s_recent_player_datum
{
	word salt;
	word unknown02;
	s_recent_player player;
};

/* a player's identifier and name, as the name lookups take it */
struct s_player_name_2b3e
{
	dword id[3];
	word name[16];
};

struct s_player_request_2b3e
{
	dword id[3];
	char name[16];
	byte unknown1c[0x70 - 0x1c];
};

/* what the name lookups take: a player (type 1) */
#pragma pack(push, 4)
struct s_name_request
{
	long type;
	union
	{
		s_player_request_2b3e player;
		unsigned __int64 friend_xuid;
	};
	byte unknown74[4];
};
#pragma pack(pop)

void __stdcall function_148893(s_name_request *request, long flag);

/* the request's player or friend, when it is one */
static __forceinline unsigned __int64 *name_request_xuid(s_name_request *request)
{
	unsigned __int64 *xuid = 0;

	switch (request->type)
	{
	case 1:
		xuid = (unsigned __int64 *)request->player.id;
		break;
	case 2:
		xuid = &request->friend_xuid;
		break;
	}
	return xuid;
}

// @retail 0x2b393c
c_y_menu_recent_players_list::c_y_menu_recent_players_list(word user_flags) :
	c_class_1474e8(user_flags),
	value88(NONE),
	handler(this, (list_item_method)&c_y_menu_recent_players_list::handle_item)
{
	s_recent_player player;
	long iterator;
	long i;

	data = user_interface_data_new("recent players list", 100, sizeof(s_recent_player_datum));
	function_16b790(data);
	iterator = g_4cf984;
	for (i = 0; i < 100 && player_configuration_cache_next_recent_player(&player, &iterator); i++)
	{
		((s_recent_player_datum *)data->data)[record_pool_allocate(data) & 0xffff].player = player;
	}
	delegate_register(&item_handlers, &handler);
}

// @retail 0x2b3a0a deleting c_y_menu_recent_players_list

// @retail 0x2b3e26
void function_2b3e26(s_player_name_2b3e const *player, s_player_request_2b3e *request)
{
	memset(request, 0, sizeof(*request));
	memcpy(request->id, player->id, sizeof(request->id));
	unicode_string_to_ascii(player->name, request->name, 16);
	request->name[15] = 0;
}

/* a recent player opens the player's screen */
// @retail 0x2b3e4d
void c_y_menu_recent_players_list::handle_item(s_controller_reference **controller, long *item)
{
	if (*item != NONE)
	{
		s_recent_player_datum *datum = &((s_recent_player_datum *)data->data)[*item & 0xffff];
		byte zero[0xc] = { 0 };

		if (memcmp(datum, zero, sizeof(zero)) != 0)
		{
			s_name_request request;
			s_screen_parameters parameters;
			unsigned __int64 *xuid;

			parameters.field_c = 0;
			request.type = 1;
			function_2b3e26((s_player_name_2b3e const *)&datum->player, &request.player);
			function_148893(&request, 1);
			xuid = name_request_xuid(&request);
			if (xuid && *xuid)
			{
				function_149f49((s_message *)&parameters, 0, 0, 1 << (*controller)->controller_index, 3, 4, (long)function_2b7212);
				parameters.load(&parameters);
			}
		}
	}
}
