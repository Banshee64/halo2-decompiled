// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"

bool function_12be90(void);
void function_137fe0(void);

// @retail 0x137f70
void function_137f70(long arg_1, real *arg_2)
{
	long local_1 = 0;
	while (local_1 < arg_1)
	{
		if (function_12be90())
			break;
		function_137fe0();
		local_1++;
	}
	if (local_1 < arg_1)
	{
		if (local_1 == 0)
		{
			*arg_2 = 0.0f;
		}
		else
		{
			real local_2 = *arg_2 - (arg_1 - local_1) * g_510c54->rate;
			*arg_2 = local_2 > 0.0f ? local_2 : 0.0f;
		}
	}
}
