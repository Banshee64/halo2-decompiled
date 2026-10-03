// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_218C60.CPP: sound definition queries: a permutation's sample
   count, and a sound's maximum distance */

#include "cseries.h"
#include "globals.h"

/* a sound tag's definition, as these functions read it */
struct s_sound_definition_view
{
	word flags;
	char promotion_index;
	byte unknown03[3];
	short class_index;
	short first_pitch_range_index;
};

struct s_sound_pitch_range_view
{
	byte unknown00[8];
	short first_permutation_index;
	short permutation_count;
};

struct s_sound_permutation_view
{
	byte unknown00[6];
	word sample_count;
	byte unknown08[8];
};

struct s_sound_globals_view
{
	byte unknown00[4];
	struct
	{
		real minimum_distance;
		real maximum_distance;
		byte unknown08[0x38 - 8];
	} *classes;
	byte unknown08[0x24 - 8];
	s_sound_pitch_range_view *pitch_ranges;
	byte unknown28[4];
	s_sound_permutation_view *permutations;
};

/* a promotion's distances (function_221810) */
struct s_sound_promotion_view
{
	byte unknown00[0x18];
	real minimum_distance;
	real maximum_distance;
};

struct s_unknown_5c;
s_unknown_5c *function_221810(short index);

static inline s_sound_definition_view *sound_definition_get(long definition_index)
{
	return (s_sound_definition_view *)g_4e3b44[definition_index & 0xffff].bytes;
}

// @retail 0x218c60
real function_218c60(long definition_index, long pitch_range_index, long permutation_index)
{
	s_sound_definition_view *definition = sound_definition_get(definition_index);
	s_sound_globals_view *globals = (s_sound_globals_view *)g_51ebd4;
	s_sound_pitch_range_view *pitch_range = &globals->pitch_ranges[definition->first_pitch_range_index + pitch_range_index];

	return (real)globals->permutations[pitch_range->first_permutation_index + permutation_index].sample_count;
}

// @retail 0x218d30
real function_218d30(long definition_index)
{
	s_sound_definition_view *definition = sound_definition_get(definition_index);

	if (definition->flags & 0x800)
	{
		return ((s_sound_promotion_view *)function_221810(definition->promotion_index))->maximum_distance;
	}
	return ((s_sound_globals_view *)g_51ebd4)->classes[definition->class_index].maximum_distance;
}
