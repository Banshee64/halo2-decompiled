// @flags /O2 /Gr
/* UNKNOWN_1552E0.CPP: the lifecycle callbacks of entry 35 */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include <string.h>

struct s_unknown_78
{
	byte unknown00[0x78];
};

dword g_4e8c38[0x143];
s_unknown_78 *g_510c6c;

// @retail 0x1552e0
void function_1552e0(void)
{
	memset(g_4e8c38, 0, sizeof(g_4e8c38));
	byte *flag = (byte *)game_state_malloc("unknown", "unknown", 4);
	*flag = 0;
	g_4e8c34 = flag;
	g_510c6c = (s_unknown_78 *)game_state_malloc("unknown", "unknown", sizeof(s_unknown_78));
}

// @retail 0x155490
void function_155490(void)
{
	memset(g_4e8c38, 0, sizeof(g_4e8c38));
	*g_4e8c34 = 0;
}
