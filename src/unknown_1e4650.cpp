// @flags /O2 /Gr
/* UNKNOWN_1E4650.CPP: an actor flag setter (lane M; called by the behaviors of
   0x1a8000..0x1affff) */

#include "cseries.h"
#include "ai_actor.h"

// @retail 0x1e4650
void function_1e4650(long actor_index, bool value)
{
	s_actor_view *actor = actor_get(actor_index);

	if (value != actor->unknown225)
	{
		actor->unknown225 = value;
		if (!actor->unknown007)
		{
			s_ai_object *unit = ai_object_get(actor->unknown018);
			dword *flags = &unit->flags134;
			if (value)
				*flags |= 0x20;
			else
				*flags &= ~0x20;
			actor->unknown226 = false;
		}
	}
}
