// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_122870.CPP: the map cache file: its header (0x800 bytes between 'head'
   and 'foot'), the tag index read from it, tag lookups by group and the
   structure bsp it loads. */

#include "unknown_11c920.h"
#include "globals.h"
#include "async.h"
#include "unknown_122870.h"
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

long function_13ddd0(const void *key, const void *base, long count, long element_size, long (__stdcall *compare)(const void *, const void *, const void *), const void *context);
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

/* the open cache files (0x804 bytes each, unknown_213760.cpp): a handle and
   the file's header */
struct s_cache_file
{
	HANDLE handle;
	byte unknown04[0x800];
};

extern s_cache_file g_557c90[3];

long cache_file_find(char const *map_name);
bool function_122d60(s_cache_file_location location, long size, void *buffer);
bool __stdcall version_is_compatible(char const *version);

/* set when a map fails to load (the main loop shows the error) */
long g_510a08;
extern bool g_510819;

/* the scenario tag of the loaded map */
extern long g_4686c0;

/* takes size bytes, rounded up to whole pages, from the bottom of the
   current physical memory stage; NULL when it is full */
static __forceinline void *physical_memory_malloc_low(long size, dword protect)
{
	void *result = NULL;
	long stage = g_global_f9ae07.field_0;
	long *bottom = &g_global_f9ae07.field_c_6[stage];
	long address = g_global_f9ae07.field_c_6[stage];
	long aligned_size = (size + 0xfff) & 0xfffff000;
	long top = address + aligned_size;

	if (top <= g_global_f9ae07.field_20[stage])
	{
		*bottom = top;
		result = (void *)address;
		if (address)
		{
			result = (void *)(address | 0x80000000);
			if (result)
				XPhysicalProtect(result, aligned_size, protect);
		}
	}
	return result;
}

static inline long cache_file_sector_align(long size)
{
	if (size & 0x1ff)
		size = (size | 0x1ff) + 1;
	return size;
}

/* loads a map's cache file: its header, then its tag data (the tags header
   and the tag instances) into physical memory; points the tag instances, the
   scenario and the globals at what it read */
// @retail 0x1228f0
bool cache_files_load_map(char const *map_name)
{
	bool result = false;
	long scenario_index = NONE;

	g_55aca8 = cache_file_find(map_name);
	cache_file_globals.header = *(s_cache_header *)g_557c90[g_55aca8].unknown04;
	if (cache_header_verify(&cache_file_globals.header) && version_is_compatible(cache_file_globals.header.build_version))
	{
		cache_file_globals.field_4_7 = physical_memory_malloc_low(cache_file_globals.header.unknown1c, PAGE_READWRITE);
		result = cache_file_globals.field_4_7 != NULL;
		if (result)
		{
			s_cache_file_location location;

			location.file_index = NONE;
			location.offset = cache_file_globals.header.tag_data_offset;
			result = function_122d60(location, cache_file_sector_align(cache_file_globals.header.tag_data_size), cache_file_globals.field_4_7);
			if (!result)
			{
				g_510a08 = 0;
				g_510819 = true;
			}
			else
			{
				location.file_index = NONE;
				location.offset = cache_file_globals.header.tag_data_offset + cache_file_globals.header.tag_data_size;
				result = function_122d60(location, cache_file_sector_align(cache_file_globals.header.unknown18),
					(byte *)cache_file_globals.field_4_7 - cache_file_globals.header.unknown18 + cache_file_globals.header.unknown1c);
				if (result)
				{
					s_cache_tags_header *tags = (s_cache_tags_header *)cache_file_globals.field_4_7;

					if (tags->instances && tags->instance_count > 0 && tags->signature == 'tags')
					{
						cache_file_globals.tags = tags;
						result = true;
						cache_file_globals.loaded = result;
						scenario_index = tags->scenario_index;
						g_4e3b44 = (s_tag_instance *)tags->instances;
					}
					else
					{
						result = false;
					}
				}
				if (!result)
				{
					g_510a08 = 0;
					g_510819 = true;
				}
			}
		}
	}
	if (!result)
	{
		if (cache_file_globals.field_4_7)
			cache_file_globals.field_4_7 = NULL;
		if (g_55aca8 != NONE)
		{
			function_213890();
			g_55aca8 = NONE;
		}
	}
	g_4686c0 = scenario_index;
	if (scenario_index != NONE)
	{
		g_4e0350 = (s_palette_source_globals *)CACHE_TAG_INSTANCES[scenario_index & 0xffff].address;
		g_4e034c = (s_tag_header_globals *)CACHE_TAG_INSTANCES[cache_file_globals.tags->globals_index & 0xffff].address;
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
	cache_file_globals.field_4_7 = NULL;
	memset(&cache_file_globals.header, 0, sizeof(cache_file_globals.header));
	g_4e3b44 = NULL;
	cache_file_globals.tags = NULL;
	cache_file_globals.bsp = NULL;
}

bool function_122d60(s_cache_file_location location, long size, void *buffer);

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
	if (function_122d60(location, size, bsp->address) && bsp->address->signature == 'sbsp')
	{
		cache_file_globals.bsp = bsp->address;
		CACHE_TAG_INSTANCES[(short)bsp->bsp_tag_index].address = cache_file_globals.bsp->bsp_address;
		if (bsp->lightmap_tag_index != NONE)
			CACHE_TAG_INSTANCES[(short)bsp->lightmap_tag_index].address = cache_file_globals.bsp->lightmap_address;
		result = true;
	}
	return result;
}

/* geometry_cache: forgets the streamed blocks of a tag (unknown_12de70.cpp) */
void function_12e150(long tag_index);

static inline void cache_files_unload_tag(long tag_index)
{
	s_cache_tag_instance *instance = &CACHE_TAG_INSTANCES[(short)tag_index];

	function_12e150(tag_index);
	instance->address = NULL;
}

// @retail 0x122bc0
void cache_files_unload_structure_bsp(s_structure_bsp_reference *bsp)
{
	cache_files_unload_tag(bsp->bsp_tag_index);
	if (bsp->lightmap_tag_index != NONE)
	{
		cache_files_unload_tag(bsp->lightmap_tag_index);
	}
	cache_file_globals.bsp = NULL;
}
s_cache_tag_group *cache_tag_group_get(long group_tag);

s_cache_tag_instance *cache_tag_instance_get(long tag_index);

static inline bool cache_tag_group_is(s_cache_tag_group const *group, long group_tag)
{
	return group->group_tag == group_tag || group->parent_group_tags[0] == group_tag || group->parent_group_tags[1] == group_tag;
}

// @retail 0x122c10
void *function_122c10(long group_tag, long tag_index)
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
		long index = function_13ddd0(&key, cache_file_globals.tags->groups, cache_file_globals.tags->group_count, sizeof(s_cache_tag_group), cache_tag_group_compare, NULL);
		if (index != NONE)
			result = &cache_file_globals.tags->groups[index];
	}
	return result;
}

// @retail 0x122d60
bool function_122d60(s_cache_file_location location, long size, void *buffer)
{
	bool result = false;
	bool volatile done;
	dword bytes_read;

	function_213760(location.offset, size, buffer, &bytes_read, (bool *)&done, 2, 6);
	function_120d50(&done, false);
	if (bytes_read == size)
		result = true;
	return result;
}
