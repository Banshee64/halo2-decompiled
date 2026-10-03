// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "data_array.h"
#include "unknown_1fb7e0.h"
#include "unknown_1f4460.h"

/* slot types 0x64, 0x65 and 0x66, the slot groups 0x62 and 0x63, and the
   slot tests 0x1c and 0x1f */

struct s_slot_64
{
	s_slot_header header;
	long prop_index;
	short unknown10;
	short unknown12;
	byte unknown14[2];
	bool unknown16;
	bool unknown17;
	bool unknown18;
	bool unknown19;
	byte unknown1a[2];
	s_node_point unknown1c;
	long unknown2c;
	byte unknown30[0x40 - 0x30];
};

struct s_slot_66
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long unknown10;
	byte unknown14[0x40 - 0x14];
};

short __stdcall function_1b3820(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1b36e0(long actor_index, s_slot *slot);
void __stdcall function_1c1990(long actor_index, s_slot *slot, long index);
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index);
bool __stdcall function_1b3600(long actor_index, s_slot *slot);
void __stdcall function_1b3880(long actor_index, s_slot *slot);
bool __stdcall function_1b3a80(long actor_index, s_slot *slot);
void __stdcall function_1b3c60(long actor_index, s_slot *slot);
short __stdcall function_1b3f60(long actor_index, s_slot *slot, bool active);
void __stdcall function_1b3fd0(long actor_index, s_slot *slot);
bool __stdcall function_1b4240(long actor_index, s_slot *slot);
short __stdcall function_1b4390(long actor_index, short level, bool active);
short __stdcall function_1b3e20(long actor_index, short level, bool active);
short __stdcall function_1b44d0(long actor_index, s_slot *slot);
short __stdcall function_1b4560(long actor_index, s_slot *slot);

inline void actor_unit_function_20ba60(long actor_index, short type)
{
	long unit_index = actor_get(actor_index)->unknown018;

	if (unit_index != NONE)
		function_20ba60(type, unit_index, NONE, NONE, NONE, NULL);
}

void function_267770(long prop_index, long actor_index);
void *function_1e4e50(long actor_index);

// @retail 0x1b3590
short __stdcall function_1b3590(long actor_index, s_slot *slot, bool active)
{
	s_slot_64 *state = (s_slot_64 *)slot;
	short result = g_46fbe8;

	if (state->prop_index == NONE)
	{
		result = g_46fbe4;
	}
	else if (state->unknown17 && actor_get(actor_index)->unknown504 == 2 || state->unknown18)
	{
		if (--state->unknown10 <= 0)
			result = g_46fbe4;
	}
	else if (--state->unknown12 <= 0)
	{
		result = g_46fbe4;
	}
	return result;
}

// @retail 0x1b3600
bool __stdcall function_1b3600(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_64 *state = (s_slot_64 *)slot;
	bool result = true;

	if (actor->unknown040 && !state->unknown17)
	{
		if (state->unknown2c == NONE)
			return false;
		result = function_1f4460(actor_index, &state->unknown1c, state->unknown2c, NONE, false);
		if (result)
		{
			actor->unknown4cc = 1.0f;
			state->unknown17 = true;
		}
	}
	return result;
}

// @retail 0x1b3670
short __stdcall function_1b3670(long actor_index)
{
	short result = 0;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);

		if (node->type == 6 && !(object_get(node->object_index)->unknownb2 & 1))
			result = 3;
	}
	return result;
}

// @retail 0x1b3820
short __stdcall function_1b3820(long actor_index, s_slot *slot, bool active)
{
	short result = function_1b3590(actor_index, slot, active);

	if (result == g_46fbe8)
	{
		s_slot_64 *state = (s_slot_64 *)slot;

		if (actor_get(actor_index)->prop_index != state->prop_index && state->prop_index != NONE)
			function_267770(state->prop_index, actor_index);
	}
	return result;
}

// @retail 0x1b3880
void __stdcall function_1b3880(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->prop_index != NONE)
	{
		s_prop_node_view *node = (s_prop_node_view *)datum_get_inlined(g_502418, actor->prop_index);
		s_slot_64 *state = (s_slot_64 *)slot;
		real_vector3d delta;

		if (state->unknown1c.output_index == NONE)
		{
			vector3d_from_points3d(&state->unknown1c.point, &actor->position, &delta);
		}
		else
		{
			real_point3d point;

			function_210850(&state->unknown1c, &point);
			vector3d_from_points3d(&point, &actor->position, &delta);
		}
		actor->unknown41c = 3;
		actor->unknown420 = 2;
		actor->unknown430 = 2;
		actor->unknown434 = 2;
		actor->unknown444 = NONE;
		if (magnitude_squared3d(&delta) < 2.25f)
		{
			actor->unknown488 = true;
			actor->unknown4a2 = true;
			if (!state->unknown16 && node)
			{
				function_1fb7e0(actor_index, 0x60, NULL, node->object_index, NONE);
				state->unknown16 = true;
			}
		}
	}
}

// @retail 0x1b39e0
short __stdcall function_1b39e0(long actor_index)
{
	long prop_index = actor_get(actor_index)->first_prop_index;

	while (prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(prop_index);

		prop_index = node->next_index;
		if (node->type == 7 && node->unknown28 < 3.5f && prop_node_state(node)->unknown00 != NONE &&
			!(object_get(node->object_index)->unknownb2 & 1))
		{
			return 3;
		}
	}
	return 0;
}

// @retail 0x1b3e20
short __stdcall function_1b3e20(long actor_index, short level, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = function_1a79e0(actor_index, level, active);

	if (actor->unknown07c != NONE && element_502420_get(actor->unknown07c)->unknown26 != 1 &&
		(actor->slots[level].unknown4 == NONE || actor->slots[level + 1].type == 5))
	{
		result = g_46fbe4;
	}
	return result;
}

