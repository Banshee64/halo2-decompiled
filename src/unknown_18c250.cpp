// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_18C250.CPP: the sound source types (the callback tables at
   0x444afc..0x444b9c) and their callbacks */

#include "cseries.h"
#include "globals.h"
#include <string.h>

#define FALSE 0
#define TRUE 1

struct s_object;

struct s_looping_sound_slot
{
	long a;
	long b;
	long datum_index;
	long d;
};

struct s_looping_sound_globals
{
	long indices[8];
	long value20;
	long value24;
	word scales[0x80];
	s_looping_sound_slot slots[16];
	real gains[4];
	long value238;
	real value23c;
	long value240;
};

extern s_looping_sound_globals *g_4ed288;
extern s_data_array *g_4e637c;

struct s_looping_sound_source
{
	byte unknown00[0xc];
	long value0c;
	byte unknown10[0xbc - 0x10];
};

s_object *function_badc0(long object_index, dword type_mask);
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
		if (slot->datum_index == source_index)
		{
			slot->a = NONE;
			slot->datum_index = NONE;
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
bool __stdcall function_18c810(long object_index, long tag_index, void *a, void *b);
void __stdcall function_23f120(long a, long b, long c);

/* the sound source types: what a playing sound asks of its source */
struct s_sound_source_callbacks
{
	bool (__stdcall *update)(long object_index, long tag_index, long a, void *b);
	void (__stdcall *proc1)(long object_index, long tag_index, long a, long b);
	void (__stdcall *proc2)(long object_index, long tag_index, long a, long b, long c, long d);
	bool (__stdcall *proc3)(long object_index, long tag_index, void *a, void *b);
	void (__stdcall *stop)(long object_index, long source_index, long unused);
	void *proc5;
	bool (__stdcall *compare)(void const *a, void const *b);
	long (__stdcall *same_source)(long a, s_sound_source_state const *state_a, long b, s_sound_source_state const *state_b);
};

s_sound_source_callbacks const g_444afc = { function_18c3b0, function_18c630, function_18c6a0, NULL, function_18c8c0, NULL, function_18c960, NULL };
s_sound_source_callbacks const g_444b1c = { function_18c3b0, function_18c630, function_18c6a0, NULL, function_18c8c0, NULL, function_18c960, function_18c980 };
s_sound_source_callbacks const g_444b3c = { function_18c250, function_18c630, function_18c6a0, function_18c810, function_18c910, NULL, NULL, NULL };
s_sound_source_callbacks const g_444b5c = { function_18c250, function_18c630, function_18c6a0, function_18c810, function_18c8c0, NULL, NULL, NULL };
s_sound_source_callbacks const g_444b7c = { NULL, NULL, NULL, NULL, function_23f120, NULL, NULL, NULL };