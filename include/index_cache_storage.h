#ifndef INDEX_CACHE_STORAGE_H
#define INDEX_CACHE_STORAGE_H

#include "unknown_11c920.h"

struct s_44940_entry
{
    dword unknown00;
    long tag;
    dword unknown08;
    dword flags;
    byte unknown10[0x10];
};

struct s_index_cache
{
    long index;
    short count;
    short unknown06;
    long values[256];
};

struct s_render_index_cache_block
{
    s_44940_entry entries[32];
    long index;
    short count;
    short unknown406;
};

// The 0x408-byte blocks have record storage followed by their metadata.
// The older metadata view starts at the second block's index field.
union s_index_cache_storage
{
    struct
    {
        s_render_index_cache_block blocks[9];
        s_44940_entry final_entries[32];
    } records;
    struct
    {
        byte unknown000[0x808];
        s_index_cache entries[8];
    } metadata;
};

extern s_index_cache_storage g_4c62f8;
#define g_4c6700 (g_4c62f8.records.blocks[1].entries)
#define g_4c6b00 (g_4c62f8.metadata.entries)

#endif
