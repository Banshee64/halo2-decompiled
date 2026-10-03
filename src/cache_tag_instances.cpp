// @flags /O2 /arch:SSE /Gr
/* CACHE_TAG_INSTANCES.CPP: a tag's instance in the loaded cache file. Retail
   inlines it into every caller (cache_files.cpp's tag_get, 0x122c10), so it
   has no address of its own; it lives in its own file so that LTCG inlines it
   after it is compiled, as retail does. */

#include "cseries.h"
#include "globals.h"
#include "cache_files.h"

struct s_cache_tag_instance
{
	long group_tag;
	long datum_index;
	void *address;
	long size;
};

struct s_cache_tags_header_view
{
	byte unknown00[0x18];
	long instance_count;
};

s_cache_tag_instance *cache_tag_instance_get(long tag_index)
{
	s_cache_tag_instance *result = NULL;

	if ((short)tag_index >= 0 && (short)tag_index < cache_file_globals.tags->instance_count)
	{
		s_cache_tag_instance *instance = &((s_cache_tag_instance *)g_4e3b44)[(short)tag_index];

		if (tag_index == instance->datum_index)
			result = instance;
	}
	return result;
}
