// @flags /O2 /Gr
/* UNKNOWN_1E1F20.CPP: the actor's weapon (lane M; called by the behaviors of
   0x1a8000..0x1affff) */

#include "cseries.h"
#include "ai_actor.h"

struct s_ai_weapon_definition
{
	byte unknown000[0x12f];
	byte flags12f_0 : 1;
};

static inline long unit_get_current_weapon(long unit_index)
{
	s_ai_object *unit = ai_object_get(unit_index);
	short slot = unit->current_weapon;
	long result = NONE;

	if (slot != NONE)
		result = unit->weapons[slot];
	return result;
}

// @retail 0x1e1f20
long actor_get_weapon(long actor_index)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	long result = NONE;

	if (actor->unknown268 && actor->unknown274 != NONE)
		result = unit_get_current_weapon(actor->unknown274);
	if (result == NONE && actor->unit_index != NONE)
		result = unit_get_current_weapon(actor->unit_index);
	return result;
}

// @retail 0x1e2030
bool function_1e2030(long actor_index)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	long weapon_index = actor_get_weapon(actor_index);
	bool result = false;

	if (weapon_index != NONE)
	{
		s_ai_weapon_definition *definition = (s_ai_weapon_definition *)g_4e3b44[ai_object_get(weapon_index)->definition_index & 0xffff].bytes;
		result = !TEST_FIELD_BIT(definition->flags12f_0);
		if (result && actor->unit_index != NONE && (ai_object_get(actor->unit_index)->flags19 & 2))
			result = false;
	}
	return result;
}
