// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1A9B50.CPP: the slot handlers of types 0x42, 0x18, 0x19, 0x10 and
   0x11 (0x47f7f0, 0x47da7c..0x47db48) */

#include "cseries.h"
#include "slot_handler.h"
#include "ai_actor.h"

/* the actor's tag entry function_1e4f90 returns (0x40 bytes) */
struct s_actor_tag_entry
{
	byte flags;
	byte unknown01[3];
	real unknown04;
	real unknown08;
	real unknown0c;
	byte unknown10[0x18 - 0x10];
	real unknown18;
	real unknown1c;
	real unknown20;
	real unknown24;
	byte unknown28[0x34 - 0x28];
	real unknown34;
	byte unknown38[0x40 - 0x38];
};

/* the state of the slots of types 0x10 and 0x11 */
struct s_slot_10
{
	s_slot_header header;
	long time;
	short next_type;
	bool unknown12;
	byte unknown13[0x17 - 0x13];
	bool unknown17;
	bool unknown18;
	byte unknown19;
	short unknown1a;
	s_node_point unknown1c;
	byte unknown2c[0x40 - 0x2c];
};

struct s_prop_datum_view
{
	byte unknown00[0x27];
	char unknown27;
	real unknown28;
	byte unknown2c[0x3c - 0x2c];
};

struct s_prop_state_64
{
	byte unknown00[0x64];
	bool unknown64;
};

real function_259a0(dword *seed);
void *function_1e4f90(long actor_index);
real function_1c9ee0(real fraction);
bool __stdcall function_1aa750(long actor_index, s_slot *slot, bool flag);
short __stdcall function_1a9ec0(long actor_index);
void __stdcall function_1aa0d0(long actor_index, s_slot *slot);
void __stdcall function_1aab50(long actor_index, s_slot *slot);
void __stdcall function_1c04c0(long actor_index, s_slot *slot);

static inline bool actor_function_50c(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	return actor->unknown50c && actor->unknown504 == 1;
}

/* ---- slot type 0x42 ---- */

// @retail 0x1a9b50
short __stdcall function_1a9b50(long actor_index, s_slot *slot)
{
	function_1e4650(actor_index, true);
	return g_46fbe4;
}

/* ---- slot type 0x18 ---- */

// @retail 0x1a9bc0
short __stdcall function_1a9bc0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown07c != NONE)
	{
		long other_index = element_502420_get(actor->unknown07c)->first_actor_index;
		while (other_index != NONE)
		{
			s_actor_view *other = actor_get(other_index);
			other_index = other->next_index;
			if (other != actor && other->unknown004 == actor->unknown004)
				return g_46fbe4;
		}
		function_1e4650(actor_index, true);
	}
	return g_46fbe4;
}

/* ---- slot type 0x19 ---- */

// @retail 0x1a9c50
short __stdcall function_1a9c50(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown358 != 0 && actor->unknown35e)
	{
		function_1e4650(actor_index, true);
		if (!actor->unknown226)
		{
			dword *seed = &g_4e7408->unknown0;
			*seed = 1664525 * *seed + 1013904223;
			if (0.5f > (real)(*seed >> 16) * (1.f / 65535.f))
				actor->unknown226 = true;
		}
	}
	return g_46fbe4;
}

/* ---- slot types 0x10 and 0x11 ---- */

// @retail 0x1a9cf0
short __stdcall function_1a9cf0(long actor_index, s_slot *slot, bool active)
{
	s_slot_10 *state = (s_slot_10 *)slot;
	short type = state->next_type;
	short result = g_46fbe8;

	if (type >= 0 && type < k_slot_type_count)
	{
		s_slot_handler *handler = g_46eeb8[type];
		if (handler->unknown8 != g_46f348 &&
			(handler->mask & g_4ee4ec) == g_4ee4ec &&
			(g_557c40[type >> 5] & (1 << (type & 31))) != 0)
		{
			result = type;
			if (result != g_46fbe8)
				return result;
		}
	}
	if (g_46eeb8[state->header.type]->unknown8 == g_46f348 || state->unknown12)
		result = g_46fbe4;
	return result;
}

void function_1f86a0(long index);
byte function_1f8640(long index);
void __stdcall function_1f4280(long actor_index);
void function_265c30(long prop_index, long actor_index, bool unknown);

/* stops the actor's prop approach once the prop is out of reach */
// @retail 0x1a9d80
void function_1a9d80(long actor_index, real distance, bool *approaching, bool *stopped)
{
	s_actor_view *actor = actor_get(actor_index);

	if (!actor->unknown007 && actor->unknown040)
	{
		*stopped = false;
		if (*approaching)
		{
			function_1f86a0(actor_index);
			if (!function_1f4810(actor_index, actor->prop_index, distance, 1))
			{
				*stopped = true;
				*approaching = false;
				function_1f4280(actor_index);
				actor_get(actor_index)->unknown040 = false;
			}
		}
	}
}

