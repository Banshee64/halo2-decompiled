// @flags /O2 /arch:SSE /Gr
/* XBOX_TEXTURE_CACHE.CPP: the texture cache's callbacks on its entries (the
   cache itself is created at 0x12c0d0, its memory set up in
   unknown_12d9f0.cpp). */

#include "cseries.h"
#include "data_array.h"
#include <xtl.h>

/* an entry of the texture cache (0x28 bytes) */
struct s_texture_cache_entry
{
	byte unknown00[2];
	byte flags;
	bool resident;
	long pending;
	byte unknown08[4];
	long hardware_format;
	byte unknown10[4];
	D3DResource resource;
	byte unknown20[8];
};

s_data_array *g_4e6454;
bool g_4e6479;

static inline s_texture_cache_entry *texture_cache_entry_get(long datum_index)
{
	return (s_texture_cache_entry *)g_4e6454->data + (datum_index & 0xffff);
}

// @retail 0x12d160
byte __stdcall texture_cache_entry_state(long datum_index)
{
	s_texture_cache_entry *entry = texture_cache_entry_get(datum_index);
	byte result = 0;

	if (entry->flags & 1)
		return 0x10;
	if (entry->pending)
		result = 0x20;
	return result;
}

// @retail 0x12d1a0
bool __stdcall texture_cache_entry_can_be_freed(long datum_index)
{
	s_texture_cache_entry *entry = texture_cache_entry_get(datum_index);

	if (!(entry->flags & 1) && (!(entry->flags & 2) || g_4e6479))
	{
		if (entry->hardware_format == NONE)
			return false;
		if (entry->resident && !D3DResource_IsBusy(&entry->resource))
			return false;
	}
	return true;
}
