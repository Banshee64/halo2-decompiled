// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FDBF0.CPP: tests of an actor's combat state (+0x6fe, +0x722) */

#include "cseries.h"
#include "globals.h"
#include "actor_moving.h"
#include "real_math.h"
#include "slot_handler.h"

/* the prop view (unknown_25d690.cpp) starts with its state */
struct prop_view;
prop_view *prop_view_get(long index);

struct s_combat_prop_view
{
	short state;
};

// @retail 0x1fdbf0
bool function_1fdbf0(long actor_index, short type)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;
	s_combat_prop_view *view;

	switch (type)
	{
	case 1:
		result = actor->unknown722 == 1 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)prop_view_get(actor->prop_index)) != 0 && view->state >= 6;
		break;
	case 2:
		result = actor->unknown722 == 0 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)prop_view_get(actor->prop_index)) != 0 && view->state >= 4 &&
			(real)actor->unknown350 * g_510c54->rate >= 2.5f;
		break;
	case 3:
		result = actor->unknown722 == 1 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)prop_view_get(actor->prop_index)) != 0 && view->state >= 6 &&
			actor->unknown268;
		break;
	}
	return result;
}
// @retail 0x1fdd90
bool function_1fdd90(long actor_index)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	if (actor->unknown722 > 0)
		result = actor->unknown6fe == 2;
	return result;
}

/* the weapon object the actor holds (unknown_1e1f20.cpp) */
long function_1e1f20(long actor_index);

struct weapon_definition;

// @retail 0x1fe0e0
weapon_definition *actor_get_weapon_definition(long actor_index)
{
	weapon_definition *result = 0;
	long weapon_index = function_1e1f20(actor_index);

	if (weapon_index != NONE)
	{
		s_moving_object *weapon = moving_object_get(weapon_index);

		result = (weapon_definition *)g_4e3b44[weapon->tag_index & 0xffff].bytes;
	}
	return result;
}

/* the seconds range of a character's combat behaviour */
struct s_combat_delay
{
	byte unknown00[0x48];
	real delay_lower;
	real delay_upper;
};

real function_1e96a0(short column, short row);

/* starts the actor's combat delay (+0x700, in ticks); false once it is engaged */
// @retail 0x1fee20
bool function_1fee20(long actor_index, s_combat_delay const *delay)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool engaged = actor->unknown48b;

	if (actor->unknown722 == 1 && prop_node_get(actor->unknown724)->unknown24 >= 3)
		engaged = true;
	if (!engaged && delay)
	{
		real seconds = _real_random_range(&g_4e7408->unknown0, __FILE__, __LINE__, delay->delay_lower, delay->delay_upper);
		real ticks;
		long result;

		seconds *= function_1e96a0(g_4e6948->state == 1 ? g_4e6948->difficulty : 1, 13);
		ticks = seconds * (real)g_510c54->ticks_per_second;
		__asm
		{
			fld ticks
			fistp result
		}
		actor->unknown700 = (short)result;
	}
	else
	{
		actor->unknown700 = 0;
	}

	return !engaged;
}