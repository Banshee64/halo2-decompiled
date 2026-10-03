// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_21D110.CPP: the sound effects in g_51ebe0 ("sounds effects",
   0x10 elements of 0x14 bytes): a sound started through an effect plays from
   the effect's source table g_44a1c0, which wraps the sound's own source and
   keeps a sound record for it */

#include "cseries.h"
#include "data_array.h"
#include "sound_sources.h"
#include "sound_records.h"
#include <string.h>

/* the permutations a sound effect plays (0x2c..0x34 of a sound class or
   platform playback) */
struct s_sound_effect_definition
{
	byte unknown00[8];
	long count;
};

struct s_sound_effect_definition_block
{
	byte unknown00[0x2c];
	long count;
	s_sound_effect_definition *definitions;
};

struct s_sound_effect_class_flags
{
	byte unknown00[8];
	byte flags;
};

#define FLAG(bit) (1 << (bit))
#define TEST_BIT(flags, bit) (((flags) >> (bit)) & 1)
#define SET_BIT(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

enum
{
	_sound_effect_finished_bit = 0,
	_sound_effect_flag1_bit,
	_sound_effect_flag2_bit,
	_sound_effect_unmanaged_bit,
	_sound_effect_stopped_bit
};

/* a sound effect (0x14 bytes) */
struct s_sound_effect
{
	short salt;
	byte unknown02;
	char type;
	byte flags;
	byte unknown05;
	short priority;
	long record_index;
	union
	{
		long sound_index;
		real scale;
	};
	s_sound_effect_definition *definition;
};

/* a sound's state as sound_manager.cpp sees it (the first 0x44 bytes are a
   s_sound_location) */
struct s_sound
{
	word flag0 : 1;
	word flag1 : 1;
	word override_minimum_distance : 1;
	word override_maximum_distance : 1;
	word unknown00_4 : 4;
	word flag8 : 1;
	word flag9 : 1;
	word unknown00 : 6;
	byte unknown02[0x34 - 2];
	real minimum_distance;
	real maximum_distance;
	byte unknown3c[8];
};

/* a playing sound in g_4e637c (0xbc bytes) */
struct s_playing_sound
{
	byte unknown00[0xc];
	long definition_index;
	byte unknown10[4];
	s_sound_source_callbacks const *source;
	s_sound_location location;
	s_sound_effect_marker marker;
	byte unknown8c[0xbc - 0x8c];
};

/* the sound system's state, as these functions read it */
struct s_sound_system_effect_view
{
	byte unknown00[0x78];
	bool initialized;
	bool hardware_available;
	bool enabled;
};

struct s_4e6380;
extern s_4e6380 *g_4e6380;
extern s_data_array *g_4e637c;
extern void *g_51ebd8;
extern void *g_51ebe0;

void *function_18d090(long tag_index, long handle);
struct s_sound_class_definition;
s_sound_class_definition *sound_get_class(long tag_index);
real sound_get_minimum_distance(s_sound const *sound, long definition_index);
real sound_get_maximum_distance(s_sound const *sound, long definition_index);
s_data_array *function_11cc20(long maximum_count, const char *name, long size);

bool __stdcall function_126c30(s_sound_play_state *state, long tag_index, s_sound_effect_definition **definition, long flags);
long __stdcall function_126ec0(long tag_index, long *permutation_index);
long __stdcall function_126000(long tag_index, s_sound_effect_definition *definition, s_sound_play_state *state, long permutation_index);
void __stdcall function_127f10(long sound_index, word *flags);
void __stdcall function_21d630(long effect_index, long mode);
void function_18cbc0(long looping_sound_index, s_sound_location *location);

extern s_sound_source_callbacks const g_44a1c0;

static inline bool sound_system_available(void)
{
	s_sound_system_effect_view *sound_system = (s_sound_system_effect_view *)g_4e6380;
	return sound_system->initialized && sound_system->hardware_available && sound_system->enabled;
}

static inline s_data_array *sound_effects(void)
{
	return (s_data_array *)g_51ebe0;
}

