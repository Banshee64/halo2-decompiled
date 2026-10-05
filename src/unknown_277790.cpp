// @flags /O2 /arch:SSE /Gr
/* Checks whether one actor's type can lead another actor's type. */

#include "unknown_11c920.h"
#include "slot_handler.h"

struct s_actor_type_definition
{
	char const *name;
	short unknown4;
	short leader_type;
};

extern s_actor_type_definition *g_471088[16];

// @retail 0x277790
bool function_277790(long actor_index, long other_actor_index)
{
	short leader_type = g_471088[actor_get(actor_index)->unknown004]->leader_type;
	bool result = false;
	if (leader_type == actor_get(other_actor_index)->unknown004)
		result = true;
	return result;
}
