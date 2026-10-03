/* SOUND_SOURCES.H: the views shared by the sound source callbacks
   (unknown_18c250.cpp, unknown_18c810.cpp) */
#ifndef SOUND_SOURCES_H
#define SOUND_SOURCES_H

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

struct s_object;

extern s_data_array *g_4e637c;

struct s_looping_sound_source
{
	byte unknown00[0xc];
	long value0c;
	byte unknown10[0xbc - 0x10];
};

s_object *function_badc0(long object_index, dword type_mask);

struct s_tag_instance_view
{
	dword group_tag;
	byte unknown04[4];
	byte *data;
	byte unknown0c[4];
};

struct s_sound_tag
{
	byte unknown00[6];
	short class_index;
};

/* the spatialization of a sound class */
struct s_sound_class_spatialization
{
	union
	{
		byte flags;
		struct
		{
			dword bit0 : 1;
			dword bit1 : 1;
			dword bit2 : 1;
		} bits;
	};
	real angle;
	real value8;
	real valuec;
};

struct s_sound_class_view
{
	byte unknown00[0x28];
	s_sound_class_spatialization spatialization;
	byte unknown38[0x38 - 0x38];
};

struct s_sound_globals_view
{
	byte unknown00[4];
	s_sound_class_view *classes;
};

struct s_sound_source_view
{
	byte unknown00;
	byte flags;
	byte unknown02[0x1a];
	long value1c;
	long value20;
};

struct s_sound_spatialization_view
{
	dword flags;
	real value4;
	real value8;
	long valuec;
	long value10;
};

static inline s_sound_class_spatialization *sound_class_get_spatialization(s_sound_class_view *sound_class)
{
	return (sound_class->spatialization.flags & 7) ? &sound_class->spatialization : NULL;
}

/* where a sound plays from: what the update callbacks fill in */
struct s_sound_location
{
	byte flag0 : 1;
	byte flag1 : 1;
	byte unknown01[2];
	char unknown03_low : 4;
	char unknown03_high : 4;
	byte unknown04[8];
	real_vector3d forward;
	dword compressed_forward;
	real_vector3d up;
	long value28;
	long value2c;
};

bool __stdcall function_18c250(long object_index, long tag_index, long a, s_sound_location *location);
bool __stdcall function_18c3b0(long object_index, long tag_index, long a, s_sound_location *location);
void __stdcall function_18c630(long object_index, long tag_index, long a, long b);
void __stdcall function_18c6a0(long object_index, long tag_index, long a, long b, long c, long d);

#endif
