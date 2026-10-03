// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1AC090.CPP: the slot handlers of types 0x2c, 0x2b and 0x54
   (0x47dbe8..0x47dcd4) */

#include "cseries.h"
#include "slot_handler.h"
#include "ai_actor.h"

/* the state of the slots of types 0x2c and 0x2b */
struct s_slot_2c
{
	s_slot_header header;
	bool unknown0c;
	bool unknown0d;
	short ticks;
	bool unknown10;
	byte unknown11;
	bool unknown12;
	bool unknown13;
	byte unknown14[0x20 - 0x14];
	short unknown20;
	short unknown22;
	bool unknown24;
	bool unknown25;
	byte unknown26[0x39 - 0x26];
	bool unknown39;
	bool unknown3a;
	bool unknown3b;
	byte unknown3c[0x40 - 0x3c];
};

/* the state of a slot of type 0x54 */
struct s_slot_54
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d;
	short ticks;
	byte unknown10[0x40 - 0x10];
};

struct s_ai_actor_view_3c
{
	byte unknown000[0x3c];
	bool unknown03c;
	byte unknown03d[0x20c - 0x3d];
	long unknown20c;
	byte unknown210[0x328 - 0x210];
	short unknown328;
	byte unknown32a[0x3f4 - 0x32a];
	long unknown3f4;
};

struct s_prop_datum_54
{
	byte unknown00[0x27];
	char unknown27;
	real unknown28;
	byte unknown2c[0x3c - 0x2c];
};

struct s_tag_element_54
{
	byte unknown00[0x98];
	real unknown98;
	real unknown9c;
};

/* the unit's current mode: a short at +0x36 of its current state, whose
   offset is at +0x346 */
#define UNIT_MODE(unit_index) \
	(*(short *)((byte *)ai_object_get(unit_index) + 0x36 + *(short *)((byte *)ai_object_get(unit_index) + 0x346)))

#define ACTOR_VIEW_3C(actor) ((s_ai_actor_view_3c *)(actor))

bool function_e68c0(long type, long unit_index);
void function_26def0(long actor_index);
bool function_1f86f0(long index);
short __stdcall function_1ac100(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1ac430(long actor_index, s_slot *slot);
short __stdcall function_1ac570(long actor_index, s_slot *slot, bool active);
void __stdcall function_1acda0(long actor_index, s_slot *slot);
void __stdcall function_1acfd0(long actor_index, s_slot *slot);
void __stdcall function_1ad130(long actor_index, s_slot *slot);
void __stdcall function_1ad6a0(long actor_index, s_slot *slot);
void __stdcall function_1ada70(long actor_index, s_slot *slot);

/* ---- slot type 0x2c ---- */

// @retail 0x1ac090
bool __stdcall function_1ac090(long actor_index, s_slot *slot)
{
	s_slot_2c *state = (s_slot_2c *)slot;
	dword *seed = &g_4e7408->unknown0;
	long ticks;

	*seed = 1664525 * *seed + 1013904223;
	real seconds = ((real)(*seed >> 16) * (1.f / 65535.f) + 1.0f) * 2.0f * g_510c54->ticks_per_second;
	__asm
	{
		fld seconds
		fistp ticks
	}
	state->ticks = (short)ticks;
	state->unknown39 = false;
	return true;
}

// @retail 0x1ac270
void __stdcall function_1ac270(long actor_index, s_slot *slot)
{
	s_slot_2c *state = (s_slot_2c *)slot;

	if (!state->unknown39)
	{
		s_ai_actor *actor = ai_actor_get(actor_index);
		s_unit_request request = {0};
		request.type = 0x27;
		function_e6900(actor->unit_index, &request);
		if (!ACTOR_VIEW_3C(actor)->unknown03c && ACTOR_VIEW_3C(actor)->unknown3f4 != NONE)
			function_26def0(actor_index);
	}
}

// @retail 0x1ac300
bool __stdcall function_1ac300(long actor_index, s_slot *slot)
{
	long unit_index = ai_actor_get(actor_index)->unit_index;
	short mode = UNIT_MODE(unit_index);
	bool result = false;

	if (mode == 6)
		return function_e68c0(0x25, unit_index);
	if (mode == 7)
		result = true;
	return result;
}

// @retail 0x1ac360
void __stdcall function_1ac360(long actor_index, s_slot *slot)
{
	s_slot_2c *state = (s_slot_2c *)slot;

	if (state->ticks > 0)
		state->ticks--;
}

// @retail 0x1ac380
void __stdcall function_1ac380(long actor_index, s_slot *slot)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	long unit_index = actor->unit_index;

	actor->unknown41c = 4;
	actor->unknown420 = 2;
	if (UNIT_MODE(unit_index) == 7)
		actor->unknown488 = true;
}

