// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot handler 0x7c (g_47ef08) */

struct s_slot_7c
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long unknown10;
	long unknown14;
	bool unknown18;
	byte unknown19[3];
	long prop_index;
	byte unknown20[0x40 - 0x20];
};

/* the prop state's position (unknown_25d690.cpp) */
struct s_prop_state_position
{
	long unknown00;
	real_point2d position;
};

real normalize2d(real_point2d *v);

short __stdcall function_1c0300(long actor_index);
short __stdcall function_1c04f0(long actor_index, s_slot *slot, bool active);
void __stdcall function_1c0670(long actor_index, s_slot *slot);

// @retail 0x1c0430
bool __stdcall function_1c0430(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_7c *state = (s_slot_7c *)slot;

	state->unknown0c = false;
	state->unknown18 = false;
	state->unknown10 = g_510c54->game_time;
	state->unknown14 = g_510c54->game_time;
	state->prop_index = actor->prop_index;
	actor_reset_state(actor_index);
	return true;
}

// @retail 0x1c04c0
void __stdcall function_1c04c0(long actor_index, s_slot *slot)
{
	actor_get(actor_index)->unknown1f4 = g_510c54->game_time;
}

// @retail 0x1c0a30
void __stdcall function_1c0a30(long actor_index, s_slot *slot)
{
	s_slot_7c *state = (s_slot_7c *)slot;

	if (state->unknown0c)
	{
		actor_get(actor_index)->unknown810.bit13 = true;
	}
	else
	{
		s_actor_view *actor = actor_get(actor_index);

		if (actor->unknown50c && actor->unknown504 == 1)
		{
			s_prop_state_position *prop = (s_prop_state_position *)prop_node_state(prop_node_get(actor->prop_index));
			real_point2d direction;
			real_point2d facing;

			direction.x = prop->position.x - actor->position.x;
			direction.y = prop->position.y - actor->position.y;
			if (normalize2d(&direction) > g_45dbd8)
			{
				facing = *(real_point2d *)&actor->unknown290;
				if (normalize2d(&facing) > g_45dbd8 && facing.x * direction.x + facing.y * direction.y > 0.9f)
				{
					actor->unknown482 = true;
				}
			}
		}
	}
}

// @retail 0x1c0b60
void __stdcall function_1c0b60(long actor_index, s_slot *slot, long index)
{
	s_slot_7c *state = (s_slot_7c *)slot;

	if (state->prop_index == index)
	{
		state->prop_index = NONE;
	}
}

s_slot_handler_2 g_47ef08 =
{
	{
		0x7c, 2, NONE, -2, 0,
		function_1c0300, function_1c04f0, function_1c0430, function_1c04c0, NONE, {0},
		0, function_1c0b60, slot_release_nothing, 0, 0, 0, 0
	},
	function_1c0670, 0, function_1c0a30
};
