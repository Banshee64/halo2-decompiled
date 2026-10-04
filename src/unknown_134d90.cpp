// @flags /O2 /Gr
/* UNKNOWN_134D90.CPP: the lifecycle callbacks of entry 64 */

#include "cseries.h"
#include "game_state.h"
#include <string.h>

struct s_unknown_134d90
{
	long values[0x81];
};

s_unknown_134d90 *g_4e6740;

// @retail 0x134d90
void function_134d90(void)
{
	g_4e6740 = (s_unknown_134d90 *)function_123d40("unknown", "unknown", sizeof(s_unknown_134d90));
}

// @retail 0x134dd0
void function_134dd0(void)
{
	g_4e6740 = 0;
}

// @retail 0x134de0
void function_134de0(void)
{
	if (g_4e6740)
	{
		memset(g_4e6740, 0, sizeof(*g_4e6740));
	}
}
