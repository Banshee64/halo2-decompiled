/* PHYSICAL_MEMORY_MAP.H: the physical memory map (physical_memory_map.cpp):
   allocations from the top of the current stage's heap. Retail inlines these
   everywhere without merging their loads with the caller's. */

#ifndef PHYSICAL_MEMORY_MAP_H
#define PHYSICAL_MEMORY_MAP_H

#include "globals.h"
#include <xtl.h>

void physical_memory_initialize(void);

/* the bytes left in the current stage */
__forceinline long physical_memory_available(void)
{
	return physical_memory_globals.high_address[physical_memory_globals.current_stage] - physical_memory_globals.low_address[physical_memory_globals.current_stage];
}

/* takes size bytes, rounded up to whole pages, from the top of the current
   stage; NULL when it is full */
__forceinline void *physical_memory_malloc_fixed(long size, dword protect)
{
	void *result = NULL;
	long stage = physical_memory_globals.current_stage;
	long limit = physical_memory_globals.low_address[stage];
	long *top = &physical_memory_globals.high_address[stage];
	long aligned_size = (size + 0xfff) & 0xfffff000;
	long address = physical_memory_globals.high_address[stage] - aligned_size;

	if (address >= limit)
	{
		*top = address;
		result = (void *)address;
		if (address)
		{
			result = (void *)(address | 0x80000000);
			if (result)
			{
				XPhysicalProtect(result, aligned_size, protect);
			}
		}
	}
	return result;
}

/* starts a new stage with the bounds of the current one */
__forceinline void physical_memory_stage_push(void)
{
	long stage = physical_memory_globals.current_stage;

	physical_memory_globals.low_address[stage + 1] = physical_memory_globals.low_address[stage];
	physical_memory_globals.high_address[physical_memory_globals.current_stage + 1] = physical_memory_globals.high_address[physical_memory_globals.current_stage];
	physical_memory_globals.current_stage = stage + 1;
}

#endif
