/* GAME_ENGINE_EVENTS.H: a game engine event (unknown_19de80.cpp). An event
   names a cause (a player and a team) and an effect (a player and a team);
   the engines (unknown_072c70.cpp, unknown_2bbf50.cpp, juggernaut.cpp) build
   one and send it. */

#ifndef GAME_ENGINE_EVENTS_H
#define GAME_ENGINE_EVENTS_H

#include "cseries.h"

/* 0x24 bytes */
struct s_event
{
	long type;
	long subtype;
	long a;
	long cause_player_index;
	long cause_team;
	long effect_player_index;
	long effect_team;
	long f;
	short g;
};

void function_a7c50(s_event *event);
void function_19eb30(s_event *event);
void function_19eb90(s_event *event);

#endif
