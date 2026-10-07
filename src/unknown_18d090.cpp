// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_18D090.CPP: sound tag lookups: the sound class of a sound tag,
   the platform playbacks found by label in the scenario's and the globals'
   tags, and a sound definition from a handle */

#include "unknown_11c920.h"
#include "globals.h"

#define SOUND_TAG 0x736e6421

struct s_tag_instance_view
{
	dword group_tag;
	byte unknown04[4];
	byte *data;
	byte unknown0c[4];
};

struct s_sound_class_definition
{
	byte unknown00[0x34];
};

struct s_platform_playback
{
	long label;
	byte unknown04[0x34];
};

struct s_platform_playback_block
{
	long count;
	s_platform_playback *playbacks;
};

struct s_sound_definition
{
	byte unknown00[0xd];
	char class_index;
};

struct s_sound_effect_definition
{
	byte unknown00[2];
	char type;
};

struct s_sound_permutation_entry
{
	byte unknown00[0x20];
	long effect_tag_index;
	byte unknown24[0x58 - 0x24];
};

struct s_sound_effects
{
	byte unknown00[0x1c];
	long count;
	s_sound_permutation_entry *entries;
};

struct s_scenario_sounds
{
	byte unknown00[0x324];
	long playback_tag_index;
};

struct s_tag_header_view
{
	byte unknown00[0xc];
	long playback_tag_index;
};

struct s_tag_header_globals_view
{
	byte unknown00[0xc0];
	void *header;
	s_tag_header_view *header_alt;
};

struct s_object_ref;
byte *function_2194a0(s_object_ref *ref);

static inline s_tag_instance_view *tag_instances(void)
{
	return (s_tag_instance_view *)g_4e3b44;
}

static inline byte *tag_get_data(long tag_index)
{
	return tag_instances()[tag_index & 0xffff].data;
}

/* the sound class of a sound tag: retail inlines this copy here and calls
   sound_get_class (0x18d170, src/sound_get_class.cpp) everywhere else */
static __forceinline s_sound_class_definition *sound_class_lookup(long tag_index)
{
	s_sound_class_definition *result = NULL;

	if (tag_index != NONE && tag_instances()[(short)tag_index].group_tag == SOUND_TAG)
	{
		s_sound_definition *definition = (s_sound_definition *)tag_get_data(tag_index);

		if (definition->class_index == NONE)
		{
			result = NULL;
		}
		else
		{
			result = (s_sound_class_definition *)g_51ebd4->entries34 + definition->class_index;
		}
	}
	return result;
}

// @retail 0x18d360
bool function_18d360(long tag_index)
{
	bool result = false;
	s_sound_effects *effects = (s_sound_effects *)tag_get_data(tag_index);

	for (short i = 0; i < effects->count; i++)
	{
		s_sound_permutation_entry const *local_0 = &effects->entries[i];
		__assume(local_0 != NULL);
		long effect_tag_index = local_0->effect_tag_index;
		if (effect_tag_index != NONE)
		{
			short type = ((s_sound_effect_definition *)tag_get_data(effect_tag_index))->type;
			if (type == 0x20 || type == 0x26)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

// @retail 0x18d630
long sound_find_platform_playback(long tag_index, long label)
{
	long result = NONE;

	if (tag_index != NONE)
	{
		s_platform_playback_block *block = (s_platform_playback_block *)tag_get_data(tag_index);
		for (long i = 0; i < block->count; i++)
		{
			if (block->playbacks[i].label == label)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

// @retail 0x18d5b0
long function_18d5b0(long label)
{
	long result = NONE;

	if (label != 0 && label != NONE)
	{
		if (label == 0x300012a)
		{
			return 0xc0000000;
		}

		long index = sound_find_platform_playback(((s_scenario_sounds *)g_4e0350)->playback_tag_index, label);
		if (index != NONE)
		{
			result = (index & 0x3fffffff) | 0x80000000;
		}
		if (result == NONE)
		{
			s_tag_header_globals_view *globals = (s_tag_header_globals_view *)g_4e034c;
			if (globals->header && globals->header_alt)
			{
				index = sound_find_platform_playback(globals->header_alt->playback_tag_index, label);
				if (index != NONE)
				{
					return (index & 0x3fffffff) | 0x40000000;
				}
			}
		}
	}
	return result;
}

// @retail 0x18d090
void *function_18d090(long tag_index, long handle)
{
	void *result = NULL;
	long playback_tag_index;

	if (handle == NONE)
	{
		result = sound_class_lookup(tag_index);
	}
	else
	{
		switch ((dword)handle >> 30)
		{
		case 0:
			if (tag_instances()[(short)tag_index].group_tag == SOUND_TAG)
			{
				result = function_2194a0((s_object_ref *)tag_get_data(tag_index));
			}
			break;
		case 1:
			{
				s_tag_header_globals_view *globals = (s_tag_header_globals_view *)g_4e034c;
				if (globals->header && globals->header_alt)
				{
					playback_tag_index = globals->header_alt->playback_tag_index;
					if (playback_tag_index != NONE)
					{
						s_platform_playback_block *block = (s_platform_playback_block *)tag_get_data(playback_tag_index);
						result = (byte *)&block->playbacks[handle & 0x3fffffff] + 4;
					}
				}
			}
			break;
		case 2:
			playback_tag_index = ((s_scenario_sounds *)g_4e0350)->playback_tag_index;
			if (playback_tag_index != NONE)
			{
				s_platform_playback_block *block = (s_platform_playback_block *)tag_get_data(playback_tag_index);
				result = (byte *)&block->playbacks[handle & 0x3fffffff] + 4;
			}
			break;
		}
	}
	return result;
}

