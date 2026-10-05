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
#include "unknown_0d0690.h"
#include "sound_sources.h"
#include "object_iterator.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

/* the last 0x24 bytes of a unit state (0xc6ef0), kept at the unit's +0x1c8 */
struct s_unit_state_tail
{
	long unknown00;
	long unknown04;
	long unknown08;
	byte unknown0c[0x18 - 0xc];
	short unknown18;
	byte unknown1a[2];
	real unknown1c;
	real unknown20;
};

/* the unit (the object fields, then the unit's own; the fields read here) */
struct s_unit
{
	long definition_index;
	dword object_flags;
	byte unknown008[4];
	long next_sibling_index;
	long first_child_index;
	long parent_index;
	byte unknown018[0x70 - 0x18];
	vector3f forward;
	byte unknown07c[0xaa - 0x7c];
	byte type;
	byte unknownab[0xd4 - 0xab];
	long unknown0d4;
	byte unknownd8[0xec - 0xd8];
	real unknownec;
	real unknownf0;
	byte unknownf4[0x10a - 0xf4];
	byte flags_10a;
	byte unknown10b[0x12a - 0x10b];
	short animation_offset;
	long actor_index;
	long unknown130;
	dword flags_134;
	short unknown138;
	byte unknown13a[2];
	long unknown13c;
	byte unknown140[0x148 - 0x140];
	long unknown148;
	long unknown14c;
	vector3f unknown150;
	vector3f unknown15c;
	vector3f unknown168;
	byte unknown174[0x180 - 0x174];
	vector3f unknown180;
	real unknown18c;
	real unknown190;
	byte unknown194[0x1a4 - 0x194];
	vector3f unknown1a4;
	vector3f unknown1b0;
	char unknown1bc;
	byte unknown1bd[3];
	long unknown1c0;
	long unknown1c4;
	s_unit_state_tail unknown1c8;
	long unknown1ec;
	long unknown1f0;
	byte unknown1f4;
	byte unknown1f5[0x1fc - 0x1f5];
	short parent_seat_index;
	byte unknown1fe[0x210 - 0x1fe];
	short unknown210;
	char current_weapon_index;
	char next_weapon_index;
	short unknown214;
	char unknown216;
	char unknown217;
	long weapon_object_indices[4];
	long unknown228[4];
	long unknown238;
	char current_grenade_index;
	char next_grenade_index;
	char grenade_counts[2];
	char unknown240;
	char unknown241;
	byte unknown242[0x248 - 0x242];
	long unknown248;
	long unknown24c;
	byte unknown250[0x270 - 0x250];
	point3f unknown270;
	vector3f unknown27c;
	byte unknown288[0x2a0 - 0x288];
	long unknown2a0;
	short unknown2a4;
	byte unknown2a6[2];
	long unknown2a8;
	long unknown2ac;
	real unknown2b0;
	real unknown2b4;
	real unknown2b8;
	char unknown2bc;
	byte unknown2bd;
	short unknown2be;
	byte unknown2c0[0x2c8 - 0x2c0];
	word unknown2c8;
	short unknown2ca;
	real unknown2cc;
	long unknown2d0;
	byte unknown2d4[4];
	real unknown2d8;
	byte unknown2dc[0x2e4 - 0x2dc];
	real unknown2e4;
	short unknown2e8;
	byte unknown2ea[0x33e - 0x2ea];
	short unknown33e;
	byte unknown340[0x346 - 0x340];
	short unknown346;
	byte flags_348;
};

/* the field offsets the retail code reads */
#define UNIT_OFFSET_CHECK(field, offset) typedef char unit_offset_check_##field[offsetof(s_unit, field) == (offset) ? 1 : -1]
UNIT_OFFSET_CHECK(type, 0xaa);
UNIT_OFFSET_CHECK(unknown0d4, 0xd4);
UNIT_OFFSET_CHECK(flags_10a, 0x10a);
UNIT_OFFSET_CHECK(actor_index, 0x12c);
UNIT_OFFSET_CHECK(unknown13c, 0x13c);
UNIT_OFFSET_CHECK(unknown180, 0x180);
UNIT_OFFSET_CHECK(unknown1c8, 0x1c8);
UNIT_OFFSET_CHECK(unknown1ec, 0x1ec);
UNIT_OFFSET_CHECK(parent_seat_index, 0x1fc);
UNIT_OFFSET_CHECK(weapon_object_indices, 0x218);
UNIT_OFFSET_CHECK(unknown238, 0x238);
UNIT_OFFSET_CHECK(grenade_counts, 0x23e);
UNIT_OFFSET_CHECK(unknown2a0, 0x2a0);
UNIT_OFFSET_CHECK(unknown2bc, 0x2bc);
UNIT_OFFSET_CHECK(unknown2d8, 0x2d8);
UNIT_OFFSET_CHECK(unknown2e8, 0x2e8);
UNIT_OFFSET_CHECK(unknown346, 0x346);
UNIT_OFFSET_CHECK(flags_348, 0x348);

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
bool function_101640(long weapon_index);
bool function_e68c0(long type, long unit_index);
struct s_effect_owner;
void function_b7930(void *data, long tag_index, long object_index, s_effect_owner const *owner);
long function_b7b40(void *data);
void function_101980(long weapon_index, short rounds_loaded, short magazine_index, short rounds_total);
void function_10cdf0(long object_index);
void function_10ca80(long object_index, long a);
void function_ce6b0(long unit_index, long name, long object_index, real scale);
short function_101010(long weapon_index, short field_240);
bool function_10f630(long object_index, long *first, long *second);
void function_b7360(long object_index);
bool function_c9040(long unit_index, long a, short seat_index, vector3f const *forward);
long function_1e4a10(long index);
void function_c5740(long unit_index, bool start);
void function_1520f0(long player_index);
long __stdcall function_cdeb0(long unit_index, long state_name, long a, long b);
bool function_100f00(long weapon_index);
bool function_1e3370(long actor_index, void *unknown);
void function_10cec0(long unit_index, long weapon_index, long parent_marker_name, long marker_name);
void function_1c9c80(long object_index, long unknown2d0, word unknown2c8, real unknown2cc, long a, bool b);
bool unit_has_weapon_definition(long unit_index, long definition_index);
bool __stdcall function_cd0c0(long unit_index, long weapon_index, short mode);
void function_10cd50(long weapon_index);
bool unit_action_active(long unit_index, long action_type);
bool function_10fcd0(long unit_index, long unknown, long state_name, long action_name);
bool function_100880(long weapon_index, long magazine_index);
void function_c98a0(long unit_index, long a, long b, long c);
bool function_10f930(long object_index, real time, bool from_end);
struct s_damage_owner;
extern s_damage_owner const *g_467420;
void function_d6800(long object_index, s_damage_owner const *owner, bool notify_parent, bool unknown);
void function_d6a70(long object_index);
void __stdcall function_caa60(long unit_index, long a, long b, long c, long d);
void __stdcall function_bd020(long object_index);
void function_1e9070(long player_index);
void function_a7a30(long object_index, dword mask);
void function_a8950(long unit_index, long definition_index);
long unit_get_player_index(long unit_index);
void function_f8110(long equipment_index);
extern bool g_4f55dc[16];
struct s_game_allegiance_globals;
extern s_game_allegiance_globals *g_4f55ec;
bool function_0bfe60(const dword *flags, long bit);
struct s_time_entry;
struct s_unknown_1e6a40;
extern s_unknown_1e6a40 *g_51e9c0;
void function_1e6980(s_time_entry *entries, short a, byte b);

