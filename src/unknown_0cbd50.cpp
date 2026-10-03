// @flags /O2 /Gr
/* UNKNOWN_0CBD50.CPP: two unit weapon queries (lane T decompiled them for
   first_person_weapons.cpp, which calls both with register arguments) */

#include "cseries.h"
#include "globals.h"

/* the unit fields read here */
struct s_unit_weapons_view
{
	byte unknown000[0x212];
	char current_weapon_index;
	char current_dual_weapon_index;
	byte unknown214[4];
	long weapon_object_indices[4];
};

struct s_unit_weapons_header
{
	byte unknown00[8];
	s_unit_weapons_view *unit;
};

static inline s_unit_weapons_view *unit_weapons_get(long unit_index)
{
	return ((s_unit_weapons_header *)g_4e0300->data)[unit_index & 0xffff].unit;
}

// @retail 0xcbd50
long function_cbd50(long unit_index, short weapon_index)
{
	long result = NONE;

	if (weapon_index != NONE)
	{
		result = unit_weapons_get(unit_index)->weapon_object_indices[weapon_index];
	}
	return result;
}

// @retail 0xcd660
bool function_cd660(long unit_index)
{
	s_unit_weapons_view *unit = unit_weapons_get(unit_index);
	bool result = false;

	if (unit->current_weapon_index != NONE && unit->current_dual_weapon_index != NONE)
	{
		result = true;
	}
	return result;
}
