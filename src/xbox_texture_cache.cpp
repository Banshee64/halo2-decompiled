// @flags /O2 /arch:SSE /Gr
/* XBOX_TEXTURE_CACHE.CPP: the texture cache: a data array of entries (one per
   bitmap level in memory) whose memory is a block of the physical memory
   allocator g_4e6464, a data array of predicted bitmaps waiting to be loaded,
   and the cache's scale, raised and lowered with how full it is. Its memory
   is set up in unknown_12d9f0.cpp's neighbour 0x12c1e0. */

#include "cseries.h"
#include "data_array.h"
#include "globals.h"
#include "physical_memory.h"
#include "physical_memory_map.h"
#include "async.h"
#include <xtl.h>
#include <string.h>

/* the part of a bitmap's data block the cache keeps track of */
struct s_bitmap_data
{
	byte unknown00[0xe];
	word flags;
	byte unknown10[0xc];
	long data_offsets[3];
	long block_indices[3];
	long data_sizes[3];
	long unknown40[4];
	long unknown50;
	long unknown54;
	byte unknown58[0x18];
	long unknown70;
};

/* an entry of the texture cache (0x28 bytes) */
struct s_texture_cache_entry
{
	byte unknown00[2];
	byte flags;
	bool resident;
	long pending;
	byte unknown08[4];
	long hardware_format;
	s_bitmap_data *bitmap;
	D3DResource resource;
	byte unknown20[8];
};

/* a bitmap predicted to be needed soon (8 bytes) */
struct s_texture_cache_request
{
	short salt;
	short unknown02;
	s_bitmap_data *bitmap;
};

/* a block of the cache's memory lent out (bink movies, the simulation world):
   the header sits just below the memory it describes */
typedef void (__stdcall *texture_cache_lock_proc)(void *address, long user_data);

struct s_texture_cache_lock
{
	long block_index;
	dword signature;
	void *address;
	long size;
	long user_data;
	texture_cache_lock_proc update;
	texture_cache_lock_proc release;
	s_texture_cache_lock *previous;
	s_texture_cache_lock *next;
};

s_data_array *g_4e6454;
s_data_array *g_4e6458;
s_texture_cache_lock *g_4e645c;
long g_4e646c;
dword g_4e6460;
s_physical_object *g_4e6464;
bool g_4e6479;
bool g_4e647a;
real g_4e647c;
dword g_4e6480;
long g_4e6484;
long g_4e6488;
dword g_55e724;
bool g_468841 = true;

long function_120bf0(void);
void __stdcall texture_cache_block_delete(long datum_index);
byte __stdcall texture_cache_entry_state(long datum_index);
bool __stdcall texture_cache_entry_busy(long datum_index);

static inline s_texture_cache_entry *texture_cache_entry_get(long datum_index)
{
	return (s_texture_cache_entry *)g_4e6454->data + (datum_index & 0xffff);
}

// @retail 0x12c0d0
void texture_cache_initialize(void)
{
	g_4e6454 = data_new_inlined("xbox texture", 0x200, sizeof(s_texture_cache_entry), 0, g_468758);
	g_4e6458 = data_new_inlined("xbox predicted texture", 0xc8, sizeof(s_texture_cache_request), 0, g_468758);
	g_4e6464 = physical_memory_new("xbox texture cache", 0, 0xc, 0x200, texture_cache_block_delete, texture_cache_entry_busy, texture_cache_entry_state, g_468758);
	g_4e6464->state = 2;
}

// @retail 0x12c190
void texture_cache_dispose(void)
{
	if (g_4e6454)
	{
		data_dispose(g_4e6454);
		g_4e6454 = NULL;
	}
	if (g_4e6464)
	{
		g_4e6464->allocator->deallocate(g_4e6464);
		g_4e6464 = NULL;
	}
}

// @retail 0x12c1e0
void texture_cache_initialize_for_new_map(void)
{
	long available = physical_memory_available();
	long pages = available / 4096;

	g_4e6460 = (dword)physical_memory_malloc_fixed(pages * 4096, PAGE_READWRITE | PAGE_WRITECOMBINE);
	g_4e6464->method_13d8b0(pages);
	g_4e6454->valid = true;
	data_delete_all(g_4e6454);
	g_4e6458->valid = true;
	data_delete_all(g_4e6458);
}

