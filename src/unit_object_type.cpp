// @flags /O2 /arch:SSE /Gr
/* UNIT_OBJECT_TYPE.CPP: the unit object type

The unit object type's callbacks (its definition at 0x4679b0) and the unit
functions around them: unit placement, creation, update, seats, inventory,
weapons and grenades, melee, aiming, custom animations and scripting. The
functions upstream already has stay in their files (units.cpp,
unknown_0c7070.cpp, unknown_0c86e0.cpp, unknown_0c8880.cpp,
unknown_0cafc0.cpp, unknown_0cbd50.cpp, unknown_0cc2b0.cpp,
unknown_0cd660.cpp, unknown_0d0690.cpp, unknown_0d0e00.cpp). */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_0259d0.h"
#include "units.h"
#include "unit_requests.h"
#include "object_markers.h"
#include "unknown_1c62f0.h"
#include <string.h>

/* the unit (the object fields, then the unit's own; the fields read here) */
struct s_unit
{
	long definition_index;
	dword object_flags;
	byte unknown008[4];
	long next_sibling_index;
	long first_child_index;
	long parent_index;
	byte unknown018[0xaa - 0x18];
	byte type;
	byte unknownab[0xd4 - 0xab];
	long unknown0d4;
	byte unknownd8[0xec - 0xd8];
	real unknownec;
	byte unknownf0[0x10a - 0xf0];
	byte flags_10a;
	byte unknown10b[0x12a - 0x10b];
	short animation_offset;
	long actor_index;
	long unknown130;
	dword flags_134;
	byte unknown138[0x13c - 0x138];
	long unknown13c;
	byte unknown140[0x168 - 0x140];
	vector3f unknown168;
	byte unknown174[0x1ec - 0x174];
	long unknown1ec;
	long unknown1f0;
	byte unknown1f4[0x1fc - 0x1f4];
	short parent_seat_index;
	byte unknown1fe[0x210 - 0x1fe];
	short unknown210;
	char current_weapon_index;
	char next_weapon_index;
	short unknown214;
	char unknown216;
	char unknown217;
	long weapon_object_indices[4];
	byte unknown228[0x238 - 0x228];
	long unknown238;
	char current_grenade_index;
	char next_grenade_index;
	char grenade_counts[2];
	char unknown240;
	byte unknown241[0x248 - 0x241];
	long unknown248;
	long unknown24c;
	byte unknown250[0x270 - 0x250];
	point3f unknown270;
	vector3f unknown27c;
	byte unknown288[0x2a8 - 0x288];
	long unknown2a8;
	long unknown2ac;
	byte unknown2b0[0x2bc - 0x2b0];
	char unknown2bc;
	byte unknown2bd;
	short unknown2be;
	byte unknown2c0[0x33e - 0x2c0];
	short unknown33e;
	byte unknown340[0x348 - 0x340];
	byte flags_348;
};

/* the unit's animation state, at the offset +0x12a holds */
struct s_unit_animation
{
	long unknown00;
	byte unknown04[2];
	short unknown06;
	byte unknown08[0x68 - 0x8];
	long unknown68;
	byte unknown6c[0x7c - 0x6c];
	long name;
};

struct s_unit_header
{
	byte unknown00[8];
	s_unit *unit;
};

#define UNIT_GET(index) (((s_unit_header *)g_4e0300->data)[(index) & 0xffff].unit)
#define UNIT_DEFINITION_GET(unit) (g_4e3b44[(unit)->definition_index & 0xffff].bytes)
#define UNIT_SEAT_COUNT(definition) (*(long *)((definition) + 0x1c8))
#define UNIT_SEATS(definition) (*(s_unit_seat_definition **)((definition) + 0x1cc))
#define UNIT_ANIMATION(unit) ((s_unit_animation *)((byte *)(unit) + (unit)->animation_offset))

/* a float rounded to an integer as the x87 does (fld, fistp) */
__forceinline long unit_round(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return result;
}

/* the units among an object's children, with their seats (as damage.cpp) */
struct s_unit_child_iterator
{
	long object_index;
	long unit_index;
	short seat_index;
	long next_index;
};

struct s_damage_object;

