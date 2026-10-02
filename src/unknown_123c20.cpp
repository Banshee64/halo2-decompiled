// @flags /O2 /Gr
/* UNKNOWN_123C20.CPP: the memory arena lifecycle callbacks (entry 16) */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include <xtl.h>
#include <string.h>

struct s_arena_header
{
	long checksum;
	long base_address;
	char map_name[0x100];
	char version[0x20];
	dword unknown128;
	dword unknown12c;
	long game_options[0x446];
	short unknown1248;
};

struct s_unknown_4e3b60
{
	byte unknown0;
	bool flag1;
	bool flag2;
	bool flag3;
	dword unknown4;
	dword values[0x944];
};

s_unknown_4e3b60 g_4e3b60;
#define ARENA (game_state_globals.arena)
char g_5478bc[0x100];
char g_450698[0x20];
dword g_547844;

PRIVATE void csstrncpy(char *destination, char const *source, long size)
{
	strncpy(destination, source, size);
	destination[size - 1] = 0;
}

// @retail 0x123c20
void arena_initialize_for_new_map(void)
{
	byte *base_address = game_state_globals.base_address;

	XPhysicalProtect(base_address, 0x3be000, PAGE_READWRITE);
	XPhysicalProtect(base_address + 0x3be000, 0x40000, PAGE_READWRITE | PAGE_WRITECOMBINE);

	g_4e3b60.flag1 = true;
	g_4e3b60.flag2 = false;
	g_4e3b60.flag3 = false;
	memset(g_4e3b60.values, 0, sizeof(g_4e3b60.values));

	game_state_globals.game_time = NONE;
	memset(ARENA, 0, 0x1288);
	ARENA->checksum = game_state_globals.allocation_size_checksum;
	ARENA->base_address = (long)game_state_globals.base_address;
	csstrncpy(ARENA->map_name, g_5478bc, sizeof(ARENA->map_name));
	csstrncpy(ARENA->version, g_450698, sizeof(ARENA->version));
	ARENA->unknown128 = g_547844;
	memcpy(ARENA->game_options, (byte *)g_4e6948 + 8, sizeof(ARENA->game_options));
	game_state_globals.arena_flag = false;
}

// @retail 0x123d20
void arena_initialize_for_new_structure_bsp(void)
{
	ARENA->unknown1248 = g_4686c4;
}
