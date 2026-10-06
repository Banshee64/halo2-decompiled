/* UNKNOWN_123B30.CPP: the game state allocator

Retail inlines function_123d40 into callers optimized for speed and calls
it from callers optimized for size.

- The size is checksummed from a local copy. Taking the parameter's address
  keeps it on the stack, where retail passes it in eax.
- game_state_malloc_aligned is one instruction short of a match: retail
  computes the aligned pointer as lea eax, [ebx + ecx], this source as
  lea eax, [ecx + ebx]. */

#include "unknown_11c920.h"
#include "unknown_123b30.h"
#include "crc.h"
#include "data_array.h"
#include "loop_allocator.h"
#include "globals.h"
#include "unknown_0820f0.h"
#include "unknown_059ad0.h"
#include "files.h"
#include <new>
#include <string.h>

void function_18e250(s_loop_allocator *loop, long size, const char *name, c_memory_source *source);

s_game_state_globals game_state_globals;

// @retail 0x123d40
void *function_123d40(
	char const *name,
	char const *type,
	long size)
{
	long aligned_size = (size + 3) & ~3;
	void *result = game_state_globals.base_address + game_state_globals.cpu_allocation_size;

	game_state_globals.cpu_allocation_size += aligned_size;
	function_163ba0(&game_state_globals.allocation_size_checksum, &aligned_size, sizeof(aligned_size));

	return result;
}

// @retail 0x123d80
void *game_state_malloc_aligned(
	char const *name,
	char const *type,
	long size,
	long alignment_bits)
{
	long alignment = 1 << alignment_bits;
	long aligned_size = (size + alignment + 3) & ~3;
	byte *result = game_state_globals.base_address + game_state_globals.cpu_allocation_size;
	long mask;

	game_state_globals.cpu_allocation_size += aligned_size;
	function_163ba0(&game_state_globals.allocation_size_checksum, &aligned_size, sizeof(aligned_size));

	mask = alignment - 1;
	return (void *)((dword)(result + mask) & ~mask);
}

/* a loop allocator whose header and pool are in the game state */
// @retail 0x123dd0
s_loop_allocator *game_state_loop_allocator_new(long size, char const *name)
{
	s_loop_allocator *loop = (s_loop_allocator *)function_123d40(name, "loop allocator", size + 0x50);

	function_18e250(loop, size, name, NULL);
	return loop;
}

/* the allocator of the data arrays kept in the game state (its vtable is at
   0x453498; the deallocation slot, 0x72c70, is shared with other allocators) */
class c_game_state_allocator : public c_data_allocator
{
public:
	virtual void *allocate(long size);
	virtual void deallocate(void *block) { }
};

// @retail 0x124700
void *c_game_state_allocator::allocate(long size)
{
	long aligned_size = (size + 3) & ~3;
	void *result = game_state_globals.base_address + game_state_globals.cpu_allocation_size;

	game_state_globals.cpu_allocation_size += aligned_size;
	function_163ba0(&game_state_globals.allocation_size_checksum, &aligned_size, sizeof(aligned_size));

	return result;
}

/* the game state's memory and its copy on the utility drive (unknown_214f10.cpp) */
void *game_state_cache_allocate(long cpu_size, long gpu_size);
void game_state_cache_files_open(void);
void game_state_cache_files_close(void);
void game_state_cache_write(short slot);
extern bool g_5020d8;

/* the time of the last save (copied to the network connection timers) */
s_connection_counter g_485ab0;
extern s_connection_counter g_4e6398;

// @retail 0x123b30
void function_123b30(void)
{
	if (!game_state_globals.initialized)
	{
		game_state_globals.allocation_size_checksum = NONE;
		game_state_globals.base_address = (byte *)game_state_cache_allocate(0x3be000, 0x40000);
		memset(game_state_globals.base_address, 0, 0x3be000 + 0x40000);
		game_state_cache_files_open();
		game_state_globals.arena = (s_arena_header *)function_123d40("game state header", "header", sizeof(s_arena_header));
		game_state_globals.initialized = true;
		g_510c2c = (c_data_allocator *)function_123d40("game state allocator", "allocator", sizeof(c_game_state_allocator));
		if (g_510c2c)
		{
			new (g_510c2c) c_game_state_allocator;
		}
	}
}

// @retail 0x123bf0
void function_123bf0(void)
{
	if (game_state_globals.initialized)
	{
		g_5020d8 = false;
		game_state_cache_files_close();
		memset(&game_state_globals, 0, sizeof(game_state_globals));
	}
}

/* saves the game state: its header to the next of the two saved copies, its
   memory to the cache file of that slot */
// @retail 0x123e20
void function_123e20(void)
{
	for (short i = 0; i < 4; i++)
	{
		g_46e320[i](0);
	}
	game_state_globals.counter = 0;
	game_state_globals.time = g_510c54->game_time;
	game_state_globals.flag3 = game_state_globals.flag2;
	game_state_globals.slot = (game_state_globals.slot + 1) % 2;
	game_state_cache_write(game_state_globals.slot);
	game_state_globals.flag2 = true;
	memcpy(&game_state_globals.saved_headers[game_state_globals.slot], game_state_globals.arena, sizeof(s_arena_header));
	g_46e320[4](0);
	g_4e6398 = g_485ab0;
}

