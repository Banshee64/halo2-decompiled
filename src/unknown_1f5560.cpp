// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F5560.CPP: small accessors of an actor's movement state */

#include "cseries.h"
#include "globals.h"
#include "actor_moving.h"
#include "unit_requests.h"
#include <string.h>

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

/* asks the actor's unit (unless it rides a vehicle) to play an animation,
   request 0x19, aimed at a target when one is given */
// @retail 0x1f57f0
bool function_1f57f0(long actor_index, long animation, long const *target)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	if (actor->unknown26c == NONE)
	{
		s_unit_request request;

		memset(&request, 0, sizeof(request));
		request.type = 0x19;
		request.type19.animation = animation;
		if (target)
		{
			request.type19.target[0] = target[0];
			request.type19.target[1] = target[1];
			request.type19.has_target = true;
		}

		switch (animation)
		{
		case 0x9000011:
		case 0x9000012:
		case 0xa000010:
		case 0xa000013:
		case 0xd00002b:
		case 0xe00002a:
			request.type19.mode = 1;
			break;
		default:
			request.type19.mode = 2;
			break;
		}

		result = function_e6900(actor->unit_index, &request);
	}

	return result;
}