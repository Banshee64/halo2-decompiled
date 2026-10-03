// @flags /O2 /arch:SSE /Gr
/* WEAPONS.CPP: weapon state queries and updates (barrels, triggers,
   magazines, zoom). The functions of 0x101e80..0x102100 are in
   unknown_101e80.cpp. */

#include "cseries.h"
#include "globals.h"
#include <math.h>

/* the weapon definition (the tag data) */
struct s_weapon_magazine_definition
{
	byte unknown00[0xa];
	short rounds_loaded_maximum;
	short rounds_total_maximum;
	byte unknown0e[0x5c - 0xe];
};

struct s_weapon_trigger_definition
{
	byte unknown00[6];
	short behavior;
	byte unknown08[2];
	short primary_barrel;
	byte unknown0c[0x2c - 0xc];
	real charging_time;
	byte unknown30[0x40 - 0x30];
};

struct s_weapon_barrel_definition
{
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword flag6 : 1;
	dword flag7 : 1;
	dword flag8 : 1;
	dword flag9 : 1;
	dword flag10 : 1;
	dword flag11 : 1;
	dword : 20;
	byte unknown04[0x20 - 4];
	real value_20;
	real value_24;
	byte unknown28[0x30 - 0x28];
	long name;
	byte unknown34[0x9c - 0x34];
	real value_9c;
	byte unknowna0[0xa8 - 0xa0];
	real value_a8;
	byte unknownac[0xec - 0xac];
};

struct s_weapon_definition
{
	byte unknown000[0x12c];
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword prevents_grenade_throwing : 1;
	dword flag7 : 1;
	dword flag8 : 1;
	dword flag9 : 1;
	dword flag10 : 1;
	dword flag11 : 1;
	dword flag12 : 1;
	dword flag13 : 1;
	dword flag14 : 1;
	dword flag15 : 1;
	dword flag16 : 1;
	dword flag17 : 1;
	dword flag18 : 1;
	dword flag19 : 1;
	dword flag20 : 1;
	dword flag21 : 1;
	dword flag22 : 1;
	dword flag23 : 1;
	dword flag24 : 1;
	dword flag25 : 1;
	dword flag26 : 1;
	dword flag27 : 1;
	dword flag28 : 1;
	dword flag29 : 1;
	dword flag30 : 1;
	dword flag31 : 1;
	byte unknown130[0x1fe - 0x130];
	short zoom_level_count;
	real zoom_magnification_minimum;
	real zoom_magnification_maximum;
	byte unknown208[0x294 - 0x208];
	short value_294;
	byte unknown296[0x2c0 - 0x296];
	long magazine_count;
	s_weapon_magazine_definition *magazines;
	long trigger_count;
	s_weapon_trigger_definition *triggers;
	long barrel_count;
	s_weapon_barrel_definition *barrels;
};

/* the weapon (the object data) */
struct s_weapon_barrel
{
	char timer;
	byte state;
	short ticks;
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word : 8;
	word value06;
	byte unknown08[0x14 - 8];
	real value14;
	byte unknown18[0x28 - 0x18];
	real accumulator;
	byte unknown2c[0x34 - 0x2c];
};

struct s_weapon_trigger
{
	byte state;
	byte unknown01;
	short timer;
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword flag6 : 1;
	dword flag7 : 1;
	dword : 24;
	byte unknown08[4];
};

struct s_weapon_magazine
{
	short state;
	byte unknown02[4];
	short rounds_unloaded;
	short rounds_loaded;
	byte unknown0a[6];
};

struct s_weapon
{
	long definition_index;
	byte unknown004[0x12c - 4];
	byte item_flags;
	byte unknown12d[0x154 - 0x12d];
	long unit_index;
	byte unknown158[0x178 - 0x158];
	long state;
	byte unknown17c[0x184 - 0x17c];
	real heat;
	byte unknown188[0x1a4 - 0x188];
	s_weapon_barrel barrels[2];
	s_weapon_trigger triggers[2];
	s_weapon_magazine magazines[2];
	byte unknown244[0x250 - 0x244];
	long value250;
	short value254;
	short value256;
	real value258;
};

struct s_weapon_header
{
	byte unknown00[8];
	s_weapon *weapon;
};

#define WEAPON_GET(index) (((s_weapon_header *)g_4e0300->data)[(index) & 0xffff].weapon)
#define WEAPON_DEFINITION(weapon) ((s_weapon_definition *)g_4e3b44[(weapon)->definition_index & 0xffff].bytes)

