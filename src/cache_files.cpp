// @flags /O2 /arch:SSE /Gr
/* CACHE_FILES.CPP: the map cache file: its header (0x800 bytes between 'head'
   and 'foot'), the tag index read from it, tag lookups by group and the
   structure bsp it loads. */

#include "cseries.h"
#include "globals.h"
#include "async.h"
#include "cache_files.h"
#include <xtl.h>
#include <string.h>

struct s_cache_tag_instance
{
	long group_tag;
	long datum_index;
	void *address;
	long size;
};

struct s_cache_tag_group
{
	long group_tag;
	long parent_group_tags[2];
};

struct s_structure_bsp_reference
{
	dword offset;
	long size;
	s_structure_bsp_header *address;
	byte unknown0c[8];
	long bsp_tag_index;
	byte unknown18[4];
	long lightmap_tag_index;
};

struct s_tag_iterator
{
	long unknown00;
	long unknown04;
	long datum_index;
	long next_index;
	long group_tag;
};

long bsearch_elements(const void *key, const void *base, long count, long element_size, long (__stdcall *compare)(const void *, const void *, const void *), const void *context);
long function_213760(dword location, long size, void *buffer, dword *bytes_read, bool *done, long category, long priority);
void function_213890(void);
long cache_file_get_maximum_size(long type);

extern long g_55aca8;

s_cache_file_globals cache_file_globals;

#define CACHE_TAG_INSTANCES ((s_cache_tag_instance *)g_4e3b44)

// @retail 0x122870
bool cache_header_verify(s_cache_header const *header)
{
	bool result = false;
	FILETIME time;

	GetSystemTimeAsFileTime(&time);
	if (header->header_signature == 'head' &&
		header->footer_signature == 'foot' &&
		header->size >= 0 && header->size <= 0x20800000 &&
		header->type >= 0 && header->type < 5 &&
		header->size <= cache_file_get_maximum_size(header->type) &&
		strlen(header->name) < sizeof(header->name) &&
		header->version == 8)
	{
		result = true;
	}
	return result;
}

// @retail 0x122af0
void cache_files_dispose_map(void)
{
	if (g_55aca8 != NONE)
	{
		function_213890();
		g_55aca8 = NONE;
	}
	cache_file_globals.loaded = false;
	cache_file_globals.tag_data = NULL;
	memset(&cache_file_globals.header, 0, sizeof(cache_file_globals.header));
	g_4e3b44 = NULL;
	cache_file_globals.tags = NULL;
	cache_file_globals.bsp = NULL;
}

bool cache_file_read(s_cache_file_location location, long size, void *buffer);

// @retail 0x122b40
bool cache_files_load_structure_bsp(s_structure_bsp_reference *bsp)
{
	bool result = false;
	long size = bsp->size;

	if (size & 0x1ff)
		size = (size | 0x1ff) + 1;
	s_cache_file_location location;

	location.file_index = NONE;
	location.offset = bsp->offset;
	if (cache_file_read(location, size, bsp->address) && bsp->address->signature == 'sbsp')
	{
		cache_file_globals.bsp = bsp->address;
		CACHE_TAG_INSTANCES[(short)bsp->bsp_tag_index].address = cache_file_globals.bsp->bsp_address;
		if (bsp->lightmap_tag_index != NONE)
			CACHE_TAG_INSTANCES[(short)bsp->lightmap_tag_index].address = cache_file_globals.bsp->lightmap_address;
		result = true;
	}
	return result;
}

s_cache_tag_group *cache_tag_group_get(long group_tag);

static inline s_cache_tag_instance *cache_tag_instance_get(long tag_index)
{
	s_cache_tag_instance *result = NULL;

	if ((short)tag_index >= 0 && (short)tag_index < cache_file_globals.tags->instance_count)
	{
		s_cache_tag_instance *instance = &CACHE_TAG_INSTANCES[(short)tag_index];

		if (tag_index == instance->datum_index)
			result = instance;
	}
	return result;
}

static inline bool cache_tag_group_is(s_cache_tag_group const *group, long group_tag)
{
	return group->group_tag == group_tag || group->parent_group_tags[0] == group_tag || group->parent_group_tags[1] == group_tag;
}

// @retail 0x122c10
void *tag_get(long group_tag, long tag_index)
{
	s_cache_tag_instance *instance = cache_tag_instance_get(tag_index);
	void *result = NULL;

	if (instance && cache_tag_group_is(cache_tag_group_get(instance->group_tag), group_tag))
		return instance->address;
	return result;
}

// @retail 0x122c70
long function_122c70(s_tag_iterator *iterator)
{
	if (iterator->next_index < cache_file_globals.tags->instance_count)
	{
		do
		{
		s_cache_tag_instance *instance = &CACHE_TAG_INSTANCES[iterator->next_index++];

		if (instance && instance->group_tag != NONE && instance->datum_index != NONE)
		{
			if (iterator->group_tag != NONE)
			{
				s_cache_tag_group *group = cache_tag_group_get(instance->group_tag);

				if (iterator->group_tag != group->group_tag &&
					iterator->group_tag != group->parent_group_tags[0] &&
					iterator->group_tag != group->parent_group_tags[1])
				{
					continue;
				}
			}
			iterator->datum_index = instance->datum_index;
			return instance->datum_index;
		}
		}
		while (iterator->next_index < cache_file_globals.tags->instance_count);
	}
	return NONE;
}

// @retail 0x122cf0
long __stdcall cache_tag_group_compare(void const *a, void const *b, void const *context)
{
	return *(long const *)a - *(long const *)b;
}

// @retail 0x122d00
s_cache_tag_group *cache_tag_group_get(long group_tag)
{
	s_cache_tag_group *result = NULL;

	if (cache_file_globals.tags)
	{
		s_cache_tag_group key = { group_tag, NONE, NONE };
		long index = bsearch_elements(&key, cache_file_globals.tags->groups, cache_file_globals.tags->group_count, sizeof(s_cache_tag_group), cache_tag_group_compare, NULL);
		if (index != NONE)
			result = &cache_file_globals.tags->groups[index];
	}
	return result;
}

// @retail 0x122d60
bool cache_file_read(s_cache_file_location location, long size, void *buffer)
{
	bool result = false;
	bool volatile done;
	dword bytes_read;

	function_213760(location.offset, size, buffer, &bytes_read, (bool *)&done, 2, 6);
	async_yield_until_done(&done, false);
	if (bytes_read == size)
		result = true;
	return result;
}
