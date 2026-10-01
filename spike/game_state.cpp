/*
GAME_STATE.CPP: the game state allocator (retail 0x123d40, 0x123d80).

Retail inlines game_state_malloc into callers optimized for speed and calls
it from callers optimized for size. These files rebuild both kinds:

    python tools/match.py "/O2 /Gr" spike/game_state.cpp spike/game_state_o2.cpp         "/O2 /Ob1 /Gr" spike/crc.cpp "/O1 /Gr" spike/game_state_os.cpp --         "?game_state_malloc@@YIPAXPBD0J@Z=123d40"         "?game_state_malloc_aligned@@YIPAXPBD0JJ@Z=123d80"         "?game_state_initialize_1edbc0@@YIXXZ=1edbc0"         "?game_state_initialize_24c819@@YIXXZ=24c819"         "?game_state_initialize_165cc3@@YIXXZ=165cc3"         "?crc_checksum_buffer@@YIXPAKPBXJ@Z=163ba0"         "?build_crc_table@@YIXPAK@Z=163c00"

- crc.cpp needs /Ob1: with /Ob2, the compiler inlines crc_checksum_buffer
  into game_state_malloc, which retail does not.
- The size is checksummed from a local copy. Taking the parameter's address
  keeps it on the stack, where retail passes it in eax.
- game_state_malloc_aligned is one instruction short of a match: retail
  computes the aligned pointer as lea eax, [ebx + ecx], this source as
  lea eax, [ecx + ebx].
*/

#include "game_state.h"

void crc_checksum_buffer(unsigned long *crc_reference, void const *buffer, long buffer_size);

s_game_state_globals game_state_globals;

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
	return (void *)((unsigned long)(result + mask) & ~mask);
}
