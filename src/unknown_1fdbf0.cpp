// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FDBF0.CPP: tests of an actor's combat state (+0x6fe, +0x722) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1e3920.h"
#include "unknown_0259d0.h"
#include "slot_handler.h"

/* the prop view (unknown_25d690.cpp) starts with its state */
struct s_type_f95cd3;
s_type_f95cd3 *function_25d700(long index);

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
			(view = (s_combat_prop_view *)function_25d700(actor->prop_index)) != 0 && view->state >= 6;
		break;
	case 2:
		result = actor->unknown722 == 0 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)function_25d700(actor->prop_index)) != 0 && view->state >= 4 &&
			(real)actor->unknown350 * g_510c54->rate >= 2.5f;
		break;
	case 3:
		result = actor->unknown722 == 1 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)function_25d700(actor->prop_index)) != 0 && view->state >= 6 &&
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

struct s_type_67e06b;

// @retail 0x1fe0e0
s_type_67e06b *function_1fe0e0(long actor_index)
{
	s_type_67e06b *result = 0;
	long weapon_index = function_1e1f20(actor_index);

	if (weapon_index != NONE)
	{
		s_moving_object *weapon = moving_object_get(weapon_index);

		result = (s_type_67e06b *)g_4e3b44[weapon->tag_index & 0xffff].bytes;
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
		real seconds = function_259d0(&g_4e7408->unknown0, __FILE__, __LINE__, delay->delay_lower, delay->delay_upper);
		real ticks;
		long result;

		seconds *= function_1e96a0(g_4e6948->state == 1 ? g_4e6948->difficulty : 1, 13);
		ticks = seconds * (real)g_510c54->field_2_3;
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