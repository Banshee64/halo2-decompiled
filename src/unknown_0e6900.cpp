// @flags /O2 /Gr
/* UNKNOWN_0E6900.CPP: sending a request to a unit (an outside function lane A's
   AI script functions need; its callers are in many regions) */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"

/* a unit request type's handlers (0x467564..0x4677b4, 0x10 bytes each, in
   0xe7280..0xec330): the first performs the request */
struct s_unit_request_definition
{
	bool (__stdcall *perform)(long unit_index, s_unit_request *request);
	void *unknown04;
	void *unknown08;
	void *unknown0c;
};

/* the definition of each request type (60 types, in .data) */
s_unit_request_definition *g_4677c8[60];

void function_b7360(long object_index);
void function_1e77c0(long player_index, long type, byte result);

// @retail 0xe6900
bool function_e6900(long unit_index, s_unit_request *request)
{
	s_unit_request_definition *definition = g_4677c8[request->type];
	bool result;

	function_b7360(unit_index);
	result = definition->perform(unit_index, request);

	if (unit_index != NONE)
	{
		s_slot_object_view *unit = object_get(unit_index);

		if (unit->player_index != NONE)
			function_1e77c0(unit->player_index, request->type, result);
	}

	return result;
}
