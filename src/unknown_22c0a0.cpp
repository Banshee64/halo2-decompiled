// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_22C0A0.CPP: the fixed buffer allocator Havok's memory uses
   (vtable 0x4576a4; unknown_146a20.h): allocations grow up from the buffer's
   start and their table (an offset and a size each) down from its end */

#include "unknown_11c920.h"
#include "unknown_146a20.h"

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

/* what the fixed buffers report: the header's size, the bytes allocated and
   the allocations */
struct s_fixed_memory_statistics
{
	long header_size;
	long allocated_size;
	long unknown08;
	long allocation_count;
	long unknown10;
};

struct s_fixed_memory_statistics_16
{
	long header_size;
	long allocated_size;
	long allocation_count;
	long unknown0c;
};

// @retail 0x22cbd0
void fixed_memory_get_statistics(c_havok_fixed_memory *memory, s_fixed_memory_statistics *statistics)
{
	if (memory)
	{
		statistics->header_size = 0x30;
		statistics->allocation_count = memory->m_count;
		statistics->allocated_size = 0;
		for (long i = 0; i < memory->m_count; i++)
		{
			s_fixed_memory_entry *entries = (s_fixed_memory_entry *)((byte *)memory->m_buffer + memory->m_size);

			statistics->allocated_size += entries[-1 - i].size;
		}
		statistics->unknown10 = 0;
	}
	else
	{
		statistics->header_size = 0;
		statistics->allocated_size = 0;
		statistics->unknown08 = 0;
		statistics->allocation_count = 0;
		statistics->unknown10 = 0;
	}
}

// @retail 0x22cc30
void fixed_memory_get_statistics_16(c_havok_fixed_memory *memory, s_fixed_memory_statistics_16 *statistics)
{
	if (memory)
	{
		statistics->header_size = 0x30;
		statistics->allocation_count = memory->m_count;
		statistics->allocated_size = 0;
		for (long i = 0; i < memory->m_count; i++)
		{
			s_fixed_memory_entry *entries = (s_fixed_memory_entry *)((byte *)memory->m_buffer + memory->m_size);

			statistics->allocated_size += entries[-1 - i].size;
		}
		statistics->allocation_count = memory->m_count;
		statistics->unknown0c = 0;
	}
	else
	{
		statistics->header_size = 0;
		statistics->allocated_size = 0;
		statistics->allocation_count = 0;
		statistics->unknown0c = 0;
	}
}