/* raises the cache's scale while it is nearly full, lowers it while it is not */
// @retail 0x12c2e0
void texture_cache_update_scale(void)
{
	real usage = 0.0f;
	long page_count = g_4e6464->page_count;

	if (page_count > 0)
	{
		usage = (real)physical_memory_used_pages(g_4e6464, 5) / (real)page_count;
	}
	g_4e6464->state = 2;
	if (g_468841 && page_count > 0)
	{
		if (usage >= 0.95f || g_4e647a)
		{
			g_4e647c += 0.05f;
		}
		else if (usage >= 0.85f)
		{
			g_4e647c += 0.02f;
		}
		else if (usage <= 0.5f)
		{
			g_4e647c -= 0.05f;
		}
		else if (usage <= 0.6f)
		{
			g_4e647c -= 0.02f;
		}
		else if (usage <= 0.7f)
		{
			g_4e647c -= 0.01f;
		}
		if (g_4e647c < 0.0f)
		{
			g_4e647c = 0.0f;
		}
		else if (g_4e647c > 2.0f)
		{
			g_4e647c = 2.0f;
		}
	}
	else
	{
		g_4e647c = 0.0f;
	}
	g_4e647a = false;
}

static inline bool texture_cache_lock_exists(s_texture_cache_lock *lock)
{
	s_texture_cache_lock *other;

	for (other = g_4e645c; other; other = other->next)
	{
		if (other == lock)
		{
			return true;
		}
	}
	return false;
}

// @retail 0x12c400
void texture_cache_update_locks(void)
{
	s_texture_cache_lock *lock = g_4e645c;

	while (lock)
	{
		if (lock->update)
		{
			lock->update(lock->address, lock->user_data);
		}
		if (!texture_cache_lock_exists(lock))
		{
			lock = g_4e645c;
		}
		else
		{
			lock = lock->next;
		}
	}
}

// @retail 0x12c530
void function_12c530(void)
{
	if (g_4e6454->valid)
	{
		s_data_iterator iterator;
		s_texture_cache_entry *entry;

		iterator.data = g_4e6454;
		iterator.index = NONE;
		iterator.datum_index = NONE;
		while ((entry = (s_texture_cache_entry *)data_iterator_next_inlined(&iterator)) != NULL)
		{
			if (entry->bitmap)
			{
				entry->bitmap->unknown54 = 0;
				entry->bitmap->unknown50 = 0;
				entry->bitmap->unknown70 = 0;
			}
		}
	}
	g_4e6488 = 4;
}

// @retail 0x12c5b0
void function_12c5b0(void)
{
	if (++g_4e6488 == 0)
	{
		function_12c530();
	}
	physical_memory_new_frame(g_4e6464);
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
bool __stdcall texture_cache_entry_busy(long datum_index)
{
	s_texture_cache_entry *entry = texture_cache_entry_get(datum_index);

	if (!(entry->flags & 1) && (!(entry->flags & 2) || g_4e6479))
	{
		if (entry->hardware_format == NONE)
			return false;
		if (!entry->resident || D3DResource_IsBusy(&entry->resource))
			return true;
		return false;
	}
	return true;
}

/* the physical memory's callback when a block is freed: waits for the GPU to
   be done with the texture, then forgets it */
// @retail 0x12d200
void __stdcall texture_cache_block_delete(long datum_index)
{
	s_texture_cache_entry *entry = texture_cache_entry_get(datum_index);

	if (entry->bitmap)
	{
		while (texture_cache_entry_busy(datum_index))
		{
			async_globals.tasks_added = function_120bf0();
			if (entry->resident)
			{
				D3DResource_BlockUntilNotBusy(&entry->resource);
			}
		}
		entry->bitmap->block_indices[entry->pending] = NONE;
		entry->bitmap->unknown40[entry->pending] = 0;
		if (entry->pending == 0)
		{
			entry->bitmap->unknown54 = 0;
			entry->bitmap->unknown50 = 0;
			entry->bitmap->unknown70 = 0;
		}
	}
	datum_delete(g_4e6454, datum_index);
}

/* whether a bitmap format is one the cache scales down (not the compressed
   formats 12-16) */
// @retail 0x12ccb0
bool texture_cache_format_scalable(long format)
{
	bool result = false;

	switch (format)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 17:
	case 18:
		result = true;
		break;
	}
	return result;
}
/* asks for a bitmap: touches its block when it is in the cache, else queues
   a request for it */
// @retail 0x12cc10
bool texture_cache_bitmap_request(s_bitmap_data *bitmap)
{
	bool result = false;

	if (g_4e6458->valid)
	{
		if (bitmap->block_indices[0] != NONE)
		{
			s_texture_cache_entry *entry = texture_cache_entry_get(bitmap->block_indices[0]);

			((s_physical_block *)g_4e6464->blocks->data)[bitmap->block_indices[0] & 0xffff].time = g_4e6464->time;
			if (entry->resident)
			{
				result = true;
			}
		}
		else if (!(bitmap->flags & 0x400))
		{
			long request_index = datum_new(g_4e6458);

			if (request_index != NONE)
			{
				((s_texture_cache_request *)g_4e6458->data)[request_index & 0xffff].bitmap = bitmap;
				bitmap->flags |= 0x400;
			}
			else if (GetTickCount() > g_55e724)
			{
				g_55e724 = GetTickCount() + 30000;
			}
		}
	}
	return result;
}