/* the unit holding a weapon (a view of the unit) */
struct s_weapon_unit
{
	byte unknown000[0x13c];
	long player_index;
	byte unknown140[0x218 - 0x140];
	long weapon_indices[4];
};

struct s_weapon_unit_header
{
	byte unknown00[8];
	s_weapon_unit *unit;
};

/* an object marker (the markers of a name at a node) */
struct s_object_marker
{
	short node_index;
	short unknown02;
	real_matrix4x3 node_matrix;
	long unknown38;
	real_matrix4x3 matrix;
};

/* a sound event of a weapon's animation */
struct s_weapon_sound_event
{
	short type;
	byte unknown02[2];
	long sound_index;
	byte unknown08[8];
	long marker_name;
};

short function_b8d30(bool flag, long object_index, long marker_name, short count, s_object_marker *markers);
long function_189060(long object_index, short value, real scale, real_point3d const *position, real_vector3d const *direction, long tag_index);

#define WEAPON_UNIT_GET(index) (((s_weapon_unit_header *)g_4e0300->data)[(index) & 0xffff].unit)

bool function_100880(long weapon_index, long magazine_index);
bool __stdcall function_159dd0(long player_index);
bool function_159d40(void);

// @retail 0x100390
bool function_100390(long weapon_index, long barrel_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	bool result = false;

	if (definition->barrel_count > barrel_index)
	{
		s_weapon_barrel *barrel = &weapon->barrels[barrel_index];
		barrel->flag2 = true;
		result = true;
	}
	return result;
}

// @retail 0x1003e0
bool function_1003e0(long weapon_index, long barrel_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	bool result = false;

	if (definition->barrel_count > barrel_index)
	{
		s_weapon_barrel *barrel = &weapon->barrels[barrel_index];
		barrel->flag5 = true;
		result = true;
	}
	return result;
}

// @retail 0x100f00
bool function_100f00(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	bool result = true;

	if (weapon->heat >= 1.0f)
	{
		result = false;
	}
	else if (definition->magazine_count > 0 && definition->magazines[0].rounds_loaded_maximum > 0
		&& weapon->magazines[0].rounds_loaded == 0 && weapon->magazines[0].rounds_unloaded == 0)
	{
		result = false;
	}
	return result;
}

// @retail 0x101010
short function_101010(long weapon_index, short zoom_level)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

	if (!function_100880(weapon_index, NONE))
	{
		if (zoom_level >= 0 && zoom_level < definition->zoom_level_count - 1)
		{
			zoom_level++;
		}
		else
		{
			zoom_level = zoom_level == definition->zoom_level_count - 1 ? NONE : 0;
		}
	}
	return zoom_level;
}

// @retail 0x100f70
bool function_100f70(long weapon_index)
{
	return function_101010(weapon_index, NONE) != NONE;
}

// @retail 0x100fd0
bool function_100fd0(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

	return definition->zoom_level_count > 0;
}

// @retail 0x101090
real function_101090(long weapon_index, short zoom_level)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	real result = 1.0f;

	if (zoom_level >= 0 && zoom_level < definition->zoom_level_count)
	{
		real fraction;
		real minimum;
		real maximum;

		if (definition->zoom_level_count > 1)
			fraction = (real)zoom_level / (real)(definition->zoom_level_count - 1);
		else
			fraction = 0.0f;
		if (definition->zoom_magnification_minimum > 0.0f)
			minimum = definition->zoom_magnification_minimum;
		else
			minimum = 1.0f;
		if (definition->zoom_magnification_maximum > 0.0f)
			maximum = definition->zoom_magnification_maximum;
		else
			maximum = 1.0f;
		result = (real)pow(maximum / minimum, fraction) * minimum;
	}
	return result;
}

// @retail 0x101160
bool function_101160(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	bool result = false;

	for (long i = 0; i < definition->trigger_count; i++)
	{
		byte state = weapon->triggers[i].state;
		if (state == 3 || state == 7 || state == 5 || state == 6 || state == 2 && definition->value_294)
			result = true;
	}
	return result;
}

// @retail 0x1011d0
bool function_1011d0(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	bool result = false;

	for (long i = 0; i < definition->trigger_count; i++)
	{
		if (weapon->triggers[i].state == 0 && definition->triggers[i].behavior == 5)
			result = true;
	}
	return result;
}

// @retail 0x101240
bool function_101240(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

	bool result = TEST_FIELD_BIT(definition->flag3);
	return result;
}

