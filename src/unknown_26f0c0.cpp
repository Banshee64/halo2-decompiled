// @flags /O2 /Gr
/* UNKNOWN_26F0C0.CPP: iterating the entries of an actor's slot memory
   (actor +0x194) of one type that have not expired */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"

// @retail 0x26f0c0
s_slot_memory_entry *function_26f0c0(s_slot_entry_iterator *iterator)
{
	s_slot_memory_entry *result = NULL;

	if (iterator->reference.unknown0 < 4)
	{
		if (iterator->reference.unknown0 == NONE)
			iterator->reference.unknown0 = 0;
		else
			iterator->reference.unknown0++;

		s_actor_view *actor = actor_get(iterator->actor_index);

		for (; iterator->reference.unknown0 < 4; iterator->reference.unknown0++)
		{
			s_slot_memory_entry *entry = &actor->memory[iterator->reference.unknown0];
			if (entry->type == iterator->reference.unknown2 && entry->time > g_510c54->game_time)
			{
				result = entry;
				break;
			}
		}
	}

	return result;
}
