// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"

void function_19043f(long arg_1, long arg_2, long arg_3);
void function_12b980(void);
void function_1389c0(void);
void function_18e960(void);

// @retail 0x1388e0
void function_1388e0(void)
{
	s_game_options_view *local_1 = g_4e6948;
	if (local_1 && local_1->flag1120)
	{
		if (local_1->state == 1)
		{
			if (local_1->state == 1 && local_1->position_b != NONE)
			{
				long local_2 = 0;
				do
				{
					function_19043f(local_2, local_1->position_b, local_1->difficulty);
					local_2 = local_2 >= 0 && local_2 < 3 ? local_2 + 1 : NONE;
				}
				while (local_2 != NONE);
				function_12b980();
			}
			function_18e960();
		}
		else if (local_1->state == 2)
		{
			function_1389c0();
		}
	}
}
