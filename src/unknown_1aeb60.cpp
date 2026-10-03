// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_1AEB60.CPP: the slot handlers of types 0xe and 0xf
   (0x47de20..0x47dec0) */

#include "cseries.h"
#include "slot_handler.h"
#include "ai_actor.h"

/* the state of a slot of type 0xe */
struct s_slot_0e
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[0x40 - 0xd];
};

/* the state of a slot of type 0xf */
struct s_slot_0f
{
	s_slot_header header;
	bool unknown0c;
	bool unknown0d;
	short ticks;
	short timer;
	bool unknown12;
	bool unknown13;
	bool unknown14;
	bool unknown15;
	bool unknown16;
	bool unknown17;
	byte unknown18[0x34 - 0x18];
	bool unknown34;
	byte unknown35[0x40 - 0x35];
};

struct s_prop_datum_0f
{
	byte unknown00[0x24];
	short unknown24;
	byte unknown26;
	char unknown27;
	byte unknown28[0x3c - 0x28];
};

bool function_1f8660(long index);
short __stdcall function_1aec30(long actor_index, short level, bool active);
void __stdcall function_1af810(long actor_index, s_slot *slot);
void __stdcall function_1afb30(long actor_index, s_slot *slot);
void __stdcall function_1afcf0(long actor_index, s_slot *slot);

/* ---- slot type 0xe ---- */

// @retail 0x1aeb60
short __stdcall function_1aeb60(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE && prop_node_get(actor->prop_index)->type == 1)
		result = 3;
	return result;
}

// @retail 0x1aebb0
short __stdcall function_1aebb0(long actor_index, s_slot *slot, bool active)
{
	return function_1aeb60(actor_index) > 0 ? g_46fbe8 : g_46fbe4;
}

// @retail 0x1aebd0
bool __stdcall function_1aebd0(long actor_index, s_slot *slot)
{
	s_slot_0e *state = (s_slot_0e *)slot;
	long i = 0;

	actor_get(actor_index)->unknown084 = 4;
	s_actor_view *actor = actor_get(actor_index);
	actor->unknown3fe = 3;
	do
	{
		actor->unknown400[i].reference = g_470fa0;
		i++;
	}
	while (i < 4);
	state->unknown0c = false;
	return true;
}

/* ---- slot type 0xf ---- */

// @retail 0x1aef40
short __stdcall function_1aef40(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (function_1e2030(actor_index) && actor->prop_index != NONE)
	{
		s_prop_datum_0f *prop = (s_prop_datum_0f *)prop_node_get(actor->prop_index);
		if (prop->unknown24 >= 1 && prop->unknown24 <= 2 && prop->unknown27 >= 1)
			result = 3;
	}
	return result;
}

// @retail 0x1aefc0
bool __stdcall function_1aefc0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_0f *state = (s_slot_0f *)slot;
	dword *seed = &g_4e7408->unknown0;
	long rounded;

	*seed = 1664525 * *seed + 1013904223;
	real seconds = (real)(*seed >> 16) * (1.f / 65535.f) * 7.0f + 3.0f;
	real timer = (real)g_510c54->ticks_per_second * 0.2f;
	__asm
	{
		fld timer
		fistp rounded
	}
	state->timer = (short)rounded;
	real ticks = g_510c54->ticks_per_second * seconds;
	__asm
	{
		fld ticks
		fistp rounded
	}
	state->ticks = (short)rounded;
	state->unknown12 = false;
	state->unknown13 = false;
	state->unknown34 = false;
	state->unknown16 = false;
	state->unknown17 = false;
	state->unknown14 = false;
	state->unknown15 = !actor->unknown5d4;
	return true;
}

// @retail 0x1af0a0
short __stdcall function_1af0a0(long actor_index, s_slot *slot, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_0f *state = (s_slot_0f *)slot;
	short result = g_46fbe4;

	if (actor->prop_index != NONE)
	{
		s_prop_datum_0f *prop = (s_prop_datum_0f *)prop_node_get(actor->prop_index);
		short other = g_46fbe8;
		if (state->unknown12)
		{
			state->timer = 3;
			state->ticks = 2;
			state->unknown0d = true;
			state->unknown12 = true;
			state->unknown0c = true;
			result = 5;
		}
		else if (prop->unknown27 >= 2 ||
			!REFERENCE_EQUAL(actor->unknown418, g_470fa0) && function_1f8660(actor_index) && !state->unknown13)
		{
			result = other;
		}
	}
	return result;
}

/* the actor's weapon entry (function_1e5240) as 0x1af530 reads it */
struct s_weapon_entry_0f
{
	byte unknown00[0x14];
	long unknown14;
	long unknown18;
	byte unknown1c[0x24 - 0x1c];
	long unknown24;
	long unknown28;
	long unknown2c;
	long unknown30;
};