/* a unit request type's handlers (unknown_0e6900.cpp) */
typedef bool (__stdcall *t_unit_request_proc)(long unit_index, s_unit_request *request);
typedef bool (__stdcall *t_unit_request_update_proc)(long unit_index, long type);
typedef void (__stdcall *t_unit_request_end_proc)(long unit_index, long type);

struct s_unit_request_definition
{
	t_unit_request_proc perform;
	t_unit_request_update_proc update;
	t_unit_request_end_proc finished;
	t_unit_request_end_proc interrupted;
};

extern s_unit_request_definition *g_4677c8[60];
bool function_1012c0(long weapon_index);
long function_baf80(long object_index);
struct s_location;
void function_188180(point3f const *point, vector3f const *forward, long tag_index, long object_index, long index, long variant,
	long unused, long effect_value, s_location const *location, real scale);
long function_189060(long object_index, short value, real scale, point3f const *position, vector3f const *direction,
	long tag_index);

/* the damage data (as vehicles.cpp reads it) */
struct s_type_1e6529
{
	long definition_index;
	byte unknown04[0x7c - 0x4];
	short material_index;
	byte unknown7e[0x88 - 0x7e];
};

void function_d6660(s_type_1e6529 *data, long definition_index);
void function_d7b80(s_type_1e6529 *data, long object_index, short node_index, short unknown0c, short region_entry_index,
	vector3f const *unknown14);

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
	byte unknown0e[2];
	long unknown10;
	vector3f unknown14;
	long unknown20;
	long unknown24;
	vector3f unknown28;
	vector3f unknown34;
	vector3f unknown40;
	vector3f unknown4c;
	s_unit_state_tail tail;
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
	state->tail.unknown1c = 0.0f;
	state->tail.unknown20 = 0.0f;
	state->tail.unknown00 = NONE;
	state->tail.unknown04 = NONE;
	state->tail.unknown08 = NONE;
	state->tail.unknown18 = 0;
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

/* sends request 0x13 when the unit holds two weapons and either of them
   fails 0x101640 */
// @retail 0xd1540
void __stdcall function_d1540(long unit_index)
{
	if (UNIT_GET(unit_index)->current_weapon_index != NONE && UNIT_GET(unit_index)->next_weapon_index != NONE)
	{
		s_unit *unit = UNIT_GET(unit_index);
		short index = unit->current_weapon_index;
		long weapon_index = index != NONE ? unit->weapon_object_indices[index] : NONE;
		s_unit *other = UNIT_GET(unit_index);
		short other_index = other->next_weapon_index;
		long other_weapon_index = other_index != NONE ? other->weapon_object_indices[other_index] : NONE;

		if (!function_101640(weapon_index) || !function_101640(other_weapon_index))
		{
			function_e68c0(0x13, unit_index);
		}
	}
}

/* a weapon the unit starts with: its tag and rounds */
struct s_unit_starting_weapon
{
	byte unknown00[4];
	long tag_index;
	short rounds_loaded;
	word rounds_total;
};

/* creates a starting weapon for the unit; flags it at +0x12d when asked */
// @retail 0xccd60
long function_ccd60(s_unit_starting_weapon const *weapon, long unit_index, bool flag)
{
	long object_index = NONE;

	if (weapon->tag_index != NONE)
	{
		byte data[0xc4];

		function_b7930(data, weapon->tag_index, unit_index, 0);
		object_index = function_b7b40(data);
		if (object_index != NONE)
		{
			s_unit *object = UNIT_GET(object_index);

			if (*(long *)(UNIT_DEFINITION_GET(object) + 0x2c0) > 0)
			{
				function_101980(object_index, weapon->rounds_loaded, 0, weapon->rounds_total);
			}
			if (flag)
			{
				*((byte *)object + 0x12d) |= 1;
			}
		}
	}
	return object_index;
}

/* whether either weapon the unit holds is in state 1 or 2 (+0x20c) */
// @retail 0xcc0c0
bool function_cc0c0(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	bool result = false;
	short index = unit->current_weapon_index;

	if (index != NONE && unit->weapon_object_indices[index] != NONE)
	{
		char state = *((char *)UNIT_GET(unit->weapon_object_indices[index]) + 0x20c);

		result = state == 1 || state == 2;
	}
	index = unit->next_weapon_index;
	if (index != NONE && unit->weapon_object_indices[index] != NONE && !result)
	{
		char state = *((char *)UNIT_GET(unit->weapon_object_indices[index]) + 0x20c);

		result = state == 1 || state == 2;
	}
	return result;
}

/* gets rid of an object the unit held: deletes it (mode 1, or when the
   unit or the object's definition asks) or has 0xce6b0 drop it with an
   animation by mode */
// @retail 0xce470
void function_ce470(long mode, long object_index, long unit_index)
{
	byte *definition = UNIT_DEFINITION_GET(UNIT_GET(object_index));

	if (UNIT_GET(unit_index)->flags_134 & 0x10000)
	{
		mode = 1;
	}
	if (*(long *)(definition + 0x38) == NONE)
	{
		mode = 1;
	}
	function_10cdf0(object_index);
	function_10ca80(object_index, NONE);
	if (mode == 1)
	{
		function_b8540(object_index);
		return;
	}
	switch (mode)
	{
	case 2:
		function_ce6b0(unit_index, 0xa0005ad, object_index, 1.0f);
		break;
	case 3:
		function_ce6b0(unit_index, 0x9000536, object_index, 1.0f);
		break;
	default:
		function_ce6b0(unit_index, 0, object_index, 1.0f);
		break;
	}
}

/* the unit's next weapon state: its weapon's when it has rounds (+0x1fe),
   else the next of the globals' count after the given one */
// @retail 0xc8960
short function_c8960(long unit_index, short value)
{
	s_unit *unit = UNIT_GET(unit_index);
	short index = unit->current_weapon_index;
	long weapon_index = index != NONE ? unit->weapon_object_indices[index] : NONE;
	short result = NONE;

	if (weapon_index != NONE)
	{
		byte *definition = UNIT_DEFINITION_GET(UNIT_GET(weapon_index));
		bool flag = *(definition + 0x12e) & 1;

		if (*(short *)(definition + 0x1fe) > 0)
		{
			return function_101010(weapon_index, value);
		}
		if (flag)
		{
			return result;
		}
	}
	long count = *(long *)(*(byte **)((byte *)g_4e034c + 0x134) + 0xb8);
	if (count > 0 && value + 1 < count)
	{
		result = value + 1;
	}
	return result;
}

/* sends request 0x17 when the unit's seat has bit 15 */
// @retail 0xd12b0
void function_d12b0(long unit_index, long seat_index, bool a, bool b)
{
	if (unit_index != NONE && seat_index != NONE &&
		(*(dword *)&UNIT_SEATS(UNIT_DEFINITION_GET(UNIT_GET(unit_index)))[seat_index].flags >> 15) & 1)
	{
		s_unit_request request;

		memset(&request, 0, sizeof(request));
		request.type = 0x17;
		request.type17.unknown4 = a;
		request.type17.unknown5 = b;
		function_e6900(unit_index, &request);
	}
}

/* sends request 0x18 when the unit's seat has bit 15 */
// @retail 0xd1360
void __stdcall function_d1360(long unit_index, long seat_index, bool a, bool b)
{
	if (unit_index != NONE && seat_index != NONE &&
		(*(dword *)&UNIT_SEATS(UNIT_DEFINITION_GET(UNIT_GET(unit_index)))[seat_index].flags >> 15) & 1)
	{
		s_unit_request request;

		memset(&request, 0, sizeof(request));
		request.type = 0x18;
		request.type17.unknown4 = a;
		request.type17.unknown5 = b;
		function_e6900(unit_index, &request);
	}
}