void function_ccab0(long unit_index);
void __stdcall function_cc810(long vehicle_index);
bool function_c48f0(long unit_index);
bool __stdcall function_c49b0(long unit_index);
bool __stdcall function_c58f0(long unit_index);
bool __stdcall function_c60c0(long unit_index);
bool function_c5eb0(long unit_index);
bool function_c6740(long unit_index);
void function_114c60(long unit_index);
void function_c6990(long unit_index);
void function_c6810(long unit_index);
void function_c50a0(long unit_index);
bool function_0c7070(long unit_index);
void function_b8b70(long object_index);
void function_bb950(long object_index, bool add, long delta);
void function_db5c0(long object_index);
void function_e4bd0(long biped_index);
void __stdcall function_b8540(long object_index);
void function_ce920(long unit_index, long slot_index, long mode, bool flag);
bool function_e4050(long object_index);
bool function_e6900(long unit_index, s_unit_request *request);
void function_ccf20(long unit_index);
void function_b58c0(long index, dword mask);
void function_c8bb0(long unit_index, long a, long *object_index, long *seat_index, long *result, real *distance,
	bool *flag);
bool function_d1080(long unit_index, transform4x3f *matrix, void *unknown);
void function_e69c0(long unit_index, long type);
void function_114240(long unit_index);
void function_a94b0(long unit_index);
void function_a9440(long unit_index, long player_index);
void function_a7bc0(long unit_index);
void function_a9500(long unit_index, long index);

struct s_sound_label_play
{
	long label;
	long tag_index;
	real scale;
	char const *variant;
};

long function_189210(long object_index, long marker_name, s_sound_label_play const *play);

/* sends the unit request 8 between two updates of the unit */
// @retail 0xc4820
void __stdcall function_c4820(long unit_index)
{
	s_unit_request request;

	function_ccab0(unit_index);
	memset(&request, 0, sizeof(request));
	request.type = 8;
	function_e6900(unit_index, &request);
	function_cc810(unit_index);
}

/* whether nothing holds the unit back: false when +0x13c is set and the
   unit has no parent, or is in a third-person seat of it */
// @retail 0xc4970
bool function_c4970(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	bool result = true;

	if (unit->unknown13c != NONE && (unit->parent_index == NONE || function_c48f0(unit_index)))
	{
		result = false;
	}
	return result;
}

/* plays one of the globals' sounds at the object's marker 0x4000095 */
// @retail 0xc5890
void function_c5890(long object_index, short index)
{
	long tag_index = *(long *)(*(byte **)((byte *)g_4e034c + 0x134) + index * 8 + 0xe8);

	if (tag_index != NONE)
	{
		s_sound_label_play play;

		play.label = NONE;
		play.tag_index = tag_index;
		play.scale = 1.0f;
		play.variant = 0;
		function_189210(object_index, 0x4000095, &play);
	}
}

/* the unit's per-tick checks: whether any of them changed something */
// @retail 0xc6b30
bool __stdcall function_c6b30(long unit_index)
{
	bool changed = function_c49b0(unit_index);

	changed |= function_c58f0(unit_index);
	changed |= function_c60c0(unit_index);
	changed |= function_c5eb0(unit_index);
	changed |= function_c6740(unit_index);
	function_114c60(unit_index);
	function_c6990(unit_index);
	function_c6810(unit_index);
	function_c50a0(unit_index);
	return changed;
}

// @retail 0xc6f80
void __stdcall function_c6f80(long unit_index, long a, long b)
{
	s_unit *unit = UNIT_GET(unit_index);

	unit->unknown1ec = a;
	unit->unknown1f0 = b;
}

/* whether the unit's animation is one of three (or 0x0c7070 holds) */
// @retail 0xc70b0
bool function_c70b0(long unit_index)
{
	if (function_0c7070(unit_index))
	{
		return true;
	}
	long name = UNIT_ANIMATION(UNIT_GET(unit_index))->name;
	return name == 0x80000c4 || name == 0x400004a || name == 0x50000c3;
}

/* the last object of the chain the units' +0x24c links */
// @retail 0xc7100
long function_c7100(long unit_index)
{
	long *next = &UNIT_GET(unit_index)->unknown24c;
	long result = NONE;

	while (*next != NONE)
	{
		result = *next;
		next = &UNIT_GET(result)->unknown24c;
	}
	return result;
}

// @retail 0xc8860
short function_c8860(long unit_index)
{
	return UNIT_GET(unit_index)->unknown240;
}

