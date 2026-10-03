// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1C25A0.CPP: the physics (Havok) system's lifecycle callbacks
   (0x441624..0x441638 in the lifecycle table) */

#include "cseries.h"
#include "game_state.h"
#include "data_array.h"
#include <xtl.h>

/* the havok components (unknown_1cec30.cpp) */
struct s_manager_globals;
extern s_manager_globals *g_51e9b8;
extern long *g_51e9a0;
void havok_components_initialize(void);

void game_state_initialize_1edbc0(void);
void function_2263c0(void);
void function_146b30(void);
void function_146b80(void);
void function_146de0(void);
void function_226440(void);

/* an aligned block of physical memory: the offset back to the allocation
   sits before it */
void *g_479888;

// @retail 0x1c25a0
void havok_initialize(void)
{
	g_51e9a0 = (long *)game_state_malloc("havok", "havok", sizeof(long));
	*g_51e9a0 = 0;
	game_state_initialize_1edbc0();
	function_2263c0();
	havok_components_initialize();
	function_146b30();
	function_146b80();
}

// @retail 0x1c2600
void havok_dispose(void)
{
	byte *block;

	function_146de0();
	block = (byte *)g_479888;
	if (!VirtualFree(block - ((long *)block)[-1], 0, MEM_RELEASE))
	{
		GetLastError();
	}
	g_479888 = NULL;
	data_dispose((s_data_array *)g_51e9b8);
	g_51e9b8 = NULL;
	function_226440();
}