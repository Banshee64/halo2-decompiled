// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_19EC40.CPP: finds entries of the 4e0350 globals' second table by
   position and by three optional 16-bit keys */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "unknown_19ec40.h"
#include <math.h>

// @retail 0x19ec40
long function_19ec40(
	real_point3d const *point,
	real height,
	short key_a,
	short key_b,
	short key_c,
	long maximum_count,
	long *results,
	real radius)
{
	s_palette_source_globals *globals = g_4e0350;
	real best = 0.0f;
	real radius_squared = radius * radius;
	long count = 0;
	short i = 0;

	if (globals->marker_count > 0)
	{
		do
		{
			s_marker_entry *entry = &globals->marker_entries[i];

			if (key_a == -1 || key_a == entry->key_a)
			{
				if (key_b == -1 || key_b == entry->key_b)
				{
					if (key_c == -1 || key_c == entry->key_c)
					{
						real distance_squared = 0.0f;

						if (point)
						{
							real_point3d position = globals->marker_entries[i].position;
							real_vector3d delta;

							vector3d_from_points3d(&position, point, &delta);
							distance_squared = magnitude_squared3d(&delta);
							if (radius >= 0.0f && distance_squared > radius_squared)
								goto next;
							if (height > 0.0f && fabs(entry->position.z - point->z) > height)
								goto next;
						}

						if (count < maximum_count)
							results[count++] = i;
						else if (count == 1 && best > distance_squared)
						{
							results[0] = i;
							best = distance_squared;
						}
					}
				}
			}
next:
			i++;
		}
		while (i < globals->marker_count);
	}

	return count;
}