/* whether a biped faces along the direction: always while in animation
   0x6000084, else by the sign of its facing's dot product with it */
// @retail 0xcc010
bool function_cc010(long object_index, vector3f const *direction)
{
	s_unit *unit = (s_unit *)function_badc0(object_index, 3);
	bool result = false;

	if (unit && unit->type == 0)
	{
		long unknown;
		long name = NONE;

		function_10f630(object_index, &unknown, &name);
		if (!(*(UNIT_DEFINITION_GET(unit) + 0xbe) & 1))
		{
			if (name == 0x6000084)
			{
				return true;
			}
			return unit->unknown190 * direction->j + direction->i * unit->unknown18c > 0.0f;
		}
	}
	return result;
}

/* the unit's death outside mode 4: clears its flags, applies the globals'
   damage at +0x144 to it and, unless flag 2 at +0x10a, marks it */
// @retail 0xcffc0
void function_cffc0(long unit_index)
{
	if (g_4e6948->mode != 4)
	{
		s_unit *unit = UNIT_GET(unit_index);
		byte *globals = *(byte **)((byte *)g_4e034c + 0x144);

		unit->flags_134 &= ~0x200000;
		unit->flags_134 &= ~0x20;
		unit->flags_10a &= ~0x80;
		if (globals && *(long *)(globals + 0x48) != NONE)
		{
			s_type_1e6529 damage;

			function_d6660(&damage, *(long *)(globals + 0x48));
			damage.material_index = NONE;
			function_d7b80(&damage, unit_index, NONE, NONE, NONE, 0);
		}
		if (!((unit->flags_10a >> 2) & 1))
		{
			unit->flags_10a |= 0x20;
			function_b7360(unit_index);
		}
	}
}

/* whether 0xc9040 holds for any of the seat's markers on the unit */
// @retail 0xc6fb0
bool __stdcall function_c6fb0(long unit_index, long a, short seat_index)
{
	long marker_name = *(long *)((byte *)&UNIT_SEATS(UNIT_DEFINITION_GET(UNIT_GET(unit_index)))[seat_index] + 0xc);
	bool found = false;
	s_object_marker markers[4];
	long count = function_b8d30(unit_index, marker_name, markers, 4, false);

	for (long i = 0; i < count && !found; i++)
	{
		if (function_c9040(unit_index, a, seat_index, &markers[i].matrix.forward))
		{
			found = true;
		}
	}
	return found;
}

/* whether a rider of the unit sits in a seat with bit 11 whose +0x3e is
   the given value */
// @retail 0xc9200
bool __stdcall function_c9200(long unit_index, short value)
{
	byte *definition = UNIT_DEFINITION_GET(UNIT_GET(unit_index));
	bool result = false;
	s_object_child_iterator iterator;

	function_d0620(unit_index, &iterator);
	while (function_d0690(&iterator) && !result)
	{
		if (iterator.child_value == unit_index && iterator.child_short != NONE)
		{
			s_unit_seat_definition *seat = &UNIT_SEATS(definition)[iterator.child_short];

			if ((*(dword *)&seat->flags >> 11) & 1 && *(short *)((byte *)seat + 0x3e) == value)
			{
				result = true;
			}
		}
	}
	return result;
}

/* how far out of range two values are for the unit: 0 within, 1 past the
   first limits, 2 past the second (its actor's, else its definition's) */
// @retail 0xc96b0
void function_c96b0(long unit_index, long *result, real a, real b)
{
	s_unit *unit = UNIT_GET(unit_index);

	*result = 0;
	if (unit->actor_index != NONE)
	{
		byte *limits = (byte *)function_1e4a10(*(long *)((byte *)g_4f55f0->data + (unit->actor_index & 0xffff) * 0x888 + 0x54));

		if (limits)
		{
			if (a > *(real *)(limits + 0x24) || b > *(real *)(limits + 0x28))
			{
				*result = 2;
			}
			else if (a > *(real *)(limits + 0x18) || b > *(real *)(limits + 0x1c))
			{
				*result = 1;
			}
			return;
		}
	}
	byte *definition = UNIT_DEFINITION_GET(unit);
	if (b > *(real *)(definition + 0x10c))
	{
		*result = 2;
	}
	else if (b > *(real *)(definition + 0x104) || a > *(real *)(definition + 0x104))
	{
		*result = 1;
	}
}

/* marks the unit's weapon slot changed: the object +0xd4 names gets the
   slot's two bits, then the counter steps */
static void unit_weapon_slot_changed(long unit_index, long slot_index)
{
	long index = UNIT_GET(unit_index)->unknown0d4;

	if (index != NONE)
	{
		function_b58c0(index, (1 << (slot_index + 0x12)) | (1 << (slot_index + 0xe)));
	}
	function_cea00(unit_index);
}

/* puts a weapon in the unit's slot, or empties the slot and gets rid of
   the weapon there by mode (mode 4 keeps it) */
// @retail 0xcea70
void __stdcall function_cea70(long mode, long weapon_index, short slot_index, long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (weapon_index != NONE)
	{
		function_10ca80(weapon_index, unit_index);
		function_10cd50(weapon_index);
		unit->weapon_object_indices[slot_index] = weapon_index;
		unit->unknown228[slot_index] = 0;
	}
	else
	{
		long old_weapon_index = unit->weapon_object_indices[slot_index];

		unit->weapon_object_indices[slot_index] = NONE;
		if (mode != 4)
		{
			function_ce470(mode, old_weapon_index, unit_index);
		}
	}
	unit_weapon_slot_changed(unit_index, slot_index);
}

/* attaches the weapon to the unit at its hand's marker (the unit
   definition's +0x158 or +0x15c), at the unit's marker +0x160 when the
   weapon has it, else at the weapon definition's +0x284 */
// @retail 0xd0870
void __stdcall function_d0870(long weapon_index, long unit_index, bool secondary)
{
	byte *unit_definition = UNIT_DEFINITION_GET(UNIT_GET(unit_index));
	byte *weapon_definition = UNIT_DEFINITION_GET(UNIT_GET(weapon_index));
	long parent_marker_name;

	if (!secondary)
	{
		parent_marker_name = *(long *)(unit_definition + 0x158);
	}
	else
	{
		parent_marker_name = *(long *)(unit_definition + 0x15c);
	}
	long marker_name = *(long *)(unit_definition + 0x160);
	if (marker_name == NONE || marker_name == 0)
	{
		marker_name = *(long *)(weapon_definition + 0x284);
	}
	else
	{
		s_object_marker marker;

		if (!function_b8d30(weapon_index, marker_name, &marker, 1, true))
		{
			marker_name = *(long *)(weapon_definition + 0x284);
		}
	}
	function_10cec0(unit_index, weapon_index, parent_marker_name, marker_name);
}

/* drops every weapon the unit holds (mode 1: deletes them) */
// @retail 0xccf20
void function_ccf20(long unit_index)
{
	long *slot = UNIT_GET(unit_index)->weapon_object_indices;

	for (long i = 0; i < 4; i++, slot++)
	{
		if (*slot != NONE)
		{
			s_unit *unit = UNIT_GET(unit_index);
			long weapon_index = unit->weapon_object_indices[(short)i];

			unit->weapon_object_indices[(short)i] = NONE;
			function_ce470(1, weapon_index, unit_index);
			unit_weapon_slot_changed(unit_index, (short)i);
		}
	}
}

