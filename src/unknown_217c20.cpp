// @flags /O2 /Gr
/* UNKNOWN_217C20.CPP: a lifecycle callback (entry 10, dispose) */

#include "unknown_11c920.h"
#include <string.h>
#include <xtl.h>
#include "globals.h"

byte g_51ea18[0x6f * 4];

// @retail 0x217c20
void function_217c20(void)
{
	memset(g_51ea18, 0, sizeof(g_51ea18));
}

struct s_gamepad_preferences
{
	real look_sensitivity_horizontal;
	real look_sensitivity_vertical;
	char button_map[16];
	short unknown18;
	bool unknown1a;
	bool unknown1b;
};
void input_preferences_set_defaults(s_gamepad_preferences *preferences);
extern bool g_51ebd0;
extern dword g_51ebc8;

bool g_51ebcc[4];

/* retail calls the defaults routine, which sits in a translation unit built
   without automatic inlining */
#pragma inline_depth(0)
// @retail 0x217bc0
void function_217bc0(void)
{
	memset(g_51ea18, 0, sizeof(g_51ea18));
	for (long index = 0; index != NONE; )
	{
		long const *index_reference = &index;
		input_preferences_set_defaults(&((s_gamepad_preferences *)g_51ea18)[*index_reference]);
		g_51ebcc[index] = g_4e61cc[(short)index] != 0;
		long next = NONE;
		if (index >= 0 && index < 3)
			next = index + 1;
		index = next;
	}
	g_51ebc8 = GetTickCount();
	g_51ebd0 = true;
}
#pragma inline_depth()
