// @flags /O2 /Gr
/* UNKNOWN_0B68C0.CPP: function_b68c0 (entry 27, dispose) */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_0b68c0.h"
#include <string.h>

struct s_callback_entry
{
	void (*callback)(void);
	byte unknown04[0x34];
};

struct s_data_header_40
{
	byte unknown00[0x24];
	c_data_allocator *allocator;
	byte unknown28[0x18];
};

s_callback_entry g_4674ac[3];
s_callback_node *g_4e0330;
void *g_4e0310;
void *g_4e0314;
void *g_4e0318;
bool g_4de2f0;
s_data_header_40 *g_4de2ec;
void *g_4de2e0;
void *g_4de2e4;
void *g_4de2e8;
void *g_4de2d4;
void *g_4de2d8;
void *g_4de2dc;

// @retail 0xb68c0
void function_b68c0(void)
{
	s_callback_entry *entry = g_4674ac;
	long count = 3;
	do
	{
		if (entry->callback)
		{
			entry->callback();
		}
		entry++;
		count--;
	} while (count);

	for (s_callback_node *node = g_4e0330; node; node = node->next)
	{
		if (node->callback)
		{
			node->callback();
		}
	}

	if (g_4e0310)
	{
		g_4e0310 = 0;
	}
	if (g_4e0318)
	{
		g_4e0318 = 0;
	}
	if (g_4e0314)
	{
		g_4e0314 = 0;
	}

	if (g_4de2f0)
	{
		s_data_header_40 *data = g_4de2ec;
		c_data_allocator *allocator = data->allocator;

		memset(data, 0, sizeof(*data));
		allocator->deallocate(data);

		data_dispose(g_4e0300);
	}

	g_4e0300 = 0;
	g_4de2ec = 0;
	if (g_4de2e0)
	{
		g_4de2e0 = 0;
	}
	if (g_4de2e8)
	{
		g_4de2e8 = 0;
	}
	if (g_4de2e4)
	{
		g_4de2e4 = 0;
	}
	if (g_4de2d4)
	{
		g_4de2d4 = 0;
	}
	if (g_4de2dc)
	{
		g_4de2dc = 0;
	}
	if (g_4de2d8)
	{
		g_4de2d8 = 0;
	}
}


struct s_object_visibility_header_ab
{
    short salt;
    byte flags;
    byte type;
    volatile short cluster;
    byte unknown06[2];
    byte *object;
};
bool function_108d30(long object_index, long a, long b);

static __forceinline bool object_cluster_contains_ab(dword const *clusters, long index)
{
    long word_index = index >> 5;
    dword mask = 1 << (index & 31);
    dword flags = clusters[word_index];
    bool result = (flags & mask) != 0;
    return result;
}

// @retail 0xb6d60
bool function_b6d60(long object_index, dword const *clusters)
{
    s_object_visibility_header_ab *header = &((s_object_visibility_header_ab *)g_4e0300->data)[object_index & 0xffff];
    byte *object = header->object;
    bool result = false;
    if ((bool)((*(dword *)(object + 4) >> 1) & 1))
        return true;
    if (((1 << header->type) & 0x80) && function_108d30(object_index, (long)clusters, (long)&result))
        return result;
    if (header->cluster != NONE)
        return object_cluster_contains_ab(clusters, header->cluster);
    return result;
}


void function_d4890(void);
void function_d48d0(void);
void function_c0040(void);
void __stdcall function_bc300(long object_index);
extern long *g_4de2d0;
extern s_record_pool *g_4e030c;
struct s_object_list;
extern s_object_list *g_4de2f4;

// @retail 0xb69d0
void function_b69d0(void)
{
    function_d4890();
    for (s_callback_node *node = g_4e0330; node; node = node->next)
    {
        void (*callback)(void) = *(void (**)(void))((byte *)node + 0x18);
        if (callback) callback();
    }
    function_c0040();
    s_record_pool *pool = g_4e0300;
    pool->valid = true;
    record_pool_release_all(pool);
    memset(g_4de2d0, 0xff, 0x280 * sizeof(long));
    memset(g_4de2e0, 0xff, 0x200 * sizeof(long));
    pool = (s_record_pool *)g_4de2e8;
    pool->valid = true;
    record_pool_release_all(pool);
    pool = (s_record_pool *)g_4de2e4;
    pool->valid = true;
    record_pool_release_all(pool);
    memset(g_4de2d4, 0xff, 0x200 * sizeof(long));
    pool = (s_record_pool *)g_4de2dc;
    pool->valid = true;
    record_pool_release_all(pool);
    pool = (s_record_pool *)g_4de2d8;
    pool->valid = true;
    record_pool_release_all(pool);
    byte *state = (byte *)g_4de2f4;
    state[3] = 0;
    *(short *)(state + 4) = 0;
    *(long *)(state + 0xc) = 0;
    *(long *)(state + 0x10) = 0;
    *(long *)(state + 0x14) = 0;
    state[0x81] = 0;
    state[0x80] = 0;
    *(long *)(state + 8) = NONE;
}

static inline void pool_invalidate_ab(void *data)
{
    s_record_pool *pool = (s_record_pool *)data;
    if (pool->valid) pool->valid = false;
}

// @retail 0xb6ab0
void function_b6ab0(void)
{
    function_d48d0();
    for (s_callback_node *node = g_4e0330; node; node = node->next)
    {
        void (*callback)(void) = *(void (**)(void))((byte *)node + 0x1c);
        if (callback) callback();
    }
    g_4e030c->valid = false;
    pool_invalidate_ab(g_4e0318);
    pool_invalidate_ab(g_4e0314);
    s_record_pool *pool = g_4e0300;
    if (pool->valid)
    {
        long index = data_datum_index(pool, function_16bc00(pool, 0));
        while (index != NONE)
        {
            function_bc300(index);
            long next = index == NONE ? 0 : (index & 0xffff) + 1;
            index = data_datum_index(pool, function_16bc00(pool, next));
        }
        pool->valid = false;
    }
    pool_invalidate_ab(g_4de2e8);
    pool_invalidate_ab(g_4de2e4);
    pool_invalidate_ab(g_4de2dc);
    pool_invalidate_ab(g_4de2d8);
}
