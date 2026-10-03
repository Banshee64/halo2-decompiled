// @flags /O2 /Ob1 /Gr
/* UNKNOWN_18C250.CPP: the sound source types (the callback tables at
   0x444afc..0x444b9c) and their callbacks */

#include "cseries.h"
#include "globals.h"
#include <string.h>

#define FALSE 0
#define TRUE 1

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
	byte unknown04[4];
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
void function_d0dc0(long object_index, long value);
// @retail 0x18c8c0
void __stdcall function_18c8c0(long object_index, long source_index, long unused)
{
	if (g_4ed28c->valid && function_badc0(object_index, 3))
	{
		s_looping_sound_source *source = (s_looping_sound_source *)g_4e637c->data + (source_index & 0xffff);
		function_d0dc0(object_index, source->value0c);
	}
}

// @retail 0x18c910
void __stdcall function_18c910(long object_index, long source_index, long unused)
{
	if (object_index != NONE)
	{
		function_18c8c0(object_index, source_index, unused);
	}

	for (long i = 0; i < 16; i++)
	{
		s_looping_sound_slot *slot = &g_4ed288->slots[i];
		if (slot->source_index == source_index)
		{
			slot->datum_index = NONE;
			slot->source_index = NONE;
			return;
		}
	}
}

// @retail 0x18c960
bool __stdcall function_18c960(void const *a, void const *b)
{
	return memcmp(a, b, 0x28) == 0;
}

struct s_sound_source_state
{
	byte unknown00[0x24];
	long value24;
};

// @retail 0x18c980
long __stdcall function_18c980(long a, s_sound_source_state const *state_a, long b, s_sound_source_state const *state_b)
{
	if (state_a->value24 != NONE && state_b->value24 != NONE && state_a->value24 == state_b->value24)
	{
		return TRUE;
	}
	return FALSE;
}

bool __stdcall function_18c250(long object_index, long tag_index, long a, void *b);
bool __stdcall function_18c3b0(long object_index, long tag_index, long a, void *b);
void __stdcall function_18c630(long object_index, long tag_index, long a, long b);
void __stdcall function_18c6a0(long object_index, long tag_index, long a, long b, long c, long d);
void __stdcall function_23f120(long a, long b, long c);

// @retail 0x18c810
long __stdcall function_18c810(long object_index, long tag_index, s_sound_source_view const *source, s_sound_spatialization_view *spatialization)
{
	if (source->flags & 1)
	{
		s_sound_tag *sound = (s_sound_tag *)((s_tag_instance_view *)g_4e3b44)[tag_index & 0xffff].data;
		s_sound_class_view *sound_class = &((s_sound_globals_view *)g_51ebd4)->classes[sound->class_index];
		s_sound_class_spatialization *class_spatialization = sound_class_get_spatialization(sound_class);

		if (class_spatialization)
		{
			if (class_spatialization->flags & 1)
			{
				spatialization->flags |= 2;
			}
			if (TEST_FIELD_BIT(class_spatialization->bits.bit1))
			{
				spatialization->value4 = class_spatialization->value8;
			}
			if (TEST_FIELD_BIT(class_spatialization->bits.bit2))
			{
				spatialization->flags |= 1;
				spatialization->value8 = class_spatialization->valuec;
			}
		}
	}
	if (source->flags & 2)
	{
		spatialization->flags = 1;
		spatialization->value8 = 0.0f;
		spatialization->value4 = -64.0f;
		spatialization->valuec = source->value1c;
		spatialization->value10 = source->value20;
	}
	return source->flags != 0;
}

/* the sound source types: what a playing sound asks of its source */
struct s_sound_source_callbacks
{
	bool (__stdcall *update)(long object_index, long tag_index, long a, void *b);
	void (__stdcall *proc1)(long object_index, long tag_index, long a, long b);
	void (__stdcall *proc2)(long object_index, long tag_index, long a, long b, long c, long d);
	long (__stdcall *spatialize)(long object_index, long tag_index, s_sound_source_view const *source, s_sound_spatialization_view *spatialization);
	void (__stdcall *stop)(long object_index, long source_index, long unused);
	void *proc5;
	bool (__stdcall *compare)(void const *a, void const *b);
	long (__stdcall *same_source)(long a, s_sound_source_state const *state_a, long b, s_sound_source_state const *state_b);
};

extern s_sound_source_callbacks const g_444afc = { function_18c3b0, function_18c630, function_18c6a0, NULL, function_18c8c0, NULL, function_18c960, NULL };
extern s_sound_source_callbacks const g_444b1c = { function_18c3b0, function_18c630, function_18c6a0, NULL, function_18c8c0, NULL, function_18c960, function_18c980 };
extern s_sound_source_callbacks const g_444b3c = { function_18c250, function_18c630, function_18c6a0, function_18c810, function_18c910, NULL, NULL, NULL };
extern s_sound_source_callbacks const g_444b5c = { function_18c250, function_18c630, function_18c6a0, function_18c810, function_18c8c0, NULL, NULL, NULL };
extern s_sound_source_callbacks const g_444b7c = { NULL, NULL, NULL, NULL, function_23f120, NULL, NULL, NULL };