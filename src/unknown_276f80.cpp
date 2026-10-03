// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_276F80.CPP: what the command script being run looks at and aims at */

#include "cseries.h"
#include "globals.h"
#include "command_scripts.h"
#include "unknown_276f80.h"

/* the radii of something the command script being run does, squared */
// @retail 0x276d50
void function_276d50(real a, real b, real c)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->type = 0x14;
		script->flagac = true;
		script->flagd0 = false;
		script->indexb0 = NONE;
		script->valueb4 = a * a;
		script->valueb8 = b * b;
		script->valuebc = c * c;
	}
}
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
// @retail 0x2771d0
void function_2771d0(bool enable, long object_index)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->flag45 = enable;
		if (enable && object_index != NONE)
		{
			script->flag52 = true;
			script->type54 = 1;
			script->index58 = object_index;
			script->flag46 = false;
		}
	}
}

// @retail 0x277210
void function_277210(bool enable, long point_index)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		script->flag45 = enable;
		if (enable && point_index != NONE)
		{
			script->flag52 = true;
			script->type54 = 2;
			script->index58 = point_index;
			script->flag46 = false;
		}
	}
}

/* the names of something the command script being run does */
long const g_46fca8[7] = { 0x06000085, 0x06000084, 0x06000086, 0x04000089, 0x07000039, 0x070000c9, 0x0700002c };

// @retail 0x2775d0
void function_2775d0(short index)
{
	long script_index = g_502410;
	if (script_index != NONE)
	{
		s_command_script *script = command_script_get(script_index);
		if (index >= 0 && index < 7)
			script->name94 = g_46fca8[index];
		else
			script->name94 = NONE;
	}
}

// @retail 0x277620
void function_277620(short value, long index_a, long index_b, long index_c)
{
	long script_index = g_502410;
	if (script_index != NONE && value >= 0 && value < 6)
	{
		s_command_script *script = command_script_get(script_index);
		script->value_short = value;
		*(long *)&script->value8 = index_a;
		script->type = 0x12;
		script->index_a = index_b;
		script->index_b = index_c;
		script->flag5c = true;
	}
}