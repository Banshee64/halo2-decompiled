// @flags /O2 /Ob1 /Gr
/* UNKNOWN_1130A0.CPP */

#include "cseries.h"
#include "globals.h"
#include "unknown_107590.h"

/* the units (a local view of the object data) */
struct s_unit_1130a0
{
	byte unknown000[0x134];
	dword unit_flags;
	byte unknown138[0x1f6 - 0x138];
	char index1f6;
	char index1f7;
};

struct s_object_header_1130a0
{
	byte unknown00[8];
	s_unit_1130a0 *object;
};

// @retail 0x1130a0
void function_1130a0(long unit_index, bool flag)
{
	s_unit_1130a0 *unit = ((s_object_header_1130a0 *)g_4e0300->data)[unit_index & 0xffff].object;
	bool enable = flag;
	if (enable && (unit->index1f6 == NONE || unit->index1f7 == NONE))
		enable = false;
	if (enable)
		unit->unit_flags |= 0x1000000;
	else
		unit->unit_flags &= ~0x1000000;
}