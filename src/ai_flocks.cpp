// @flags /O2 /Ob1 /arch:SSE /Gr
/* AI_FLOCKS.CPP: the flocks of the scenario: creating one, activating one and
   finding one by its scenario name (outside functions lane A's script
   functions need) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "real_math.h"

/* a flock (g_51ecb4, ai.cpp): the index of its scenario definition at +2 */
struct s_flock
{
	short salt;
	short definition_index;
	long unknown04;
	short unknown08;
	short unknown0a;
	bool unknown0c;
	byte unknown0d;
	bool unknown0e;
	byte unknown0f[0x24 - 0xf];
	short unknown24;
	byte unknown26[0x28 - 0x26];
};

/* the scenario's flocks (0x84 bytes each), named at +0x80 */
struct s_scenario_flock
{
	short structure_bsp_index;
	byte unknown02[2];
	short trigger_volume_index;
	byte flags;
	byte unknown07[0x2c - 7];
	long unknown2c;
	byte unknown30[0x80 - 0x30];
	long name;
};

struct s_scenario_flocks_view
{
	byte unknown000[0x350];
	long flock_count;
	s_scenario_flock *flocks;
};

extern s_data_array *g_51ecb4;

/* a new flock of a scenario flock definition */
// @retail 0x293070
long flock_new(short definition_index)
{
	long flock_index = datum_new(g_51ecb4);
	if (flock_index != NONE)
	{
		s_flock *flock = &((s_flock *)g_51ecb4->data)[flock_index & 0xffff];
		flock->unknown04 = NONE;
		flock->unknown0a = NONE;
		flock->unknown08 = 0;
		flock->definition_index = definition_index;
		flock->unknown24 = 0;
		flock->unknown0c = false;
		flock->unknown0e = true;
	}
	return flock_index;
}

/* an object's flock membership: its flock and the next member */
struct s_flock_member
{
	long flock_index;
	byte unknown04[4];
	long next_object_index;
};

/* the object fields a flock member has (+0x134 is 1 for a creature) */
struct s_flock_object
{
	byte unknown000[0x134];
	long type134;
	byte unknown138[2];
	short flock_member_offset;
};

struct s_flock_object_header
{
	byte unknown00[8];
	s_flock_object *object;
};

inline s_flock_member *flock_object_get_member(s_flock_object *object)
{
	s_flock_member *member = NULL;
	if (object->type134 == 1)
		member = (s_flock_member *)((byte *)object + object->flock_member_offset);
	return member;
}

void __stdcall function_b8540(long object_index);

/* deletes a flock and every object of it */
// @retail 0x2937a0
void flock_delete(long flock_index)
{
	long next_object_index = ((s_flock *)g_51ecb4->data)[flock_index & 0xffff].unknown04;
	while (next_object_index != NONE)
	{
		long object_index = next_object_index;
		s_flock_object *object = ((s_flock_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		s_flock_member *member = flock_object_get_member(object);
		next_object_index = member ? member->next_object_index : NONE;

		member = flock_object_get_member(object);
		if (member)
			member->flock_index = NONE;
		function_b8540(object_index);
	}
	datum_delete(g_51ecb4, flock_index);
}

/* creates the flock of a scenario flock definition */
// @retail 0x2930c0
bool flock_create(long definition_index)
{
	bool result = false;
	s_scenario_flocks_view *scenario = (s_scenario_flocks_view *)g_4e0350;

	if (definition_index >= 0 && definition_index < scenario->flock_count)
	{
		s_scenario_flock *definition = &scenario->flocks[definition_index];
		if (definition->unknown2c != NONE)
		{
			long flock_index = flock_new((short)definition_index);
			if (flock_index != NONE)
			{
				if (definition->flags & 2)
					((s_flock *)g_51ecb4->data)[flock_index & 0xffff].unknown0e = false;
				result = true;
			}
		}
	}
	return result;
}

/* the scenario's trigger volumes (0x44 bytes each), their extents at +0x30 */
struct s_flock_trigger_volume
{
	byte unknown00[0x30];
	real extents[3];
	byte unknown3c[0x44 - 0x3c];
};

struct s_scenario_flock_volumes_view
{
	byte unknown000[0x108];
	long trigger_volume_count;
	s_flock_trigger_volume *trigger_volumes;
	byte unknown110[0x350 - 0x110];
	long flock_count;
	s_scenario_flock *flocks;
};

bool function_11c380(long trigger_volume_index, real_matrix4x3 *matrix); /* unknown_11c380.cpp */

/* whether a point (within a radius) is inside the trigger volume of a flock's
   scenario definition */
// @retail 0x295490
bool function_295490(long flock_index, real_point3d const *point, real radius)
{
	s_scenario_flock_volumes_view *scenario = (s_scenario_flock_volumes_view *)g_4e0350;
	s_scenario_flock *definition = &scenario->flocks[((s_flock *)g_51ecb4->data)[flock_index & 0xffff].definition_index];
	bool result = false;

	if (definition->trigger_volume_index >= 0 && definition->trigger_volume_index < scenario->trigger_volume_count)
	{
		real_matrix4x3 matrix;

		if (function_11c380(definition->trigger_volume_index, &matrix))
		{
			s_flock_trigger_volume *volume = &scenario->trigger_volumes[definition->trigger_volume_index];
			real_vector3d vector;

			vector3d_from_points3d(&matrix.position, point, &vector);
			result = true;
			for (short i = 0; i < 3; i++)
			{
				real distance = dot_product3d(&vector, &(&matrix.forward)[i]);
				if (-radius > distance || distance > volume->extents[i] + radius)
				{
					result = false;
					break;
				}
			}
		}
	}
	return result;
}

/* the scenario flock definition with the name, or NONE */
// @retail 0x295860
short flock_definition_find(long name)
{
	s_scenario_flocks_view *scenario = (s_scenario_flocks_view *)g_4e0350;
	short result = NONE;

	for (short i = 0; i < scenario->flock_count; i++)
	{
		s_scenario_flock *flock = &scenario->flocks[i];
		if (flock->name == name)
		{
			result = i;
			break;
		}
	}
	return result;
}

/* the flock whose scenario definition has the name, or NONE */
// @retail 0x2958a0
long function_2958a0(long name)
{
	s_data_iterator iterator;

	if (g_4f55d0->active)
	{
		iterator.data = g_51ecb4;
		iterator.index = NONE;
	}
	while (g_4f55d0->active)
	{
		s_flock *flock = (s_flock *)data_iterator_next_inlined(&iterator);

		if (!flock)
			break;
		if (((s_scenario_flocks_view *)g_4e0350)->flocks[flock->definition_index].name == name)
			return iterator.datum_index;
	}
	return NONE;
}
