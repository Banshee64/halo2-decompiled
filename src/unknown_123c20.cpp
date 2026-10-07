// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_123C20.CPP: the memory arena lifecycle callbacks (entry 16) */

#include "unknown_11c920.h"
#include "unknown_123b30.h"
#include "globals.h"
#include <xtl.h>
#include <string.h>

#define ARENA (game_state_globals.arena)
char g_5478bc[0x100];
char g_450698[0x20];
dword g_547844;

PRIVATE void csstrncpy(char *destination, char const *source, long size)
{
	strncpy(destination, source, size);
	destination[size - 1] = 0;
}

static __forceinline void function_123c21(byte *arg_0)
{
	XPhysicalProtect(arg_0, 0x3be000, PAGE_READWRITE);
	XPhysicalProtect(arg_0 + 0x3be000, 0x40000, PAGE_READWRITE | PAGE_WRITECOMBINE);
}

// @retail 0x123c20
void arena_initialize_for_new_map(void)
{
	function_123c21(game_state_globals.base_address);

	game_state_globals.flag1 = true;
	game_state_globals.flag2 = false;
	game_state_globals.flag3 = false;
	memset(game_state_globals.saved_headers, 0, sizeof(game_state_globals.saved_headers));

	game_state_globals.game_time = NONE;
	memset(ARENA, 0, 0x1288);
	ARENA->checksum = game_state_globals.allocation_size_checksum;
	ARENA->base_address = (long)game_state_globals.base_address;
	csstrncpy(ARENA->map_name, g_5478bc, sizeof(ARENA->map_name));
	csstrncpy(ARENA->version, g_450698, sizeof(ARENA->version));
	ARENA->unknown128 = g_547844;
	memcpy(ARENA->field_130_2, (byte *)g_4e6948 + 8, sizeof(ARENA->field_130_2));
	game_state_globals.arena_flag = false;
}

// @retail 0x123d20
void arena_initialize_for_new_structure_bsp(void)
{
	ARENA->unknown1248 = g_4686c4;
}

// @retail 0x123ed0
void function_123ed0(void)
{
	if (game_state_globals.flag2 && game_state_globals.flag3)
	{
		dword elapsed = g_510c54->game_time - game_state_globals.time;

		if ((real)elapsed * g_510c54->rate < 8.0f)
		{
			if (++game_state_globals.counter >= 5)
			{
				game_state_globals.slot = (game_state_globals.slot + 1) % 2;
				game_state_globals.flag2 = true;
				game_state_globals.flag3 = false;
			}
			return;
		}
	}
	game_state_globals.counter = 0;
}