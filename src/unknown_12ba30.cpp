// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "main_globals.h"
void function_123f60(dword arg_0);

// @retail 0x12ba30
void function_12ba30(void)
{
	dword local_0 = main_globals.unknown74 != 0;
	if (main_globals.unknown75) local_0 |= 2;
	else local_0 &= ~2;
	if (main_globals.switch_structure_bsp)
	{
		main_globals.switch_structure_bsp = false;
		main_globals.field_0_3 = NONE;
	}
	function_123f60(local_0);
	main_globals.unknown74 = false;
	main_globals.unknown75 = false;
	main_globals.unknown6f = false;
}