// @retail 0x1012c0
bool function_1012c0(long weapon_index)
{
	bool result = false;

	if (weapon_index != NONE)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
		result = TEST_FIELD_BIT(definition->flag24);
	}
	return result;
}

// @retail 0x101300
bool function_101300(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

	bool result = TEST_FIELD_BIT(definition->flag28);
	return result;
}

// @retail 0x101340
bool function_101340(long weapon_index)
{
	bool result = false;

	if (weapon_index != NONE)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
		result = TEST_FIELD_BIT(definition->flag18);
	}
	return result;
}

// @retail 0x101380
bool function_101380(long weapon_index)
{
	bool result = false;

	if (weapon_index != NONE)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

		result = TEST_FIELD_BIT(definition->flag20);
		if (g_4e6948->state == 2 && !function_159d40())
			result = false;
	}
	return result;
}

// @retail 0x1013e0
bool function_1013e0(long weapon_index)
{
	bool result = false;

	if (weapon_index != NONE)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

		result = TEST_FIELD_BIT(definition->flag21);
		if (g_4e6948->state == 2 && !function_159d40())
			result = false;
	}
	return result;
}

// @retail 0x101440
bool weapon_prevents_grenade_throwing(long weapon_index)
{
	bool result = false;

	if (weapon_index != NONE)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
		result = TEST_FIELD_BIT(definition->prevents_grenade_throwing);
		if (weapon->state >= 5 && weapon->state <= 10)
			result = true;
	}
	return result;
}

// @retail 0x101640
bool function_101640(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

	if (TEST_FIELD_BIT(definition->flag22) || TEST_FIELD_BIT(definition->flag23))
		return true;
	return false;
}

// @retail 0x101980
void function_101980(long weapon_index, short rounds_loaded, short magazine_index, short rounds_total)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	short maximum = definition->magazines[magazine_index].rounds_loaded_maximum;
	short loaded = rounds_loaded < 0 ? 0 : rounds_loaded;
	short unloaded = rounds_total - loaded < 0 ? 0 : rounds_total - loaded;

	weapon->magazines[magazine_index].rounds_loaded = loaded <= maximum ? loaded : maximum;
	weapon->magazines[magazine_index].rounds_unloaded = unloaded;
}

// @retail 0x101b10
long function_101b10(long barrel_index, long weapon_index)
{
	long result = 0;

	if (barrel_index >= 0 && barrel_index < 2)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

		result = barrel_index ? 0x110000dc : 0xf0000db;
		if (barrel_index < definition->barrel_count)
		{
			s_weapon_barrel_definition *barrel = &definition->barrels[barrel_index];
			if (barrel->name && barrel->name != NONE)
				result = barrel->name;
		}
	}
	return result;
}

