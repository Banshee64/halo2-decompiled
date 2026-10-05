#include "unknown_11c920.h"
#include "unknown_0259d0.h"
// @flags /O2 /arch:SSE /Gr

struct s_sphere_frustum;
long function_181800(s_sphere_frustum const *frustum, point3f const *center, real radius);

struct s_frustum_section_ranges
{
	short unknown0;
	short counts[6];
	short first_indices[6];
};

struct s_frustum_set_view
{
	short group_count;
	byte unknown2[0x1974 - 2];
};

// @retail 0x165010
bool function_165010(s_frustum_set_view const *set, long section_index, point3f const *center, real radius, bool *contained)
{
	bool result = false;
	s_frustum_section_ranges const *section = (s_frustum_section_ranges const *)((byte const *)set + 0xa6e) + section_index;
	long group;

	*contained = false;
	for (group = 0; group < set->group_count; group++)
	{
		long i;

		for (i = 0; i < section->counts[group] && !*contained; i++)
		{
			long frustum_index = section->first_indices[group] + i;
			s_sphere_frustum const *frustum = (s_sphere_frustum const *)((byte const *)set + 0x1974 + frustum_index * 0x108);
			long classification = function_181800(frustum, center, radius);

			if (classification)
			{
				result = true;
				*contained = classification == 2;
			}
		}
	}
	return result;
}
