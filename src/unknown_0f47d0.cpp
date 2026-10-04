// @flags /O2 /Gr
/* UNKNOWN_0F47D0.CPP: starts a vehicle trick on a unit (an outside function
   the vehicle trick event, 0x9fdb0, calls) */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"

void __stdcall function_a9120(long unit_index, long trick);

/* the unit object, as the vehicle trick sees it */
struct s_unit_trick_view
{
	byte unknown000[0x350];
	byte trick;
	byte trick_started;
};

struct s_trick_object_header
{
	byte unknown00[8];
	s_unit_trick_view *object;
};

// @retail 0xf47d0
bool function_f47d0(long unit_index, long trick)
{
	s_unit_trick_view *unit = ((s_trick_object_header *)g_4e0300->data)[unit_index & 0xffff].object;
	unit->trick = (byte)trick;
	unit->trick_started = 0;
	function_a9120(unit_index, trick);
	return true;
}
