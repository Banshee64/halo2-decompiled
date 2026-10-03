// @flags /O2 /Ob1 /Gr
/* AI_FLOCKS.CPP: finding a flock by its scenario name (an outside function
   lane A's script functions need) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"

/* a flock (g_51ecb4, ai.cpp): the index of its scenario definition at +2 */
struct s_flock
{
	short salt;
	short definition_index;
	byte unknown04[0x28 - 4];
};

/* the scenario's flocks (0x84 bytes each), named at +0x80 */
struct s_scenario_flock
{
	byte unknown00[0x80];
	long name;
};

struct s_scenario_flocks_view
{
	byte unknown000[0x350];
	long flock_count;
	s_scenario_flock *flocks;
};

extern s_data_array *g_51ecb4;

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