/* whether the unit, a biped, has bit 4 of +0x348 set */
// @retail 0xc8a10
bool function_c8a10(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	bool result = false;

	if (unit->type == 0)
	{
		result = (unit->flags_348 >> 4) & 1;
	}
	return result;
}

/* the number of seats function_c8a40 lists for the object */
// @retail 0xc8b80
short __stdcall function_c8b80(long object_index, s_object_seat *seats, short maximum_count)
{
	short count = 0;

	function_c8a40(object_index, seats, &count, maximum_count);
	return count;
}

/* clears the unit's flag 2 at +0x10a and its state that depends on it */
// @retail 0xcaf00
void function_caf00(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	unit->flags_10a &= ~4;
	function_b8b70(unit_index);
	function_bb950(unit_index, false, NONE);
	function_db5c0(unit_index);
	*((byte *)unit + unit->unknown33e) &= ~4;
	if ((1 << unit->type) & 1)
	{
		function_e4bd0(unit_index);
	}
}

/* the position of the unit's marker 0x4000095 */
// @retail 0xcaf60
void function_caf60(long unit_index, point3f *position)
{
	s_object_marker marker;

	function_b8d30(unit_index, 0x4000095, &marker, 1, false);
	*position = marker.matrix.position;
}

/* the position of the unit's marker 0x40000bd */
// @retail 0xcaf90
void function_caf90(long unit_index, point3f *position)
{
	s_object_marker marker;

	function_b8d30(unit_index, 0x40000bd, &marker, 1, false);
	*position = marker.matrix.position;
}

// @retail 0xcb7e0
void function_cb7e0(long unit_index, vector3f *vector)
{
	*vector = UNIT_GET(unit_index)->unknown168;
}

/* the first weapon the unit holds that is neither its current nor its next */
// @retail 0xcbe60
long function_cbe60(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	for (long i = 0; i < 4; i++)
	{
		if (i != unit->current_weapon_index && i != unit->next_weapon_index && unit->weapon_object_indices[i] != NONE)
		{
			return unit->weapon_object_indices[i];
		}
	}
	return NONE;
}

/* whether the unit (or what +0x248 names) has an actor */
// @retail 0xcc380
bool function_cc380(long object_index)
{
	s_unit *unit = UNIT_GET(object_index);

	if (unit->unknown248 != NONE)
	{
		unit = UNIT_GET(unit->unknown248);
	}
	return unit->actor_index != NONE;
}

/* whether the unit's animation is 0x600008c */
// @retail 0xcc3c0
bool function_cc3c0(long unit_index)
{
	s_unit_animation *animation = UNIT_ANIMATION(UNIT_GET(unit_index));
	bool result = false;

	if (animation->unknown68 != NONE && animation->unknown00 != NONE && animation->unknown06 != NONE &&
		animation->name == 0x600008c)
	{
		result = true;
	}
	return result;
}

/* whether the unit's animation is 0xd000042 or 0xc000043 */
// @retail 0xcc410
bool function_cc410(long unit_index)
{
	s_unit_animation *animation = UNIT_ANIMATION(UNIT_GET(unit_index));

	if (animation->unknown68 != NONE && animation->unknown00 != NONE && animation->unknown06 != NONE &&
		(animation->name == 0xd000042 || animation->name == 0xc000043))
	{
		return true;
	}
	return false;
}

/* whether the unit's seat has bit 2 of its flags */
// @retail 0xcc750
bool function_cc750(long unit_index, short seat_index)
{
	byte *definition = UNIT_DEFINITION_GET(UNIT_GET(unit_index));
	bool result = false;

	if (seat_index >= 0 && seat_index < UNIT_SEAT_COUNT(definition))
	{
		result = UNIT_SEATS(definition)[seat_index].flags.bit2;
	}
	return result;
}

/* whether the unit's seat has bit 3 of its flags */
// @retail 0xcc7b0
bool function_cc7b0(long unit_index, short seat_index)
{
	byte *definition = UNIT_DEFINITION_GET(UNIT_GET(unit_index));
	bool result = false;

	if (seat_index >= 0 && seat_index < UNIT_SEAT_COUNT(definition))
	{
		result = UNIT_SEATS(definition)[seat_index].flags.bit3;
	}
	return result;
}

