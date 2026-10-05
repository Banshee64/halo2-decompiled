// @flags /O2 /Gr
/* UNKNOWN_E70E0.CPP: a unit's current zoom level */

#include "unknown_11c920.h"
#include "globals.h"

struct s_zoom_unit
{
	byte unknown00[0xc];
	char zoom_levels[0x346 - 0xc];
	short zoom_index;
};

struct s_zoom_object_header
{
	byte unknown00[8];
	s_zoom_unit *object;
};

// @retail 0xe70e0
long function_e70e0(long unit_index)
{
	s_zoom_unit *unit = ((s_zoom_object_header *)g_4e0300->data)[unit_index & 0xffff].object;
	return unit->zoom_levels[unit->zoom_index];
}
