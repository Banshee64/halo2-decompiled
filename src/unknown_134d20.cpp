// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_134D20.CPP: the scenario's named interpolators (the block at
   +0x3c0 of g_4e0350) and their state in g_4e6740 (unknown_134d90.cpp) */

#include "unknown_11c920.h"
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

/* the state of an interpolator by name (no retail function: always inlined) */
inline s_interpolator_state *interpolator_find(long name)
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
		return &globals->states[index];
	return NULL;
}

/* the same lookup with a single result, for the one caller that needs this shape */
__forceinline s_interpolator_state *interpolator_find_flagged(long name)
{
	s_interpolator_globals *globals = (s_interpolator_globals *)g_4e6740;
	s_interpolator_state *result = NULL;
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
		result = &globals->states[index];
	return result;
}

/* starts an interpolator that stops at its target */
// @retail 0x135180
long function_135180(long name, real target, real seconds)
{
	long index = interpolator_start(name, target, seconds);
	if (index != NONE)
		interpolator_find_flagged(name)->flag1 = true;
	return index;
}

// @retail 0x135330
real interpolator_get_value18(long name)
{
	s_interpolator_state *state = interpolator_find(name);
	return state ? state->value18 : 0.0f;
}

// @retail 0x135450
real interpolator_get_time10(long name)
{
	s_interpolator_state *state = interpolator_find(name);
	return state ? state->time10 : 0.0f;
}

// @retail 0x1354c0
real interpolator_get_end_time(long name)
{
	s_interpolator_state *state = interpolator_find(name);
	return state ? state->end_time : 0.0f;
}

// @retail 0x135750
void function_135750(void)
{
	s_interpolator_globals *globals = (s_interpolator_globals *)g_4e6740;
	if (globals && g_4e0350)
	{
		s_scenario_interpolators_view *scenario = (s_scenario_interpolators_view *)g_4e0350;
		for (long i = 0; i < scenario->interpolator_count; i++)
			globals->states[i].active = false;
	}
}

// @retail 0x135790
void function_135790(void)
{
	s_interpolator_globals *globals = (s_interpolator_globals *)g_4e6740;
	if (globals)
	{
		s_scenario_interpolators_view *scenario = (s_scenario_interpolators_view *)g_4e0350;
		if (scenario)
		{
			real time = game_time_get_seconds();
			for (long i = 0; i < scenario->interpolator_count; i++)
			{
				s_interpolator_state *state = &globals->states[i];
				if (!state->active)
				{
					real shifted = state->time10;
					real elapsed = time - state->start_time;
					shifted += elapsed;
					state->time10 = shifted;
					shifted = state->end_time;
					shifted += elapsed;
					state->end_time = shifted;
					state->start_time = time;
					state->active = true;
				}
			}
		}
	}
}

// @retail 0x135820
void function_135820(void)
{
	s_interpolator_globals *globals = (s_interpolator_globals *)g_4e6740;
	if (globals && g_4e0350)
	{
		long name = globals->last_name;
		if (name)
		{
			long index;
			s_interpolator_state *state = interpolator_get(name, &index);
			real target = 0.0f;
			real value = state ? state->value : target;
			if (value < 0.5f)
				target = 1.0f;
			interpolator_start(name, target, 2.0f);
		}
	}
}

real function_134c50(real value);

// @retail 0x1353a0
real function_1353a0(long name)
{
	s_interpolator_state *state = interpolator_find(name);
	real result = 0.0f;
	if (state && state->end_time > state->time10)
	{
		result = 0.0f > (state->start_time - state->time10) / (state->end_time - state->time10) ?
			0.0f : (state->start_time - state->time10) / (state->end_time - state->time10);
		if (state->flag1)
			result = function_134c50(result);
	}
	return result;
}