// @retail 0xccb80
long function_ccb80(long unit_index)
{
	return UNIT_GET(unit_index)->unknown238;
}

/* adds to the unit's count of a grenade type and makes it current */
// @retail 0xcccd0
short function_cccd0(long unit_index, short grenade_type, char delta)
{
	UNIT_GET(unit_index)->grenade_counts[grenade_type] += delta;
	s_unit *unit = UNIT_GET(unit_index);
	unit->next_grenade_index = (char)grenade_type;
	unit->current_grenade_index = (char)grenade_type;
	return UNIT_GET(unit_index)->grenade_counts[grenade_type];
}

/* deletes the object +0x238 names */
// @retail 0xccd20
void function_ccd20(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (unit->unknown238 != NONE)
	{
		function_b8540(unit->unknown238);
		unit->unknown238 = NONE;
	}
}

/* the unit's first empty weapon slot, or NONE */
// @retail 0xcd620
short function_cd620(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	for (long i = 0; i < 4; i++)
	{
		if (unit->weapon_object_indices[i] == NONE)
		{
			return (short)i;
		}
	}
	return NONE;
}

/* the unit's count of a grenade type */
// @retail 0xcdff0
short function_cdff0(long unit_index, short grenade_type)
{
	if (grenade_type == NONE)
	{
		return 0;
	}
	return UNIT_GET(unit_index)->grenade_counts[grenade_type];
}

// @retail 0xce020
short function_ce020(long unit_index)
{
	return UNIT_GET(unit_index)->current_grenade_index;
}

/* runs 0xce920 on each weapon slot but the current and the next */
// @retail 0xcece0
void function_cece0(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	for (long i = 0; i < 4; i++)
	{
		if (i != unit->current_weapon_index && i != unit->next_weapon_index)
		{
			function_ce920(unit_index, i, 0, false);
		}
	}
}

/* forgets the object +0x2a8 names when it is this one */
// @retail 0xceea0
void __stdcall function_ceea0(long unit_index, long object_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (unit->unknown2a8 == object_index)
	{
		unit->unknown2a8 = NONE;
	}
}

/* whether the unit is a biped that 0xe4050 holds for */
// @retail 0xd03b0
bool function_d03b0(long unit_index)
{
	if (UNIT_GET(unit_index)->type != 0)
	{
		return false;
	}
	return function_e4050(unit_index);
}

/* starts listing the units among the object's children */
// @retail 0xd0590
void function_d0590(s_unit_child_iterator *iterator, long object_index)
{
	s_unit *object = UNIT_GET(object_index);

	iterator->object_index = object_index;
	iterator->unit_index = NONE;
	iterator->seat_index = NONE;
	iterator->next_index = object->first_child_index;
}

/* the next unit among the object's children, or 0 */
// @retail 0xd05c0
s_damage_object *function_d05c0(s_unit_child_iterator *iterator)
{
	while (iterator->next_index != NONE)
	{
		long unit_index = iterator->next_index;
		s_unit *unit = UNIT_GET(unit_index);

		iterator->next_index = unit->next_sibling_index;
		if ((1 << unit->type) & 3)
		{
			iterator->unit_index = unit_index;
			iterator->seat_index = unit->parent_seat_index;
			return (s_damage_object *)unit;
		}
	}
	return 0;
}

/* clears the unit's weapon indices, then its weapons and +0x238 */
// @retail 0xc4880
void __stdcall function_c4880(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	unit->unknown216 = NONE;
	unit->unknown217 = NONE;
	unit->current_weapon_index = NONE;
	unit->next_weapon_index = NONE;
	function_ccf20(unit_index);
	function_ccd20(unit_index);
}

/* outside mode 4: steps the unit's 4-bit counter at +0x210 and marks the
   object +0xd4 names */
// @retail 0xcea00
void function_cea00(long unit_index)
{
	if (g_4e6948->mode != 4)
	{
		short counter = (UNIT_GET(unit_index)->unknown210 + 1) & 0xf;

		if (counter == NONE)
		{
			counter = 0;
		}
		UNIT_GET(unit_index)->unknown210 = counter;
		UNIT_GET(unit_index)->unknown214 = counter;
		if (UNIT_GET(unit_index)->unknown0d4 != NONE)
		{
			function_b58c0(UNIT_GET(unit_index)->unknown0d4, 0x2000);
		}
	}
}

