// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0D0E00.CPP: ending a unit's timed state (an outside function lane
   A's unit script functions need) */

#include "cseries.h"
#include "globals.h"

struct s_unit_0d0e00
{
	byte unknown000[0xd4];
	long index_d4;
	byte unknown0d8[0x134 - 0xd8];
	dword : 3;
	dword flag3 : 1;
	dword flag4 : 1;
	dword : 27;
	byte unknown138[0x2b8 - 0x138];
	real rate;
};

struct s_object_header_0d0e00
{
	byte unknown00[8];
	s_unit_0d0e00 *object;
};

#define UNIT_GET_0D0E00(index) (((s_object_header_0d0e00 *)g_4e0300->data)[(index) & 0xffff].object)

void function_b58c0(long index, dword mask);

// @retail 0xd0e00
void function_d0e00(long unit_index, real rate)
{
	s_unit_0d0e00 *unit = UNIT_GET_0D0E00(unit_index);
	unit->flag3 = false;
	unit->flag4 = false;
	unit->rate = rate * 0.25f;

	long index = UNIT_GET_0D0E00(unit_index)->index_d4;
	if (index != NONE)
		function_b58c0(index, 0x800000);
}