/* counts down the unit's timers: +0x2e8 (clearing +0x2e4), +0x2ca (then
   runs 0x1c9c80 with +0x2c8..+0x2d0) and +0x1f4 (then 0xcffc0); whether
   any ran */
// @retail 0xc6740
bool function_c6740(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	bool result = false;

	if (unit->unknown2e8 > 0)
	{
		if (--unit->unknown2e8 == 0)
		{
			unit->unknown2e4 = 0.0f;
		}
		result = true;
	}
	if (unit->unknown2ca > 0)
	{
		if (--unit->unknown2ca == 0)
		{
			function_1c9c80(unit_index, unit->unknown2d0, unit->unknown2c8, unit->unknown2cc, 0, true);
			unit->unknown2c8 = 0;
			unit->unknown2d0 = NONE;
			unit->unknown2cc = 0.0f;
		}
		result = true;
	}
	if (unit->unknown1f4 > 0)
	{
		if (--unit->unknown1f4 == 0)
		{
			function_cffc0(unit_index);
		}
		return true;
	}
	return result;
}

/* creates the unit definition's starting weapons (+0x1c0, +0x1c4) and
   gives them to the unit; deletes those it cannot take (or, in state 2,
   already has) */
// @retail 0xccab0
void function_ccab0(long unit_index)
{
	byte *definition = UNIT_DEFINITION_GET(UNIT_GET(unit_index));

	for (long i = 0; i < *(long *)(definition + 0x1c0); i++)
	{
		long tag_index = *(long *)(*(byte **)(definition + 0x1c4) + i * 8 + 4);

		if (tag_index != NONE)
		{
			byte data[0xc4];

			function_b7930(data, tag_index, unit_index, 0);
			long weapon_index = function_b7b40(data);
			if (weapon_index != NONE)
			{
				if ((g_4e6948->state == 2 &&
					unit_has_weapon_definition(unit_index, UNIT_GET(weapon_index)->definition_index)) ||
					!function_cd0c0(unit_index, weapon_index, 1))
				{
					function_b8540(weapon_index);
				}
			}
		}
	}
}

/* lowers both of the unit's weapons (requests 8 and 0x12), then drops them */
// @retail 0xccff0
void __stdcall function_ccff0(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	s_unit_request request;

	unit->unknown216 = NONE;
	if (unit->current_weapon_index != NONE)
	{
		memset(&request, 0, sizeof(request));
		request.type = 8;
		request.type17.unknown4 = true;
		function_e6900(unit_index, &request);
	}
	unit->unknown217 = NONE;
	if (unit->next_weapon_index != NONE)
	{
		memset(&request, 0, sizeof(request));
		request.type = 0x12;
		request.type17.unknown4 = true;
		function_e6900(unit_index, &request);
	}
	function_ccf20(unit_index);
}

/* takes an amount from the unit's +0x2b0 and caps +0x2b8 (a quarter by
   default), unless its state +0x2bc is set (then 0xc5740 runs) */
// @retail 0xd0e60
void function_d0e60(long unit_index, real amount, real limit)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (unit->unknown2bc)
	{
		function_c5740(unit_index, 0);
		return;
	}
	if (limit == 0.0f)
	{
		limit = 0.25f;
	}
	if (limit > unit->unknown2b8)
	{
		limit = unit->unknown2b8;
	}
	unit->unknown2b8 = limit;
	unit->unknown2b0 -= amount;
	if ((unit->flags_134 >> 3) & 1 && 0.05f > unit->unknown2b0)
	{
		unit->unknown2b0 = 0.05f;
	}
	if (UNIT_GET(unit_index)->unknown0d4 != NONE)
	{
		function_b58c0(UNIT_GET(unit_index)->unknown0d4, 0x800000);
	}
}

/* which hands the unit holds weapons in, and whether its seat lacks bit 5 */
struct s_unit_weapon_hands
{
	byte unknown00[8];
	bool one;
	bool two;
	bool seat_allows;
	byte unknown0b;
};

// @retail 0xcb430
void function_cb430(long unit_index, s_unit_weapon_hands *hands)
{
	s_unit *unit = UNIT_GET(unit_index);

	memset(hands, 0, sizeof(*hands));
	s_unit *current = UNIT_GET(unit_index);
	short index = current->current_weapon_index;
	if (index != NONE && current->weapon_object_indices[index] != NONE)
	{
		s_unit *next = UNIT_GET(unit_index);
		short next_index = next->next_weapon_index;

		if (next_index != NONE && next->weapon_object_indices[next_index] != NONE)
		{
			hands->two = true;
		}
		else
		{
			hands->one = true;
		}
	}
	if (unit->parent_index != NONE && unit->parent_seat_index != NONE)
	{
		s_unit *parent = UNIT_GET(unit->parent_index);

		hands->seat_allows = !((*(dword *)&UNIT_SEATS(UNIT_DEFINITION_GET(parent))[unit->parent_seat_index].flags >> 5) & 1);
	}
}

/* asks the unit sharing the seat group of the unit's seat to get out
   (request 0x20); returns false */
// @retail 0xd0f30
bool __stdcall function_d0f30(long unit_index, bool a, bool b)
{
	s_unit *unit = UNIT_GET(unit_index);
	long parent_index = unit->parent_index;

	if (parent_index != NONE)
	{
		s_unit *parent = UNIT_GET(parent_index);

		if ((1 << parent->type) & 3)
		{
			short seat_index = unit->parent_seat_index;

			if (seat_index != NONE)
			{
				short other_seat_index = *(short *)((byte *)&UNIT_SEATS(UNIT_DEFINITION_GET(parent))[seat_index] + 0x3e);

				if (other_seat_index != NONE)
				{
					long occupant_index = function_c8f60(parent_index, other_seat_index);

					if (occupant_index != NONE && occupant_index != unit_index)
					{
						s_unit_request request;

						request.type = 0x20;
						request.type17.unknown4 = a;
						request.type17.unknown5 = b;
						if (function_e6900(occupant_index, &request))
						{
							function_cc810(parent_index);
						}
					}
				}
			}
		}
	}
	return false;
}

/* lowers the weapon in one of the unit's hands (request 8, or 0x12 for the
   second hand) */
// @retail 0xcd4e0
bool __stdcall function_cd4e0(long unit_index, short hand, bool flag)
{
	s_unit *unit = UNIT_GET(unit_index);
	char *indices = &unit->current_weapon_index + hand;
	bool result = false;

	if (indices[0] != NONE && indices[4] != NONE)
	{
		s_unit_request request;

		memset(&request, 0, sizeof(request));
		indices[4] = NONE;
		request.type = hand != 0 ? 0x12 : 8;
		request.type17.unknown4 = flag;
		if (!function_e6900(unit_index, &request))
		{
			return false;
		}
		if (hand)
		{
			function_cea00(unit_index);
		}
		if (unit->unknown13c != NONE)
		{
			function_1520f0(unit->unknown13c);
		}
		return true;
	}
	return result;
}

/* the weapon the unit, or the first rider down its children with bit 27 of
   its definition's +0xbc, holds; which unit holds it */
// @retail 0xcbd80
long __stdcall function_cbd80(long object_index, long *holder_index)
{
	s_unit *unit = UNIT_GET(object_index);
	short index = unit->current_weapon_index;
	long weapon_index = index != NONE ? unit->weapon_object_indices[index] : NONE;

	if (weapon_index != NONE)
	{
		if (holder_index)
		{
			*holder_index = object_index;
		}
		return weapon_index;
	}
	for (long child_index = UNIT_GET(object_index)->first_child_index; child_index != NONE;
		child_index = UNIT_GET(child_index)->next_sibling_index)
	{
		s_unit *child = UNIT_GET(child_index);

		if ((1 << child->type) & 3 && (*(dword *)(UNIT_DEFINITION_GET(child) + 0xbc) >> 27) & 1)
		{
			weapon_index = function_cbd80(child_index, holder_index);
			if (weapon_index != NONE)
			{
				break;
			}
		}
	}
	return weapon_index;
}