/* the nearest seat 0xc8bb0 finds for the unit: its object and seat
   index; returns 0xc8bb0's third result */
// @retail 0xc8ef0
short __stdcall function_c8ef0(long unit_index, long a, long *object_index, short *seat_index)
{
	long found_object_index = NONE;
	long found_seat_index = NONE;
	long result = 0;
	real distance = 3.4028235e38f;
	bool flag = false;

	function_c8bb0(unit_index, a, &found_object_index, &found_seat_index, &result, &distance, &flag);
	*object_index = found_object_index;
	*seat_index = (short)found_seat_index;
	return (short)result;
}

/* a number of ticks that shrinks with the difficulty: eight seconds' worth
   on easy and normal, six on heroic, four on legendary */
// @retail 0xc5340
short function_c5340()
{
	real scale = 1.0f;
	short difficulty;

	if (g_4e6948->state == 1)
	{
		difficulty = g_4e6948->difficulty;
	}
	else
	{
		difficulty = 1;
	}
	switch (difficulty)
	{
	case 2:
		scale = 0.75f;
		break;
	case 3:
		scale = 0.5f;
		break;
	}
	return (short)unit_round(g_510c54->field_2_3 * (scale * 8.0f));
}

/* the first of the unit's weapons whose definition has bit 3 at +0x12c */
// @retail 0xcfe40
short function_cfe40(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	for (long i = 0; i < 4; i++)
	{
		long weapon_index = unit->weapon_object_indices[i];

		if (weapon_index != NONE && (*(dword *)(UNIT_DEFINITION_GET(UNIT_GET(weapon_index)) + 0x12c) >> 3) & 1)
		{
			return (short)i;
		}
	}
	return NONE;
}

/* the next grenade type the unit has, from the given one in a direction
   (0 keeps the given one when the unit has it) */
// @retail 0xcbec0
short function_cbec0(long unit_index, short grenade_type, short direction)
{
	s_unit *unit = UNIT_GET(unit_index);
	short result = NONE;
	short first = grenade_type == NONE ? 0 : grenade_type;
	short type = first;

	do
	{
		if (unit->grenade_counts[type] > 0)
		{
			result = type;
			if (type != first || direction == 0)
			{
				break;
			}
		}
		if (direction < 0)
		{
			type = type == 0 ? 1 : type - 1;
		}
		else
		{
			type = type == 1 ? 0 : type + 1;
		}
	} while (type != first);
	return result;
}

/* whether the unit rides in a seat of its parent unit with bit 6 */
// @retail 0xc48f0
bool function_c48f0(long unit_index)
{
	bool result = false;

	if (unit_index != NONE)
	{
		s_unit *unit = UNIT_GET(unit_index);

		if (unit->parent_index != NONE)
		{
			s_unit *parent = UNIT_GET(unit->parent_index);

			if ((1 << parent->type) & 3)
			{
				result = (*(dword *)&UNIT_SEATS(UNIT_DEFINITION_GET(parent))[unit->parent_seat_index].flags >> 6) & 1;
			}
		}
	}
	return result;
}

/* stores the position 0xd1080 finds for the unit at +0x270 and clears the
   vector at +0x27c */
// @retail 0xd1000
void function_d1000(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	byte unknown[0x14];
	transform4x3f matrix;

	if (function_d1080(unit_index, &matrix, unknown))
	{
		unit->unknown270 = matrix.position;
	}
	unit->unknown27c.i = 0.0f;
	unit->unknown27c.j = 0.0f;
	unit->unknown27c.k = 0.0f;
}

/* how far the unit's animation 0x700005c (rising) or 0x700005d (falling)
   has run, as 0 to 1; 1 otherwise */
// @retail 0xd1410
real function_d1410(long unit_index)
{
	real result = 1.0f;

	if (unit_index != NONE)
	{
		s_unit_animation *animation = UNIT_ANIMATION(UNIT_GET(unit_index));

		if (animation->name == 0x700005c)
		{
			result = 0.0f;
			if (animation->unknown00 != NONE && animation->unknown06 != NONE)
			{
				result = ((c_animation_channel *)animation)->get_frame_ratio();
			}
		}
		else if (animation->name == 0x700005d)
		{
			real ratio = 0.0f;

			if (animation->unknown00 != NONE && animation->unknown06 != NONE)
			{
				ratio = ((c_animation_channel *)animation)->get_frame_ratio();
			}
			result = 1.0f - ratio;
		}
	}
	return result;
}