// @retail 0x101d20
bool function_101d20(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

	bool result = false;

	if (!definition->magazine_count)
	{
		result = true;
	}
	else
	{
		for (short i = 0; i < definition->barrel_count; i++)
		{
			if (definition->barrels[i].value_a8 > 0.0f)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

// @retail 0x1029d0
bool function_1029d0(long weapon_index, short magazine_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);

	return weapon->magazines[magazine_index].state == 2;
}

// @retail 0x102e10
void function_102e10(long weapon_index, short trigger_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	s_weapon_trigger_definition *trigger_definition = &definition->triggers[trigger_index];

	if (trigger_definition->charging_time > 0.0f)
	{
		real ticks_real = (real)g_510c54->ticks_per_second * trigger_definition->charging_time;
		long ticks;
		__asm
		{
			fld ticks_real
			fistp ticks
		}
		weapon = WEAPON_GET(weapon_index);
		weapon->triggers[trigger_index].state = 4;
		weapon->triggers[trigger_index].timer = (short)ticks;
	}
	else
	{
		short barrel_index = trigger_definition->primary_barrel;
		if (barrel_index != NONE)
		{
			s_weapon_barrel *barrel = &weapon->barrels[barrel_index];
			barrel->flag0 = true;
		}
		weapon = WEAPON_GET(weapon_index);
		weapon->triggers[trigger_index].state = 0;
		weapon->triggers[trigger_index].timer = 0;
		s_weapon_trigger *trigger = &WEAPON_GET(weapon_index)->triggers[trigger_index];
		trigger->flag3 = false;
		trigger->flag4 = false;
	}
}

// @retail 0x102f70
void function_102f70(long weapon_index, short barrel_index)
{
	if (barrel_index >= 0 && barrel_index < 2)
	{
		s_weapon_barrel *barrel = &WEAPON_GET(weapon_index)->barrels[barrel_index];
		if (barrel->state != 1)
			barrel->flag6 = false;
	}
}

// @retail 0x103a90
bool function_103a90(long weapon_index, short trigger_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	s_weapon_trigger_definition *trigger = &definition->triggers[trigger_index];
	short barrel_index = trigger->primary_barrel;
	bool result = true;

	if (barrel_index != NONE && barrel_index < definition->barrel_count && weapon->barrels[barrel_index].state)
		result = false;
	if (weapon->heat >= 1.0f)
		result = false;
	return result;
}

void function_b7360(long object_index);

// @retail 0x101740
void function_101740(long weapon_index, long other_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon *other = WEAPON_GET(other_index);

	function_b7360(weapon_index);
	function_b7360(other_index);
	if (weapon->definition_index == other->definition_index)
	{
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);

		for (long i = 0; i < definition->magazine_count; i++)
		{
			s_weapon_magazine_definition *magazine_definition = &definition->magazines[i];
			s_weapon_magazine *magazine = &weapon->magazines[i];
			s_weapon_magazine *other_magazine = &other->magazines[i];

			if (magazine->rounds_unloaded < magazine_definition->rounds_total_maximum && other_magazine->rounds_unloaded > 0)
			{
				long space = magazine_definition->rounds_total_maximum - magazine->rounds_unloaded;
				short count = (short)(other_magazine->rounds_unloaded <= space ? other_magazine->rounds_unloaded : space);
				magazine->rounds_unloaded += count;
				other_magazine->rounds_unloaded -= count;
			}
			if (magazine->rounds_unloaded < magazine_definition->rounds_total_maximum && other_magazine->rounds_loaded > 0)
			{
				long space = magazine_definition->rounds_total_maximum - magazine->rounds_unloaded;
				short count = (short)(other_magazine->rounds_loaded <= space ? other_magazine->rounds_loaded : space);
				magazine->rounds_unloaded += count;
				other_magazine->rounds_loaded -= count;
			}
		}
		if (weapon->heat > other->heat)
		{
			real heat = weapon->heat;
			weapon->heat = other->heat;
			other->heat = heat;
		}
	}
}

// @retail 0x102a00
void function_102a00(long weapon_index, long trigger_index, bool flag)
{
	if (weapon_index != NONE)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);

		if (trigger_index >= 0 && trigger_index < WEAPON_DEFINITION(weapon)->trigger_count)
		{
			s_weapon_trigger *trigger = &weapon->triggers[trigger_index];

			if (flag)
				trigger->flag7 = true;
			else
				trigger->flag7 = false;
			function_b7360(weapon_index);
		}
	}
}

// @retail 0x103dd0
void function_103dd0(long weapon_index, short barrel_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	s_weapon_barrel_definition *barrel_definition = &definition->barrels[barrel_index];
	s_weapon_barrel *barrel = &weapon->barrels[barrel_index];

	barrel->timer = 0;
	barrel->value06 = 0;
	barrel->state = 0;
	if (!TEST_FIELD_BIT(barrel_definition->flag11))
	{
		barrel = &WEAPON_GET(weapon_index)->barrels[barrel_index];
		barrel->flag0 = false;
	}
}

// @retail 0x105740
void function_105740(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);

	switch (weapon->state)
	{
	case 3:
	{
		s_weapon_barrel_definition *barrel_definition = &WEAPON_DEFINITION(weapon)->barrels[0];
		if (barrel_definition->value_9c > 0.0f && TEST_FIELD_BIT(barrel_definition->flag4))
			weapon->barrels[0].value14 = 1.0f;
		break;
	}
	case 4:
	{
		s_weapon_barrel_definition *barrel_definition = &WEAPON_DEFINITION(weapon)->barrels[1];
		if (barrel_definition->value_9c > 0.0f && TEST_FIELD_BIT(barrel_definition->flag4))
			weapon->barrels[1].value14 = 1.0f;
		break;
	}
	}
}

// @retail 0x105ba0
long function_105ba0(short type, bool flag)
{
	long result = NONE;

	switch (type)
	{
	case 1:
		result = flag ? 7 : 8;
		break;
	case 2:
		result = 9;
		break;
	case 3:
		result = flag ? 10 : 11;
		break;
	case 4:
		break;
	default:
		__assume(0);
	}
	return result;
}

