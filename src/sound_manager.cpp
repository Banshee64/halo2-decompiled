// @flags /O2 /arch:SSE /Gr
/* SOUND_MANAGER.CPP: the playing sounds and their definitions: per-sound
   overrides of the definition's class (distances, cone angles and gain) and
   the random gain and pitch drawn from the class. */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "data_array.h"
#include "unknown_218850.h"
#include "unknown_2ae170.h"
#include <string.h>
#include <float.h>
#include <math.h>

#define k_pi 3.14159274f

/* a sound being played */
struct s_sound
{
	word flag0 : 1;
	word flag1 : 1;
	word override_minimum_distance : 1;
	word override_maximum_distance : 1;
	word override_inner_cone_angle : 1;
	word override_outer_cone_angle : 1;
	word override_outer_cone_gain : 1;
	word unknown00 : 9;
	byte unknown02[6];
	word flag8_0 : 1;
	word flag8_1 : 1;
	word flag8_2 : 1;
	word flag8_3 : 1;
	word unknown08 : 12;
	word unknown0a_0 : 11;
	word flag0a_11 : 1;
	word unknown0a : 4;
	byte unknown0c[0x28];
	real minimum_distance;
	real maximum_distance;
	word inner_cone_angle;
	word outer_cone_angle;
	long outer_cone_gain;
};

/* a sound tag's definition */
struct s_sound_definition
{
	word flags;
	char promotion_index;
	byte unknown03[3];
	short class_index;
	short pitch_range_base;
};

/* a sound class, in the sound globals (0x38 bytes) */
struct s_sound_class
{
	real minimum_distance;
	real maximum_distance;
	byte unknown08[8];
	real gain_base;
	real gain_variance;
	short pitch_lower;
	short pitch_upper;
	real inner_cone_angle;
	real outer_cone_angle;
	long outer_cone_gain;
	byte unknown28[0x10];
};

