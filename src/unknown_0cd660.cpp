// @flags /O2 /Ob1 /Gr
/* UNKNOWN_0CD660.CPP: whether a unit holds weapons in both of its slots.
   Moved out of unknown_0cbd50.cpp (lane T) into its own /Ob1 file: retail
   calls it out of line from 0x101690. */

#include "unknown_11c920.h"
#include "globals.h"

/* the unit fields read here */
struct s_unit_slots_view
{
	byte unknown000[0x212];
	char weapon_slots[2];
};

struct s_unit_slots_header
{
	byte unknown00[8];
	s_unit_slots_view *unit;
};

// @retail 0xcd660
bool function_cd660(long unit_index)
{
	s_unit_slots_view *unit = ((s_unit_slots_header *)g_4e0300->data)[unit_index & 0xffff].unit;
	bool result = false;

	if (unit->weapon_slots[0] != NONE && unit->weapon_slots[1] != NONE)
	{
		result = true;
	}
	return result;
}
