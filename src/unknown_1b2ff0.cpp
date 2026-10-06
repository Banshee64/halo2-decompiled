// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"

/* slot type 0x6d */

short __stdcall function_1b2ff0(long actor_index);
bool __stdcall function_1b3360(long actor_index, s_slot *slot);
void __stdcall function_1b3380(long actor_index, s_slot *slot);

/* iterates the squads of an encounter (function_204ec0 and function_205010) */
struct s_squad_iterator
{
	byte unknown00[0x14];
};

void function_204ec0(s_squad_iterator *iterator, short encounter_index, short a, short b);
short function_205010(s_squad_iterator *iterator);

// @retail 0x1b2ff0
short __stdcall function_1b2ff0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;
	short count = 0;
	s_squad_iterator iterator;

	function_204ec0(&iterator, (short)actor->unknown030, 5, actor->unknown26c != NONE);
	while (function_205010(&iterator) != NONE)
	{
		if (++count >= 2)
			return 1;
	}
	return result;
}

/* the squads slot type 0x6d has tried (a bit per squad) and the one it
   chose */
struct s_squad_choice
{
	dword tried;
	short unknown4;
	short unknown6;
};

struct s_slot_6d
{
	s_slot_header header;
	s_squad_choice choice;
	byte unknown14[0x40 - 0x14];
};

bool __stdcall function_1b3070(long actor_index, s_squad_choice *choice);

// @retail 0x1b3360
bool __stdcall function_1b3360(long actor_index, s_slot *slot)
{
	s_squad_choice *choice = &((s_slot_6d *)slot)->choice;

	choice->tried = 0;
	return function_1b3070(actor_index, choice);
}

// @retail 0x1b3540
void __stdcall function_1b3540(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	actor->unknown4ae = true;
	actor->unknown484 = true;
	if (actor->unknown26c != NONE)
		actor->unknown4b4 = 0.6f;
}

s_slot_handler_2 g_47e1c0 =
{
	{
		0x6d, 2, 0xfff, -2, 0,
		function_1b2ff0, function_1bced0, function_1b3360, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b3380, 0, function_1b3540
};
