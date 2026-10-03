// @flags /O2 /Gr
/* UNKNOWN_1E1F20.CPP: the weapon an actor uses: that of the unit it rides
   (+0x274) when it fires from it, else that of its own unit (an outside
   function lane I's firing position evaluators call) */

#include "cseries.h"
#include "globals.h"
#include "unknown_1e1f20.h"

/* the actor fields read here (the actor of g_4f55f0, 0x888 bytes) */
struct s_actor_weapon_view
{
	byte unknown000[0x18];
	long unit_index;
	byte unknown01c[0x268 - 0x1c];
	bool unknown268;
	byte unknown269[0x274 - 0x269];
	long unknown274;
	byte unknown278[0x888 - 0x278];
};

// @retail 0x1e1f20
long function_1e1f20(long actor_index)
{
	s_actor_weapon_view *actor = (s_actor_weapon_view *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_actor_weapon_view));
	long weapon_index = NONE;

	if (actor->unknown268 && actor->unknown274 != NONE)
	{
		weapon_index = unit_get_current_weapon(actor->unknown274);
	}
	if (weapon_index == NONE && actor->unit_index != NONE)
	{
		weapon_index = unit_get_current_weapon(actor->unit_index);
	}
	return weapon_index;
}
