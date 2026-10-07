// @flags /O2 /Ob1 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "unknown_059ad0.h"
#include "network_voice.h"

bool function_59670(c_class_58d20 **arg_0);
bool function_596a0(c_class_58d20 **arg_0);
bool voice_is_enabled(void);
long __stdcall function_53b00(long arg_0, byte *arg_1, long arg_2);

// @retail 0x765c0
long __stdcall function_765c0(long arg_0, long arg_1, byte *arg_2)
{
	long local_0 = 0;
	c_class_58d20 *local_1 = NULL;
	if (voice_is_enabled())
	{
		bool local_2 = false;
		switch (g_4c9878.session_kind)
		{
		case 1: local_2 = function_59670(&local_1); break;
		case 2: local_2 = function_596a0(&local_1); break;
		}
		if (local_2)
		{
			long local_3 = local_1->find_member_by_channel(arg_0);
			if (local_3 != NONE)
				local_0 = function_53b00(local_3, arg_2, arg_1);
		}
	}
	return local_0;
}