static inline s_sound_effect *sound_effect_get(long effect_index)
{
	return &((s_sound_effect *)sound_effects()->data)[effect_index & 0xffff];
}

static inline s_playing_sound *playing_sound_get(long sound_index)
{
	return &((s_playing_sound *)g_4e637c->data)[sound_index & 0xffff];
}

static inline long sound_effect_get_sound_index(long effect_index, s_sound_effect *effect)
{
	return effect->type == 0 ? (effect_index | 0x4000) : effect->sound_index;
}

static inline long sound_effect_new(s_sound_effect_definition *definition)
{
	long effect_index = NONE;

	if (definition->count)
	{
		effect_index = datum_new(sound_effects());
		if (effect_index == NONE)
		{
			return effect_index;
		}

		s_sound_effect *effect = sound_effect_get(effect_index);
		effect->definition = definition;
		effect->record_index = NONE;
	}
	return effect_index;
}

/* starts a sound without an effect */
static inline long sound_start(s_sound_play_state *state, long tag_index)
{
	long result = NONE;
	s_sound_effect_definition *definition;
	long permutation_index;

	if (sound_system_available() && function_126c30(state, tag_index, &definition, 0))
	{
		if (function_126ec0(tag_index, &permutation_index))
		{
			result = NONE;
		}
		else
		{
			result = function_126000(tag_index, definition, state, permutation_index);
		}
	}
	return result;
}

// @retail 0x21dd30
bool sound_effect_get_definition(long tag_index, long platform_playback, s_sound_effect_definition **definition)
{
	bool result = false;
	s_sound_effect_definition_block *playback = (s_sound_effect_definition_block *)function_18d090(tag_index, platform_playback);
	s_sound_effect_definition_block *sound_class = (s_sound_effect_definition_block *)sound_get_class(tag_index);
	s_sound_effect_definition_block *block = NULL;

	if (playback && playback->count)
	{
		block = playback;
	}
	else if (platform_playback == NONE && sound_class && sound_class->count)
	{
		block = sound_class;
	}

	if (block && playback->count > 0)
	{
		s_sound_effect_definition *definitions = playback->definitions;
		if (definitions)
		{
			*definition = definitions;
			return definitions->count > 0;
		}
	}
	return result;
}

// @retail 0x21dc60
void sound_effect_attach(long effect_index, s_sound_play_state *state)
{
	s_sound_effect *effect = sound_effect_get(effect_index);

	state->marker_size = sizeof(s_sound_effect_marker);
	if (state->flags & 0x20)
	{
		state->marker.link.source = state->source;
	}
	else
	{
		effect->flags |= FLAG(_sound_effect_unmanaged_bit);
		state->marker.link.source = NULL;
	}
	state->marker.link.effect_index = effect_index;
	state->source = &g_44a1c0;
	state->flags |= 0x20;

	if (state->flags & 0x10)
	{
		effect->record_index = sound_record_add_reference(state->record_key);
	}
	else
	{
		long key = effect_index | 0x4000;

		effect->record_index = sound_record_new(key);
		if (effect->record_index != NONE)
		{
			state->record_key = key;
			state->flags |= 0x10;
		}
	}
}

// @retail 0x21dcf0
void sound_effect_delete(long effect_index)
{
	s_data_array *effects = sound_effects();
	s_sound_effect *effect = &((s_sound_effect *)effects->data)[effect_index & 0xffff];

	if (effect->record_index != NONE)
	{
		sound_record_release(effect->record_index);
		effect->record_index = NONE;
	}
	datum_delete(effects, effect_index);
}

