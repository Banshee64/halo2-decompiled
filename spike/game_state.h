/* GAME_STATE.H: the spike's view of the game state allocator */

typedef unsigned char byte;

struct s_game_state_globals
{
	byte *base_address;
	long cpu_allocation_size;
	long unknown8;
	unsigned long allocation_size_checksum;
};

extern s_game_state_globals game_state_globals;

void *game_state_malloc(char const *name, char const *type, long size);
void *game_state_malloc_aligned(char const *name, char const *type, long size, long alignment_bits);
