// @flags /O2 /Ob1 /Gr
/* UNKNOWN_1E4290.CPP: an actor flag setter (lane M; called by the behaviors of
   0x1a8000..0x1affff) */

#include "cseries.h"
#include "ai_actor.h"

// @retail 0x1e4290
void function_1e4290(long actor_index, bool value)
{
	s_ai_actor *actor = ai_actor_get(actor_index);

	if (value)
	{
		if (!actor->unknown229 && actor->unknown26c != NONE)
			value = !ai_object_get(actor->unknown26c)->unknown34c;
		if (value)
			actor->flags810 |= 0x800;
	}
	else
	{
		actor->flags810 &= ~0x800;
	}
}
