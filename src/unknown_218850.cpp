// @flags /O2 /Gr
/* UNKNOWN_218850.CPP: the sound cache: requesting, locking and loading the
   cache pages that hold sound data chunks */

#include "cseries.h"
#include <xtl.h>
#include "unknown_218850.h"
#include "physical_memory.h"

s_data_array *g_502104;
dword g_502108;
s_sound_cache_allocator *g_50210c;

/* the cache page allocator is a physical memory allocator (function_13d370 is
   lane F's stub, src/stubs/lane_f.cpp) */
long function_213760(dword location, long size, void *buffer, long unknown, bool *done, long type, long priority);

void function_218a10(s_sound_chunk *chunk, long owner);

// @retail 0x218850
dword function_218850(long owner, s_sound_chunk *chunk, dword flags)
{
	dword result = 0;
	bool wait = (flags & 1) != 0;
	bool lock = ((flags >> 2) & 1) != 0;

	if (chunk->cache_index == NONE && owner != NONE && (flags & 2))
	{
		function_218a10(chunk, owner);
	}

	if (chunk->cache_index != NONE)
	{
		SOUND_CACHE_PAGE(chunk->cache_index)->last_used = g_50210c->time;
		s_sound_cache_entry *entry = SOUND_CACHE_ENTRY(chunk->cache_index);

		if (wait)
		{
			if (!entry->loaded)
			{
				while (!entry->loaded)
				{
					SwitchToThread();
				}
			}
		}

		if (entry->loaded)
		{
			if (!entry->used)
			{
				entry->used = 1;
			}
			result = 2;
		}

		if (lock)
		{
			entry->lock_count++;
			result |= 4;
		}
	}

	if (!(result & 2) && chunk->cache_index != NONE)
	{
		result |= 1;
	}
	else
	{
		result &= ~1;
	}

	return result;
}

// @retail 0x218950
byte *sound_cache_chunk_get_data(s_sound_chunk *chunk)
{
	s_sound_cache_entry *entry = SOUND_CACHE_ENTRY(chunk->cache_index);

	if (entry->reference_count < 0xff)
	{
		entry->reference_count++;
	}

	return (byte *)((SOUND_CACHE_PAGE(chunk->cache_index)->offset << g_50210c->page_shift) + g_502108);
}

// @retail 0x218a10
void function_218a10(s_sound_chunk *chunk, long owner)
{
	long index = function_13d370((s_physical_object *)g_50210c, SOUND_CHUNK_SIZE(chunk), 0);

	if (index != NONE)
	{
		byte *buffer = (byte *)((SOUND_CACHE_PAGE(index)->offset << g_50210c->page_shift) + g_502108);
		datum_new_at_index_with_salt(g_502104, index);
		s_sound_cache_entry *entry = SOUND_CACHE_ENTRY(index);
		chunk->cache_index = index;
		entry->owner = owner;
		entry->chunk = chunk;
		entry->lock_count = 0;
		entry->reference_count = 0;
		entry->used = 0;

		dword size = SOUND_CHUNK_SIZE(chunk);
		if (size & 0x1ff)
		{
			size = (size | 0x1ff) + 1;
		}
		function_213760(chunk->file_offset, size, buffer, 0, (bool *)&entry->loaded, 5, 4);
	}
}
