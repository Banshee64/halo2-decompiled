/* UNKNOWN_053310.H: the physical memory allocation used by the init callbacks
   of the table at 0x453c00  */

#ifndef UNKNOWN_053310_H
#define UNKNOWN_053310_H

#include <xtl.h>
#include "globals.h"

/* allocates aligned_size bytes from the top of the current physical block into
   result (0 when the block is full) */
#define PHYSICAL_MEMORY_ALLOCATE(result, aligned_size) \
	{ \
		long *top_pointer = &g_4e6420.tops[g_4e6420.current_block]; \
		long limit = g_4e6420.lows[g_4e6420.current_block]; \
		long top = *top_pointer - (aligned_size); \
		(result) = 0; \
		if (top >= limit) \
		{ \
			*top_pointer = top; \
			(result) = top; \
			if (top) \
			{ \
				(result) = top | 0x80000000; \
				if (result) \
				{ \
					XPhysicalProtect((void *)(result), (aligned_size), 4); \
				} \
			} \
		} \
	}

#endif