/* forgets a bitmap: its request and its blocks */
// @retail 0x12c770
void texture_cache_bitmap_unload(s_bitmap_data *bitmap)
{
	if (bitmap->flags & 0x200)
	{
		long *block_index;
		long count;

		if (bitmap->flags & 0x400)
		{
			s_data_iterator iterator;
			s_texture_cache_request *request;

			iterator.data = g_4e6458;
			iterator.index = NONE;
			iterator.datum_index = NONE;
			while ((request = (s_texture_cache_request *)data_iterator_next_calling(&iterator)) != NULL)
			{
				if (request->bitmap == bitmap)
				{
					datum_delete(g_4e6458, iterator.datum_index);
					break;
				}
			}
		}
		block_index = bitmap->block_indices;
		count = 3;
		do
		{
			if (*block_index != NONE)
			{
				g_4e6464->block_delete(*block_index);
				*block_index = NONE;
			}
			block_index[6] = 0;
			block_index++;
		}
		while (--count);
		bitmap->flags &= ~0x600;
		bitmap->unknown54 = 0;
	}
}

/* lends out size bytes of the cache's memory, page aligned, or NULL */
// @retail 0x12d2f0
long __stdcall function_12d2f0(long size, long user_data, long update, long release)
{
	long result = 0;
	long block_index = function_13d370(g_4e6464, size + 0x2023, 1);

	if (block_index != NONE)
	{
		byte *address = (byte *)((((((s_physical_block *)g_4e6464->blocks->data)[block_index & 0xffff].offset << g_4e6464->page_shift) + g_4e6460 + 0x1023)) & 0xfffff000);
		s_texture_cache_lock *lock = (s_texture_cache_lock *)address - 1;
		s_texture_cache_entry *entry = texture_cache_entry_get(datum_new_at_index_with_salt(g_4e6454, block_index));

		lock->signature = (dword)address ^ 0x2281972;
		lock->block_index = block_index;
		lock->address = address;
		lock->size = size;
		lock->update = (texture_cache_lock_proc)update;
		lock->release = (texture_cache_lock_proc)release;
		lock->user_data = user_data;
		lock->previous = NULL;
		lock->next = g_4e645c;
		if (g_4e645c)
		{
			g_4e645c->previous = lock;
		}
		g_4e645c = lock;
		XPhysicalProtect(lock->address, lock->size, PAGE_READWRITE);
		memset(&entry->flags, 0, sizeof(s_texture_cache_entry) - 2);
		entry->flags |= 1;
		result = (long)address;
	}
	return result;
}

double timing_ticks_to_seconds(__int64 ticks);

static __int64 read_tsc(void)
{
	volatile __int64 t = 0;
	__asm rdtsc
}

void function_12c450(void);

/* updates the locks and the loads, and every 200 milliseconds the scale */
// @retail 0x12c600
void function_12c600(void)
{
	texture_cache_update_locks();
	function_12c450();
	function_12c5b0();
	if (GetTickCount() > g_4e6480)
	{
		g_4e6480 = GetTickCount() + 200;
		texture_cache_update_scale();
	}
	g_4e6484 = 0;
}

static inline long texture_cache_next_used_index(s_data_array *data, long index)
{
	if (index >= 0 && index < data->high_water_index)
	{
		do
		{
			if (data->bitmap[index >> 5] & (1 << (index & 0x1f)))
			{
				return index;
			}
			index++;
		}
		while (index < data->high_water_index);
	}
	return NONE;
}

/* takes back every lent block, waits for the GPU, and forgets the predicted
   bitmaps */
// @retail 0x12d0a0
void texture_cache_flush(void)
{
	while (g_4e645c)
	{
		g_4e645c->release(g_4e645c->address, g_4e645c->user_data);
	}
	D3DDevice_KickPushBuffer();
	D3DDevice_IsBusy();
	physical_memory_flush(g_4e6464);
	if (g_4e6458->valid)
	{
		s_data_array *data = g_4e6458;
		long index = NONE;

		while ((index = texture_cache_next_used_index(data, index + 1)) != NONE)
		{
			s_texture_cache_request *request = (s_texture_cache_request *)(data->data + data->size * index);

			request->bitmap->flags &= ~0x400;
			datum_delete(data, (request->salt << 16) | index);
		}
	}
}

// @retail 0x12c290
void texture_cache_dispose_from_old_map(void)
{
	g_4e6479 = true;
	texture_cache_flush();
	g_4e6454->valid = false;
	g_4e6458->valid = false;
	if (g_4e646c)
	{
		g_4e646c = 0;
	}
	g_4e6464->method_13d8b0(0);
	g_4e6460 = 0;
}

