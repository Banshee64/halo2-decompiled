// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1AB3D0.CPP: the slot handlers of types 0x53 and 0x52
   (0x47db48..0x47dbe8) */

#include "cseries.h"
#include "slot_handler.h"
#include "ai_actor.h"

/* the state of a slot of type 0x53 */
struct s_slot_53
{
	s_slot_header header;
	short ticks;
	byte unknown0e[2];
	real_vector3d direction;
	byte unknown1c[4];
	short timer;
	byte unknown22[0x40 - 0x22];
};

struct s_prop_datum_53
{
	byte unknown00[0x24];
	short unknown24;
	byte unknown26;
	char unknown27;
	real unknown28;
	byte unknown2c[0x3c - 0x2c];
};

struct s_prop_state_53
{
	byte unknown00[4];
	real_point3d position;
	byte unknown10[0x1c - 0x10];
	real_vector3d velocity;
	byte unknown28[0x3c - 0x28];
	long object_index;
	byte unknown40[0x64 - 0x40];
	bool unknown64;
};

struct s_definition_244
{
	byte unknown000[0x244];
	short unknown244;
};

struct s_weapon_entry
{
	byte flags00_0 : 1;
	byte unknown01[0x7a - 0x1];
	short unknown7a;
};

struct s_tag_element_53
{
	byte unknown00[0x8c];
	real unknown8c;
	real unknown90;
};

long function_1469f0(real seconds);
void __stdcall function_1f4280(long actor_index);
real function_30bf0(real_vector3d *v);
void __stdcall function_1ab770(long actor_index, s_slot *slot);
short __stdcall function_1abbc0(long actor_index);
short __stdcall function_1abda0(long actor_index, s_slot *slot, bool active);

#define object_definition_244(object_index) \
	((s_definition_244 *)g_4e3b44[ai_object_get(object_index)->definition_index & 0xffff].bytes)

/* ---- slot type 0x53 ---- */

// @retail 0x1ab3d0
short __stdcall function_1ab3d0(long actor_index)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	short result = 0;

	if (!actor->unknown229 && actor->prop_index != NONE)
	{
		s_prop_datum_53 *prop = (s_prop_datum_53 *)prop_node_get(actor->prop_index);
		if (prop->unknown24 >= 1 && prop->unknown24 <= 2)
		{
			s_prop_state_53 *state = (s_prop_state_53 *)prop_state_get((s_prop_datum *)prop);
			long object_index = state->object_index;
			if (object_index == NONE ||
				object_definition_244(actor->unknown26c)->unknown244 >= object_definition_244(object_index)->unknown244)
			{
				result = 3;
			}
		}
	}
	return result;
}

// @retail 0x1ab4b0
bool __stdcall function_1ab4b0(long actor_index, s_slot *slot)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	bool result = false;

	if (!actor->unknown229 && actor->prop_index != NONE)
	{
		s_prop_datum_53 *prop = (s_prop_datum_53 *)prop_node_get(actor->prop_index);
		if (prop->unknown24 >= 1 && prop->unknown24 <= 2)
		{
			s_prop_state_53 *state = (s_prop_state_53 *)prop_state_get((s_prop_datum *)prop);
			s_slot_53 *slot_state = (s_slot_53 *)slot;
			real_vector3d *direction = &slot_state->direction;

			direction->i = (state->velocity.i * 0.5f + state->position.x) - actor->position.x;
			direction->j = (state->velocity.j * 0.5f + state->position.y) - actor->position.y;
			direction->k = (state->velocity.k * 0.5f + state->position.z) - actor->position.z;
			if (0.0f > actor->unknown290.k * direction->k + actor->unknown290.j * direction->j + direction->i * actor->unknown290.i)
			{
				direction->i = state->position.x - actor->position.x;
				direction->j = state->position.y - actor->position.y;
				direction->k = state->position.z - actor->position.z;
			}
			direction->k = 0.0f;
			if (function_30bf0(direction) > g_45dbd8)
			{
				s_tag_element_53 *element = (s_tag_element_53 *)function_1e5450(actor_index, ai_object_get(actor->unknown26c)->definition_index);
				short ticks;
				if (!element || (ticks = (short)function_1469f0(element->unknown8c)) <= 0)
				{
					real seconds = (real)g_510c54->ticks_per_second * 2.5f;
					long rounded;
					__asm
					{
						fld seconds
						fistp rounded
					}
					ticks = (short)rounded;
				}
				slot_state->ticks = ticks;
				slot_state->timer = NONE;
				return true;
			}
		}
	}
	return result;
}

