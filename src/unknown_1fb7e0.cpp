// @flags /O2 /Gr
/* UNKNOWN_1FB7E0.CPP: events of an actor's unit */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "slot_handler.h"
#include "unknown_1fb7e0.h"

struct s_1fb7e0_object
{
	byte unknown000[0x12c];
	long unknown12c;
};

struct s_1fb7e0_object_header
{
	byte unknown00[8];
	s_1fb7e0_object *object;
};

void __stdcall function_1fbac0(long unknown, long unit_index, bool unknown2, long unknown3, s_1fbac0_event *event);

// @retail 0x1fb7e0
bool function_1fb7e0(long actor_index, short type, s_1fb7e0_data const *data, long target_index, long unknown)
{
	long unit_index = actor_get(actor_index)->unknown018;
	bool result = false;

	if (unit_index != NONE)
	{
		if (type != NONE)
			return function_20ba60(type, unit_index, target_index, NONE, unknown, data);

		s_1fb7e0_object *unit = ((s_1fb7e0_object_header *)g_4e0300->data)[unit_index & 0xffff].object;
		s_1fbac0_event event;

		event.unknown00 = 0;
		event.unknown02 = 1;
		event.data = *data;
		function_1fbac0(unit->unknown12c, unit_index, true, 0, &event);
		result = true;
	}

	return result;
}