// @retail 0x1a9e00
void function_1a9e00(long actor_index, real distance, bool force)
{
	if (function_1e2030(actor_index))
	{
		s_actor_view *actor = actor_get(actor_index);

		if (!actor->unknown007 && actor->unknown040)
		{
			long prop_index = actor->prop_index;

			if (prop_index != NONE)
			{
				short *view = (short *)prop_view_get(prop_index);

				if (view && *view >= 6)
				{
					bool close = false;

					if (prop_node_get(prop_index)->unknown28 > distance &&
						(force || !function_1f8640(actor_index) || actor->unknown524 > distance))
					{
						close = true;
					}
					function_265c30(prop_index, actor_index, close);
				}
			}
		}
	}
}
// @retail 0x1aa6e0
void __stdcall function_1aa6e0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown41c = 4;
	actor->unknown420 = 2;
	if (actor_function_50c(actor_index))
	{
		actor->unknown3e0 = 0;
		actor->unknown44d = true;
	}
	else
	{
		actor->unknown3e0 = 0;
	}
}

// @retail 0x1aa990
bool __stdcall function_1aa990(long actor_index, s_slot *slot)
{
	bool result = false;

	if (!actor_get(actor_index)->unknown264)
	{
		result = function_1aa750(actor_index, slot, false);
		if (result)
		{
			s_actor_tag_entry *entry = (s_actor_tag_entry *)function_1e4f90(actor_index);
			if (entry && (entry->flags & 2))
				function_1e4650(actor_index, true);
		}
	}
	return result;
}

// @retail 0x1aa9f0
short __stdcall function_1aa9f0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	s_ai_object *unit = ai_object_get(actor->unknown018);
	s_actor_tag_entry *entry = (s_actor_tag_entry *)function_1e4f90(actor_index);
	short result = 0;

	if (!(unit->flags19 & 1) && entry && actor->prop_index != NONE &&
		entry->unknown24 >= g_45dbd8 && !actor->unknown229 &&
		(actor->times[4] == NONE || (real)(g_510c54->game_time - actor->times[4]) * g_510c54->rate > entry->unknown34) &&
		!function_110ab0(actor->unknown018))
	{
		s_prop_datum_view *prop = (s_prop_datum_view *)prop_node_get(actor->prop_index);
		s_prop_state_64 *state = (s_prop_state_64 *)prop_state_get((s_prop_datum *)prop);
		if (prop->unknown27 >= 1)
		{
			bool flag = state->unknown64;
			if ((flag || prop->unknown28 > entry->unknown1c) && entry->unknown20 > prop->unknown28)
			{
				if (flag && !function_1e2030(actor_index))
					return 4;
				if (function_1c9ee0(entry->unknown24) > function_259a0(&g_4e7408->unknown0))
					result = 3;
			}
		}
	}
	return result;
}

// @retail 0x1ab2c0
void __stdcall function_1ab2c0(long actor_index, s_slot *slot)
{
	s_slot_10 *state = (s_slot_10 *)slot;

	if (state->unknown17 && !state->unknown12 && !actor_get(actor_index)->unknown264)
		state->unknown1a++;
}

// @retail 0x1ab300
void __stdcall function_1ab300(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_10 *state = (s_slot_10 *)slot;

	function_1aa6e0(actor_index, slot);
	if (state->unknown18)
	{
		actor->unknown464 = true;
		actor->unknown465 = true;
		actor->unknown466 = true;
		actor->unknown468 = state->unknown1c;
		state->unknown17 = true;
		state->unknown18 = false;
		state->unknown1a = 0;
	}
}

// @retail 0x1ab380
bool __stdcall function_1ab380(long actor_index, s_slot *slot)
{
	return function_1aa750(actor_index, slot, true);
}

// @retail 0x1ab3a0
void __stdcall function_1ab3a0(long actor_index, s_slot *slot)
{
	actor_get(actor_index)->times[4] = g_510c54->game_time;
}

/* ---- the handlers ---- */

s_slot_handler_0 g_47f7f0 =
{
	0x42, 0, 0, -2, 0, function_1a9b50
};

s_slot_handler_0 g_47da7c =
{
	0x18, 0, 0x7ff, -2, 0, function_1a9bc0
};

s_slot_handler_0 g_47da90 =
{
	0x19, 0, 0x7ff, -2, 0, function_1a9c50
};

s_slot_handler_2 g_47daa8 =
{
	{
		0x10, 2, 0, -2, 0,
		function_1a9ec0, function_1a9cf0, function_1aa990, function_1c04c0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1aa0d0, 0, function_1aa6e0
};

s_slot_handler_2 g_47daf8 =
{
	{
		0x11, 2, 0, -2, 0,
		function_1aa9f0, function_1a9cf0, function_1ab380, function_1ab3a0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1aab50, function_1ab2c0, function_1ab300
};
