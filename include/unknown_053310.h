/* UNKNOWN_053310.H: the physical memory allocation used by the init callbacks
   of the table at 0x453c00  */

#ifndef UNKNOWN_053310_H
#define UNKNOWN_053310_H

#include <xtl.h>
#include "physical_memory_map.h"

/* allocates size bytes from the top of the current physical memory stage
   into result (0 when the stage is full) */
#define PHYSICAL_MEMORY_ALLOCATE(result, size) \
	((result) = (long)physical_memory_malloc_fixed((size), PAGE_READWRITE))

#endif