/* ---- slot type 0x2b ---- */

// @retail 0x1ac3f0
short __stdcall function_1ac3f0(long actor_index)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	short result = 0;

	if (actor->prop_index != NONE && !actor->unknown007)
		result = 3;
	return result;
}

// @retail 0x1acd30
bool function_1acd30(long actor_index, s_prop_datum_54 *prop)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	bool result = false;

	if (ACTOR_VIEW_3C(actor)->unknown328 >= 12)
	{
		result = true;
	}
	else if (UNIT_MODE(actor->unit_index) == 5)
	{
		if (2.0f > prop->unknown28)
			result = true;
	}
	else if (prop->unknown27 >= 1)
	{
		result = true;
	}
	return result;
}

// @retail 0x1ad400
void __stdcall function_1ad400(long actor_index, s_slot *slot)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	s_slot_2c *state = (s_slot_2c *)slot;
	long unit_index = actor->unit_index;

	ACTOR_VIEW_3C(actor)->unknown20c = g_510c54->game_time;
	short mode = UNIT_MODE(unit_index);
	if (!state->unknown39)
	{
		if (mode == 6 || mode == 7)
			function_e68c0(0x27, unit_index);
		if (!ACTOR_VIEW_3C(actor)->unknown03c && ACTOR_VIEW_3C(actor)->unknown3f4 != NONE)
			function_26def0(actor_index);
	}
}

// @retail 0x1ad4a0
void __stdcall function_1ad4a0(long actor_index, s_slot *slot)
{
	s_slot_2c *state = (s_slot_2c *)slot;

	if (state->unknown25)
		state->unknown25 = false;
}

/* ---- slot type 0x54 ---- */

// @retail 0x1ad4c0
bool function_1ad4c0(long actor_index, long prop_index)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	bool result = false;
	s_tag_element_54 *element = (s_tag_element_54 *)function_1e5450(actor_index, ai_object_get(actor->unknown26c)->definition_index);

	if (element)
	{
		s_prop_datum_54 *prop = (s_prop_datum_54 *)prop_node_get(prop_index);
		real range = element->unknown98 > g_45dbd8 ? element->unknown98 : 20.0f;
		return prop->unknown28 > range;
	}
	return result;
}

// @retail 0x1ad550
short __stdcall function_1ad550(long actor_index)
{
	short result = 0;

	if (ai_actor_get(actor_index)->prop_index != NONE)
		result = 3;
	return result;
}

// @retail 0x1ad590
bool __stdcall function_1ad590(long actor_index, s_slot *slot)
{
	s_slot_54 *state = (s_slot_54 *)slot;

	state->unknown0c = false;
	state->ticks = 0;
	return true;
}

// @retail 0x1ad5b0
short __stdcall function_1ad5b0(long actor_index, s_slot *slot, bool active)
{
	s_ai_actor *actor = ai_actor_get(actor_index);
	s_slot_54 *state = (s_slot_54 *)slot;

	if (actor->unknown26c == NONE)
		return g_46fbe4;

	s_tag_element_54 *element = (s_tag_element_54 *)function_1e5450(actor_index, ai_object_get(actor->unknown26c)->definition_index);
	if (actor->prop_index == NONE || !element)
		return g_46fbe4;

	short result = g_46fbe8;
	if (function_1ad4c0(actor_index, actor->prop_index))
		state->ticks++;
	else
		state->ticks = 0;
	if (state->unknown0c && function_1f86f0(actor_index) ||
		(real)state->ticks * g_510c54->rate > element->unknown9c)
	{
		return 0xe;
	}
	return result;
}

/* ---- the handlers ---- */

s_slot_handler_2 g_47dbe8 =
{
	{
		0x2c, 2, 0, -2, 0,
		0, function_1ac100, function_1ac090, function_1ac270, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	(t_slot_proc)function_1ac300, function_1ac360, function_1ac380
};

s_slot_handler_2 g_47dc38 =
{
	{
		0x2b, 2, 0, -2, 0,
		function_1ac3f0, function_1ac570, function_1ac430, function_1ad400, NONE, {0},
		0, 0, 0, function_1ad4a0, 0, 0, 0
	},
	function_1acda0, function_1acfd0, function_1ad130
};

s_slot_handler_2 g_47dc88 =
{
	{
		0x54, 2, 0, -2, 0,
		function_1ad550, function_1ad5b0, function_1ad590, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1ad6a0, 0, function_1ada70
};
