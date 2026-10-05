// @flags /O2 /Gr
/* UNKNOWN_03F2B0.CPP: visibility lists, resource prediction and index caches */

#include "unknown_11c920.h"
#include "globals.h"
#include "physical_memory.h"
#include <string.h>
#include <xtl.h>

void *g_509438;

// @retail 0x43990
void function_43990(void)
{
	physical_memory_new_frame((s_physical_object *)g_509448);
}

extern D3DResource *g_509444;

// @retail 0x439d0
void *function_439d0(long index)
{
    s_physical_object *manager = (s_physical_object *)g_509448;
    s_physical_block *block = &((s_physical_block *)((s_physical_object *)g_509448)->blocks->data)[index & 0xffff];
    D3DResource *resource = g_509444;
    long offset = block->offset << manager->page_shift;
    block->time = manager->time;
    D3DDevice_FlushVertexCache();
    return (void *)((resource->Data | 0x80000000) + offset);
}

void function_1cdd0(real const *bounds, byte flags);
bool function_13d9f0(long type);

// @retail 0x4d7c0
void function_4d7c0(dword const *source, word const *type)
{
    (void)&type;
    long index = source[1];
    if (index != NONE)
    {
        byte *tag = g_4e3b44[index & 0xffff].bytes;
        if (*(long *)(tag + 0x14) > 0)
        {
            byte *entries = *(byte **)(tag + 0x28);
            long flags = *(word *)(entries + ((source[3] >> 9) & 0x1ff) * 0x5c + 0x1a);
            real const *bounds = *(real **)(tag + 0x18);
            if (function_13d9f0(*type))
                flags |= 1;
            function_1cdd0(bounds, (byte)flags);
            return;
        }
    }
    function_1cdd0(0, 0);
}

struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;
extern byte g_509415;

// @retail 0x335c0
byte function_335c0(void)
{
	if (g_510c50 && ((byte *)g_510c50)[5])
		return g_509415;
	return true;
}

// @retail 0x46220
c_allocator *function_46220(void)
{
	return g_480118;
}

short g_4c1bd4;

struct s_render_object_header
{
	byte unknown00[8];
	long *object;
};