/* empties a weapon slot of the unit (deleting the weapon in mode 4, when
   it is used up, or in state 2 when it is neither kept nor fit), and
   resets the hands that would have taken it */
// @retail 0xce920
void function_ce920(long unit_index, long slot_index, long mode, bool flag)
{
	s_unit *unit = UNIT_GET(unit_index);
	long weapon_index = unit->weapon_object_indices[(short)slot_index];

	if (weapon_index != NONE)
	{
		if (g_4e6948->mode == 4 || *(real *)((byte *)UNIT_GET(weapon_index) + 0x184) >= 1.0f ||
			(g_4e6948->state == 2 && !function_100f00(weapon_index) && !function_101640(weapon_index)))
		{
			mode = 1;
		}
		function_cea70(mode, NONE, (short)slot_index, unit_index);
		for (long hand = 0; hand < 2; hand++)
		{
			if ((short)slot_index == (&unit->unknown216)[hand])
			{
				if (flag)
				{
					(&unit->unknown216)[hand] = (char)function_cdeb0(unit_index, NONE, (&unit->current_weapon_index)[hand], 0);
				}
				else
				{
					(&unit->unknown216)[hand] = NONE;
				}
			}
		}
	}
}

/* a motion in three phases: an acceleration, a coast and a deceleration */
struct s_unit_motion
{
	bool done;
	byte unknown01[0x8 - 0x1];
	real velocity;
	real acceleration;
	real acceleration_time;
	real coast_time;
	real deceleration;
	real deceleration_time;
};

/* advances a position and velocity along the motion for a time; whether
   time remains after its last phase */
// @retail 0xc7750
bool function_c7750(s_unit_motion const *motion, real position, real velocity, real time, real *out_velocity,
	real *out_position)
{
	bool result = motion->done;

	if (!result && time > 0.0f)
	{
		real step = motion->acceleration_time;

		if (step > 0.0f)
		{
			if (time <= step)
			{
				step = time;
			}
			real change = motion->acceleration * step;
			position = (change * 0.5f + velocity) * step + position;
			velocity = change + velocity;
			time -= step;
		}
		if (time > 0.0f)
		{
			step = motion->coast_time;
			if (step > 0.0f)
			{
				if (time <= step)
				{
					step = time;
				}
				position = step * velocity + position;
				time -= step;
			}
			if (time > 0.0f)
			{
				step = motion->deceleration_time;
				if (step > 0.0f)
				{
					if (time <= step)
					{
						step = time;
					}
					real change = motion->deceleration * step;
					position = (change * 0.5f + velocity) * step + position;
					velocity = change + velocity;
					time -= step;
				}
				if (time > 0.0f)
				{
					result = true;
				}
			}
		}
	}
	*out_position = position;
	*out_velocity = velocity;
	return result;
}

/* once: sets the unit's flag 21 and a random angle at +0x2d8 around its
   actor's direction (or its own facing) */
// @retail 0xcfec0
void function_cfec0(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (!((unit->flags_134 >> 21) & 1))
	{
		real range;
		byte unknown[8];

		unit->flags_134 |= 0x200000;
		if (unit->actor_index != NONE && function_1e3370(unit->actor_index, unknown))
		{
			unit->unknown2d8 = 0.0f;
			range = 0.43633232f;
		}
		else
		{
			real angle = (real)atan2(unit->forward.j, unit->forward.i);

			if (angle > 3.1415927f)
			{
				angle -= 6.2831855f;
			}
			unit->unknown2d8 = angle;
			range = 1.7453293f;
		}
		unit->unknown2d8 += function_259d0(&g_4e7408->unknown0, 0, 0, -range, range);
	}
}

/* the object that answers for the unit: the vehicle when the unit's seat
   has bit 0 or 3, or a rider of that vehicle with bit 27 of its
   definition's +0xbc and its object flag 26; else the unit */
// @retail 0xcb6d0
long function_cb6d0(long unit_index)
{
	long result = unit_index;

	if (unit_index != NONE)
	{
		s_unit *unit = UNIT_GET(unit_index);
		long parent_index = unit->parent_index;

		if (parent_index != NONE)
		{
			short seat_index = unit->parent_seat_index;

			if (seat_index != NONE)
			{
				s_unit *parent = UNIT_GET(parent_index);
				s_unit_seat_definition *seat = &UNIT_SEATS(UNIT_DEFINITION_GET(parent))[seat_index];

				if (*(byte *)seat & 1 || (*(dword *)seat >> 3) & 1)
				{
					result = parent_index;
				}
				for (long child_index = parent->first_child_index; child_index != NONE;
					child_index = UNIT_GET(child_index)->next_sibling_index)
				{
					s_unit *child = (s_unit *)function_badc0(child_index, 3);

					if (child && (*(dword *)(UNIT_DEFINITION_GET(child) + 0xbc) >> 27) & 1 &&
						(child->object_flags >> 26) & 1)
					{
						return child_index;
					}
				}
			}
		}
	}
	return result;
}

/* applies a unit state (0xc6ef0's) to the unit */
// @retail 0xc6de0
void function_c6de0(long object_index, void *control)
{
	s_unit *unit = UNIT_GET(object_index);
	s_unit_state_c6ef0 *state = (s_unit_state_c6ef0 *)control;

	unit->unknown1b0 = state->unknown14;
	unit->unknown1c0 = state->unknown20;
	unit->unknown1c4 = state->unknown24;
	unit->unknown1bc = (char)state->unknown04;
	if (state->unknown06 == unit->unknown214)
	{
		*(dword *)&unit->unknown214 = *(dword *)&state->unknown06;
	}
	if (state->unknown0a != NONE)
	{
		unit->next_grenade_index = (char)state->unknown0a;
	}
	unit->unknown241 = (char)state->unknown0c;
	unit->unknown148 = state->unknown10;
	unit->unknown180 = state->unknown40;
	unit->unknown15c = state->unknown34;
	unit->unknown150 = state->unknown28;
	unit->unknown14c = state->name;
	unit->unknown1a4 = state->unknown4c;
	unit->unknown1c8 = state->tail;
}

/* in mode 4: follows the unit's seat changes with requests: leaving the
   seat it last had (+0x2a0, +0x2a4) and, when it has no parent, going back
   to it (request 0x1c) */
// @retail 0xd0c10
void function_d0c10(long unit_index)
{
	if (g_4e6948->mode == 4)
	{
		s_unit *unit = UNIT_GET(unit_index);

		if (unit->parent_index != NONE && unit->parent_seat_index != NONE)
		{
			long last_parent_index = unit->unknown2a0;

			if (unit->parent_index != last_parent_index || unit->parent_seat_index != unit->unknown2a4)
			{
				if (unit_action_active(unit_index, 0x1f) && last_parent_index == NONE)
				{
					function_e69c0(unit_index, 0x1f);
				}
				if (!unit_action_active(unit_index, 0x1d) && !unit_action_active(unit_index, 0x1e) &&
					!unit_action_active(unit_index, 0x20))
				{
					function_e68c0(0x1e, unit_index);
				}
			}
		}
		if (unit->parent_index == NONE && unit->unknown2a0 != NONE)
		{
			s_unit_request request;

			request.type = 0x1c;
			request.type1c.object_index = unit->unknown2a0;
			request.type1c.seat_index = unit->unknown2a4;
			request.type1c.unknowna = unit_action_active(unit_index, 0x1f);
			request.type1c.unknownb = true;
			function_e6900(unit_index, &request);
		}
	}
}

