// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_10AAC0.CPP: the interpolated values of g_5107f4
   (unknown_10a980.cpp): each of its 32 entries blends from one value to
   another over a time, for one object and name */

#include "cseries.h"
#include "globals.h"

struct s_unknown_10a980;
extern s_unknown_10a980 *g_5107f4;

struct s_interpolated_value
{
	long name;
	real initial_value;
	real final_value;
	long start_time;
	long duration;
};

struct s_interpolated_values
{
	s_interpolated_value entries[32];
	long object_indices[32];
};

// @retail 0x10aac0
bool function_10aac0(long object_index, long name, real *value, long *index)
{
	bool result = false;

	*index = NONE;
	if (object_index != NONE)
	{
		s_interpolated_values *values = (s_interpolated_values *)g_5107f4;

		for (long i = 0; i < sizeof(values->object_indices) / sizeof(values->object_indices[0]); i++)
		{
			if (values->object_indices[i] == object_index && values->entries[i].name == name)
			{
				s_interpolated_value *entry = &values->entries[i];
				real initial_value = entry->initial_value;
				real final_value = entry->final_value;
				long duration = entry->duration;
				long elapsed = g_510c54->game_time - entry->start_time;

				if (elapsed < duration && duration)
					*value = (real)elapsed / (real)duration * (final_value - initial_value) + initial_value;
				else
					*value = final_value;
				*index = i;
				return true;
			}
		}
	}
	return result;
}
