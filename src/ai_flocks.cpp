// @flags /O2 /Ob1 /Gr
/* AI_FLOCKS.CPP: the flocks of the scenario: creating one, activating one and
   finding one by its scenario name (outside functions lane A's script
   functions need) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"

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
	byte unknown02[4];
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
