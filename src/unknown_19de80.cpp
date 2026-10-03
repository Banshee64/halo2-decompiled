#include "cseries.h"
#include "globals.h"
#include "game_engine_events.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_19DE80.CPP: the game engine events. An event names a cause (a
   player and a team) and an effect (a player and a team); each player is
   shown the response the multiplayer globals define for the event's type and
   subtype, chosen by how the player relates to the event (the cause player,
   the cause team, the effect player, the effect team, or anyone else). */

/* one response of the multiplayer globals to an event (0xa8 bytes) */
struct s_event_response
{
	byte unknown00[4];
	short subtype;
	short audience;
	byte unknown08[0x10 - 0x08];
	short required;
	short excluded;
	byte unknown14[0xa8 - 0x14];
};

struct s_event_response_block
{
	long count;
	s_event_response *responses;
};

/* the multiplayer globals' runtime data: the responses by event type */
struct s_event_globals
{
	byte unknown00[0x98];
	s_event_response_block blocks[11];
};

struct s_event_globals_definition
{
	byte unknown00[0xc];
	s_event_globals *globals;
};

/* the players (0x21c bytes each), as the events see them */
struct s_event_player
{
	byte unknown00[0xc0];
	char team;
	byte unknownc1[0x21c - 0xc1];
};

/* the iterator over the players of 0x19f240 */
struct s_player_iterator
{
	s_event_player *player;
	s_data_array *data;
	long index;
	long absolute_index;
};

bool function_19f240(long *iterator);
void function_19e890(long player_index, s_event_response *response, s_event *event);

static inline s_event_player *event_player_get(long player_index)
{
	return (s_event_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_event_player));
}

// @retail 0x19df10
s_event_response *function_19df10(long player_index, s_event *event, long audience)
{
	s_event_globals *globals = ((s_event_globals_definition *)g_4e3b44[g_4e034c->index & 0xffff].bytes)->globals;
	s_event_response_block *block = 0;

	switch (event->type)
	{
	case 0:
		block = &globals->blocks[0];
		break;
	case 2:
		block = &globals->blocks[2];
		break;
	case 3:
		block = &globals->blocks[3];
		break;
	case 4:
		block = &globals->blocks[4];
		break;
	case 6:
		block = &globals->blocks[6];
		break;
	case 1:
		block = &globals->blocks[1];
		break;
	case 8:
		block = &globals->blocks[8];
		break;
	case 9:
		block = &globals->blocks[9];
		break;
	case 10:
		block = &globals->blocks[10];
		break;
	}

	for (long i = 0; i < block->count; i++)
	{
		s_event_response *response = &block->responses[i];

		if (response->subtype == event->subtype && response->audience == audience)
		{
			bool valid = true;
			bool match;

			switch (response->required)
			{
			case 1:
				valid = event->cause_player_index != NONE;
				break;
			case 2:
				valid = event->cause_team != NONE;
				break;
			case 3:
				valid = event->effect_player_index != NONE;
				break;
			case 4:
				valid = event->effect_team != NONE;
				break;
			}

			switch (response->excluded)
			{
			case 1:
				match = valid && event->cause_player_index != player_index;
				break;
			case 2:
				match = valid && event->cause_team != event_player_get(player_index)->team;
				break;
			case 3:
				match = valid && event->effect_player_index != player_index;
				break;
			case 4:
				match = valid && event->effect_team != event_player_get(player_index)->team;
				break;
			default:
				match = valid;
				break;
			}

			if (match)
				return response;
		}
	}

	return 0;
}

// @retail 0x19de80
void function_19de80(s_event *event, long player_index)
{
	s_event_player *player = event_player_get(player_index);
	s_event_response *response;

	if (player_index == event->cause_player_index && (response = function_19df10(player_index, event, 0)) != 0 ||
		player_index == event->effect_player_index && (response = function_19df10(player_index, event, 2)) != 0 ||
		player->team == event->cause_team && (response = function_19df10(player_index, event, 1)) != 0 ||
		player->team == event->effect_team && (response = function_19df10(player_index, event, 3)) != 0 ||
		(response = function_19df10(player_index, event, 4)) != 0)
	{
		function_19e890(player_index, response, event);
	}
}

// @retail 0x19eb30
void function_19eb30(s_event *event)
{
	if (event->a == NONE)
	{
		s_player_iterator iterator;

		iterator.data = g_4e8c24;
		iterator.absolute_index = NONE;
		iterator.index = NONE;
		while (function_19f240((long *)&iterator))
			function_19de80(event, iterator.index);
	}
	else
	{
		function_19de80(event, event->a);
	}
}

// @retail 0x19eb90
void function_19eb90(s_event *event)
{
	if (g_4e6948->mode != 4)
	{
		function_a7c50(event);
		function_19eb30(event);
	}
}

// @retail 0x19ebb0
void game_engine_event_initialize(s_event *event, long type, long subtype)
{
	event->type = type;
	event->subtype = subtype;
	event->a = NONE;
	event->cause_player_index = NONE;
	event->cause_team = NONE;
	event->effect_player_index = NONE;
	event->effect_team = NONE;
	event->f = 0;
	event->g = NONE;
}

// @retail 0x19ebe0
void game_engine_event_set_cause_player(s_event *event, long player_index)
{
	event->cause_player_index = player_index;
	event->cause_team = event_player_get(player_index)->team;
}

// @retail 0x19ec10
void game_engine_event_set_effect_player(s_event *event, long player_index)
{
	event->effect_player_index = player_index;
	event->effect_team = event_player_get(player_index)->team;
}
