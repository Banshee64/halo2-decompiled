// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_11A4D0.CPP: unit queries and flag setters of the script functions */

#include "cseries.h"
#include "globals.h"
#include "unknown_11a4d0.h"

#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

/* the units (a local view of the object data) */
struct s_unit_11a4d0
{
	long definition_index;
	byte unknown004[0x88 - 4];
	real_vector3d vector88;
	byte unknown094[0xaa - 0x94];
	byte object_type;
	byte unknown0ab[0x10a - 0xab];
	union
	{
		word flags10a;
		struct
		{
			word : 2;
			word flag10a_2 : 1;
			word : 13;
		};
	};
	byte unknown10c[0x12a - 0x10c];
	short animation_offset;
	byte unknown12c[0x134 - 0x12c];
	dword unit_flags;
	byte unknown138[0x212 - 0x138];
	char weapon_index_a;
	char weapon_index_b;
	byte unknown214[0x218 - 0x214];
	long weapon_object_indices[4];
	byte unknown228[0x33e - 0x228];
	short offset33e;
	byte unknown340[0x346 - 0x340];
	short offset346;
	struct
	{
		byte bit0 : 1;
		byte : 7;
	} flags348;
};

struct s_unit_animation_11a4d0
{
	long index0;
	byte unknown04[2];
	short index6;
	byte unknown08[0x68 - 8];
	long index68;
	byte unknown6c[0x7c - 0x6c];
	long state_name;
};

struct s_unit_346_11a4d0
{
	byte unknown0[8];
	dword : 18;
	dword flag18 : 1;
	dword : 13;
};

struct s_object_header_11a4d0
{
	byte unknown00[8];
	s_unit_11a4d0 *object;
};

inline s_unit_11a4d0 *unit_get_11a4d0(long object_index)
{
	return ((s_object_header_11a4d0 *)g_4e0300->data)[object_index & 0xffff].object;
}

struct s_1d9240;
void function_1d9240(s_1d9240 *p, char flag, real x);

/* whether the unit holds a weapon of the given definition */
// @retail 0x11a4d0
bool function_11a4d0(long unit_index, long definition_index)
{
	bool result = false;
	if (unit_index != NONE && definition_index != NONE)
	{
		s_unit_11a4d0 *unit = unit_get_11a4d0(unit_index);
		short weapon_index = unit->weapon_index_a;
		if (weapon_index != NONE)
		{
			long weapon_object_index = unit->weapon_object_indices[weapon_index];
			if (weapon_object_index != NONE && unit_get_11a4d0(weapon_object_index)->definition_index == definition_index)
				result = true;
		}
		weapon_index = unit->weapon_index_b;
		if (weapon_index != NONE)
		{
			long weapon_object_index = unit->weapon_object_indices[weapon_index];
			if (weapon_object_index != NONE && unit_get_11a4d0(weapon_object_index)->definition_index == definition_index)
				result = true;
		}
	}
	return result;
}

// @retail 0x11a770
void function_11a770(long unit_index, bool flag)
{
	if (unit_index != NONE)
	{
		s_unit_11a4d0 *unit = unit_get_11a4d0(unit_index);
		SET_FLAG(unit->unit_flags, 20, flag);
		unit->vector88 = *g_4687a4;
		if (unit->object_type == 0)
			unit_get_11a4d0(unit_index)->flags348.bit0 = false;
	}
}

// @retail 0x11a7f0
void function_11a7f0(long unit_index, bool flag)
{
	if (unit_index != NONE)
	{
		dword *flags = &unit_get_11a4d0(unit_index)->unit_flags;
		SET_FLAG(*flags, 18, !flag);
	}
}

// @retail 0x11a960
bool function_11a960(long unit_index)
{
	bool result = false;
	if (unit_index != NONE)
	{
		s_unit_11a4d0 *unit = unit_get_11a4d0(unit_index);
		if (!TEST_FIELD_BIT(unit->flag10a_2))
			result = ((s_unit_346_11a4d0 *)((byte *)unit + unit->offset346))->flag18;
	}
	return result;
}

// @retail 0x11aac0
void function_11aac0(long unit_index, short ticks)
{
	if (unit_index != NONE)
	{
		s_unit_11a4d0 *unit = unit_get_11a4d0(unit_index);
		function_1d9240((s_1d9240 *)((byte *)unit + unit->offset33e + 0x88), false, (real)ticks * (1.0f / 30.0f));
	}
}

// @retail 0x11ab10
void function_11ab10(long unit_index, short ticks)
{
	if (unit_index != NONE)
	{
		s_unit_11a4d0 *unit = unit_get_11a4d0(unit_index);
		function_1d9240((s_1d9240 *)((byte *)unit + unit->offset33e + 0x88), true, (real)ticks * (1.0f / 30.0f));
	}
}

// @retail 0x11b3e0
void function_11b3e0(long unit_index, bool flag)
{
	if (unit_index != NONE)
	{
		dword *flags = &unit_get_11a4d0(unit_index)->unit_flags;
		SET_FLAG(*flags, 11, flag);
	}
}

// @retail 0x11b420
void function_11b420(long unit_index, bool flag)
{
	if (unit_index != NONE)
	{
		dword *flags = &unit_get_11a4d0(unit_index)->unit_flags;
		SET_FLAG(*flags, 12, !flag);
	}
}

// @retail 0x11b460
void function_11b460(long unit_index, bool flag)
{
	if (unit_index != NONE)
	{
		dword *flags = &unit_get_11a4d0(unit_index)->unit_flags;
		SET_FLAG(*flags, 31, flag);
	}
}

/* whether the unit's current animation state is 0xe0000c2 */
// @retail 0x11b930
bool function_11b930(long unit_index)
{
	bool result = false;
	if (unit_index != NONE)
	{
		s_unit_11a4d0 *unit = unit_get_11a4d0(unit_index);
		s_unit_animation_11a4d0 *animation = (s_unit_animation_11a4d0 *)((byte *)unit + unit->animation_offset);
		long state_name = NONE;
		if (animation->index68 != NONE && animation->index0 != NONE && animation->index6 != NONE)
			state_name = animation->state_name;
		result = state_name == 0xe0000c2;
	}
	return result;
}
