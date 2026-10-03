// @flags /O2 /Ob1 /Gr
/* UNKNOWN_276F80.CPP: what the command script being run looks at and aims at */

#include "cseries.h"
#include "globals.h"
#include "command_scripts.h"
#include "unknown_276f80.h"

inline void command_script_set_look(bool enable, short type, long index)
{
	long script_index = g_502410;
	s_command_script *script = command_script_get(script_index);
	script->flag52 = enable;
	if (enable)
	{
		script->type54 = type;
		script->index58 = index;
		script->flag51 = false;
	}
}

inline void command_script_set_aim(bool enable, short type, long index)
{
	long script_index = g_502410;
	s_command_script *script = command_script_get(script_index);
	script->flag46 = enable;
	if (enable)
	{
		script->type48 = type;
		script->index4c = index;
	}
}

// @retail 0x276f80
void function_276f80(bool enable, long point_index)
{
	if (g_502410 != NONE)
	{
		command_script_set_look(enable, 2, point_index);
		command_script_set_aim(enable, 2, point_index);
	}
}

// @retail 0x277060
void function_277060(bool enable, long object_index)
{
	if (g_502410 != NONE)
	{
		long script_index = g_502410;
		s_command_script *script = command_script_get(script_index);
		script->flag52 = enable;
		if (enable)
		{
			script->flag46 = false;
			script->type54 = 1;
			script->index58 = object_index;
			script->flag51 = false;
		}
		command_script_set_aim(enable, 1, object_index);
	}
}