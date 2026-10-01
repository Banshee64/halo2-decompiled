/* GAME_STATE.CPP: the game state allocator

Retail inlines game_state_malloc into callers optimized for speed and calls
it from callers optimized for size.

- The size is checksummed from a local copy. Taking the parameter's address
  keeps it on the stack, where retail passes it in eax.
- game_state_malloc_aligned is one instruction short of a match: retail
  computes the aligned pointer as lea eax, [ebx + ecx], this source as
  lea eax, [ecx + ebx]. */

#include "cseries.h"
#include "game_state.h"
#include "crc.h"

s_game_state_globals game_state_globals;

// @retail 0x123d40
void *game_state_malloc(
	char const *name,
	char const *type,
	long size)
{
	long aligned_size = (size + 3) & ~3;
	void *result = game_state_globals.base_address + game_state_globals.cpu_allocation_size;

	game_state_globals.cpu_allocation_size += aligned_size;
	crc_checksum_buffer(&game_state_globals.allocation_size_checksum, &aligned_size, sizeof(aligned_size));

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
	crc_checksum_buffer(&game_state_globals.allocation_size_checksum, &aligned_size, sizeof(aligned_size));

	mask = alignment - 1;
	return (void *)((dword)(result + mask) & ~mask);
}