/* whether the unit may ready the weapon in a state: always in a vehicle,
   never from a seat without bit 5, else by 0x10fcd0; never while +0x13c
   is set and the weapon has bit 29 at +0x12c */
// @retail 0xcd6a0
bool __stdcall function_cd6a0(long unit_index, long unknown, long weapon_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	byte *weapon_definition = UNIT_DEFINITION_GET(UNIT_GET(weapon_index));
	long seat_state_name;
	bool seat_blocks;
	bool result;

	if (unit->parent_index != NONE && unit->parent_seat_index != NONE)
	{
		s_unit_seat_definition *seat =
			&UNIT_SEATS(UNIT_DEFINITION_GET(UNIT_GET(unit->parent_index)))[unit->parent_seat_index];

		seat_state_name = seat->label;
		seat_blocks = !((*(dword *)&seat->flags >> 5) & 1);
	}
	else
	{
		seat_state_name = 0x7000101;
		seat_blocks = false;
	}
	if (unknown == NONE || unknown == 0x7000101)
	{
		unknown = seat_state_name;
	}
	if ((1 << unit->type) & 2)
	{
		result = true;
	}
	else if (seat_blocks)
	{
		result = false;
	}
	else
	{
		result = function_10fcd0(unit_index, unknown, *(long *)(weapon_definition + 0x288),
			*(long *)(weapon_definition + 0x28c));
	}
	if (unit->unknown13c != NONE && (*(dword *)(weapon_definition + 0x12c) >> 29) & 1)
	{
		result = false;
	}
	return result;
}

/* gives the unit a starting profile (the globals' +0xfc, 0x44 bytes each):
   resets it first when asked, then its two weapons, its two reals and
   its grenades */
// @retail 0xcce00
void function_cce00(long unit_index, short starting_profile_index, bool reset, bool flag)
{
	if (unit_index != NONE && starting_profile_index != NONE)
	{
		byte *profile = *(byte **)((byte *)g_4e0350 + 0xfc) + starting_profile_index * 0x44;
		s_unit *unit = UNIT_GET(unit_index);

		if (reset)
		{
			function_ccff0(unit_index);
			unit->unknownf0 = 1.0f;
			unit->unknownec = 1.0f;
			*(word *)unit->grenade_counts = 0;
		}
		s_unit_starting_weapon *weapon = (s_unit_starting_weapon *)(profile + 0x28);
		for (long i = 2; i != 0; i--, weapon++)
		{
			if (weapon->tag_index != NONE)
			{
				long weapon_index = function_ccd60(weapon, unit_index, flag);

				if (weapon_index != NONE && !function_cd0c0(unit_index, weapon_index, 1))
				{
					function_b8540(weapon_index);
				}
			}
		}
		unit->unknownf0 -= *(real *)(profile + 0x24);
		unit->unknownec -= *(real *)(profile + 0x20);
		unit->grenade_counts[0] += profile[0x40];
		unit->grenade_counts[1] += profile[0x41];
	}
}

/* whether the unit is free to act: not riding, without object flags 26
   and 27 in its animation state, no blocking action, and neither hand's
   weapon busy (0x100880) */
// @retail 0xc85c0
bool function_c85c0(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);
	bool result = true;

	if (unit->parent_index != NONE ||
		(*(dword *)((byte *)unit + unit->unknown346 + 4) >> 26) & 1 ||
		(*(dword *)((byte *)unit + unit->unknown346 + 4) >> 27) & 1 ||
		unit_action_active(unit_index, 0x16) || unit_action_active(unit_index, 8) ||
		unit_action_active(unit_index, 0x12) || unit_action_active(unit_index, 0) ||
		unit_action_active(unit_index, 1) || unit_action_active(unit_index, 0xa) ||
		unit_action_active(unit_index, 0xb))
	{
		return false;
	}
	unit = UNIT_GET(unit_index);
	for (long hand = 0; hand < 2; hand++)
	{
		short index = (&unit->current_weapon_index)[hand];

		if (index != NONE && unit->weapon_object_indices[index] != NONE &&
			function_100880(unit->weapon_object_indices[index], NONE))
		{
			return false;
		}
	}
	return result;
}

/* a unit's scenario placement data (the part the unit reads) */
struct s_unit_placement
{
	real unknown00;
	byte flags;
};

/* places the unit from the scenario: its +0xec and flags, and, when the
   placement says so, starts it dead (animation, damage, no weapons or
   grenades) */
// @retail 0xc42e0
void __stdcall function_c42e0(long unit_index, void const *placement)
{
	s_unit_placement const *data = (s_unit_placement const *)placement;
	s_unit *unit = UNIT_GET(unit_index);

	if (data->unknown00 > 0.0f)
	{
		unit->unknownec = data->unknown00;
	}
	if (data->flags & 4)
	{
		unit->flags_134 |= 0x1000;
	}
	unit->object_flags |= 0x8000;
	if (data->flags & 1)
	{
		unit->flags_134 |= 0x40;
		function_c98a0(unit_index, 3, NONE, 0);
		s_unit_animation *animation = UNIT_ANIMATION(UNIT_GET(unit_index));
		if (animation->unknown68 != NONE && animation->unknown00 != NONE && animation->unknown06 != NONE &&
			animation->name == 0xc000043)
		{
			function_10f930(unit_index, 0.1f, true);
		}
		unit->unknownec = 0.0f;
		unit->unknownf0 = 0.0f;
		function_d6800(unit_index, g_467420, false, false);
		function_d6a70(unit_index);
		function_ccff0(unit_index);
		function_ccd20(unit_index);
		*(word *)unit->grenade_counts = 0;
		function_caa60(unit_index, NONE, 0, 1, 0);
		function_bd020(unit_index);
	}
}

/* picks up a grenade: when the globals allow its type (+0x104, 0x2c bytes
   each) and the unit carries fewer than the type's maximum, counts it,
   tells the player code, and deletes it */
// @retail 0xccba0
bool __stdcall function_ccba0(long unit_index, long grenade_index)
{
	byte *definition = UNIT_DEFINITION_GET(UNIT_GET(grenade_index));
	s_unit *unit = UNIT_GET(unit_index);
	byte *globals = (byte *)g_4e034c;

	if (!*(void **)(globals + 0x100))
	{
		return false;
	}
	short type = *(short *)(definition + 0x12e);
	short *maximum = (short *)(*(byte **)(globals + 0x104) + type * 0x2c);
	if (!maximum || unit->grenade_counts[type] >= *maximum)
	{
		return false;
	}
	if (unit->unknown13c != NONE)
	{
		function_1e9070(unit->unknown13c);
	}
	if (g_4e6948->mode == 4)
	{
		return false;
	}
	unit->grenade_counts[type]++;
	function_a7a30(unit_index, 0x400000);
	function_a8950(unit_index, UNIT_GET(grenade_index)->definition_index);
	s_unit *object = (s_unit *)function_badc0(unit_index, 3);
	if (object && object->unknown13c != NONE &&
		*(short *)((byte *)g_4e8c24->data + (unit_get_player_index(unit_index) & 0xffff) * 0x21c + 0x28) != NONE)
	{
		function_f8110(grenade_index);
	}
	function_b8540(grenade_index);
	return true;
}

