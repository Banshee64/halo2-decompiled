/* UNKNOWN_218850.H: the sound cache (src/unknown_218850.cpp): sound data
   chunks are paged into cache pages, and g_502104 tracks one 16-byte entry per
   page */

#ifndef UNKNOWN_218850_H
#define UNKNOWN_218850_H

#include "cseries.h"
#include "data_array.h"

/* a chunk of sound data in the cache file (12 bytes) */
struct s_sound_chunk
{
	dword file_offset;
	dword size_flags;	/* the low 30 bits are the size */
	long cache_index;	/* the cache entry (g_502104) holding the chunk, or NONE */
};

#define SOUND_CHUNK_SIZE(chunk) ((chunk)->size_flags & 0x3fffffff)

/* an entry of g_502104 (16 bytes) */
struct s_sound_cache_entry
{
	short salt;
	byte volatile loaded;
	byte used;
	byte lock_count;
	byte reference_count;
	word unknown06;
	long owner;
	s_sound_chunk *chunk;
};

/* an entry of the cache page array (24 bytes) */
struct s_sound_cache_page
{
	byte unknown00[8];
	long offset;
	byte unknown0c[8];
	dword last_used;
};

/* the cache page allocator */
struct s_sound_cache_allocator
{
	byte unknown00[0x34];
	long page_shift;
	dword time;
	byte unknown3c[0x28];
	s_record_pool *pages;
};

extern s_record_pool *g_502104;
extern dword g_502108;
extern s_sound_cache_allocator *g_50210c;

#define SOUND_CACHE_ENTRY(index) (&((s_sound_cache_entry *)g_502104->data)[(index) & 0xffff])
#define SOUND_CACHE_PAGE(index) (&((s_sound_cache_page *)g_50210c->pages->data)[(index) & 0xffff])

dword __stdcall function_218850(long owner, s_sound_chunk *chunk, dword flags);
byte *sound_cache_chunk_get_data(s_sound_chunk *chunk);

#endif