// @retail 0x105be0
void function_105be0(long weapon_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);

	weapon->value254 = NONE;
	weapon->value256 = NONE;
	weapon->value258 = 0.0f;
	weapon->value250 = NONE;
}
// @retail 0x106030
bool function_106030(long weapon_index)
{
	bool result = false;

	if (g_4e6948->state == 2)
	{
		s_weapon *weapon = WEAPON_GET(weapon_index);

		if (weapon->item_flags & 1)
		{
			long unit_index = weapon->unit_index;

			if (unit_index != NONE)
			{
				s_weapon_unit *unit = WEAPON_UNIT_GET(unit_index);

				if (unit->player_index != NONE && function_159dd0(unit->player_index))
					result = true;
			}
		}
	}
	return result;
}

static inline long weapon_magazine_rounds_unloaded(s_weapon *weapon, long magazine_index, bool infinite)
{
	long rounds = 0;

	if (infinite)
	{
		s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
		if (definition->magazine_count > magazine_index)
			rounds = definition->magazines[magazine_index].rounds_total_maximum;
	}
	else
	{
		rounds = weapon->magazines[magazine_index].rounds_unloaded;
	}
	return rounds;
}

// @retail 0x1008f0
long function_1008f0(long magazine_index, long weapon_index, bool weapon_only)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	bool infinite = function_106030(weapon_index);
	long rounds = weapon_magazine_rounds_unloaded(weapon, magazine_index, infinite);

	if (weapon->unit_index != NONE && !weapon_only)
	{
		s_weapon_unit *unit = WEAPON_UNIT_GET(weapon->unit_index);

		for (long i = 0; i < 4; i++)
		{
			long other_index = unit->weapon_indices[i];

			if (other_index != NONE && other_index != weapon_index)
			{
				s_weapon *other = WEAPON_GET(other_index);
				if (other->definition_index == weapon->definition_index)
					rounds += weapon_magazine_rounds_unloaded(other, magazine_index, infinite);
			}
		}
	}
	return rounds;
}

// @retail 0x100b40
long function_100b40(long magazine_index, long weapon_index, bool weapon_only)
{
	s_weapon_magazine *magazine = &WEAPON_GET(weapon_index)->magazines[magazine_index];

	return function_1008f0(magazine_index, weapon_index, weapon_only) + magazine->rounds_loaded;
}

// @retail 0x103e60
void function_103e60(long weapon_index, short barrel_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	s_weapon_barrel_definition *barrel_definition = &definition->barrels[barrel_index];
	s_weapon_barrel *barrel = &weapon->barrels[barrel_index];
	real total = (real)g_510c54->ticks_per_second * barrel_definition->value_20;
	real partial = (1.0f - barrel_definition->value_24) * total;
	long ticks = (long)floor(partial);
	real fraction = (real)(total - floor(total));

	barrel->accumulator += fraction;
	if (barrel->accumulator >= 1.0f)
	{
		real whole = (real)floor(barrel->accumulator);
		barrel->accumulator -= whole;
		ticks += (long)whole;
	}
	barrel->ticks = (short)ticks;
	barrel->state = 2;
}

// @retail 0x103f60
void function_103f60(long weapon_index, short barrel_index)
{
	s_weapon *weapon = WEAPON_GET(weapon_index);
	s_weapon_definition *definition = WEAPON_DEFINITION(weapon);
	s_weapon_barrel_definition *barrel_definition = &definition->barrels[barrel_index];
	s_weapon_barrel *barrel = &weapon->barrels[barrel_index];
	long ticks = (long)floor(g_510c54->ticks_per_second * barrel_definition->value_20);

	ticks -= (long)floor((1.0f - barrel_definition->value_24) * g_510c54->ticks_per_second * barrel_definition->value_20);
	barrel->state = 3;
	barrel->ticks = (short)ticks;
	if (!ticks)
		function_103dd0(weapon_index, barrel_index);
}

// @retail 0x105dd0
void __stdcall function_105dd0(long object_index, long unused, s_weapon_sound_event *event)
{
	if (event->type == 1 && event->sound_index != NONE)
	{
		s_object_marker marker;

		if (event->marker_name == NONE || event->marker_name == 0x600008a || function_b8d30(false, object_index, event->marker_name, 1, &marker) < 1)
		{
			marker.node_index = 0;
			marker.node_matrix.position = *g_468788;
			marker.node_matrix.forward = *g_4687a8;
		}
		function_189060(object_index, marker.node_index, 1.0f, &marker.node_matrix.position, &marker.node_matrix.forward, event->sound_index);
	}
}
