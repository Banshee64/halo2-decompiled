// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_1887D0.CPP: the object looping sounds: the data array g_4ed28c
   ("object looping sounds", 0x400 elements of 0x18 bytes) and the state
   g_4ed288 (0x244 bytes); the lifecycle callbacks of entry 46, the sound
   class gains they reset, and the looping sound callbacks of the sound
   source tables */

#include "cseries.h"
#include "globals.h"
#include "game_state.h"
#include <string.h>

#define FALSE 0
#define TRUE 1

struct s_object;
class c_engine_peer;

extern s_data_array *g_4e637c;

struct s_looping_sound_source
{
	byte unknown00[0xc];
	long value0c;
	byte unknown10[0xbc - 0x10];
};

s_object *function_badc0(long object_index, dword type_mask);
void function_d0dc0(long object_index, long value);
void __stdcall function_1889d0(dword flags);
void __stdcall function_18bb80(real value);
void function_188d60(void);
struct s_sound_driver_volumes;
void function_220fd0(s_sound_driver_volumes const *volumes);
long function_2197f0(real gain);
void __stdcall function_221980(char const *name, long value_bits, real time);

static inline void data_make_valid_inlined(s_data_array *data)
{
	data->valid = true;
	data_delete_all(data);
}

static __forceinline short real_to_short(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return (short)result;
}

// @retail 0x1887d0
void object_looping_sounds_initialize(void)
{
	g_4ed28c = data_new_inlined("object looping sounds", 0x400, 0x18, 0, g_510c2c);
	g_4ed288 = (s_looping_sound_globals *)game_state_malloc("object looping sounds", NULL, sizeof(s_looping_sound_globals));
}

// @retail 0x188870
void object_looping_sounds_initialize_for_new_map(void)
{
	s_data_array *data = g_4ed28c;

	if (data)
	{
		data_make_valid_inlined(data);

		s_looping_sound_globals *globals = g_4ed288;
		for (long i = 0; i < 8; i++)
		{
			globals->indices[i] = NONE;
		}
		globals->value20 = 0;
		globals->value24 = 0;

		short scale = real_to_short(0.0f);
		for (long j = 0; j < 0x80; j++)
		{
			globals->scales[j] = scale;
		}
		memset(globals->slots, 0xff, sizeof(globals->slots));

		for (long k = 0; k < 4; k++)
		{
			globals->gains[k] = 1.0f;
		}
		globals->value238 = 8000;
		globals->value23c = 0.0f;
		function_220fd0((s_sound_driver_volumes const *)globals->gains);
	}
	function_188d60();
}

// @retail 0x188940
void object_looping_sounds_dispose_from_old_map(void)
{
	s_data_array *data = g_4ed28c;

	if (data && data->valid)
	{
		function_1889d0(0);
		data->valid = false;
		g_4ed288->value20 = 0;
		g_4ed288->value24 = 0;
	}
}

// @retail 0x188980
void object_looping_sounds_initialize_for_new_structure_bsp(void)
{
	if (g_4ed288)
	{
		function_18bb80(0.0f);

		short scale = real_to_short(0.0f);
		for (long i = 0; i < 0x80; i++)
		{
			g_4ed288->scales[i] = scale;
		}
	}
}

// @retail 0x188cd0
void function_188cd0(void)
{
	if (g_55e4d0[g_4e9ae8->engine_index])
	{
		function_221980("", function_2197f0(0.0f), 1.0f);
		function_221980("ambient_nature", function_2197f0(0.2f), 1.0f);
		function_221980("ambient_machinery", function_2197f0(0.2f), 1.0f);
		function_221980("ambient_computers", function_2197f0(0.2f), 1.0f);
	}
}

// @retail 0x188d60
void function_188d60(void)
{
	if (g_55e4d0[g_4e9ae8->engine_index])
	{
		function_221980("", function_2197f0(0.0f), 0.0f);
		function_221980("", function_2197f0(1.0f), 2.0f);
		function_221980("scripted_dialog_player", function_2197f0(1.0f), 0.0f);
	}
}

// @retail 0x188790
short function_188790(real angle)
{
	real clamped = angle < 0.0f ? 0.0f : (angle > 3.1415927f ? 3.1415927f : angle);

	return real_to_short(clamped * 20860.438f);
}

// @retail 0x18cac0
real function_18cac0(long index)
{
	long scale = g_4ed288->scales[index];

	if (scale == 0)
	{
		return 0.0f;
	}
	if (scale >= 0xffff)
	{
		return 1.0f;
	}
	return ((real)(0xffff - scale) * 0.0f + (real)scale) * (1.0f / 65535.0f);
}