/* finds a unit that is dying or down: object flags 22, 26 or 27 in its
   animation state, or in animation 0xc000043 or 0xd000042 without flag 2
   at +0x33e and bit 0 at its animation's +0x6c */
// @retail 0xcc170
bool __stdcall function_cc170(long *value)
{
	s_type_f1af8e iterator;
	s_unit *object;

	function_bae80(&iterator, 3, 1);
	while ((object = (s_unit *)function_baeb0(&iterator)) != 0)
	{
		long unit_index = iterator.object_index;
		s_unit *unit = UNIT_GET(unit_index);
		s_unit_animation *animation = UNIT_ANIMATION(unit);
		long name = NONE;

		if (animation->unknown68 != NONE && animation->unknown00 != NONE && animation->unknown06 != NONE)
		{
			name = animation->name;
		}
		if ((*(dword *)((byte *)unit + unit->unknown346 + 4) >> 22) & 1 ||
			(*(dword *)((byte *)unit + unit->unknown346 + 4) >> 26) & 1 ||
			(*(dword *)((byte *)unit + unit->unknown346 + 4) >> 27) & 1)
		{
			*value = unit_index;
			return true;
		}
		if ((name == 0xc000043 || name == 0xd000042) && !((*((byte *)object + object->unknown33e) >> 2) & 1) &&
			!(*((byte *)object + object->animation_offset + 0x6c) & 1))
		{
			*value = unit_index;
			return true;
		}
	}
	return false;
}

/* the next weapon slot from the given one in a direction whose weapon the
   unit may ready in the state (0xcd6a0), not its second hand's; prefers
   weapons with bit 3 at +0x12c, then (direction 0) the newest */
// @retail 0xcdeb0
long __stdcall function_cdeb0(long unit_index, long state_name, long a, long b)
{
	s_unit *unit = UNIT_GET(unit_index);
	short slot = (short)a;
	short direction = (short)b;
	short result = NONE;
	bool result_flag = false;
	long best = 0;
	short first = slot == NONE ? 0 : (slot + direction + 4) % 4;
	short i = first;

	do
	{
		long weapon_index = unit->weapon_object_indices[i];

		if (weapon_index != NONE && function_cd6a0(unit_index, state_name, weapon_index) &&
			i != unit->next_weapon_index)
		{
			long time = unit->unknown228[i];
			bool flag = (*(dword *)(UNIT_DEFINITION_GET(UNIT_GET(weapon_index)) + 0x12c) >> 3) & 1;

			if (result == NONE || (!result_flag && (flag || (direction == 0 && time > best))))
			{
				result = i;
				best = time;
				result_flag = flag;
			}
		}
		i = direction < 0 ? (i + 3) % 4 : (i + 1) % 4;
	} while (i != first);
	return result;
}

/* starts (state 1, its timer from 0xc5340, flag 3) or ends (state 2, ten
   seconds) the unit's state +0x2bc while its player is in state 1 at
   +0x88 (or the campaign flag allows it), with the globals' sounds */
// @retail 0xc5740
void function_c5740(long unit_index, bool start)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (unit->unknown13c != NONE)
	{
		byte state = *((byte *)g_4e8c24->data + (unit->unknown13c & 0xffff) * 0x21c + 0x88);

		if (state == 1 || (g_4e6948->state == 1 && g_4f55dc[0]))
		{
			if (start)
			{
				if (unit->parent_seat_index == NONE && !unit->unknown2bc)
				{
					unit->unknown2bc = 1;
					unit->unknown2be = function_c5340();
					unit->flags_134 |= 8;
					unit->unknown2b8 = g_510c54->field_2_3 * 0.05f;
					function_c5890(unit_index, 0);
				}
				else
				{
					function_c5890(unit_index, 2);
				}
			}
			else if (unit->unknown2bc == 1)
			{
				unit->unknown2bc = 2;
				unit->unknown2be = (short)unit_round(g_510c54->field_2_3 * 10.0f);
				unit->flags_134 &= ~8;
				unit->unknown2b8 = g_510c54->field_2_3 * 0.1f;
				function_c5890(unit_index, 1);
			}
		}
	}
}

/* plays the sounds of a unit's impact: its weapon's (+0x58) when it holds
   a weapon that allows it (0x1012c0), else the globals' material sound
   (+0x154, 0xb4 bytes each) and the definition's +0xac */
// @retail 0xceee0
void __stdcall function_ceee0(long unit_index, long definition_index, short material_index, point3f const *point,
	vector3f const *forward)
{
	s_unit *unit = UNIT_GET(unit_index);
	short index = unit->current_weapon_index;

	if (index != NONE)
	{
		long weapon_index = unit->weapon_object_indices[index];

		if (weapon_index != NONE && function_1012c0(weapon_index))
		{
			long tag_index = *(long *)(UNIT_DEFINITION_GET(UNIT_GET(weapon_index)) + 0x58);

			if (tag_index != NONE)
			{
				byte *root = (byte *)UNIT_GET(function_baf80(weapon_index));

				function_188180(point, forward, tag_index, unit_index, 0xf, 0, material_index, (long)(root + 0x70),
					(s_location const *)(root + 0x28), 1.0f);
			}
			return;
		}
	}
	byte *material = 0;
	if (material_index != NONE && material_index >= 0 && material_index < *(long *)((byte *)g_4e034c + 0x150))
	{
		material = *(byte **)((byte *)g_4e034c + 0x154) + material_index * 0xb4;
	}
	long tag_index = *(long *)(material + 0x64);
	if (tag_index != NONE)
	{
		function_189060(unit_index, NONE, 1.0f, g_468788, g_4687a8, tag_index);
	}
	if (definition_index != NONE)
	{
		tag_index = *(long *)(g_4e3b44[definition_index & 0xffff].bytes + 0xac);
		if (tag_index != NONE)
		{
			function_189060(unit_index, NONE, 1.0f, g_468788, g_4687a8, tag_index);
		}
	}
}

/* forgets an object everywhere the unit refers to it: +0x248, +0x24c, its
   weapon slots (and hands), +0x238, the seat it last had and its
   animation state's targets */
// @retail 0xced30
void __stdcall function_ced30(long unit_index, long object_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (unit->unknown248 == object_index)
	{
		unit->unknown248 = NONE;
	}
	if (unit->unknown24c == object_index)
	{
		unit->unknown24c = NONE;
	}
	long *slot = unit->weapon_object_indices;
	for (long i = 0; i < 4; i++, slot++)
	{
		if (*slot == object_index)
		{
			if (i == unit->current_weapon_index)
			{
				unit->current_weapon_index = NONE;
			}
			if (i == unit->next_weapon_index)
			{
				unit->next_weapon_index = NONE;
			}
			UNIT_GET(unit_index)->weapon_object_indices[(short)i] = NONE;
			unit_weapon_slot_changed(unit_index, (short)i);
		}
	}
	if (unit->current_weapon_index == NONE)
	{
		unit->unknown216 = (char)function_cdeb0(unit_index, NONE, NONE, 0);
	}
	if (unit->unknown238 == object_index)
	{
		unit->unknown238 = NONE;
	}
	if (unit->unknown2a0 == object_index)
	{
		unit->unknown2a0 = NONE;
		unit->unknown2a4 = NONE;
	}
	s_unit *again = UNIT_GET(unit_index);
	byte *state = (byte *)again + again->unknown346;
	if (*(long *)(state + 0x10) == object_index)
	{
		*(long *)(state + 0x10) = NONE;
		*(short *)(state + 0x14) = NONE;
	}
	if (*(long *)(state + 0x18) == object_index)
	{
		*(long *)(state + 0x18) = NONE;
	}
}

