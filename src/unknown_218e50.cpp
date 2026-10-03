// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_218E50.CPP: a sample of a sound permutation's promotion data.
   Decompiled by lane F for 0x18c720. */

#include "cseries.h"
#include "globals.h"
#include "sound_promotions.h"

// @retail 0x218e50
real function_218e50(long tag_index, short set_index, short permutation_index, short sample_index)
{
	s_sound_promotion_tag *sound = (s_sound_promotion_tag *)g_4e3b44[tag_index & 0xffff].bytes;
	s_sound_globals_promotion_view *globals = (s_sound_globals_promotion_view *)g_51ebd4;
	s_sound_permutation_set *set = &globals->sets[sound->permutation_base + set_index];
	long valid_index;

	if (permutation_index < 0)
	{
		valid_index = 0;
	}
	else
	{
		valid_index = set->permutation_count - 1;
		if (permutation_index <= valid_index)
		{
			valid_index = permutation_index;
		}
	}

	if (valid_index == permutation_index)
	{
		s_sound_promotion_data *data = globals->promotions[sound->promotion_index].data;
		s_sound_promotion_entry *entry = &data->entries[globals->permutations[set->first_permutation + permutation_index].entry_index];
		long count = entry->sample_count;

		if (count > 0)
		{
			short index = sample_index < 0 ? 0 : (sample_index > count - 1 ? (short)(count - 1) : sample_index);
			return data->samples[index + entry->sample_offset] * (1.0f / 255.0f);
		}
	}
	return 0.0f;
}