/* the texture cache's lent blocks (unknown_12c0d0.cpp) */
void function_12d520(long address);
extern long g_51ea10;

/* gives back the texture cache block holding a copy of the game state,
   unless (g_51ea10) something still needs it */
// @retail 0x124190
void __stdcall game_state_cache_block_release(void *address, long user_data)
{
	if (g_51ea10 <= 0)
	{
		function_12d520((long)address);
	}
}

/* ---- the game state's core files (d:\core\<name>.bin) ---- */

char *csprintf_1024(char *buffer, const char *format, ...);
bool __stdcall version_is_compatible(char const *version);
bool __stdcall function_138180(s_session_options const *options);
bool __stdcall function_1384a0(s_session_options const *a, s_session_options const *b);

static inline void function_x454397(s_type_acf665 *reference)
{
	memset(reference, 0, sizeof(*reference));
	reference->signature = 'filo';
	reference->location = NONE;
}

static inline void function_x73bce5(s_type_acf665 *reference, char const *name)
{
	if (reference->flags & 1)
	{
		function_1373c0(reference->path);
	}
	function_137320(reference->path, name);
	reference->flags |= 1;
}

/* the file of a core: a full path as it is, a name in d:\core */
// @retail 0x124490
void game_state_core_get_file(char const *name, s_type_acf665 *reference)
{
	char path[1024];

	path[0] = 0;
	if (((name[0] >= 'a' && name[0] <= 'z') || (name[0] >= 'A' && name[0] <= 'Z')) && name[1] == ':' && name[2] == '\\')
	{
		csprintf_1024(path, "%s.bin", name);
	}
	else
	{
		csprintf_1024(path, "d:\\core\\%s.bin", name);
	}
	function_x454397(reference);
	function_x73bce5(reference, path);
}

/* whether a core's header belongs to this build and this game state */
// @retail 0x124520
bool function_124520(s_arena_header const *header)
{
	bool result = false;

	if (version_is_compatible(header->version) &&
		header->checksum == (long)game_state_globals.allocation_size_checksum &&
		header->base_address == (long)game_state_globals.base_address)
	{
		long length = strlen(header->map_name);

		if (length && strchr(header->map_name, '\\') && function_138180((s_session_options const *)header->field_130_2))
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x124590
bool game_state_headers_match(s_arena_header const *header, s_arena_header const *other)
{
	bool result = false;

	if (function_124520(header) &&
		!strcmp(other->map_name, header->map_name) &&
		other->unknown128 == header->unknown128 &&
		function_1384a0((s_session_options const *)other->field_130_2, (s_session_options const *)header->field_130_2) &&
		other->unknown1248 == header->unknown1248)
	{
		result = true;
	}
	return result;
}

// @retail 0x124640
bool game_state_core_read(char const *name, void *buffer, dword size)
{
	s_type_acf665 reference;
	dword error;
	bool result = false;

	game_state_core_get_file(name, &reference);
	if (function_136970(&reference, 1, &error))
	{
		result = function_136ca0(&reference, buffer, size, false);
		function_136bb0(&reference);
	}
	return result;
}

#include "main_globals.h"
extern long g_5020e4;
extern bool g_5020f2[2];
void __stdcall game_state_cache_lock_release(void *arg_0, long arg_1);
bool game_state_cache_read(short arg_0);
bool __stdcall function_11c1b0(short arg_0, bool arg_1);
void function_199520(dword arg_0);
void function_199540(dword arg_0);

// @retail 0x123f60
void function_123f60(dword arg_0)
{
	s_arena_header local_0;
	if (game_state_globals.flag2)
	{
		short local_1 = game_state_globals.slot;
		if (g_5020e4)
			game_state_cache_lock_release((void *)g_5020e4, 0);
		if (g_5020f2[local_1])
		{
			local_0 = game_state_globals.saved_headers[game_state_globals.slot];
			if (function_124520(&local_0))
			{
				*((byte *)local_0.field_130_2 + 4) = true;
				*((byte *)game_state_globals.arena->field_130_2 + 4) = true;
				if (g_4e6948 && g_4e6948->flag1120 &&
					function_1384a0((s_session_options const *)((byte *)g_4e6948 + 8), (s_session_options const *)local_0.field_130_2) &&
					function_11c1b0((short)*(long *)((byte *)&local_0 + 0x1248), true) &&
					game_state_headers_match(&local_0, game_state_globals.arena))
				{
					dword local_2 = ((byte)arg_0 & 1) << 5;
					if ((bool)((arg_0 >> 1) & 1)) local_2 |= 0x40;
					else local_2 &= ~0x40;
					function_199520(local_2);
					for (;;)
					{
						game_state_cache_read(game_state_globals.slot);
						if (game_state_headers_match(&local_0, game_state_globals.arena))
							break;
					}
					function_199540(local_2);
					return;
				}
			}
		}
	}
	main_globals.reset_map = true;
}