// @retail 0x3d3e0
short function_3d3e0(long object_index, bool cached)
{
	if (cached)
		return g_4c1bd4;
	long tag = ((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object[0];
	long next = *(long *)(g_4e3b44[tag & 0xffff].bytes + 0x38);
	return *(long *)(g_4e3b44[next & 0xffff].bytes + 4) != NONE;
}

struct s_word_bit_iterator
{
	short count;
	short index;
	dword remaining;
	dword const *words;
};

PRIVATE __forceinline long first_set_bit(dword mask)
{
	long result;
	__asm
	{
		mov ecx, -1
		bsf ecx, mask
		mov result, ecx
	}
	return result;
}

// @retail 0x44690
bool function_44690(s_word_bit_iterator *iterator, long *out)
{
	/* Retail keeps the output pointer on the stack. */
	(void)&out;
	while (!iterator->remaining)
	{
		if (++iterator->index >= iterator->count)
			break;
		iterator->remaining = iterator->words[iterator->index];
	}
	if (iterator->index < iterator->count)
	{
		long bit = first_set_bit(iterator->remaining);
		*out = iterator->index * 32 + bit;
		iterator->remaining &= ~(1 << bit);
		return true;
	}
	return false;
}

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

class c_entry_list
{
public:
	c_entry_list(long maximum_count);
	bool add(long a, short b, long c, short d);
	short find(long a);
	void swap(short index0, short index1);

private:
	long maximum_count;
	word count;
	short *shorts_b;
	long *longs_a;
	long *longs_c;
	short *shorts_d;
};
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

struct s_parent_render_object
{
	byte unknown00[0x14];
	long parent;
	byte unknown18[0x92];
	byte attached;
};

// @retail 0x31c20
void function_31c20(s_bit_vector_pool *data, bool alternate, long object_index)
{
	s_bit_vector_pool *const *reference = &data;
	long flags = alternate ? 0x3000 : 0x1000;
	if (object_index != NONE)
	{
		bool again;
		do
		{
			again = false;
			s_parent_render_object *object = (s_parent_render_object *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
			if (object->attached && object->parent != NONE)
			{
				object_index = object->parent;
				again = true;
			}
		} while (again);
		(*reference)->lists[2]->add(object_index, flags, 0, NONE);
	}
}

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

#include "geometry_cache.h"

struct s_geometry_section
{
    byte unknown00[0x38];
    s_geometry_block_info block;
};

struct s_geometry_sections
{
    byte unknown00[0x1c];
    long count;
    byte unknown20[8];
    s_geometry_section *sections;
};

// @retail 0x3e320
bool function_3e320(long tag, byte const *indices)
{
    (void)&indices;
    s_geometry_sections *data = (s_geometry_sections *)g_4e3b44[tag & 0xffff].bytes;
    bool result = true;
    for (long i = 0; i < data->count; ++i)
    {
        long index = indices[i];
        if (index != 255 && !function_12de70(&data->sections[index].block, 3))
            result = false;
    }
    return result;
}

extern byte *g_4858c4;
long g_485a2c, g_485a64, g_485a68;

// @retail 0x34060
long function_34060(long mode, dword flags)
{
    (void)&flags;
    switch (mode)
    {
    case 0: return g_485a2c;
    case 1: return g_485a2c;
    case 2: return *(short *)(g_4858c4 + 0x8a);
    case 3: return *(short *)(g_4858c4 + 0x80);
    case 4: return g_485a64;
    case 5: return g_485a68;
    case 6:
        if (!(*g_4858c4 & 2) && (bool)((g_4ba014 >> 1) & 1) && (flags & 0x2000))
            return 1;
        return 0;
    case 7: return !((*(dword *)g_4858c4 >> 1) & 1);
    default: __assume(0);
    }
}

struct s_bsp3d;
long function_14a280(s_bsp3d *bsp, point3f *point, long index);

struct s_leaf_cluster
{
    short cluster;
    byte unknown02[6];
};

struct s_leaf_cluster_map
{
    byte unknown00[0x18];
    s_bsp3d *bsp;
    byte unknown1c[0x10];
    long leaf_count;
    s_leaf_cluster *leaves;
    byte unknown34[0x68];
    long cluster_count;
};

// @retail 0x2b720
bool function_2b720(point3f *point, long *cluster, long *leaf)
{
    long *const *cluster_reference = &cluster;
    s_leaf_cluster_map *map = (s_leaf_cluster_map *)g_4e0348;
    bool result = false;
    long index = function_14a280(map->bsp, point, 0);
    if (index != NONE)
    {
        *leaf = index;
        **cluster_reference = map->leaves[index].cluster;
        result = true;
    }
    else if (*leaf < 0 || *leaf >= map->leaf_count || **cluster_reference < 0 || **cluster_reference >= map->cluster_count)
    {
        *leaf = NONE;
        **cluster_reference = NONE;
    }
    return result;
}

long function_baf80(long object_index);
long function_155760(long index);
extern long g_4b9ed8;

struct s_inactive_object_header
{
    byte unknown00[3];
    byte inactive;
    byte unknown04[4];
    byte *object;
};

// @retail 0x3e9c0
bool function_3e9c0(long object_index)
{
    bool result = false;
    if (object_index != NONE)
    {
        long index = g_4b9ed8;
        if (index != NONE && !function_155760(index))
        {
            long parent = function_baf80(object_index);
            s_inactive_object_header *header = &((s_inactive_object_header *)g_4e0300->data)[parent & 0xffff];
            if (!header->inactive)
            {
                byte *object = header->object;
                long entry = g_4e8c20->entries[index];
                if (entry == *(long *)(object + 0x13c))
                    result = true;
            }
        }
    }
    return result;
}

s_record_pool *g_4e030c;

struct s_render_entry_110
{
    long unknown00;
    long tag;
    byte unknown08[0x4c - 8];
    long object;
    long unknown50;
    short index;
    byte unknown56[0x110 - 0x56];
};

// @retail 0x31520
bool function_31520(long index)
{
    bool result = false;
    if (index != NONE)
    {
        s_render_entry_110 *entry = &((s_render_entry_110 *)g_4e030c->data)[index & 0xffff];
        byte *tag = g_4e3b44[entry->tag & 0xffff].bytes;
        bool active = false;
        if (entry->index != NONE)
            active = function_3e9c0(entry->object);
        dword flags = *(dword *)tag;
        if ((!(flags & 0x20) || active) && (!(flags & 0x40) || !active))
            result = true;
    }
    return result;
}

// @retail 0x4c0d0
void function_4c0d0(long object_index, byte *output, long mode)
{
    (void)&mode;
    byte *object = (byte *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
    byte *definition = g_4e3b44[*(long *)object & 0xffff].bytes;
    byte *model = g_4e3b44[*(long *)(definition + 0x38) & 0xffff].bytes;
    if ((signed char)object[0xb1] != -1 && mode == 1)
    {
        for (long i = 0; i < *(long *)(model + 0x70); ++i)
        {
            byte *variant = *(byte **)(model + 0x54) + (signed char)object[0xb1] * 0x38;
            signed char index = (signed char)variant[i + 4];
            if (index != -1)
                ((short *)(output + 0xc2))[i] = *(short *)(*(byte **)(variant + 0x18) + index * 20 + 0x10);
            else
                ((short *)(output + 0xc2))[i] = 0;
        }
    }
    else
        memset(output + 0xc2, 0, 0xa0);
}

#include "unknown_123b30.h"

extern double g_4ba040;
void function_3d270(void);
extern dword g_4ba034;
long g_4ba038, g_4ba03c;
long g_4ba054[4], g_4ba064[4];
struct s_fade_record;
extern s_fade_record *g_50942c;
void function_3b8a0(void);
void function_0167a0(void);

// @retail 0x2b4a0
void function_2b4a0(void)
{
    g_4ba034 = 0;
    g_4ba040 = 0.0;
    g_4ba038 = 0;
    g_4ba03c = 0;
    memset(g_4ba054, 0xff, sizeof(g_4ba054));
    memset(g_4ba064, 0xff, sizeof(g_4ba064));
    function_3b8a0();
    function_3d270();
    function_0167a0();
    g_50942c = (s_fade_record *)function_123d40(0, 0, 0x18);
}

extern real g_45dd38, g_45dd44;

// @retail 0x3f220
point3f *function_3f220(dword a, dword b, dword c, point3f *out)
{
    dword const *reference = &c;
    point3f value;
    value.x = (real)a * 8.0f * g_45dd38;
    value.y = (real)b * 8.0f * g_45dd38;
    value.z = (real)*reference * 8.0f * g_45dd44;
    *out = value;
    return out;
}


struct s_2cb30_state
{
	byte unknown00[0x2a78];
	long active;
};

struct s_2cb30_entry
{
	dword values[9];
};

struct s_2cb30_globals
{
	long current;
	long previous;
	s_2cb30_state *state;
	byte unknown0c[0x9b0 - 0xc];
	long count;
	s_2cb30_entry entries[32];
	long entry_count;
};
s_2cb30_globals g_4c0b78;

// @retail 0x2cb30
void function_2cb30(void)
{
	if (!g_4c0b78.state->active)
	{
		s_2cb30_entry empty = {};
		g_4c0b78.previous = 0;
		g_4c0b78.current = 0;
		g_4c0b78.count = 0;
		g_4c0b78.entries[0] = empty;
		// Retail's forward copy propagates the cleared first entry through the array.
		memcpy(g_4c0b78.entries + 1, g_4c0b78.entries, sizeof(g_4c0b78.entries) - sizeof(g_4c0b78.entries[0]));
		g_4c0b78.entry_count = 0;
	}
	else
		g_4c0b78.current = g_4c0b78.previous;
}

struct s_44940_entry
{
	dword unknown00;
	long tag;
	dword unknown08;
	dword flags;
	byte unknown10[0x10];
};

s_44940_entry g_4ba138[850];

// @retail 0x44940
void *function_44940(short index, bool instance)
{
	(void)&instance;
	s_44940_entry *entry = &g_4ba138[index];
	if (entry->tag != NONE)
	{
		byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
		byte *sections = *(byte **)(definition + 0x28);
		return sections + ((entry->flags >> 9) & 0x1ff) * 0x5c + 4;
	}
	byte *structure = (byte *)g_4e0348;
	if (!instance)
		return *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
	byte *instances = *(byte **)(structure + 0x144);
	short definition_index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
	return *(byte **)(structure + 0x13c) + definition_index * 0xc8;
}

// @retail 0x449e0
void *function_449e0(short index, bool load, bool instance)
{
	bool const *instance_reference = &instance;
	s_44940_entry *entry = &g_4ba138[index];
	void *result = NULL;
	if (entry->tag != NONE)
	{
		byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
		byte *section = *(byte **)(definition + 0x28) + ((entry->flags >> 9) & 0x1ff) * 0x5c;
		if (!load || function_12de70((s_geometry_block_info *)(section + 0x38), 3))
			result = *(void **)(section + 0x34);
	}
	else
	{
		byte *structure = (byte *)g_4e0348;
		byte *section;
		if (!*instance_reference)
			section = *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
		else
		{
			byte *instances = *(byte **)(structure + 0x144);
			short definition_index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
			section = *(byte **)(structure + 0x13c) + definition_index * 0xc8;
		}
		if (!load || function_12de70((s_geometry_block_info *)(section + 0x28), 3))
			result = *(void **)(section + 0x50);
	}
	return result;
}

struct s_3d4f0_entry
{
	long tag;
	long object_index;
	long flags;
	transform4x3f transforms[64];
};
s_3d4f0_entry g_4c1bd8[4];

// @retail 0x3d4f0
void function_3d4f0(bool cached, short cache_index, long *flags,
	long *source_object, long *tag, transform4x3f **transforms, dword *count, long object_index)
{
	short const *cache_reference = &cache_index;
	(void)&flags;
	(void)&source_object;
	(void)&tag;
	(void)&transforms;
	dword *const *count_reference = &count;
	if (cached)
	{
		s_3d4f0_entry *entry = &g_4c1bd8[*cache_reference];
		byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
		*tag = entry->tag;
		*transforms = entry->transforms;
		dword n = *(dword *)(definition + 0x48);
		if (n > 64) n = 64;
		**count_reference = n;
		*flags = entry->flags;
		*source_object = entry->object_index;
	}
	else
	{
		byte *object = (byte *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		byte *definition = g_4e3b44[*(long *)object & 0xffff].bytes;
		byte *model = g_4e3b44[*(long *)(definition + 0x38) & 0xffff].bytes;
		long model_tag = *(long *)(model + 4);
		byte *geometry = g_4e3b44[model_tag & 0xffff].bytes;
		*tag = model_tag;
		object = (byte *)((s_render_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		**count_reference = (dword)(long)*(short *)(object + 0x114) / sizeof(transform4x3f);
		*transforms = (transform4x3f *)(object + *(short *)(object + 0x116));
		**count_reference = *(dword *)(geometry + 0x48);
		*flags = 0;
		*source_object = object_index;
	}
}

// @retail 0x460d0
bool function_460d0(dword const *mask, short index, long part_index)
{
	// The render-entry index is passed on the stack in retail.
	short const *index_reference = &index;
	s_44940_entry *entry = &g_4ba138[*index_reference];
	bool result = true;
	if (entry->unknown00 & 0x40)
	{
		byte *data;
		result = false;
		if (entry->tag != NONE)
		{
			byte *definition = g_4e3b44[entry->tag & 0xffff].bytes;
			byte *section = *(byte **)(definition + 0x28) + ((entry->flags >> 9) & 0x1ff) * 0x5c;
			data = *(byte **)(section + 0x34);
		}
		else
		{
			byte *structure = (byte *)g_4e0348;
			byte *section;
			if (!((byte)(entry->unknown00 >> 12) & 1))
				section = *(byte **)(structure + 0xa0) + ((entry->flags >> 9) & 0x1ff) * 0xb0;
			else
			{
				byte *instances = *(byte **)(structure + 0x144);
				short definition_index = *(short *)(instances + ((entry->flags >> 18) & 0x7ff) * 0x58 + 0x34);
				section = *(byte **)(structure + 0x13c) + definition_index * 0xc8;
			}
			data = *(byte **)(section + 0x50);
		}
		byte *part = *(byte **)(data + 4) + part_index * 0x48;
		long first = *(short *)(part + 0xa);
		long count = *(short *)(part + 0xc);
		for (long i = first; i < first + count && !result; ++i)
			result = (mask[i >> 5] & (1 << (i & 31))) != 0;
	}
	return result;
}
