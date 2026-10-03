// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "unknown_20fe20.h"

/* slot type 0x1e: the actor goes to its prop's point */

struct s_slot_1e
{
	s_slot_header header;
	long start_time;
	s_node_point point;
	short ticks;
	short delay;
	byte unknown24[0x40 - 0x24];
};

/* the block of the actor's character tag function_1e4e50 returns */
struct s_character_e50
{
	byte unknown00[0x14];
	real distance;
};

void *function_1e4e50(long actor_index);
real_point3d *function_b9dd0(long object_index, real_point3d *result);

short __stdcall function_1b7920(long actor_index);
short __stdcall function_1b7b90(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1b79d0(long actor_index, s_slot *slot);
void __stdcall function_1b7c70(long actor_index, s_slot *slot);
void __stdcall function_1b7cc0(long actor_index, s_slot *slot);

/* whether the actor's prop is in state 3 within 3 of where it should be */
// @retail 0x1b7920
short __stdcall function_1b7920(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_fields *view = prop_node_view(node);

		if (view && view->unknown06 == 3 &&
			function_210970(&prop_node_state(node)->unknown48, &view->unknown18) < 3.0f)
		{
			result = 1;
		}
	}
	return result;
}

// @retail 0x1b7b90
short __stdcall function_1b7b90(long actor_index, s_slot *slot, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	s_character_e50 *character = (s_character_e50 *)function_1e4e50(actor_index);

	if (character && actor->prop_index != NONE)
	{
		s_slot_1e *state = (s_slot_1e *)slot;
		short result = g_46fbe8;

		if (g_510c54->game_time - state->start_time > state->delay)
		{
			s_prop_node_view *node = prop_node_get(actor->prop_index);
			s_prop_view_fields *view = prop_node_view(node);

			if (view)
			{
				real_point3d position;

				function_b9dd0(node->object_index, &position);
				if (!(function_210ac0(&view->unknown18, &position) > character->distance))
					return g_46fbe8;
			}
			return g_46fbe4;
		}
		return result;
	}
	return g_46fbe4;
}

// @retail 0x1b7c70
void __stdcall function_1b7c70(long actor_index, s_slot *slot)
{
	s_slot_1e *state = (s_slot_1e *)slot;

	if (state->ticks > 0 && --state->ticks == 0)
	{
		long unit_index = actor_get(actor_index)->unknown018;

		if (unit_index != NONE)
			function_20ba60(0x20, unit_index, NONE, NONE, NONE, NULL);
	}
}

s_slot_handler_2 g_47e848 =
{
	{
		0x1e, 2, 0, -2, 0,
		function_1b7920, function_1b7b90, function_1b79d0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	0, function_1b7c70, function_1b7cc0
};
