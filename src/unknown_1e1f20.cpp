// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1E1F20.CPP: the weapon an actor holds */

#include "cseries.h"
#include "globals.h"
#include "actor_moving.h"

/* a unit as its inventory sees it */
struct s_inventory_unit
{
	byte unknown000[0x212];
	char current_weapon_index;
	byte unknown213[0x218 - 0x213];
	long weapon_object_indices[4];
};

static inline long unit_get_current_weapon(long unit_index)
{
	s_inventory_unit *unit = (s_inventory_unit *)moving_object_get(unit_index);
	short weapon_index = unit->current_weapon_index;
	long result = NONE;

	if (weapon_index != NONE)
		result = unit->weapon_object_indices[weapon_index];
	return result;
}

/* the weapon of the actor's vehicle seat if it uses one, else its unit's */
// @retail 0x1e1f20
long function_1e1f20(long actor_index)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	long weapon_index = NONE;

	if (actor->unknown268 && actor->unknown274 != NONE)
		weapon_index = unit_get_current_weapon(actor->unknown274);
	if (weapon_index == NONE && actor->unit_index != NONE)
		weapon_index = unit_get_current_weapon(actor->unit_index);
	return weapon_index;
}