/* an iteration over the tags of one group (cache_files.cpp) */
struct s_tag_iterator
{
	long unknown00;
	long unknown04;
	long datum_index;
	long next_index;
	long group_tag;
};

long function_122c70(s_tag_iterator *iterator);
long function_213760(dword location, long size, void *buffer, dword *bytes_read, bool *done, long category, long priority);

/* where the cache file keeps the bitmaps' shared pixel data, and its size */
dword g_547858;
long g_54785c;
bool g_4e6468;

struct s_bitmap_group_view
{
	byte unknown00[0x44];
	long bitmap_count;
};

/* walks the bitmap tags, then reads the shared pixel data into the top of
   the physical memory */
// @retail 0x12c640
void texture_cache_load_shared_data(void)
{
	s_tag_iterator iterator;
	long tag_index;

	iterator.next_index = 0;
	iterator.group_tag = 'bitm';
	while ((tag_index = function_122c70(&iterator)) != NONE)
	{
		s_bitmap_group_view *bitmap = (s_bitmap_group_view *)g_4e3b44[tag_index & 0xffff].bytes;

		for (short i = 0; i < bitmap->bitmap_count; i++)
		{
		}
	}
	if (g_547858 && g_54785c)
	{
		long size = g_54785c;
		long aligned_size = (size + 0xfff) & 0xfffff000;
		long read_size = size;
		void *memory;
		bool volatile done;

		if (size & 0x1ff)
		{
			read_size = (size | 0x1ff) + 1;
		}
		memory = physical_memory_malloc_fixed(aligned_size, PAGE_READWRITE | PAGE_WRITECOMBINE);
		g_4e646c = (long)memory;
		g_4e6468 = true;
		function_213760(g_547858, read_size, memory, NULL, (bool *)&done, 3, 7);
		if (!done)
		{
			while (!done)
			{
				SwitchToThread();
			}
		}
	}
	else
	{
		g_4e6468 = false;
	}
}

/* the highest level (of three) worth loading at a scale: level 1 needs a
   scale of 1, level 2 of 2, and only levels over 1 KB count after the first */
// @retail 0x12cb10
long texture_cache_bitmap_level(s_bitmap_data const *bitmap, real scale)
{
	long result = 0;

	for (long i = 0; i < 3; i++)
	{
		if (bitmap->data_offsets[i] != NONE)
		{
			long size = bitmap->data_sizes[i];

			if (size && (!i || size > 0x400))
			{
				real thresholds[3] = { 0.0f, 1.0f, 2.0f };

				if (thresholds[i] > scale)
				{
					break;
				}
				result = i;
			}
		}
	}
	return result;
}

/* function_12d2f0, pumping the cache until a block is free: not at all
   (type 0), or for up to 30 pumps and 0.1 seconds (type 1) or 90 pumps and
   one second (type 2) */
// @retail 0x12d400
long function_12d400(long type, long size, long user_data, long update, long release)
{
	long result = 0;
	__int64 start = read_tsc();
	real timeout = 0.0f;
	long maximum_pumps = 0;
	long attempts = 5;
	long pumps;

	switch (type)
	{
	case 1:
		timeout = 0.1f;
		maximum_pumps = 30;
		attempts = 2;
		break;
	case 2:
		timeout = 1.0f;
		maximum_pumps = 90;
		attempts = 2;
		break;
	}

	if (size > 0 && g_4e6464->page_count > 0)
	{
		pumps = 0;
		for (;;)
		{
			result = function_12d2f0(size, user_data, update, release);
			if (result != 0)
			{
				break;
			}
			if (pumps < maximum_pumps)
			{
				pumps++;
				function_12c600();
				continue;
			}

			__int64 elapsed = read_tsc() - start;
			if (elapsed < 0)
			{
				elapsed = 0;
			}
			if (!(timing_ticks_to_seconds(elapsed) < timeout))
			{
				break;
			}
			D3DDevice_KickPushBuffer();
			D3DDevice_IsBusy();
			SwitchToThread();
		}
	}
	return result;
}

/* takes back memory lent out by function_12d2f0 */
// @retail 0x12d520
void function_12d520(long address)
{
	s_texture_cache_lock *lock = (s_texture_cache_lock *)address - 1;
	s_texture_cache_entry *entry = texture_cache_entry_get(lock->block_index);

	XPhysicalProtect(lock->address, lock->size, PAGE_READWRITE | PAGE_WRITECOMBINE);
	entry->flags &= ~1;
	lock->signature = 0;
	if (lock->next)
	{
		lock->next->previous = lock->previous;
	}
	if (lock->previous)
	{
		lock->previous->next = lock->next;
	}
	else
	{
		g_4e645c = lock->next;
	}
	g_4e6464->block_delete(lock->block_index);
}
