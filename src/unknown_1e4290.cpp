// @flags /O2 /Gr
/* UNKNOWN_1E4290.CPP: actor flag setters (lane M; called by the behaviors of
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

// @retail 0x1e4650
void function_1e4650(long actor_index, bool value)
{
	s_ai_actor *actor = ai_actor_get(actor_index);

	if (value != actor->unknown225)
	{
		actor->unknown225 = value;
		if (!actor->unknown007)
		{
			s_ai_object *unit = ai_object_get(actor->unit_index);
			dword *flags = &unit->flags134;
			if (value)
				*flags |= 0x20;
			else
				*flags &= ~0x20;
			actor->unknown226 = false;
		}
	}
}
