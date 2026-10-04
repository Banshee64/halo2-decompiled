// @flags /O2 /Gr
/* PHYSICAL_MEMORY_MAP.CPP: the physical memory map: the game's physical
   memory, from 0x61000 to 0x3145000, and the stack of stages that carve
   it up (g_global_f9ae07, globals.h) */

#include "cseries.h"
#include "physical_memory_map.h"
#include <xtl.h>
#include <string.h>

// @retail 0x12b400
void function_12b400(void)
{
	memset(&g_global_f9ae07, 0, sizeof(g_global_f9ae07));
	g_global_f9ae07.field_0 = 0;
	g_global_f9ae07.base_address = 0x61000;
	g_global_f9ae07.field_8_3 = 0x3145000;
	g_global_f9ae07.field_c_6[0] = 0x61000;
	g_global_f9ae07.field_20[0] = 0x3145000;
	XPhysicalAlloc(0x3145000 - 0x61000, 0x61000, 0, PAGE_READWRITE);
}

