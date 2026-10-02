/* GAME_STATE.H: the game state allocator */

#ifndef GAME_STATE_H
#define GAME_STATE_H

struct s_game_state_globals
{
	byte *base_address;
	long cpu_allocation_size;
	long unknown8;
	dword allocation_size_checksum;
	long game_time;
	struct s_arena_header *arena;
	bool arena_flag;
};

extern s_game_state_globals game_state_globals;

void *game_state_malloc(char const *name, char const *type, long size);
void *game_state_malloc_aligned(char const *name, char const *type, long size, long alignment_bits);

#endif
