// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_20B3C0.CPP: the length of a sound permutation in seconds (an
   outside function the looping sound code in 0x189ee0, 0x18a240 and 0x18bf90
   needs) */

#include "cseries.h"
#include "globals.h"

/* a sound tag's definition, as this file reads it */
struct s_sound_duration_definition
{
	byte unknown00[0x10];
	long duration;
};

real function_218c60(long definition_index, long pitch_range_index, long permutation_index);
real sound_definition_permutation_duration(long definition_index, long pitch_range_index, long permutation_index, real pitch);
bool function_138820();

// @retail 0x20b3c0
real sound_permutation_reference_duration(long definition_index, s_sound_permutation_reference const *reference)
{
	real result = 0.0f;

	if (definition_index != NONE)
	{
		s_sound_duration_definition *definition = (s_sound_duration_definition *)g_4e3b44[definition_index & 0xffff].data;

		if (reference->permutation_index != NONE && reference->pitch_range_index != NONE)
		{
			if (function_138820())
			{
				result = function_218c60(definition_index, reference->pitch_range_index, reference->permutation_index);
			}
			else
			{
				result = sound_definition_permutation_duration(definition_index, reference->pitch_range_index, reference->permutation_index, 0.0f);
			}
		}
		else
		{
			result = (real)definition->duration;
		}
	}
	return result * 0.001f;
}