// @retail 0x1b3ea0
short __stdcall function_1b3ea0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->unknown07c != NONE && !actor->unknown314.bit0 && !actor->unknown314.bit1)
		result = 3;
	return result;
}

// @retail 0x1b3ee0
bool __stdcall function_1b3ee0(long actor_index, s_slot *slot)
{
	s_slot_66 *state = (s_slot_66 *)slot;
	s_actor_view *actor = actor_get(actor_index);

	state->unknown0c = false;
	state->unknown10 = NONE;
	actor_reset_state(actor_index);
	actor->unknown314.bit0 = true;
	return true;
}

// @retail 0x1b3f60
short __stdcall function_1b3f60(long actor_index, s_slot *slot, bool active)
{
	s_slot_66 *state = (s_slot_66 *)slot;
	short result = g_46fbe8;

	if (state->unknown0c && state->unknown10 == NONE)
		return g_46fbe4;
	if (actor_get(actor_index)->unknown504 == 2)
	{
		if (state->unknown10 != NONE)
			function_1fb7e0(actor_index, 0x63, NULL, actor_get(state->unknown10)->unknown018, NONE);
		return g_46fbe4;
	}
	return result;
}

// @retail 0x1b4240
bool __stdcall function_1b4240(long actor_index, s_slot *slot)
{
	s_slot_64 *state = (s_slot_64 *)slot;
	s_actor_view *actor = actor_get(actor_index);
	real seconds = slot_random_range(10.0f, 15.0f);
	long ticks;

	actor_unit_function_20ba60(actor_index, 0x61);
	state->unknown10 = 0;
	state->unknown16 = false;
	state->prop_index = actor->prop_index;
	state->unknown19 = false;
	seconds = g_510c54->ticks_per_second * seconds;
	__asm
	{
		fld seconds
		fistp ticks
	}
	state->unknown12 = (short)ticks;
	*(dword *)&actor->unknown314 = 0;
	return true;
}

// @retail 0x1b4310
short __stdcall function_1b4310(long actor_index, s_slot *slot, bool active)
{
	s_slot_64 *state = (s_slot_64 *)slot;
	short result = g_46fbe8;

	if (state->unknown19)
		result = 0x63;
	return result;
}

// @retail 0x1b4330
short __stdcall function_1b4330(long actor_index)
{
	short result = 0;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown07c != NONE && actor->unknown227 && element_502420_get(actor->unknown07c)->unknown26 == 1)
		result = 3;
	return result;
}

// @retail 0x1b4480
void __stdcall function_1b4480(long actor_index, s_slot *slot)
{
	s_slot_64 *state = (s_slot_64 *)slot;

	state->unknown2c = NONE;
}

// @retail 0x1b4490
bool __stdcall function_1b4490(long actor_index, s_slot *slot)
{
	s_slot_64 *state = (s_slot_64 *)slot;
	s_actor_view *actor = actor_get(actor_index);

	state->unknown10 = 0;
	state->unknown16 = false;
	state->prop_index = actor->prop_index;
	state->unknown19 = false;
	return true;
}

// @retail 0x1b44d0
short __stdcall function_1b44d0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;

	if (function_1e4e50(actor_index) && actor->prop_index != NONE && prop_node_view(prop_node_get(actor->prop_index)))
		result = 0x23;
	return result;
}

s_slot_handler_2 g_47e210 =
{
	{
		0x65, 2, 0, -2, 0,
		function_1b3670, function_1b3820, function_1b36e0, 0, NONE, {0},
		0, function_1c1990, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1b3600, 0, function_1b3880
};

s_slot_handler_2 g_47e260 =
{
	{
		0x64, 2, 0, -2, 0,
		function_1b39e0, function_1b3590, function_1b3a80, 0, NONE, {0},
		0, function_1c1990, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1b3600, 0, function_1b3c60
};

s_slot_handler_2 g_47e2b0 =
{
	{
		0x66, 2, 0x7ff, -2, 0,
		function_1b3ea0, function_1b3f60, function_1b3ee0, 0, NONE, {0},
		function_1c1520, 0, 0, 0, 0, 0, 0
	},
	function_1b3fd0, 0, slot_proc_nothing
};

/* the children of slot group 0x62 */
s_slot_child g_46f6e4[2] =
{
	{0x65, 0, -2, {0}, -1.0f, 0, 0},
	{5, 0, NONE, {0}, 0.0f, 0, 0},
};

s_slot_handler_1 g_47e300 =
{
	{
		0x62, 1, 0, -2, 0,
		function_1b4330, function_1b4310, function_1b4240, 0, NONE, {0},
		0, function_1c1990, 0, function_1b4480, 0, 0, 0
	},
	function_1b4390, 2, g_46f6e4
};

/* the children of slot group 0x63 */
s_slot_child g_46f710[3] =
{
	{0x64, 0, NONE, {0}, 0.0f, 0, 0},
	{0x66, 0, NONE, {0}, 0.0f, 0, 0},
	{5, 0, NONE, {0}, 0.0f, 0, 0},
};

s_slot_handler_1 g_47e350 =
{
	{
		0x63, 1, 0, -2, 0,
		0, 0, function_1b4490, 0, NONE, {0},
		0, function_1c1990, 0, function_1b4480, 0, 0, 0
	},
	function_1b3e20, 3, g_46f710
};

s_slot_handler_0 g_47e39c =
{
	0x1c, 0, 0, -2, 0, function_1b44d0
};

s_slot_handler_0 g_47e3b0 =
{
	0x1f, 0, 0, -2, 0, function_1b4560
};
