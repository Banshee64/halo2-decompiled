// @flags /O2 /Gr
/* UNKNOWN_03F2B0.CPP: visibility lists, resource prediction and index caches */

#include "unknown_11c920.h"
#include "globals.h"

void *g_509438;

// @retail 0x3f2b0
void function_03f2b0(void)
{
	g_509438 = 0;
}

struct s_visible_index
{
	short index;
	byte unknown02[0x18];
};

struct s_visible_index_list
{
	byte unknown00[0xa6c];
	short count;
	s_visible_index entries[1];
};

class c_entry_list;
struct s_bit_vector_pool_sizes
{
	short unknown0;
	short list_sizes[4];
	short record_count;
};

struct s_bit_vector_pool
{
	void *context;
	byte unknown004[8];
	c_entry_list *lists[4];
	long indices[0x80];
	dword flags[0x10];
	dword pool[0x200];
	word pool_used;
	word entry_count;
	dword entries[0x200][4];
	dword flags2a60;
	byte unknown2a64[0x2a88 - 0x2a64];
	plane3f plane;
	byte unknown2a98[0x2acc - 0x2a98];
	byte *records;
	byte unknown2ad0[4];
	s_bit_vector_pool_sizes sizes;
};
extern s_bit_vector_pool g_547f88;
extern dword g_4c56c0[64];

// @retail 0x3f3f0
void function_3f3f0(void)
{
	s_visible_index_list *list = (s_visible_index_list *)g_547f88.context;
	for (long i = 0; i < list->count; ++i)
	{
		long index = list->entries[i].index;
		g_4c56c0[index >> 5] |= 1 << (index & 31);
	}
}

struct s_index_cache
{
	long index;
	short count;
	short unknown06;
	long values[256];
};

s_index_cache g_4c6b00[8];
long g_50943c;
long g_509440;

// @retail 0x3fd70
void function_3fd70(void)
{
	long j = 0;
	for (long i = 0; i < 16; ++i)
		g_4c6b00[7].values[i] = NONE;
	g_50943c = j;
	for (; j < 8; ++j)
	{
		g_4c6b00[j].index = NONE;
		g_4c6b00[j].count = 0;
	}
	g_509440 = 0;
}

// @retail 0x40de0
long function_40de0(long index, bool first, bool second)
{
	/* Retail receives both boolean arguments on the stack. */
	bool const *first_reference = &first;
	bool const *second_reference = &second;
	long result = NONE;
	if (!index)
		result = 0;
	else if (*first_reference)
		result = 1;
	else if (*second_reference)
		result = 2;
	return result;
}

// @retail 0x40e10
long function_40e10(bool first, bool second)
{
	long result = 3;
	if (first)
		result = second ? 0 : 1;
	else if (second)
		result = 2;
	return result;
}

struct s_predicted_resource;
struct s_predicted_resource_block
{
	long count;
	s_predicted_resource *resources;
};

struct s_cluster_resources
{
	byte unknown00[0x84];
	s_predicted_resource_block resources;
	long link_count;
	short *links;
	byte unknown94[0x1c];
};

struct s_cluster_link
{
	short a;
	short b;
	byte unknown04[0x20];
};

struct s_cluster_resource_map
{
	byte unknown00[0x58];
	dword *visibility;
	byte unknown5c[4];
	s_cluster_link *links;
	byte unknown64[0x38];
	long count;
	s_cluster_resources *clusters;
};

bool function_16e5e0(s_predicted_resource_block const *block, short mode);

// @retail 0x3f450
void function_3f450(long cluster_index)
{
	if (cluster_index != NONE)
	{
		s_cluster_resource_map *map = (s_cluster_resource_map *)g_4e0348;
		dword const *visible = map->visibility + ((map->count + 31) >> 5) * (short)cluster_index;
		function_16e5e0(&map->clusters[cluster_index].resources, 0);
		for (long i = 0; i < map->count; ++i)
		{
			if (visible[i >> 5] & (1 << (i & 31)))
				function_16e5e0(&map->clusters[i].resources, 0);
		}
	}
}

// @retail 0x3f500
void function_3f500(long cluster_index)
{
	if (cluster_index != NONE)
	{
		s_cluster_resource_map *map = (s_cluster_resource_map *)g_4e0348;
		dword const *visible = map->visibility + ((map->count + 31) >> 5) * (short)cluster_index;
		function_16e5e0(&map->clusters[cluster_index].resources, 1);
		for (long i = 0; i < map->count; ++i)
		{
			if (g_4c56c0[i >> 5] & (1 << (i & 31)))
			{
				s_cluster_resources *cluster = &map->clusters[i];
				for (long j = 0; j < cluster->link_count; ++j)
				{
					s_cluster_link *link = &map->links[cluster->links[j]];
					short adjacent = link->a == i ? link->b : link->a;
					if ((visible[adjacent >> 5] & (1 << (adjacent & 31))) &&
						!(g_4c56c0[adjacent >> 5] & (1 << (adjacent & 31))))
					{
						cluster = &map->clusters[adjacent];
						function_16e5e0(&cluster->resources, 1);
					}
				}
			}
		}
	}
}
