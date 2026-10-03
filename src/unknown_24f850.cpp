#include "cseries.h"
#include "loop_allocator.h"

// @flags /O2 /Gr

/* The allocation callbacks a decompressor is given (zlib's zalloc and zfree
   shape: an opaque pointer to the loop allocator, then the counts or the
   block). 0x1996c0 and 0x199830 pass their addresses. */

bool loop_allocate(s_loop_allocator *loop, void **pointer, long size, char const *file, long line);

// @retail 0x24f850
void *__stdcall loop_allocator_zalloc(void *opaque, long count, long size)
{
	void *pointer;

	loop_allocate(*(s_loop_allocator **)opaque, &pointer, count * size, NULL, 0);
	return pointer;
}

/* loop_free (0x18e430) inlined */
// @retail 0x24f880
void __stdcall loop_allocator_zfree(void *opaque, void *address)
{
	s_loop_allocator *loop = *(s_loop_allocator **)opaque;
	s_loop_block *block = (s_loop_block *)address - 1;

	loop->free += block->size;
	if (block->previous)
	{
		block->previous->next = block->next;
	}
	else
	{
		loop->first = block->next;
	}
	if (block->next)
	{
		block->next->previous = block->previous;
	}
	else
	{
		loop->last = block->previous;
	}
}
