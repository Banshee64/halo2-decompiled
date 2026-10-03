// @flags /O2 /Gr
/* PHYSICAL_MEMORY_MAP.CPP: the physical memory map: the game's physical
   memory, from 0x61000 to 0x3145000, and the stack of stages that carve
   it up (physical_memory_globals, globals.h) */

#include "cseries.h"
#include "physical_memory_map.h"
#include <xtl.h>
#include <string.h>

// @retail 0x12b400
void physical_memory_initialize(void)
{
	memset(&physical_memory_globals, 0, sizeof(physical_memory_globals));
	physical_memory_globals.current_stage = 0;
	physical_memory_globals.base_address = 0x61000;
	physical_memory_globals.end_address = 0x3145000;
	physical_memory_globals.low_address[0] = 0x61000;
	physical_memory_globals.high_address[0] = 0x3145000;
	XPhysicalAlloc(0x3145000 - 0x61000, 0x61000, 0, PAGE_READWRITE);
}

