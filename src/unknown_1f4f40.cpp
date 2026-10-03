// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* the actor's control of its unit */

/* whether the actor's unit can act (function_110ab0) */
// @retail 0x1f50e0
bool function_1f50e0(long actor_index)
{
	long unit_index = actor_get(actor_index)->unknown018;

	if (unit_index != NONE && function_110ab0(unit_index))
		return true;
	return false;
}
