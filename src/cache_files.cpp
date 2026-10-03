// @flags /O2 /arch:SSE /Gr
/* CACHE_FILES.CPP: the map cache file: its header (0x800 bytes between 'head'
   and 'foot'), the tag index read from it, tag lookups by group and the
   structure bsp it loads. */

#include "cseries.h"
#include "globals.h"
#include "async.h"
#include <xtl.h>
#include <string.h>

struct s_cache_header
{
	long header_signature;
	long version;
	long size;
	dword unknown0c;
	dword tag_data_offset;
	dword tag_data_size;
	dword unknown18;
	dword unknown1c;
	byte unknown20[0x120];
	short type;
	byte unknown142[0x56];
	char name[32];
	byte unknown1b8[0x644];
	long footer_signature;
};

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

struct s_cache_tags_header
{
	s_cache_tag_group *groups;
	long group_count;
	s_cache_tag_instance *instances;
	long scenario_index;
	long globals_index;
	dword unknown14;
	long instance_count;
	long signature;
};

struct s_structure_bsp_header
{
	dword unknown00;
	void *bsp_address;
	void *lightmap_address;
	long signature;
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

extern byte g_5476f8;
extern long g_55aca8;

s_cache_tags_header *g_547f00;
s_structure_bsp_header *g_547f04;
void *g_5476fc;
s_cache_header g_547700;

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
	g_5476f8 = false;
	g_5476fc = NULL;
	memset(&g_547700, 0, sizeof(g_547700));
	g_4e3b44 = NULL;
	g_547f00 = NULL;
	g_547f04 = NULL;
}

bool cache_file_read(long file_index, dword location, long size, void *buffer);

// @retail 0x122b40
bool cache_files_load_structure_bsp(s_structure_bsp_reference *bsp)
{
	bool result = false;
	long size = bsp->size;

	if (size & 0x1ff)
		size = (size | 0x1ff) + 1;
	if (cache_file_read(NONE, bsp->offset, size, bsp->address) && bsp->address->signature == 'sbsp')
	{
		g_547f04 = bsp->address;
		CACHE_TAG_INSTANCES[(short)bsp->bsp_tag_index].address = g_547f04->bsp_address;
		if (bsp->lightmap_tag_index != NONE)
			CACHE_TAG_INSTANCES[(short)bsp->lightmap_tag_index].address = g_547f04->lightmap_address;
		return true;
	}
	return result;
}

s_cache_tag_group *cache_tag_group_get(long group_tag);

// @retail 0x122c10
void *tag_get(long group_tag, long tag_index)
{
	s_cache_tag_instance *instance = NULL;
	void *result = NULL;

	if ((short)tag_index >= 0 && (short)tag_index < g_547f00->instance_count &&
		CACHE_TAG_INSTANCES[(short)tag_index].datum_index == tag_index)
	{
		instance = &CACHE_TAG_INSTANCES[(short)tag_index];
	}
	if (instance)
	{
		s_cache_tag_group *group = cache_tag_group_get(instance->group_tag);

		if (group->group_tag == group_tag || group->parent_group_tags[0] == group_tag || group->parent_group_tags[1] == group_tag)
			return instance->address;
	}
	return result;
}

// @retail 0x122c70
long function_122c70(s_tag_iterator *iterator)
{
	while (iterator->next_index < g_547f00->instance_count)
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

	if (g_547f00)
	{
		s_cache_tag_group key;
		long index;

		key.group_tag = group_tag;
		key.parent_group_tags[0] = NONE;
		key.parent_group_tags[1] = NONE;
		index = bsearch_elements(&key, g_547f00->groups, g_547f00->group_count, sizeof(s_cache_tag_group), cache_tag_group_compare, NULL);
		if (index != NONE)
			result = &g_547f00->groups[index];
	}
	return result;
}

// @retail 0x122d60
bool cache_file_read(long file_index, dword location, long size, void *buffer)
{
	bool result = false;
	bool volatile done;
	dword bytes_read;

	function_213760(location, size, buffer, &bytes_read, (bool *)&done, 2, 6);
	async_yield_until_done(&done, false);
	if (bytes_read == size)
		result = true;
	return result;
}
