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
	s_combat_prop_view *view;

	switch (type)
	{
	case 1:
		result = actor->unknown722 == 1 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)prop_view_get(actor->prop_index)) != 0 && view->state >= 6;
		break;
	case 2:
		result = actor->unknown722 == 0 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)prop_view_get(actor->prop_index)) != 0 && view->state >= 4 &&
			(real)actor->unknown350 * g_510c54->rate >= 2.5f;
		break;
	case 3:
		result = actor->unknown722 == 1 && actor->prop_index != NONE &&
			(view = (s_combat_prop_view *)prop_view_get(actor->prop_index)) != 0 && view->state >= 6 &&
			actor->unknown268;
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