#if 0
// retail 0x21d110
long function_21d110(s_sound_play_state *state, long tag_index)
{
	long platform_playback = (state->flags & 0x100) ? state->platform_playback : NONE;
	s_sound_effect_definition *definition = NULL;
	long effect_index = NONE;

	if (sound_effect_get_definition(tag_index, platform_playback, &definition))
	{
		effect_index = sound_effect_new(definition);
	}

	if (effect_index != NONE)
	{
		s_sound_effect_class_flags *sound_class = (s_sound_effect_class_flags *)function_18d090(tag_index, platform_playback);
		if (sound_class && (sound_class->flags & 1))
		{
			state->location.flag8 = true;
		}
		sound_effect_attach(effect_index, state);

		long sound_index = sound_start(state, tag_index);
		if (sound_index != NONE)
		{
			s_sound_effect *effect = sound_effect_get(effect_index);
			effect->type = 1;
			effect->sound_index = sound_index;
			effect->priority = (short)state->priority;
			function_21d630(effect_index, 0);
		}
		else
		{
			sound_effect_delete(effect_index);
		}
		return sound_index;
	}
	return sound_start(state, tag_index);
}

#endif

// @retail 0x21d2c0
long function_21d2c0(long platform_playback, real scale, short priority)
{
	s_sound_effect_definition *definition = NULL;
	long effect_index = NONE;

	if (sound_effect_get_definition(NONE, platform_playback, &definition))
	{
		effect_index = sound_effect_new(definition);
		if (effect_index != NONE)
		{
			s_sound_effect *effect = sound_effect_get(effect_index);
			effect->type = 0;
			effect->scale = scale;
			effect->priority = priority;
			function_21d630(effect_index, 0);
		}
	}
	return effect_index;
}

// @retail 0x21d360
void sound_effect_stop(long effect_index)
{
	s_sound_effect *effect = sound_effect_get(effect_index);

	effect->flags |= FLAG(_sound_effect_unmanaged_bit) | FLAG(_sound_effect_stopped_bit);
	function_21d630(effect_index, 2);
	effect->flags |= FLAG(_sound_effect_finished_bit);
}

// @retail 0x21d5a0
bool function_21d5a0(long effect_index)
{
	s_sound_effect *effect = sound_effect_get(effect_index);
	bool result = false;

	switch (effect->type)
	{
	case 0:
		result = !TEST_BIT(effect->flags, _sound_effect_stopped_bit);
		break;
	case 1:
		if (effect->flags & (FLAG(_sound_effect_unmanaged_bit) | FLAG(_sound_effect_stopped_bit)))
		{
			result = true;
		}
		else
		{
			word flags = 0;
			function_127f10(effect->sound_index, &flags);
			if (flags & 0x20)
			{
				effect->flags |= FLAG(_sound_effect_unmanaged_bit);
			}
		}
		break;
	case 2:
		break;
	}
	return result;
}

