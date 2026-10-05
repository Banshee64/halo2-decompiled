/* GEOMETRY_CACHE.H: a block of tag data streamed from the cache file on
   demand (unknown_12de70.cpp); the sound promotions hold one each */
#ifndef GEOMETRY_CACHE_H
#define GEOMETRY_CACHE_H

#include "unknown_11c920.h"

/* 0x24 bytes; fixup_group_apply (unknown_223b60.h) reads it as s_fixup_group */
struct s_geometry_block_info
{
	long block_offset;
	long block_size;
	long section_data_size;
	long resource_data_size;
	long resource_count;
	void *resources;
	short owner_tag_index;
	byte unknown1a[2];
	short owner_tag_section_offset;
	bool runtime_linked;
	byte flags;
	long cache_block_index;
};

bool function_12de70(s_geometry_block_info *block, dword flags);

#endif
