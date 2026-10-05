/*
UNKNOWN_123B30.CPP: the game state allocator (retail 0x123d40, 0x123d80).

Retail inlines function_123d40 into callers optimized for speed and calls
it from callers optimized for size. These files rebuild both kinds:

    python tools/match.py "/O2 /Gr" spike/unknown_123b30.cpp spike/unknown_123b30_o2.cpp         "/O2 /Ob1 /Gr" spike/crc.cpp "/O1 /Gr" spike/unknown_123b30_os.cpp --         "?function_123d40@@YIPAXPBD0J@Z=123d40"         "?game_state_malloc_aligned@@YIPAXPBD0JJ@Z=123d80"         "?game_state_initialize_1edbc0@@YIXXZ=1edbc0"         "?game_state_initialize_24c819@@YIXXZ=24c819"         "?game_state_initialize_165cc3@@YIXXZ=165cc3"         "?function_163ba0@@YIXPAKPBXJ@Z=163ba0"         "?function_163c00@@YIXPAK@Z=163c00"

- crc.cpp needs /Ob1: with /Ob2, the compiler inlines function_163ba0
  into function_123d40, which retail does not.
- The size is checksummed from a local copy. Taking the parameter's address
  keeps it on the stack, where retail passes it in eax.
- game_state_malloc_aligned is one instruction short of a match: retail
  computes the aligned pointer as lea eax, [ebx + ecx], this source as
  lea eax, [ecx + ebx].
*/

#include "unknown_123b30.h"

void function_163ba0(unsigned long *crc_reference, void const *buffer, long buffer_size);

s_game_state_globals game_state_globals;

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
	return (void *)((unsigned long)(result + mask) & ~mask);
}
