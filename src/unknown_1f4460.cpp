// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F4460.CPP: an actor's movement goal (actor +0x4ac..+0x4e8) */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_20fe20.h"
#include "unknown_1f4460.h"

void function_1f86a0(long index);
bool __stdcall function_1f8a70(long actor_index, long unknown);
void __stdcall function_2628f0(long actor_index, s_reference reference);

// @retail 0x1f4460
bool function_1f4460(long actor_index, s_node_point const *point, long target_index, long unknown, bool unknown2)
{
	s_actor_view *actor = actor_get(actor_index);

	if (!actor->unknown229 && target_index == NONE)
	{
		function_1f86a0(actor_index);
		return false;
	}

	function_2628f0(actor_index, g_470fa0);
	if (actor->unknown4ac == 2 && actor->unknown4c8 == target_index &&
		function_210a30(point, &actor->unknown4b8) <= 0.01f)
	{
		if (actor->unknown040 && !actor->unknown506)
			return function_1f8a70(actor_index, 0);
		return true;
	}

	function_1f86a0(actor_index);
	actor->unknown4b0 = 0.0f;
	actor->unknown4b4 = 0.0f;
	actor->unknown4cc = 0.0f;
	actor->unknown4d0 = 0.0f;
	actor->unknown4e4 = NONE;
	actor->unknown4ac = 0;
	actor->unknown4ae = false;
	actor->unknown4d4 = false;
	actor->unknown4d5 = false;
	actor->unknown4e8 = false;
	actor->unknown4ae = unknown2;
	actor->unknown4ac = 2;
	actor->unknown4b8 = *point;
	actor->unknown4e4 = unknown;
	actor->unknown4c8 = target_index;
	if (actor->unknown229)
		actor->unknown656 = 0;

	return function_1f8a70(actor_index, 0);
}