// @retail 0x1ab690
short __stdcall function_1ab690(long actor_index, s_slot *slot, bool active)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	s_slot_53 *state = (s_slot_53 *)slot;
	short result = g_46fbe8;

	if (state->timer == NONE)
	{
		if (actor->prop_index != NONE && state->ticks > 0)
			return result;

		s_tag_element_53 *element = (s_tag_element_53 *)function_1e5450(actor_index, ai_object_get(actor->unknown26c)->definition_index);
		if (element)
		{
			real seconds = (real)g_510c54->ticks_per_second * element->unknown90;
			long rounded;
			__asm
			{
				fld seconds
				fistp rounded
			}
			state->timer = (short)rounded;
		}
		else
		{
			state->timer = g_510c54->ticks_per_second * 2;
		}
		if (state->timer != 0)
			return result;
		return g_46fbe4;
	}
	state->timer--;
	if (state->timer > 0)
		return result;
	return g_46fbe4;
}

// @retail 0x1ab880
void __stdcall function_1ab880(long actor_index, s_slot *slot)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	s_slot_53 *state = (s_slot_53 *)slot;

	if (state->timer == NONE)
	{
		actor->unknown456 = true;
		actor->unknown458.i = state->direction.i * 20.0f;
		actor->unknown458.j = state->direction.j * 20.0f;
		actor->unknown458.k = state->direction.k * 20.0f;
		state->ticks--;
		function_1e4290(actor_index, true);
		actor->unknown41c = 4;
		actor->unknown420 = 0;
	}
	else
	{
		function_1f4280(actor_index);
		actor->unknown41c = 4;
		actor->unknown420 = 4;
		actor->unknown424 = actor->unknown290;
	}
}

/* ---- slot type 0x52 ---- */

// @retail 0x1abcf0
bool __stdcall function_1abcf0(long actor_index, s_slot *slot)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	bool result = false;
	long weapon_index = actor_get_weapon(actor_index);

	if (weapon_index != NONE)
	{
		s_weapon_entry *entry = (s_weapon_entry *)function_1e5280(actor_index, ai_object_get(weapon_index)->definition_index);
		if (entry && entry->unknown7a == 3 && actor->unknown70a > 0)
			actor->unknown70a--;
	}
	if (actor->prop_index != NONE)
		result = true;
	return result;
}

// @retail 0x1abd70
void __stdcall function_1abd70(long actor_index, s_slot *slot)
{
	ai_actor_get(actor_index)->unknown3e8 = g_510c54->game_time;
}

// @retail 0x1abfa0
bool __stdcall function_1abfa0(long actor_index, s_slot *slot)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	bool result = true;

	if (actor->prop_index == NONE)
	{
		result = false;
	}
	else if (actor->unknown040 && !function_1f4810(actor_index, actor->prop_index, 0.0f, 1))
	{
		function_1f4280(actor_index);
		ai_actor_get(actor_index)->unknown040 = false;
		result = false;
	}
	return result;
}

// @retail 0x1ac010
void __stdcall function_1ac010(long actor_index, s_slot *slot)
{
	s_ai_actor *actor = ai_actor_get(actor_index);

	actor->unknown41c = 4;
	actor->unknown420 = 2;
	actor->unknown44d = true;
	actor->unknown488 = true;
	long weapon_index = actor_get_weapon(actor_index);
	if (weapon_index != NONE)
	{
		s_weapon_entry *entry = (s_weapon_entry *)function_1e5280(actor_index, ai_object_get(weapon_index)->definition_index);
		if (entry)
			actor->unknown488 = !TEST_FIELD_BIT(entry->flags00_0);
	}
}

/* ---- the handlers ---- */

s_slot_handler_2 g_47db48 =
{
	{
		0x53, 2, 0, -2, 0,
		function_1ab3d0, function_1ab690, function_1ab4b0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1ab770, 0, function_1ab880
};

s_slot_handler_2 g_47db98 =
{
	{
		0x52, 2, 0, -2, 0,
		function_1abbc0, function_1abda0, function_1abcf0, function_1abd70, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1abfa0, 0, function_1ac010
};
