// @flags /O2 /Gr
/* UNKNOWN_12DE70.CPP: geometry blocks streamed from the cache file: a request
   queues a read job for the block, and the block is linked (its fixups
   applied) once the read is done. Decompiled by lane F for the sound
   promotions (0x18c630, 0x18c720). */

#include "cseries.h"
#include <xtl.h>
#include "globals.h"
#include "data_array.h"
#include "geometry_cache.h"
#include "unknown_223b60.h"
#include "physical_memory.h"

/* the cache file's physical memory: blocks of 24 bytes */
extern s_physical_object *g_4e649c;
extern dword g_4e6494;

/* a read in flight (12 bytes) */
struct s_cache_block
{
	short identifier;
	bool done;
	byte priority;
	long job;
	s_geometry_block_info *block;
};

s_data_array *g_4e648c;
long g_4e64a0;
long g_4e64a4;
long g_4e64a8;
bool g_4e64b0;
bool g_468c4c;

long function_213760(dword location, long size, void *buffer, long unknown, bool *done, long type, long priority);
bool function_120ce0(long job, long priority);
void function_125d60(void);

static inline s_physical_object *physical_object_get(void)
{
	return g_4e649c;
}

static inline s_physical_block *physical_block_get(long index)
{
	return &((s_physical_block *)physical_object_get()->blocks->data)[index & 0xffff];
}

// @retail 0x12e3a0
bool function_12e3a0(bool wait, s_geometry_block_info *block, bool urgent, long priority)
{
	bool immediate = wait || urgent;
	long physical_index = function_13d370(g_4e649c, block->block_size, immediate ? 1 : 5);

	if (physical_index == NONE)
	{
		return false;
	}

	dword address = (physical_block_get(physical_index)->offset << physical_object_get()->page_shift) + g_4e6494;
	long cache_index = datum_new_at_index_with_salt(g_4e648c, physical_index);
	s_cache_block *cache_block = &((s_cache_block *)g_4e648c->data)[cache_index & 0xffff];
	long size;

	cache_block->block = block;
	block->cache_block_index = physical_index;
	cache_block->block->runtime_linked = false;
	size = block->block_size;
	if (size & 0x1ff)
	{
		size = (size | 0x1ff) + 1;
	}
	cache_block->job = function_213760(block->block_offset, size, (void *)address, 0, &cache_block->done, 4, priority);
	cache_block->priority = (byte)priority;
	return true;
}

// @retail 0x12de70
bool function_12de70(s_geometry_block_info *block, dword flags)
{
	bool result = false;

	if (block->runtime_linked)
	{
		physical_block_get(block->cache_block_index)->time = physical_object_get()->time;
		return true;
	}

	if (block->section_data_size <= 0 || block->resource_data_size < 0)
	{
		return result;
	}

	bool wait = flags & 1;
	bool load = (flags >> 1) & 1;
	bool optional = (flags >> 2) & 1;
	bool background = (flags >> 3) & 1;
	bool urgent = false;
	bool structure = false;
	dword group_tag = *(dword *)&g_4e3b44[block->owner_tag_index];
	long priority;

	if (group_tag == 'ltmp' || group_tag == 'sbsp')
	{
		structure = true;
	}
	if (g_468c4c)
	{
		wait = false;
	}

	if (g_4e64a0 > 0 && load && !optional && !wait)
	{
		if (g_4e64a4 == 0 || g_4e64a4 == 1 && structure)
		{
			urgent = true;
		}
		if (g_4e64a0 == 1 && urgent)
		{
			wait = true;
		}
	}

	if (urgent)
	{
		priority = 2;
	}
	else if (wait)
	{
		priority = 7;
	}
	else if (background)
	{
		priority = 5;
	}
	else
	{
		priority = !optional + 1;
	}

	if (block->cache_block_index == NONE && load)
	{
		function_12e3a0(wait, block, urgent, priority);
	}

	long physical_index = block->cache_block_index;
	if (physical_index != NONE)
	{
		s_cache_block *cache_block = &((s_cache_block *)g_4e648c->data)[physical_index & 0xffff];

		physical_block_get(physical_index)->time = physical_object_get()->time;
		if (!cache_block->done)
		{
			if (priority > cache_block->priority)
			{
				function_120ce0(cache_block->job, 7);
				cache_block->priority = (byte)priority;
			}
			if (wait && !cache_block->done)
			{
				while (!cache_block->done)
				{
					SwitchToThread();
					function_125d60();
				}
			}
		}
		if (cache_block->done)
		{
			if (!block->runtime_linked)
			{
				fixup_group_apply((s_fixup_group *)block, (byte *)(physical_block_get(block->cache_block_index)->offset << physical_object_get()->page_shift) + g_4e6494);
				block->runtime_linked = true;
			}
			return true;
		}
	}
	else if (!optional && load)
	{
		g_4e64b0 = true;
	}

	if (structure)
	{
		g_4e64a8++;
	}
	return false;
}

/* blocks waiting to be requested (8 bytes each) */
struct s_pending_block
{
	short identifier;
	byte unknown02[2];
	s_geometry_block_info *block;
};

s_data_array *g_4e6490;
bool g_4e6098;
dword g_55e728;

// @retail 0x12dcb0
bool function_12dcb0(s_geometry_block_info *block)
{
	bool result = false;

	if (block && block->section_data_size > 0 && block->resource_data_size >= 0)
	{
		if (block->runtime_linked)
		{
			if (block->cache_block_index != NONE)
			{
				result = true;
				physical_block_get(block->cache_block_index)->time = physical_object_get()->time;
			}
			else
			{
				block->runtime_linked = false;
			}
		}
		else if (g_4e64a0 > 0 && g_4e6098)
		{
			function_12de70(block, 3);
		}
		else if (!(block->flags & 4))
		{
			long pending_index = datum_new(g_4e6490);

			if (pending_index != NONE)
			{
				((s_pending_block *)g_4e6490->data)[pending_index & 0xffff].block = block;
				block->flags |= 4;
			}
			else if (GetTickCount() > g_55e728)
			{
				g_55e728 = GetTickCount() + 30000;
			}
		}
	}
	return result;
}