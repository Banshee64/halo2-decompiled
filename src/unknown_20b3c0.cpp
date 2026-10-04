// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_20B3C0.CPP: the length in seconds of a sound's permutation (an
   outside function lane A's looping-sound script query 0x18a240 needs) */

#include "cseries.h"
#include "globals.h"

/* a pitch range and permutation of a sound (NONE bytes for none) */
struct s_sound_permutation_reference
{
	char pitch_range_index;
	char permutation_index;
};

/* a sound tag (a local view) */
struct s_sound_definition_20b3c0
{
	byte unknown00[0x10];
	long duration;
};

real sound_definition_permutation_duration(long definition_index, long pitch_range_index, long permutation_index, real pitch);
real function_218c60(long definition_index, long pitch_range_index, long permutation_index);

// @retail 0x20b3c0
real function_20b3c0(long definition_index, s_sound_permutation_reference const *permutation)
{
	real duration = 0.0f;

	if (definition_index != NONE)
	{
		s_sound_definition_20b3c0 *definition = (s_sound_definition_20b3c0 *)g_4e3b44[definition_index & 0xffff].bytes;

		if (permutation->permutation_index != NONE && permutation->pitch_range_index != NONE)
		{
			if (g_4e6948->state == 1 && g_4e6948->flag134)
				duration = function_218c60(definition_index, permutation->pitch_range_index, permutation->permutation_index);
			else
				duration = sound_definition_permutation_duration(definition_index, permutation->pitch_range_index, permutation->permutation_index, 0.0f);
		}
		else
		{
			duration = (real)definition->duration;
		}
	}
	return duration * 0.001f;
}
