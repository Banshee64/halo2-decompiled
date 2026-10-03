/* PHYSICAL_MEMORY.H: a physical memory allocator, cut into blocks of pages.
   The tag resource cache (g_4e3b54, src/unknown_123230.cpp and
   src/unknown_123680.cpp) and the geometry cache (g_4e649c,
   src/unknown_12d9f0.cpp and src/unknown_12de70.cpp) each own one; the sound
   cache's page allocator (src/unknown_218850.cpp) is one too. */

#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include "cseries.h"

struct s_data_array;

/* a block (0x18 bytes): its first page, and the allocator's clock when it was
   last used */
struct s_physical_block
{
	byte unknown00[8];
	long offset;
	byte unknown0c[8];
	long time;
};

struct s_physical_object
{
	void method_13d8b0(long pages);

	byte unknown00[0x2c];
	long state;
	byte unknown30[4];
	long page_shift;
	long time;
	byte unknown3c[0x64 - 0x3c];
	s_data_array *blocks;
};

long __stdcall function_13d370(s_physical_object *physical, long size, long type);

#endif
