// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_10E480.CPP: start an object's vibration from a set of curves.
   Decompiled by lane F for 0x18ca20; its callee 0x10de40 is in
   unknown_10dc70.cpp, whose file this function probably shares. */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_11a4d0.h"

struct s_object_list;
extern s_object_list *g_4de2f4;

struct s_vibration_curve_set;
struct s_vibration_output;
void function_10de40(s_vibration_curve_set *set, s_vibration_output *output, real time, real blend, bool skip_special);

struct s_vibration_object_view
{
	long definition_index;
	byte unknown04[0x33e - 4];
	short vibration_offset;
};

struct s_vibration_object_header
{
	byte unknown00[8];
	s_vibration_object_view *object;
};

struct s_vibration_view
{
	byte unknown00[0x30];
	byte output[6];
	short index;
	long tag_index;
};

struct s_vibration_definition_view
{
	byte unknown00[0x1e4];
	real blend;
};

struct s_vibration_globals_view
{
	byte unknown00[0x81];
	bool enabled;
};

// @retail 0x10e480
void function_10e480(long object_index, long tag_index, s_vibration_curve_set *curves, real time)
{
	s_vibration_object_view *object = ((s_vibration_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_vibration_view *vibration = (s_vibration_view *)((byte *)object + object->vibration_offset);
	s_vibration_definition_view *definition = (s_vibration_definition_view *)g_4e3b44[object->definition_index & 0xffff].bytes;
	real blend = 0.95f;

	if (definition->blend > 0.0f)
	{
		blend = definition->blend;
	}
	vibration->tag_index = tag_index;
	if (vibration->index != NONE)
	{
		bool skip_special = false;

		if (((s_vibration_globals_view *)g_4de2f4)->enabled && function_11b930(object_index))
		{
			skip_special = true;
		}
		function_10de40(curves, (s_vibration_output *)vibration->output, time, blend, skip_special);
	}
}