// @retail 0x21d390
void sound_effects_update(void)
{
	s_data_iterator iterator;
	s_sound_effect *effect;

	iterator.data = sound_effects();
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while ((effect = (s_sound_effect *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if (TEST_BIT(effect->flags, _sound_effect_finished_bit))
		{
			sound_effect_delete(iterator.datum_index);
		}
		else if (function_21d5a0(iterator.datum_index))
		{
			function_21d630(iterator.datum_index, 1);
		}
	}
}

// @retail 0x21d490
bool sound_effects_initialize(void)
{
	g_51ebe0 = function_11cc20(0x10, "sounds effects", sizeof(s_sound_effect));
	if (g_51ebe0)
	{
		s_data_array *effects = sound_effects();
		effects->valid = true;
		data_delete_all(effects);
	}
	return g_51ebe0 != NULL;
}

// @retail 0x21d4d0
void function_21d4d0(void)
{
	s_data_iterator iterator;

	iterator.data = sound_effects();
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while (data_iterator_next_inlined(&iterator))
	{
		sound_effect_delete(iterator.datum_index);
	}
}

// @retail 0x21db80
void sound_effect_update_location(long effect_index, s_sound_location *location)
{
	s_sound_effect *effect = sound_effect_get(effect_index);

	switch (effect->type)
	{
	case 0:
		location->audible = 0;
		location->requested_audible = 0;
		location->scale = effect->scale;
		location->unknown08 = 0;
		break;
	case 1:
		{
			s_playing_sound *sound = playing_sound_get(sound_effect_get_sound_index(effect_index, effect));
			s_sound_location *sound_location = &sound->location;

			if (location != sound_location)
			{
				*location = *sound_location;
				if (location->flags & 0x100)
				{
					s_sound *distances = (s_sound *)location;

					distances->minimum_distance = sound_get_minimum_distance((s_sound *)sound_location, sound->definition_index);
					location->flags |= 4;
					distances->maximum_distance = sound_get_maximum_distance((s_sound *)sound_location, sound->definition_index);
					location->flags |= 0x208;
				}
			}
		}
		break;
	case 2:
		function_18cbc0(sound_effect_get_sound_index(effect_index, effect), location);
		break;
	}
}

static inline s_sound_effect_marker const *sound_effect_marker(void const *marker)
{
	return (s_sound_effect_marker const *)marker;
}

// @retail 0x21d970
bool __stdcall sound_effect_only_update(long object_index, long tag_index, s_sound_marker const *marker, s_sound_location *location)
{
	long effect_index = sound_effect_marker(marker)->link.effect_index;
	s_sound_effect *effect = (s_sound_effect *)datum_get_inlined(sound_effects(), effect_index);

	if (effect && !TEST_BIT(effect->flags, _sound_effect_finished_bit))
	{
		if (!TEST_BIT(effect->flags, _sound_effect_stopped_bit))
		{
			sound_effect_update_location(effect_index, location);
		}
		return true;
	}
	return false;
}

// @retail 0x21d9f0
bool __stdcall sound_effect_source_update(long object_index, long tag_index, s_sound_marker const *marker, s_sound_location *location)
{
	long effect_index = sound_effect_marker(marker)->link.effect_index;
	bool result = !TEST_BIT(sound_effect_get(effect_index)->flags, _sound_effect_stopped_bit);

	if (result)
	{
		sound_effect_update_location(effect_index, location);
	}
	return result;
}

// @retail 0x21da30
void __stdcall sound_effect_source_proc1(long object_index, long tag_index, long a, long marker)
{
	s_sound_source_callbacks const *source = sound_effect_marker((void const *)marker)->link.source;

	if (source && source->proc1)
	{
		source->proc1(object_index, tag_index, a, marker);
	}
}

// @retail 0x21da50
void __stdcall sound_effect_source_proc2(long object_index, long marker, long tag_index, long set_index, long permutation, long scale)
{
	s_sound_source_callbacks const *source = sound_effect_marker((void const *)marker)->link.source;

	if (source && source->proc2)
	{
		source->proc2(object_index, marker, tag_index, set_index, permutation, scale);
	}
}

// @retail 0x21da70
bool __stdcall sound_effect_source_spatialize(long object_index, long tag_index, s_sound_source_view const *source, s_sound_spatialization_view *spatialization)
{
	s_sound_source_callbacks const *previous = sound_effect_marker(source)->link.source;
	bool result = false;

	if (previous && previous->spatialize)
	{
		result = previous->spatialize(object_index, tag_index, source, spatialization);
	}
	return result;
}

// @retail 0x21da90
void __stdcall sound_effect_source_stop(long object_index, long sound_index, long reason)
{
	if (reason != 10)
	{
		s_sound_effect_link *link = &playing_sound_get(sound_index)->marker.link;
		s_sound_effect *effect = sound_effect_get(link->effect_index);

		if (link->source && link->source->stop)
		{
			link->source->stop(object_index, sound_index, reason);
		}
		SET_BIT(effect->flags, _sound_effect_flag1_bit, reason != 2);
		sound_effect_stop(link->effect_index);
	}
}

// @retail 0x21db30
void __stdcall sound_effect_source_detach(long object_index, long sound_index)
{
	sound_effect_source_stop(object_index, sound_index, 1);

	s_playing_sound *sound = playing_sound_get(sound_index);
	s_sound_effect_link *link = &sound->marker.link;
	sound->source = link->source;
	memset(link, 0, sizeof(*link));
}

extern s_sound_source_callbacks const g_44a1c0 = { sound_effect_source_update, sound_effect_source_proc1, sound_effect_source_proc2, sound_effect_source_spatialize, sound_effect_source_stop, sound_effect_source_detach, NULL, NULL };
extern s_sound_source_callbacks const g_44a1e0 = { sound_effect_only_update, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
