// @flags /O2 /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 0x6c and slot group 7 */

struct s_slot_6c
{
	s_slot_header header;
	short unknown0c;
	byte unknown0e[0x40 - 0xe];
};

void function_1f86a0(long index);

bool __stdcall function_1bcee0(long actor_index, s_slot *slot);
void __stdcall function_1bcfd0(long actor_index, s_slot *slot);
bool __stdcall function_1bd230(long actor_index, s_slot *slot);
short __stdcall function_1bd350(long actor_index, short level, bool active);

// @retail 0x1bcf90
short __stdcall function_1bcf90(long actor_index, s_slot *slot, bool active)
{
	if (actor_get(actor_index)->unknown504 == 2)
	{
		s_slot_6c *state = (s_slot_6c *)slot;

		function_1f86a0(actor_index);
		state->unknown0c++;
	}
	return g_46fbe8;
}

// @retail 0x1bd1e0
void __stdcall function_1bd1e0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown41c = 4;
	actor->unknown420 = 4;
	actor->unknown424.vector = *g_4687a8;
}

// @retail 0x1bd330
short __stdcall function_1bd330(long actor_index, s_slot *slot, bool active)
{
	s_slot_6c *state = (s_slot_6c *)slot;
	short result = g_46fbe8;

	if (--state->unknown0c <= 0)
		result = g_46fbe4;
	return result;
}

s_slot_handler_2 g_47eba8 =
{
	{
		0x6c, 2, NONE, -2, 0,
		function_1adcd0, function_1bcf90, function_1bcee0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1bcfd0, 0, function_1bd1e0
};

/* the children of slot group 7 */
s_slot_child g_46fba8[1] =
{
	{2, 0, NONE, {0}, 0.0f, 0, 0},
};

s_slot_handler_1 g_47ebf8 =
{
	{
		7, 1, 0, -2, 0,
		function_1adcd0, function_1bd330, function_1bd230, slot_proc_nothing, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1bd350, 1, g_46fba8
};
