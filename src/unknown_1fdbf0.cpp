// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FDBF0.CPP: tests of an actor's combat state (+0x6fe, +0x722) */

#include "cseries.h"
#include "globals.h"
#include "actor_moving.h"

/* the prop view (unknown_25d690.cpp) starts with its state */
struct prop_view;
prop_view *prop_view_get(long index);

struct s_combat_prop_view
{
	short state;
};

// @retail 0x1fdbf0
bool function_1fdbf0(long actor_index, short type)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	switch (type)
	{
	case 1:
		if (actor->unknown722 == 1 && actor->prop_index != NONE)
		{
			s_combat_prop_view *view = (s_combat_prop_view *)prop_view_get(actor->prop_index);

			if (view && view->state >= 6)
				result = true;
		}
		break;
	case 2:
		if (actor->unknown722 == 0 && actor->prop_index != NONE)
		{
			s_combat_prop_view *view = (s_combat_prop_view *)prop_view_get(actor->prop_index);

			if (view && view->state >= 4 && (real)actor->unknown350 * g_510c54->rate >= 2.5f)
				result = true;
		}
		break;
	case 3:
		if (actor->unknown722 == 1 && actor->prop_index != NONE)
		{
			s_combat_prop_view *view = (s_combat_prop_view *)prop_view_get(actor->prop_index);

			if (view && view->state >= 6 && actor->unknown268)
				result = true;
		}
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
