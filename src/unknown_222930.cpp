// @flags /O2 /Gr
/* UNKNOWN_222930.CPP: the impulse parameters of a sound effect */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_222930.h"

#define PIN(n, floor, ceiling) ((n) < (floor) ? (floor) : ((n) > (ceiling) ? (ceiling) : (n)))

void *function_18d090(long tag_index, long handle);
long function_1914f0(long string_handle);

struct s_tag_block_view
{
	long count;
	void *address;
	long unused;
};

/* a function of the impulse (an element of the impulse's data blocks) */
struct s_sound_impulse_function
{
	long unknown00;
	long size;
	long unknown08;
	long address;
};

struct s_sound_impulse_entry
{
	long name;
	long size;
	long unknown08;
	long address;
};

struct s_sound_impulse_override
{
	byte unknown00[8];
	long count;
	s_sound_impulse_function *functions;
};

struct s_sound_impulse_definition
{
	byte unknown00[0xc];
	long count;
	s_sound_impulse_entry *entries;
};

struct s_sound_effect_template
{
	byte unknown00[4];
	long impulse_tag_index;
	byte unknown08[0x20 - 8];
	long override_count;
	s_sound_impulse_override *overrides;
};

struct s_sound_effect_view
{
	byte unknown00[0x2c];
	long count;
	s_sound_effect_template *effect_template;
};

// @retail 0x222930
void function_222930(long tag_index, long handle, s_looping_impulse_parameters *parameters)
{
	parameters->mixbin = NONE;
	parameters->data = NULL;
	parameters->identifier = NONE;

	s_sound_effect_view *effect = (s_sound_effect_view *)function_18d090(tag_index, handle);
	if (effect && PIN(0, 0, effect->count - 1) == 0)
	{
		s_sound_effect_template *effect_template = effect->effect_template;

		if (effect_template && effect_template->impulse_tag_index != NONE)
		{
			s_sound_impulse_definition *definition = (s_sound_impulse_definition *)g_4e3b44[effect_template->impulse_tag_index & 0xffff].bytes;

			if (definition->count > 0)
			{
				s_sound_impulse_entry *entry = definition->entries;

				parameters->mixbin = (short)function_1914f0(entry->name);
				if (effect_template->override_count > 0 && effect_template->overrides->count > 0)
				{
					s_sound_impulse_function *function = effect_template->overrides->functions;

					parameters->data_size = function->address;
					parameters->data = (s_tag_data const *)&function->size;
				}
				if (!parameters->data)
				{
					parameters->data_size = entry->address;
					parameters->data = (s_tag_data const *)&entry->size;
				}
				parameters->identifier = (long)parameters->data;
			}
		}
	}
}
