// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_22C0A0.CPP: the fixed buffer allocator Havok's memory uses
   (vtable 0x4576a4; havok_memory.h): allocations grow up from the buffer's
   start and their table (an offset and a size each) down from its end */

#include "cseries.h"
#include "havok_memory.h"

struct s_fixed_memory_entry
{
	long offset;
	long size;
};

// @retail 0x22c0a0
void *c_havok_fixed_memory::allocate(long size, long memory_class)
{
	return allocate_aligned(1, size, memory_class);
}

// @retail 0x22c0c0
void c_havok_fixed_memory::deallocate(void *address)
{
	deallocate_aligned(address);
}

/* drops the allocation's entry from the table */
// @retail 0x22c280
void c_havok_fixed_memory::deallocate_aligned(void *address)
{
	for (long i = 0; i < m_count; i++)
	{
		s_fixed_memory_entry *entries = (s_fixed_memory_entry *)((byte *)m_buffer + m_size);

		if ((byte *)m_buffer + entries[-1 - i].offset == address)
		{
			for (; i < m_count - 1; i++)
			{
				entries = (s_fixed_memory_entry *)((byte *)m_buffer + m_size);
				entries[-1 - i] = entries[-2 - i];
			}
			m_count--;
			return;
		}
	}
}

// @retail 0x22c300
void *c_havok_fixed_memory::allocate16(long size, long memory_class)
{
	return allocate_aligned(16, size, memory_class);
}

// @retail 0x22c320
void c_havok_fixed_memory::deallocate16(void *address, long size, long memory_class)
{
	deallocate_aligned(address);
}
