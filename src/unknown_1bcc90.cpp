// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x6b, and the evaluate callback many handlers share */

short __stdcall function_1bcc90(long actor_index);
void __stdcall function_1bcd00(long actor_index, s_slot *slot);

/* iterates the squads of an encounter (function_204ec0 and function_205010) */
struct s_squad_iterator
{
	byte unknown00[0x14];
};

void function_204ec0(s_squad_iterator *iterator, short encounter_index, short a, bool b);
short function_205010(s_squad_iterator *iterator);

// @retail 0x1bcc90
short __stdcall function_1bcc90(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;
	s_squad_iterator iterator;

	function_204ec0(&iterator, (short)actor->unknown030, 5, actor->unknown26c != NONE);
	if (function_205010(&iterator) != NONE)
		result = 1;
	return result;
}

// @retail 0x1bce80
void __stdcall function_1bce80(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown4ae = true;
	actor->unknown484 = true;
	actor->unknown485 = true;
	if (actor->unknown26c != NONE)
		actor->unknown4b4 = 0.6f;
}

// @retail 0x1bced0
short __stdcall function_1bced0(long actor_index, s_slot *slot, bool active)
{
	return g_46fbe8;
}

s_slot_handler_2 g_47eb58 =
{
	{
		0x6b, 2, 0xfff, -2, 0,
		function_1bcc90, function_1bced0, slot_start_true, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1bcd00, 0, function_1bce80
};
