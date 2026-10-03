// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_11A4D0.CPP: unit queries and flag setters of the script functions */

#include "cseries.h"
#include "globals.h"
#include "unknown_11a4d0.h"
#include "unknown_1dee50.h"
#include "unit_requests.h"
#include <string.h>

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
	byte unknown0ab[0xe4 - 0xab];
	real maximum_body_vitality;
	real maximum_shield_vitality;
	real body_vitality;
	real shield_vitality;
	byte unknownf4[0x10a - 0xf4];
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
	union
	{
		dword unit_flags;
		struct
		{
			dword : 16;
			dword unit_flag16 : 1;
			dword : 2;
			dword unit_flag19 : 1;
			dword : 12;
		};
	};
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

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);

inline void object_set_maximum_vitality(long object_index, real maximum_body_vitality, real maximum_shield_vitality)
{
	if (object_index != NONE)
	{
		s_unit_11a4d0 *object = unit_get_11a4d0(object_index);
		if (!TEST_FIELD_BIT(object->flag10a_2))
		{
			object->maximum_body_vitality = maximum_body_vitality;
			object->maximum_shield_vitality = maximum_shield_vitality;
			object->body_vitality = maximum_body_vitality > 0.0f ? 1.0f : 0.0f;
			object->shield_vitality = maximum_shield_vitality > 0.0f ? 1.0f : 0.0f;
		}
	}
}

/* sets the vitality of every object of an object list (full where the
   maximum is above zero) */
// @retail 0x11a220
void function_11a220(long list_index, real maximum_body_vitality, real maximum_shield_vitality)
{
	long reference_index;
	long object_index = object_list_get_first(list_index, &reference_index);
	while (object_index != NONE)
	{
		object_set_maximum_vitality(object_index, maximum_body_vitality, maximum_shield_vitality);
		object_index = object_list_get_next(&reference_index);
	}
}

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

/* sets a flag of every unit of an object list */
// @retail 0x11a570
void function_11a570(long list_index, bool flag)
{
	long reference_index;
	long object_index = object_list_get_first(list_index, &reference_index);
	while (object_index != NONE)
	{
		s_unit_11a4d0 *unit = (s_unit_11a4d0 *)function_badc0(object_index, 3);
		if (unit)
		{
			if (flag)
				unit->unit_flag19 = true;
			else
				unit->unit_flag19 = false;
		}
		object_index = object_list_get_next(&reference_index);
	}
}

/* sets another flag of every unit of an object list */
// @retail 0x11a680
void function_11a680(long list_index)
{
	long reference_index;
	long object_index = object_list_get_first(list_index, &reference_index);
	while (object_index != NONE)
	{
		s_unit_11a4d0 *unit = (s_unit_11a4d0 *)function_badc0(object_index, 3);
		if (unit)
			unit->unit_flag16 = true;
		object_index = object_list_get_next(&reference_index);
	}
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

// @retail 0x11a8c0
void function_11a8c0(long unit_index)
{
	s_unit_request request;
	memset(&request, 0, sizeof(request));
	request.type = 0x17;
	request.type17.unknown4 = false;
	request.type17.unknown5 = false;
	function_e6900(unit_index, &request);
}

// @retail 0x11a910
void function_11a910(long unit_index)
{
	s_unit_request request;
	memset(&request, 0, sizeof(request));
	request.type = 0x18;
	function_e6900(unit_index, &request);
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

long players_first_active_local_player(void);
bool function_14ddc0(long local_player_index);
long function_14de70(long local_player_index);

/* the players (g_4e8c24, a local view) */
struct s_player_11a4d0
{
	byte unknown000[0x2c];
	long unit_index;
	byte unknown030[0x21c - 0x30];
};

/* sends a request to the unit of the first local player */
// @retail 0x11b350
void function_11b350(void)
{
	long local_player_index = players_first_active_local_player();
	if (function_14ddc0(local_player_index))
	{
		long player_index = function_14de70(local_player_index);
		long unit_index = ((s_player_11a4d0 *)g_4e8c24->data)[player_index & 0xffff].unit_index;
		if (unit_index != NONE)
		{
			s_unit_request request;
			memset(&request, 0, sizeof(request));
			request.type = 0x1a;
			request.type1a.unknown4 = 0;
			function_e6900(unit_index, &request);
		}
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
