// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1BBA20.CPP: what the ai does when units and players act (called
   from the unit code) */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_1fb7e0.h"

s_ai_player *ai_player_get(long player_index);

/* a player (0x21c bytes, g_4e8c24) */
struct s_ai_event_player
{
	byte unknown000[0x2c];
	long unit_index;
	byte unknown030[0x21c - 0x30];
};

/* a seat of a unit definition (0xb0 bytes) */
struct s_ai_event_seat
{
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword flag6 : 1;
	dword flag7 : 1;
	dword flag8 : 1;
	dword flag9 : 1;
	dword flag10 : 1;
	dword flag11 : 1;
	dword unknown : 20;
	byte unknown04[0xb0 - 0x4];
};

struct s_ai_event_unit_definition
{
	byte unknown000[0x1cc];
	s_ai_event_seat *seats;
};

// @retail 0x1bba20
void function_1bba20(long actor_index)
{
	if (g_4f55d0->active)
	{
		s_actor_view *actor = actor_get(actor_index);

		if (actor->unknown018 != NONE)
			function_20ba60(0x69, actor->unknown018, actor_get(actor_index)->unknown26c, NONE, NONE, NULL);
	}
}

/* a player's unit got into a vehicle */
// @retail 0x1bbc00
void function_1bbc00(long player_index, long vehicle_index)
{
	if (g_4f55d0->active)
	{
		s_ai_event_player *player = (s_ai_event_player *)(g_4e8c24->data + (player_index & 0xffff) * sizeof(s_ai_event_player));
		s_ai_player *ai_player = ai_player_get(player_index);
		long unit_index;
		short seat_index;

		if (ai_player)
		{
			ai_player->unit_index = NONE;
			ai_player->unknown08 = NONE;
			ai_player->unknown0a = 0;
		}
		unit_index = player->unit_index;
		seat_index = object_get(unit_index)->unknown1fc;
		if (seat_index != NONE)
		{
			s_ai_event_unit_definition *definition = (s_ai_event_unit_definition *)g_4e3b44[object_get(vehicle_index)->tag_index & 0xffff].bytes;

			if (!TEST_FIELD_BIT(definition->seats[seat_index].flag11))
				function_20ba60(0x66, unit_index, vehicle_index, NONE, NONE, NULL);
		}
	}
}