/* the definition of the seat the unit rides in, or 0 */
// @retail 0xc8fc0
s_unit_seat_definition *function_c8fc0(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (unit->parent_index != NONE)
	{
		short seat_index = unit->parent_seat_index;

		if (seat_index != NONE)
		{
			s_unit *parent = UNIT_GET(unit->parent_index);

			if ((1 << parent->type) & 3)
			{
				return &UNIT_SEATS(UNIT_DEFINITION_GET(parent))[seat_index];
			}
		}
	}
	return 0;
}

/* stops five of the unit's request types, sends request 0x13 and runs
   0x114240 */
// @retail 0xce040
void function_ce040(long unit_index)
{
	s_unit_request request;

	function_e69c0(unit_index, 8);
	function_e69c0(unit_index, 0x12);
	function_e69c0(unit_index, 0);
	function_e69c0(unit_index, 0xa);
	function_e69c0(unit_index, 0x1b);
	memset(&request, 0, sizeof(request));
	request.type = 0x13;
	function_e6900(unit_index, &request);
	function_114240(unit_index);
}

/* a unit state set to its defaults: the name 0x6000086, NONE indices and
   the default vector three times */
struct s_unit_state_c6ef0
{
	long name;
	short unknown04;
	short unknown06;
	char unknown08;
	char unknown09;
	short unknown0a;
	short unknown0c;
	byte unknown0e[0x28 - 0xe];
	vector3f unknown28;
	vector3f unknown34;
	vector3f unknown40;
	byte unknown4c[0x58 - 0x4c];
	long unknown58;
	long unknown5c;
	long unknown60;
	byte unknown64[0x70 - 0x64];
	short unknown70;
	byte unknown72[2];
	real unknown74;
	real unknown78;
};

// @retail 0xc6ef0
void function_c6ef0(s_unit_state_c6ef0 *state)
{
	memset(state, 0, sizeof(*state));
	state->unknown04 = 0;
	state->name = 0x6000086;
	state->unknown06 = NONE;
	state->unknown08 = NONE;
	state->unknown09 = NONE;
	state->unknown0a = NONE;
	state->unknown0c = NONE;
	state->unknown28 = *g_4687a8;
	state->unknown34 = *g_4687a8;
	state->unknown40 = *g_4687a8;
	state->unknown74 = 0.0f;
	state->unknown78 = 0.0f;
	state->unknown58 = NONE;
	state->unknown5c = NONE;
	state->unknown60 = NONE;
	state->unknown70 = 0;
}

/* how far the unit's timer at +0x2be has run, by its kind at +0x2bc: kind 1
   counts down from 0xc5340's ticks, kind 2 up to ten seconds */
// @retail 0xc53c0
real function_c53c0(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	switch (unit->unknown2bc)
	{
	case 1:
		return 1.0f - (real)unit->unknown2be / (real)function_c5340();
	case 2:
		return (real)unit->unknown2be / (real)unit_round(g_510c54->field_2_3 * 10.0f);
	}
	return 0.0f;
}

/* in mode 4, tells the player and actor code when the unit's +0x13c and
   +0x130 no longer match the ones it last saw (+0x2a8, +0x2ac) */
// @retail 0xd0d20
void function_d0d20(long unit_index)
{
	if (g_4e6948->mode == 4)
	{
		s_unit *unit = UNIT_GET(unit_index);

		if (unit->unknown13c != unit->unknown2a8)
		{
			if (unit->unknown13c != NONE)
			{
				function_a94b0(unit_index);
			}
			long player_index = unit->unknown2a8;
			if (player_index != NONE && record_pool_lookup(g_4e8c24, player_index))
			{
				function_a9440(unit_index, player_index);
			}
		}
		if (unit->unknown130 != unit->unknown2ac)
		{
			if (unit->unknown130 != NONE)
			{
				function_a7bc0(unit_index);
			}
			if (unit->unknown2ac != NONE)
			{
				function_a9500(unit_index, unit->unknown2ac);
			}
		}
	}
}