/* the request a weapon event sends the unit holding the weapon (and the
   unit +0x24c names): by the event type, for its first or second hand */
// @retail 0xc9d00
void __stdcall function_c9d00(long unit_index, long weapon_index, long type)
{
	if (type)
	{
		s_unit *unit = UNIT_GET(unit_index);
		short index = unit->current_weapon_index;
		bool first = weapon_index == (index != NONE ? unit->weapon_object_indices[index] : NONE);
		unit = UNIT_GET(unit_index);
		index = unit->next_weapon_index;
		bool second = weapon_index == (index != NONE ? unit->weapon_object_indices[index] : NONE);
		long action;

		if (first)
		{
			switch (type)
			{
			case 1:
				action = 2;
				break;
			case 2:
				action = 3;
				break;
			case 3:
				action = 4;
				break;
			case 4:
				action = 5;
				break;
			case 7:
				action = 6;
				break;
			case 8:
				action = 7;
				break;
			default:
				return;
			}
		}
		else if (second)
		{
			switch (type)
			{
			case 1:
				action = 0xc;
				break;
			case 2:
				action = 0xd;
				break;
			case 3:
				action = 0xe;
				break;
			case 4:
				action = 0xf;
				break;
			case 7:
				action = 0x10;
				break;
			case 8:
				action = 0x11;
				break;
			default:
				return;
			}
		}
		else
		{
			return;
		}
		function_e68c0(action, unit_index);
		long rider_index = UNIT_GET(unit_index)->unknown24c;
		if (rider_index != NONE)
		{
			function_e68c0(action, rider_index);
		}
	}
}

/* evens out two motions: the one that would finish first gives up part of
   its acceleration so both end together (or stops when its speed would
   vanish) */
// @retail 0xc75d0
void function_c75d0(s_unit_motion *a, s_unit_motion *b, real scale)
{
	if (!a->done && !b->done)
	{
		real total_a = a->deceleration_time + a->coast_time + a->acceleration_time;
		real total_b = b->deceleration_time + b->coast_time + b->acceleration_time;
		real difference;
		s_unit_motion *motion;

		if (a->acceleration_time > 0.0f && total_b > total_a)
		{
			difference = total_b - total_a;
			motion = a;
		}
		else if (b->acceleration_time > 0.0f && total_a > total_b)
		{
			difference = total_a - total_b;
			motion = b;
		}
		else
		{
			return;
		}
		if (motion)
		{
			real x = (difference + motion->coast_time) * scale;
			real speed = (real)fabs(motion->acceleration_time * motion->acceleration + motion->velocity);
			real step = ((real)sqrt(x * x + 4.0f * speed * difference * scale) - x) / (scale + scale);
			real limit = motion->acceleration_time > motion->deceleration_time ? motion->deceleration_time :
				motion->acceleration_time;

			if (step > limit)
			{
				step = motion->acceleration_time > motion->deceleration_time ? motion->deceleration_time :
					motion->acceleration_time;
			}
			if (step > 0.0f)
			{
				real time = motion->acceleration_time - step;
				real velocity = time * motion->acceleration + motion->velocity;

				if (0.0001f > (real)fabs(velocity))
				{
					motion->done = true;
					motion->acceleration_time = 0.0f;
					motion->deceleration_time = 0.0f;
					motion->coast_time = 0.0f;
					return;
				}
				motion->acceleration_time = time;
				motion->deceleration_time -= step;
				motion->coast_time = (motion->acceleration * step + velocity * 2.0f) * step / velocity;
			}
		}
	}
}

/* moves the unit's two fades toward their targets each tick: +0x2b0 by
   +0x2b8 per second up to 1 while flag 3 is set (set here when its team
   +0x138 is not at peace with the campaign's team 16), else down to 0;
   +0x2b4 by a third per second by flag 4 */
// @retail 0xc6810
void function_c6810(long unit_index)
{
	s_unit *unit = UNIT_GET(unit_index);

	if (unit->actor_index != NONE && g_4e6948->state == 1 && g_4f55dc[2])
	{
		short team = unit->unknown138;

		if (team == NONE || team < 0 || team >= 0x10 ||
			!function_0bfe60((dword const *)((byte *)g_4f55ec + 0xc4), team + 0x10))
		{
			unit->flags_134 |= 8;
		}
	}
	if ((unit->flags_134 >> 3) & 1)
	{
		unit->unknown2b0 += unit->unknown2b8 * g_510c54->rate;
		if (unit->unknown2b0 > 1.0f)
		{
			unit->unknown2b0 = 1.0f;
			if (UNIT_GET(unit_index)->unknown0d4 != NONE)
			{
				function_b58c0(UNIT_GET(unit_index)->unknown0d4, 0x800000);
			}
		}
	}
	else
	{
		unit->unknown2b0 -= unit->unknown2b8 * g_510c54->rate;
		if (0.0f > unit->unknown2b0)
		{
			unit->unknown2b0 = 0.0f;
			if (UNIT_GET(unit_index)->unknown0d4 != NONE)
			{
				function_b58c0(UNIT_GET(unit_index)->unknown0d4, 0x800000);
			}
		}
	}
	if ((unit->flags_134 >> 4) & 1)
	{
		unit->unknown2b4 += g_510c54->rate * 0.33333334f;
		if (unit->unknown2b4 > 1.0f)
		{
			unit->unknown2b4 = 1.0f;
		}
	}
	else
	{
		unit->unknown2b4 -= g_510c54->rate * 0.33333334f;
		if (0.0f > unit->unknown2b4)
		{
			unit->unknown2b4 = 0.0f;
		}
	}
}

/* drops a weapon of the unit: from its first hand at once (request 9's
   handler, then the player's controller hears of it), from its second
   hand by request 0x13, else from its slot (0xce920) */
// @retail 0xce520
void function_ce520(long unit_index, long weapon_index, bool flag)
{
	s_unit *unit = UNIT_GET(unit_index);
	long slot = NONE;

	for (long i = 0; i < 4; i++)
	{
		if (unit->weapon_object_indices[i] == weapon_index)
		{
			slot = i;
			break;
		}
	}
	if (slot == unit->current_weapon_index)
	{
		s_unit_request request;

		memset(&request, 0, sizeof(request));
		request.type = 9;
		request.type17.unknown4 = !flag;
		function_b7360(unit_index);
		bool result = g_4677c8[9]->perform(unit_index, &request);
		if (unit_index != NONE)
		{
			long player_index = UNIT_GET(unit_index)->unknown13c;

			if (player_index != NONE)
			{
				short controller = *(short *)((byte *)g_4e8c24->data + (player_index & 0xffff) * 0x21c + 0x28);

				if (controller != NONE)
				{
					function_1e6980((s_time_entry *)((byte *)g_51e9c0 + controller * 0x1b0 + 0x150),
						(short)request.type, result);
				}
			}
		}
	}
	else if (slot == unit->next_weapon_index)
	{
		s_unit_request request;

		memset(&request, 0, sizeof(request));
		request.type = 0x13;
		request.type17.unknown4 = !flag;
		function_e6900(unit_index, &request);
	}
	else
	{
		function_ce920(unit_index, slot, 0, flag);
	}
}

typedef char unit_state_size_check[sizeof(s_unit_state_c6ef0) == 0x7c ? 1 : -1];
typedef char unit_motion_offset_check[offsetof(s_unit_motion, deceleration_time) == 0x1c ? 1 : -1];
