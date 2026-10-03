// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_134D20.CPP: the scenario's named interpolators (the block at
   +0x3c0 of g_4e0350) and their state in g_4e6740 (unknown_134d90.cpp) */

#include "cseries.h"
#include "globals.h"
#include "unknown_134d20.h"

/* the scenario's interpolators, 0x18 bytes each */
struct s_scenario_interpolator
{
	long name;
	byte unknown04[0x18 - 4];
};

struct s_scenario_interpolators_view
{
	byte unknown000[0x3c0];
	long interpolator_count;
	s_scenario_interpolator *interpolators;
};

/* g_4e6740: the name last looked up, then the state of each interpolator */
struct s_unknown_134d90;
extern s_unknown_134d90 *g_4e6740;

struct s_interpolator_globals
{
	long last_name;
	s_interpolator_state states[16];
};

/* unknown_146240.cpp */
real game_time_get_seconds(void);

// @retail 0x134d20
s_interpolator_state *interpolator_get(long name, long *index_out)
{
	s_interpolator_globals *globals = (s_interpolator_globals *)g_4e6740;
	long index = NONE;
	if (globals && g_4e0350 && name)
	{
		s_scenario_interpolators_view *scenario = (s_scenario_interpolators_view *)g_4e0350;
		for (long i = 0; i < scenario->interpolator_count; i++)
		{
			long interpolator_name = scenario->interpolators[i].name;
			if (interpolator_name && interpolator_name == name)
			{
				globals->last_name = name;
				index = i;
				break;
			}
		}
	}
	if (index != NONE)
	{
		if (index_out)
			*index_out = index;
		return &globals->states[index];
	}
	if (index_out)
		*index_out = NONE;
	return NULL;
}
// @retail 0x135110
long interpolator_start(long name, real target, real seconds)
{
	long index = NONE;
	s_interpolator_state *state = interpolator_get(name, &index);
	if (state)
	{
		real time = game_time_get_seconds();
		state->flag1 = false;
		state->active = true;
		state->start_time = time;
		state->time10 = time;
		state->start_value = state->value;
		state->target_value = target;
		state->end_time = time + seconds;
		state->value18 = 0.0f;
	}
	return index;
}

// @retail 0x135210
long interpolator_resume(long name)
{
	long index = NONE;
	s_interpolator_state *state = interpolator_get(name, &index);
	if (state && !TEST_FIELD_BIT(state->active))
	{
		real time = game_time_get_seconds();
		real elapsed = time - state->start_time;
		state->time10 += elapsed;
		state->end_time += elapsed;
		state->start_time = time;
		state->active = true;
	}
	return index;
}

// @retail 0x135290
bool interpolator_exists(long name)
{
	s_interpolator_globals *globals = (s_interpolator_globals *)g_4e6740;
	if (globals && g_4e0350 && name)
	{
		s_scenario_interpolators_view *scenario = (s_scenario_interpolators_view *)g_4e0350;
		for (long index = 0; index < scenario->interpolator_count; index++)
		{
			long interpolator_name = scenario->interpolators[index].name;
			if (interpolator_name && interpolator_name == name)
			{
				globals->last_name = name;
				break;
			}
		}
	}
	return false;
}