/* the squad's variant (function_272a00) */
struct s_squad_variant_0f
{
	byte unknown00[0x24];
	short unknown24;
	byte unknown26[0x31 - 0x26];
	char unknown31;
};

real function_1f8940(long actor_index);
void *function_272a00(long actor_index);

// @retail 0x1af530
bool function_1af530(long actor_index, long *b, long *a)
{
	s_weapon_entry_0f *entry = (s_weapon_entry_0f *)function_1e5240(actor_index);
	bool result = false;

	if (entry)
	{
		s_squad_variant_0f *variant = (s_squad_variant_0f *)function_272a00(actor_index);

		*a = entry->unknown18;
		*b = entry->unknown14;
		if (variant)
		{
			short index = variant->unknown31 <= 0 ? variant->unknown24 : variant->unknown31 - 1;

			switch (index)
			{
			case 1:
				*a = entry->unknown28;
				*b = entry->unknown24;
				break;
			case 2:
				*a = entry->unknown30;
				*b = entry->unknown2c;
				break;
			}
		}
		result = true;
	}
	return result;
}

// @retail 0x1afcf0
void __stdcall function_1afcf0(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_0f *state = (s_slot_0f *)slot;

	if (actor->prop_index != NONE)
	{
		s_prop_datum_0f *prop = (s_prop_datum_0f *)prop_node_get(actor->prop_index);

		actor->unknown41c = 2;
		actor->unknown420 = 2;
		actor->unknown4a2 = true;
		if (prop->unknown27 >= 1)
		{
			actor->unknown488 = true;
			actor->unknown41c = 4;
			actor->unknown420 = 2;
		}
		else
		{
			if (!function_1f8660(actor_index) || 2.0f > function_1f8940(actor_index))
			{
				actor->unknown41c = 4;
				actor->unknown420 = 2;
			}
			if (actor->unknown229)
				actor->unknown482 = true;
		}
		if (actor->unknown5d0)
			state->unknown15 = false;
		if (state->unknown15)
			actor->unknown449 = false;
	}
}

/* ---- the handlers ---- */

s_slot_child g_46f440[27] =
{
	{0x2e, 1, NONE, {0}, 0.0f, 0, 0},
	{0x30, 1, NONE, {0}, 0.0f, 0, 0},
	{0x55, 1, NONE, {0}, 0.0f, 0, 0},
	{0x56, 1, NONE, {0}, 0.0f, 0, 0},
	{0x57, 1, NONE, {0}, 0.0f, 0, 0},
	{0x35, 1, NONE, {0}, 0.0f, 0, 0},
	{0x34, 1, NONE, {0}, 0.0f, 0, 0},
	{0x15, 1, NONE, {0}, 0.0f, 0, 0},
	{0x50, 1, 13, {0}, 1.0f, 0, 0},
	{0x33, 1, NONE, {0}, 0.0f, 0, 0},
	{0x13, 1, NONE, {0}, 0.0f, 0, 0},
	{0x14, 1, NONE, {0}, 0.0f, 0, 0},
	{0x12, 1, NONE, {0}, 0.0f, 0, 0},
	{0x4f, 1, NONE, {0}, 0.0f, 0, 0},
	{0x11, 1, NONE, {0}, 0.0f, 0, 0},
	{0x10, 1, NONE, {0}, 0.0f, 0, 0},
	{0x31, 1, NONE, {0}, 0.0f, 0, 0},
	{0xf, 1, NONE, {0}, 0.0f, 0, 0},
	{0x58, 1, NONE, {0}, 0.0f, 0, 0},
	{0x52, 1, NONE, {0}, 0.0f, 0, 0},
	{0x51, 1, NONE, {0}, 0.0f, 0, 0},
	{0x1b, 1, NONE, {0}, 0.0f, 0, 0},
	{0x22, 1, NONE, {0}, 0.0f, 0, 0},
	{0x16, 1, NONE, {0}, 0.0f, 0, 0},
	{0x5f, 0, NONE, {0}, 0.0f, 0, 0},
	{0x60, 1, NONE, {0}, 0.0f, 0, 0},
	{0x5, 1, NONE, {0}, 0.0f, 0, 0},
};

s_slot_handler_1 g_47de20 =
{
	{
		0xe, 1, 0, -2, 0,
		function_1aeb60, function_1aebb0, function_1aebd0, 0, 1, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1aec30, 27, g_46f440
};

s_slot_handler_2 g_47de70 =
{
	{
		0xf, 2, 0, -2, 0,
		function_1aef40, function_1af0a0, function_1aefc0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1af810, function_1afb30, function_1afcf0
};
