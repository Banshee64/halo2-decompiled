// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F5560.CPP: small accessors of an actor's movement state */

#include "cseries.h"
#include "globals.h"
#include "actor_moving.h"

static inline void vector3d_set(real_vector3d *vector, real i, real j, real k)
{
	vector->i = i;
	vector->j = j;
	vector->k = k;
}

/* takes the pending facing (+0x622) unless the actor rides a vehicle */
// @retail 0x1f5560
bool function_1f5560(long actor_index, real_vector3d *facing)
{
	s_actor_moving *actor = actor_moving_get(actor_index);

	if (actor->unknown26c == NONE)
	{
		if (actor->unknown622)
			vector3d_set(facing, actor->unknown624 * actor->unknown62c, actor->unknown628 * actor->unknown62c, actor->unknown630);
		actor->unknown622 = false;
	}
	else
	{
		actor->unknown622 = false;
	}
	return true;
}

// @retail 0x1f9450
void function_1f9450(long actor_index, long value)
{
	s_actor_moving *actor = actor_moving_get(actor_index);

	actor->unknown300 = value;
	actor->unknown304 = g_510c54->ticks_per_second * 5;
}
