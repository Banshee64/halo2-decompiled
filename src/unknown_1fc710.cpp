// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"

struct s_actor_aim_unit
{
	byte unknown000[0x212];
	char weapon_slot;
	byte unknown213[0x218 - 0x213];
	long weapons[4];
};

bool function_101b80(long weapon_index, short barrel_index, point3f *point);
void function_cafc0(long unit_index, point3f *position);

PRIVATE inline bool actor_weapon_position(long unit_index, point3f *position)
{
	s_actor_aim_unit *unit = (s_actor_aim_unit *)object_get(unit_index);
	short slot = unit->weapon_slot;
	bool result = false;

	if (slot != NONE)
	{
		long weapon_index = unit->weapons[slot];
		if (weapon_index != NONE)
			result = function_101b80(weapon_index, 0, position);
	}
	return result;
}

/* Uses a weapon marker when available, otherwise the unit or actor origin. */
// @retail 0x1fc710
void function_1fc710(long actor_index, point3f *position)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown274 != NONE)
	{
		if (!actor_weapon_position(actor->unknown274, position))
			function_cafc0(actor->unknown018, position);
	}
	else if (!actor_weapon_position(actor->unknown018, position))
	{
		*position = *(point3f *)((byte *)actor + 0x22c);
	}
}