struct s_sound_globals_classes_view
{
	byte unknown00[4];
	s_sound_class *classes;
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

static inline s_sound_definition *sound_definition_get(long definition_index)
{
	return (s_sound_definition *)g_4e3b44[definition_index & 0xffff].bytes;
}

static inline s_sound_class *sound_class_get(short class_index)
{
	return &((s_sound_globals_classes_view *)g_51ebd4)->classes[class_index];
}

static inline real real_decompress_angle(long value)
{
	if (value == 0)
		return 0.f;
	if (value >= 0xffff)
		return k_pi;
	return ((0xffff - value) * 0.f + value * k_pi) * (1.f / 65535.f);
}

// @retail 0x124f90
long function_124f90(s_sound const *sound)
{
	if (TEST_FIELD_BIT(sound->flag8_1) || TEST_FIELD_BIT(sound->flag8_3) || TEST_FIELD_BIT(sound->flag0a_11))
		return 1;
	return 0;
}

// @retail 0x124fc0
real sound_get_minimum_distance(s_sound const *sound, long definition_index)
{
	s_sound_definition *definition;

	if (TEST_FIELD_BIT(sound->override_minimum_distance))
		return sound->minimum_distance;
	definition = sound_definition_get(definition_index);
	if (definition->flags & 0x400)
		return ((s_sound_promotion_view *)function_221810(definition->promotion_index))->minimum_distance;
	return sound_class_get(definition->class_index)->minimum_distance;
}

// @retail 0x125010
real sound_get_maximum_distance(s_sound const *sound, long definition_index)
{
	s_sound_definition *definition;

	if (TEST_FIELD_BIT(sound->override_maximum_distance))
		return sound->maximum_distance;
	definition = sound_definition_get(definition_index);
	if (definition->flags & 0x800)
		return ((s_sound_promotion_view *)function_221810(definition->promotion_index))->maximum_distance;
	return sound_class_get(definition->class_index)->maximum_distance;
}

// @retail 0x125060
long function_125060(s_sound const *sound, long definition_index)
{
	if (TEST_FIELD_BIT(sound->override_inner_cone_angle) ||
		TEST_FIELD_BIT(sound->override_outer_cone_angle) ||
		!(sound_definition_get(definition_index)->flags & 0x1000))
	{
		return 1;
	}
	return 0;
}

// @retail 0x1250a0
real sound_get_inner_cone_angle(s_sound const *sound, long definition_index)
{
	if (TEST_FIELD_BIT(sound->override_inner_cone_angle))
		return real_decompress_angle(sound->inner_cone_angle);
	return sound_class_get(sound_definition_get(definition_index)->class_index)->inner_cone_angle;
}

// @retail 0x125120
real sound_get_outer_cone_angle(s_sound const *sound, long definition_index)
{
	if (TEST_FIELD_BIT(sound->override_outer_cone_angle))
		return real_decompress_angle(sound->outer_cone_angle);
	return sound_class_get(sound_definition_get(definition_index)->class_index)->outer_cone_angle;
}

// @retail 0x1251a0
long sound_get_outer_cone_gain(s_sound const *sound, long definition_index)
{
	if (TEST_FIELD_BIT(sound->override_outer_cone_gain))
		return sound->outer_cone_gain;
	return sound_class_get(sound_definition_get(definition_index)->class_index)->outer_cone_gain;
}

// @retail 0x125260
real sound_definition_random_gain(s_sound_definition const *definition)
{
	s_sound_class *sound_class = sound_class_get(definition->class_index);

	return _real_random_range(&g_4e7408->seed, __FILE__, __LINE__, 0.f, sound_class->gain_variance) + sound_class->gain_base;
}

// @retail 0x1252f0
real sound_definition_random_pitch(s_sound_definition const *definition, dword *seed)
{
	s_sound_class *sound_class = sound_class_get(definition->class_index);

	return _real_random_range(seed, __FILE__, __LINE__, (real)sound_class->pitch_lower, (real)sound_class->pitch_upper);
}

/* ---- the sound system's globals ---- */

struct s_4e6380;
extern s_4e6380 *g_4e6380;
extern s_data_array *g_4e637c;
struct s_bink_sound_settings;
extern s_bink_sound_settings *g_51ebe4;

/* the sound system's state, as these functions read it */
struct s_sound_system_view
{
	byte unknown00[0x70];
	dword channel_bits[2];
	bool initialized;
	bool hardware_available;
	bool enabled;
	byte unknown7b[0xd];
	struct
	{
		byte unknown00[0x30];
		real_point3d position;
		byte unknown3c[0xc];
	} listeners[4];
	byte unknown1a8[0x50];
	long ambience_index;
	real ambience_fade;
	long previous_ambience_index;
	real previous_ambience_fade;
};

struct s_sound_channel_flags
{
	byte unknown00[3];
	byte flags;
};

/* 0x34 bytes */
struct s_sound_channel
{
	byte unknown00[0xc];
	s_sound_channel_flags state;
	byte unknown10[0x24];
};

struct s_sound_permutation;

/* 0x24 bytes */
struct s_sound_voice
{
	long sound_index;
	byte unknown04[5];
	bool stream_reset;
	byte unknown0a[2];
	short channel_index;
	byte unknown0e[2];
	real unknown10;
	byte unknown14[4];
	short chunk_index;
	short next_chunk_index;
	s_sound_permutation const *permutation;
	s_sound_permutation const *next_permutation;
};

struct s_sound_reference_holder
{
	byte unknown00[8];
	long reference_index;
};

/* a playing sound's reference count, in the 0x502104 array */
struct s_sound_reference
{
	byte unknown00[4];
	byte reference_count;
	byte unknown05[0xb];
};

struct s_sound_location_source
{
	byte unknown00[2];
	char type;
	char spatialization : 4;
	char unknown03 : 4;
	byte unknown04[8];
	real_point3d position;
	byte unknown18[0x18];
	real height;
};

struct s_sound_definition_flags
{
	byte unknown00[4];
	byte type;
	byte format;
	byte unknown06[2];
	short pitch_range_index;
	bool has_pitch_ranges;
};

struct s_sound_globals_entries_view
{
	byte unknown00[0x24];
	struct
	{
		byte unknown00[0xa];
		short count;
	} *pitch_ranges;
};

long g_4e6374;
s_sound_voice *g_4e6378;
void *g_502110;
s_data_array *g_502114;
void *g_51ebd8;
void *g_51ebdc;
void *g_51ebe0;
void *g_47f0e4;

void function_2186b0(void);
void function_21eae0(void);

#define SOUND_SYSTEM ((s_sound_system_view *)g_4e6380)

// @retail 0x125600
void sound_dispose(void)
{
	if (--g_4e6374 == 0)
	{
		function_2186b0();
		function_21eae0();
		if (g_51ebe0)
			g_51ebe0 = NULL;
		if (g_502114)
		{
			g_502114->valid = false;
			g_502114 = NULL;
		}
		if (g_502110)
			g_502110 = NULL;
		if (g_51ebd8)
			g_51ebd8 = NULL;
		if (g_51ebdc)
			g_51ebdc = NULL;
		if (g_4e6380)
			g_4e6380 = NULL;
		if (g_4e637c)
		{
			g_4e637c->valid = false;
			g_4e637c = NULL;
		}
		if (g_4e6378)
			g_4e6378 = NULL;
	}
}

// @retail 0x125a60
long sound_system_available(void)
{
	if (SOUND_SYSTEM->initialized && SOUND_SYSTEM->hardware_available && SOUND_SYSTEM->enabled)
		return 1;
	return 0;
}

// @retail 0x125ef0
void sound_reference_release(s_sound_reference_holder *holder)
{
	s_sound_reference *reference = (s_sound_reference *)g_502104->data + (holder->reference_index & 0xffff);

	reference->reference_count--;
}

// @retail 0x126bd0
long function_126bd0(long definition_index)
{
	s_sound_definition_flags *definition = (s_sound_definition_flags *)g_4e3b44[definition_index & 0xffff].bytes;

	if ((definition->format == 1 && definition->type != 2) || (definition->type == 2 && g_47f0e4))
	{
		if (definition->has_pitch_ranges &&
			((s_sound_globals_entries_view *)g_51ebd4)->pitch_ranges[definition->pitch_range_index].count != 0)
		{
			return 1;
		}
	}
	return 0;
}

// @retail 0x1272c0
void sound_set_ambience(long ambience_index)
{
	s_sound_system_view *sound_system = SOUND_SYSTEM;

	if (ambience_index != sound_system->ambience_index)
	{
		if (ambience_index != NONE && ambience_index == sound_system->previous_ambience_index)
		{
			sound_system->ambience_index = sound_system->previous_ambience_index;
			sound_system->ambience_fade = sound_system->previous_ambience_fade;
			sound_system->previous_ambience_fade = 0.f;
		}
		else
		{
			sound_system->previous_ambience_index = sound_system->ambience_index;
			sound_system->ambience_index = ambience_index;
			sound_system->ambience_fade = 0.f;
			sound_system->previous_ambience_fade = 0.f;
		}
	}
}

// @retail 0x12a420
void sound_voice_mark_channel(short voice_index)
{
	s_sound_channel_flags *state = &((s_sound_channel *)g_51ebe4)[g_4e6378[voice_index].channel_index].state;

	state->flags |= 8;
}

// @retail 0x12abe0
void sound_source_get_position(s_sound_location_source const *source, long listener_index, real_point3d *position)
{
	switch (source->type)
	{
	case 0:
		*position = source->position;
		break;
	case 1:
		position->x = source->position.x;
		position->y = source->position.y;
		position->z = SOUND_SYSTEM->listeners[listener_index].position.z;
		break;
	}
}

/* how far a sound is from a listener: none for a sound without a position,
   the square of the distance for one placed in the world (on the ground
   plane, unless the listener is out of its height range), the distance from
   the listener for one attached to it */
real magnitude3d(real_vector3d const *v);

// @retail 0x127e20
real sound_source_get_listener_distance(s_sound_location_source const *source, long listener_index)
{
	switch (source->spatialization)
	{
	case 0:
		return 0.0f;
	case 1:
	{
		real_point3d const *listener = &SOUND_SYSTEM->listeners[listener_index].position;
		real dz = SOUND_SYSTEM->listeners[listener_index].position.z - source->position.z;

		if (source->type)
		{
			real clamped = 0.0f;

			if (!(0.0f > dz))
			{
				clamped = dz > source->height ? source->height : dz;
			}
			if (clamped == dz)
			{
				real dy = source->position.y - listener->y;
				real dx = source->position.x - listener->x;

				return dx * dx + dy * dy;
			}
			return FLT_MAX;
		}
		else
		{
			real dy = listener->y - source->position.y;
			real dx = listener->x - source->position.x;

			return dz * dz + dy * dy + dx * dx;
		}
	}
	default:
		return magnitude3d((real_vector3d const *)&source->position);
	}
}

// @retail 0x12af90
void sound_channel_clear(short channel_index)
{
	if (channel_index != NONE)
		SOUND_SYSTEM->channel_bits[channel_index >> 5] &= ~(1 << (channel_index & 31));
}

// @retail 0x12afc0
void bit_vector_fill(dword *vector, long count, byte value)
{
	memset(vector, value, ((count + 31) >> 5) * sizeof(dword));
}

/* a sound being played, in the 0x4e637c array (0xbc bytes) */
struct s_sound_playback
{
	byte unknown00[4];
	word flag0 : 1;
	word holds_reference : 1;
	word unknown04 : 14;
	byte unknown06[6];
	long definition_index;
	byte unknown10[0x8c];
	char pitch_range_index;
	char permutation_index;
	short chunk_index;
	byte unknown_a0[0x1c];
};

struct s_sound_promotion_flags
{
	byte unknown00[0xa];
	word disabled : 1;
	word unknown0a : 15;
};

struct s_sound_globals_tables_view
{
	byte unknown00[0x24];
	struct
	{
		byte unknown00[8];
		short first_permutation;
		short count;
	} *pitch_ranges;
	byte unknown28[4];
	struct
	{
		byte unknown00[0xc];
		short first_chunk;
		byte unknown0e[2];
	} *permutations;
	byte unknown30[0x14];
	struct
	{
		byte unknown00[8];
		long reference_index;
	} *chunks;
};

struct s_sound_system_channels_view
{
	byte unknown00[0x58];
	dword available_bits[6];
	dword used_bits[2];
	byte unknown78[0x192];
	short channel_count;
};

// @retail 0x126960
void sound_playback_release_reference(s_sound_playback *sound)
{
	if (TEST_FIELD_BIT(sound->holds_reference))
	{
		s_sound_definition *definition = sound_definition_get(sound->definition_index);
		long pitch_range = definition->pitch_range_base + sound->pitch_range_index;
		s_sound_globals_tables_view *tables = (s_sound_globals_tables_view *)g_51ebd4;
		long permutation = tables->pitch_ranges[pitch_range].first_permutation + sound->permutation_index;
		long chunk = tables->permutations[permutation].first_chunk + sound->chunk_index;
		s_sound_reference *reference = (s_sound_reference *)g_502104->data + (tables->chunks[chunk].reference_index & 0xffff);

		reference->reference_count--;
		sound->holds_reference = false;
	}
}

// @retail 0x129fe0
bool sound_voice_promotion_enabled(short voice_index)
{
	long sound_index = g_4e6378[voice_index].sound_index;
	bool result = true;

	if (sound_index != NONE)
	{
		s_sound_playback *sound = (s_sound_playback *)g_4e637c->data + (sound_index & 0xffff);
		char promotion_index = sound_definition_get(sound->definition_index)->promotion_index;

		if ((promotion_index < 0 ? 0 : (promotion_index > 0x35 ? 0x35 : promotion_index)) == promotion_index)
			result = !TEST_FIELD_BIT(((s_sound_promotion_flags *)function_221810(promotion_index))->disabled);
	}
	return result;
}

// @retail 0x12af10
short sound_channel_allocate(void)
{
	s_sound_system_channels_view *sound_system = (s_sound_system_channels_view *)g_4e6380;
	short channel_count = sound_system->channel_count;
	short result = NONE;

	for (short i = 0; i < channel_count; i++)
	{
		if ((sound_system->available_bits[i >> 5] & (1 << (i & 31))) && !(sound_system->used_bits[i >> 5] & (1 << (i & 31))))
		{
			sound_system->used_bits[i >> 5] |= 1 << (i & 31);
			return i;
		}
	}
	return result;
}

/* the sound driver's streams, one per channel (g_51ebe4) */
struct s_sound_driver_streams_view
{
	byte unknown00[0xc];
	s_sound_stream streams[1];
};

#define SOUND_DRIVER_STREAMS ((s_sound_driver_streams_view *)g_51ebe4)

/* a permutation of a pitch range (16 bytes) */
struct s_sound_permutation
{
	byte unknown00[0xc];
	short first_chunk;
	byte unknown0e[2];
};

/* the tables of the sound globals, as the chunk lookups read them */
struct s_sound_globals_chunks_view
{
	byte unknown00[0x24];
	struct
	{
		byte unknown00[8];
		short first_permutation;
		short count;
	} *pitch_ranges;
	byte unknown28[4];
	s_sound_permutation *permutations;
	byte unknown30[0x14];
	s_sound_chunk *chunks;
};

#define SOUND_GLOBALS_CHUNKS ((s_sound_globals_chunks_view *)g_51ebd4)

/* requests the first chunk of a sound's first permutation */
// @retail 0x125f10
void sound_definition_request_first_chunk(long definition_index)
{
	if (definition_index != NONE)
	{
		s_sound_definition_flags *definition = (s_sound_definition_flags *)g_4e3b44[definition_index & 0xffff].bytes;

		if (definition->has_pitch_ranges)
		{
			s_sound_globals_chunks_view *tables = SOUND_GLOBALS_CHUNKS;

			if (tables->pitch_ranges[definition->pitch_range_index].count != 0)
			{
				s_sound_permutation *permutation = &tables->permutations[tables->pitch_ranges[definition->pitch_range_index].first_permutation];

				function_218850(definition_index, &tables->chunks[permutation->first_chunk], 2);
			}
		}
	}
}

// @retail 0x1268e0
void sound_playback_acquire_reference(s_sound_playback *sound)
{
	if (!TEST_FIELD_BIT(sound->holds_reference))
	{
		s_sound_globals_chunks_view *tables;
		s_sound_definition *definition;
		long permutation;
		long chunk;

		long pitch_range;

		sound->holds_reference = true;
		definition = sound_definition_get(sound->definition_index);
		pitch_range = definition->pitch_range_base + sound->pitch_range_index;
		tables = SOUND_GLOBALS_CHUNKS;
		permutation = tables->pitch_ranges[pitch_range].first_permutation + sound->permutation_index;
		chunk = tables->permutations[permutation].first_chunk + sound->chunk_index;
		function_218850(NONE, &tables->chunks[chunk], 4);
	}
}

// @retail 0x1286b0
void sound_playback_set_chunk(long sound_index, long definition_index, char pitch_range_index, char permutation_index, short chunk_index)
{
	s_sound_playback *sound = (s_sound_playback *)g_4e637c->data + (sound_index & 0xffff);

	sound_playback_release_reference(sound);
	sound->definition_index = definition_index;
	sound->pitch_range_index = pitch_range_index;
	sound->permutation_index = permutation_index;
	sound->chunk_index = chunk_index;
}

/* queues a chunk on a voice's stream: the first one, or the one after it */
// @retail 0x129f20
void sound_voice_queue_chunk(short voice_index, s_sound_permutation const *permutation, short chunk_index)
{
	s_sound_voice *voice = &g_4e6378[voice_index];
	s_sound_chunk *chunks = SOUND_GLOBALS_CHUNKS->chunks;
	s_sound_chunk *chunk = &chunks[permutation->first_chunk + chunk_index];

	if (voice->next_permutation)
	{
		s_sound_chunk *next_chunk = &chunks[voice->next_permutation->first_chunk + voice->next_chunk_index];

		SOUND_CACHE_ENTRY(next_chunk->cache_index)->lock_count--;
		voice->next_chunk_index = NONE;
	}
	sound_stream_add_chunk(&SOUND_DRIVER_STREAMS->streams[voice->channel_index], chunk);
	function_218850(NONE, chunk, 4);
	if (voice->permutation)
	{
		voice->next_chunk_index = chunk_index;
		voice->next_permutation = permutation;
	}
	else
	{
		voice->chunk_index = chunk_index;
		voice->permutation = permutation;
		voice->unknown10 = 0.0f;
	}
}

// @retail 0x12a060
void sound_voice_reset_stream(short voice_index)
{
	s_sound_voice *voice = &g_4e6378[voice_index];

	if (voice->channel_index != NONE && !voice->stream_reset && sound_voice_promotion_enabled(voice_index))
	{
		sound_stream_reset(&SOUND_DRIVER_STREAMS->streams[voice->channel_index]);
		voice->stream_reset = true;
	}
}
