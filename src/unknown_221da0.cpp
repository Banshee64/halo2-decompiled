// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_221DA0.CPP: sound transmission helpers: whether a listener hears
   a sound through the structure's clusters, the effect parameters of a
   sound's environment, and the conversions of its DSP parameters */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "local_cameras.h"
#include <math.h>

#define PIN(n, floor, ceiling) ((n) < (floor) ? (floor) : ((n) > (ceiling) ? (ceiling) : (n)))

/* a tag's data block, as the function evaluator reads it */
struct s_tag_data_view
{
	long size;
	byte *address;
};

real function_13b390(void const *function, real input, real range);

/* rounds as the x87 does (real_math's fld/fistp idiom) */
static __forceinline long real_to_long(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return result;
}

struct s_structure_bsp_view;
real function_249d60(long cluster_a, long cluster_b, s_structure_bsp_view *bsp);

/* a sound as the transmission test reads it */
struct s_sound_transmission_view
{
	byte flags;
	byte unknown01[2];
	byte type : 4;
	byte unknown03_4 : 4;
	byte unknown04[0x28 - 4];
	long bsp_index;
	short cluster_index;
};

/* whether a listener may hear a sound: always without a listener, never
   when the sound is in another structure or cluster too far away */
// @retail 0x221da0
bool function_221da0(long listener_index, s_sound_transmission_view const *sound, real scale)
{
	bool result = listener_index == NONE;

	if (!result && !(sound->flags & 1))
	{
		byte type = sound->type;

		if (type == 1)
		{
			s_local_camera *listener = local_camera_get(listener_index);
			short sound_cluster = sound->cluster_index;

			if (sound_cluster == NONE)
			{
				return type;
			}

			short listener_cluster = listener->index;
			if (listener_cluster == NONE)
			{
				return type;
			}

			if (*(long *)listener->unknown00 == sound->bsp_index || listener_cluster == sound_cluster ||
				!(function_249d60(sound_cluster, listener_cluster, (s_structure_bsp_view *)g_4e0348) * scale < 256.0f))
			{
				return false;
			}
			return type;
		}
	}
	return result;
}

long function_2221d0(real value);

real function_30bf0(real_vector3d *v);
real function_192b90(real_point3d const *direction, long speaker, bool linear);

/* the gain of a speaker for a direction */
// @retail 0x222250
real function_222250(real_vector3d const *direction, long speaker)
{
	real result = 1.0f;

	if (PIN(speaker, 0, 4) == speaker)
	{
		real_vector3d normal = *direction;

		function_30bf0(&normal);
		result = function_192b90((real_point3d const *)&normal, speaker, true);
		result = PIN(result, 0.0f, 1.0f);
	}
	return result;
}

/* ---- the effect data blocks of a sound effect ---- */

/* a DSP effect data block (src/sound_dsound_xbox.cpp reads them) */
struct s_effect_data_header
{
	dword effect_mask;
	byte flags;
	byte unknown05[3];
	word offset;
	word size;
};

struct s_effect_data_block
{
	s_effect_data_header header;
	long data[1];
};

/* a parameter of an effect component (16 bytes) */
struct s_effect_parameter
{
	long type;
	real default_value;
	real minimum;
	real maximum;
};

/* an effect component (0x18 bytes): the effect it drives and the data
   written to it, then its parameters */
struct s_effect_component
{
	long name;
	s_effect_data_header header;
	long parameter_count;
	s_effect_parameter *parameters;
};

/* the components of a sound effect definition */
struct s_effect_components
{
	long count;
	s_effect_component *components;
};

/* a function driving a parameter from the effect's inputs (16 bytes) */
struct s_effect_function
{
	short input;
	short range_input;
	s_tag_data_view function;
	real period;
};

/* how a template sets the parameters of one component (0x1c bytes) */
struct s_effect_component_overrides
{
	long function_count;
	s_effect_function *functions;
	long constant_count;
	real *constants;
	long source_count;
	char *sources;
	dword parameter_mask;
};

/* a sound effect template's settings of the components */
struct s_effect_overrides
{
	long count;
	s_effect_component_overrides *components;
	byte unknown08[8];
	dword component_mask;
};

/* the inputs of the effect functions: four values, then the direction of
   the sound */
struct s_effect_inputs
{
	real values[4];
	real_vector3d direction;
};

dword function_191660(long string_id, long index);

/* fills the data block of one component of a sound effect */
// @retail 0x2222c0
void function_2222c0(s_effect_data_block *block, s_effect_component const *component, s_effect_component_overrides const *overrides,
	s_effect_inputs const *inputs, long speaker)
{
	long source_index = 0;
	long i;

	block->header = component->header;
	block->header.effect_mask = function_191660(component->name, speaker);
	if (component->header.flags & 1)
	{
		long effect = NONE;

		switch (component->name)
		{
		case 0xd00012e:
			effect = 10;
			break;
		case 0x1300012d:
			effect = 4;
			break;
		}
		block->data[0] = effect;
	}

	for (i = 0; i < component->parameter_count; i++)
	{
		s_effect_parameter const *parameter = &component->parameters[i];
		real value = parameter->default_value;

		if (overrides && (overrides->parameter_mask & (1 << i)))
		{
			char source = overrides->sources[source_index];

			if (source & 1)
			{
				s_effect_function const *function = &overrides->functions[source >> 1];
				short input = function->input;
				bool ranged = TEST_FIELD_BIT(function->function.address[1] & 1);
				real x = inputs->values[input];
				real y = ranged ? inputs->values[function->range_input] : 0.0f;

				if (input == 1)
				{
					x = (real)fmod((double)(x / function->period), 1.0);
				}
				if (speaker != NONE && input == 3)
				{
					x *= function_222250(&inputs->direction, speaker);
				}
				if (ranged)
				{
					short range_input = function->range_input;

					if (range_input == 1)
					{
						y = (real)fmod((double)(y / function->period), 1.0);
					}
					if (speaker != NONE && range_input == 3)
					{
						y *= function_222250(&inputs->direction, speaker);
					}
				}
				if (function->function.address && function->function.size > 0)
				{
					value = function_13b390(&function->function, x, y);
					byte const *data_header = function->function.address;

					if (!(data_header[1] & 0xf0))
					{
						real lower = *(real const *)(data_header + 4);
						real upper = *(real const *)(data_header + 8);

						value = lower + (upper - lower) * PIN(value, 0.0f, 1.0f);
					}
				}
				else
				{
					value = 0.0f;
				}
			}
			else
			{
				value = overrides->constants[source >> 1];
			}
			source_index++;
		}

		value = PIN(value, parameter->minimum, parameter->maximum);
		if (component->header.flags & 1)
		{
			switch (parameter->type)
			{
			case 0:
				block->data[i + 1] = real_to_long(value);
				break;
			case 1:
				*(real *)&block->data[i + 1] = value;
				break;
			}
		}
		else
		{
			switch (parameter->type)
			{
			case 0:
				block->data[i] = real_to_long(value);
				break;
			case 1:
				block->data[i] = function_2221d0(value);
				break;
			}
		}
	}
}

/* writes the data blocks of a sound effect's components to a buffer: their
   count, then each block; the size in bytes goes to buffer_size */
// @retail 0x2225a0
void function_2225a0(s_effect_overrides const *overrides, s_effect_components const *components, s_effect_inputs const *inputs,
	long *buffer, long *buffer_size)
{
	long offset = 0;
	long override_index = 0;
	long i;

	buffer[0] = components->count;
	*buffer_size = 4;
	for (i = 0; i < components->count; i++)
	{
		s_effect_component const *component = &components->components[i];
		s_effect_component_overrides const *component_overrides = NULL;
		bool by_speaker;
		long speaker;
		long speaker_count;

		if (overrides->component_mask & (1 << i))
		{
			component_overrides = &overrides->components[override_index++];
		}
		by_speaker = false;
		if (component->name == 0x11000138)
		{
			by_speaker = true;
		}
		speaker_count = by_speaker ? 4 : 0;
		for (speaker = by_speaker ? 0 : NONE; speaker < speaker_count; speaker++)
		{
			function_2222c0((s_effect_data_block *)&buffer[offset + 1], component, component_overrides, inputs, speaker);
			*buffer_size += ((s_effect_data_block *)&buffer[offset + 1])->header.size + 0xc;
			offset += (((s_effect_data_block *)&buffer[offset + 1])->header.size + 0xf) >> 2;
		}
	}
}

/* the environment effects of a sound */
struct s_sound_effect_entry
{
	byte unknown00[8];
	long name;
};

struct s_sound_effect_definition
{
	byte unknown00[0x14];
	long entry_count;
	s_sound_effect_entry *entries;
};

struct s_sound_effect_template_view
{
	byte unknown00[4];
	long definition_tag_index;
};

struct s_sound_effect_source
{
	byte unknown00[0x2c];
	long template_count;
	s_sound_effect_template_view *templates;
};

struct s_sound_effect_request
{
	dword flags;
	byte unknown04[0x8e - 4];
	short voice_effect_index;
	char mixbin;
	byte unknown91[0x98 - 0x91];
	long handle;
};

/* the voice effect settings (include/network_voice.h) */
struct s_voice_effects;
extern s_voice_effects *g_510c90;

struct s_sound_class_definition;
void *function_18d090(long tag_index, long handle);
s_sound_class_definition *sound_get_class(long tag_index);
long function_1914f0(long string_id);

static inline s_sound_effect_template_view *sound_effect_source_get_template(s_sound_effect_source *source)
{
	return source->template_count > 0 ? source->templates : NULL;
}

// @retail 0x2226b0
void function_2226b0(long tag_index, s_sound_effect_request *request)
{
	long handle = (request->flags & 0x100) ? request->handle : NONE;
	s_sound_effect_source *effect = (s_sound_effect_source *)function_18d090(tag_index, handle);
	s_sound_effect_source *sound_class = (s_sound_effect_source *)sound_get_class(tag_index);
	s_sound_effect_source *source = NULL;

	if (sound_class && sound_class->template_count > 0)
	{
		source = sound_class;
	}
	if (effect && effect->template_count > 0)
	{
		source = effect;
	}

	if (source)
	{
		long definition_tag_index = sound_effect_source_get_template(source)->definition_tag_index;

		if (definition_tag_index != NONE)
		{
			s_sound_effect_definition *definition = (s_sound_effect_definition *)g_4e3b44[definition_tag_index & 0xffff].bytes;

			if (definition->entry_count > 0)
			{
				s_sound_effect_entry *entry = definition->entries;

				if (entry->name == 0x1300012d)
				{
					short voice_effect_index = ((short *)g_510c90)[3];

					if (voice_effect_index != NONE)
					{
						request->voice_effect_index = voice_effect_index;
						request->flags |= 0x40;
					}
				}

				short mixbin = (short)function_1914f0(entry->name);
				if (mixbin != NONE)
				{
					request->mixbin = (char)mixbin;
					request->flags |= 0x200;
				}
			}
		}
	}
}

/* ---- the effects of a sound's playback ---- */

/* a sound effect of a playback (its template): the effect definition's tag
   and the template's settings */
struct s_playback_sound_effect
{
	byte unknown00[4];
	long definition_tag_index;
	byte unknown08[0x20 - 8];
	long override_count;
	s_effect_overrides *overrides;
};

/* a platform playback, or a sound class's (0x34 bytes) */
struct s_platform_playback_view
{
	byte unknown00[0x14];
	long filter_count;
	void *filters;
	long pitch_lfo_count;
	void *pitch_lfos;
	long filter_lfo_count;
	void *filter_lfos;
	long effect_count;
	s_playback_sound_effect *effects;
};

/* a sound effect tag: its components */
struct s_sound_effect_tag_view
{
	byte unknown00[0x14];
	long count;
	s_effect_components *components;
};

/* the effect data of a sound: data blocks and their size in bytes */
struct s_sound_effect_buffer
{
	long data[0x100];
	long size;
};

/* what a sound's playback applies to it */
struct s_sound_playback_effects
{
	byte unknown00[8];
	long unknown08;
	long unknown0c;
	void *filter;
	void *pitch_lfo;
	void *filter_lfo;
	s_sound_effect_buffer effect;
};

static inline void *tag_block_get_first(long count, void *elements)
{
	if (PIN(0, 0, count - 1) == 0)
	{
		return elements;
	}
	return NULL;
}

static inline bool playback_sound_effect_valid(s_playback_sound_effect const *effect)
{
	return effect && effect->definition_tag_index != NONE && effect->override_count > 0;
}

static inline s_effect_components *sound_effect_tag_get_components(long tag_index)
{
	return ((s_sound_effect_tag_view *)g_4e3b44[tag_index & 0xffff].bytes)->components;
}

/* the effects of a sound played through its class and its playback */
// @retail 0x222770
void function_222770(long tag_index, long handle, s_effect_inputs const *inputs, s_sound_playback_effects *effects)
{
	s_platform_playback_view *source = NULL;
	s_platform_playback_view *playback = (s_platform_playback_view *)function_18d090(tag_index, handle);
	s_platform_playback_view *class_playback = (s_platform_playback_view *)sound_get_class(tag_index);

	if (class_playback)
	{
		void *filter = tag_block_get_first(class_playback->filter_count, class_playback->filters);
		void *pitch_lfo = tag_block_get_first(class_playback->pitch_lfo_count, class_playback->pitch_lfos);
		void *filter_lfo = tag_block_get_first(class_playback->filter_lfo_count, class_playback->filter_lfos);
		s_playback_sound_effect *effect = (s_playback_sound_effect *)tag_block_get_first(class_playback->effect_count, class_playback->effects);

		effects->unknown08 = 0;
		effects->unknown0c = 0;
		effects->filter = filter;
		effects->pitch_lfo = pitch_lfo;
		effects->filter_lfo = filter_lfo;
		if (playback_sound_effect_valid(effect))
		{
			source = class_playback;
		}
	}
	if (playback && playback != class_playback)
	{
		void *filter = tag_block_get_first(playback->filter_count, playback->filters);
		void *pitch_lfo = tag_block_get_first(playback->pitch_lfo_count, playback->pitch_lfos);
		void *filter_lfo = tag_block_get_first(playback->filter_lfo_count, playback->filter_lfos);
		s_playback_sound_effect *effect = (s_playback_sound_effect *)tag_block_get_first(playback->effect_count, playback->effects);

		if (filter)
		{
			effects->filter = filter;
		}
		if (pitch_lfo)
		{
			effects->pitch_lfo = pitch_lfo;
		}
		if (filter_lfo)
		{
			effects->filter_lfo = filter_lfo;
		}
		if (playback_sound_effect_valid(effect))
		{
			source = playback;
		}
	}

	if (source)
	{
		/* retail reads the playback's effect here, whichever was chosen */
		s_playback_sound_effect *effect = playback->effect_count > 0 ? playback->effects : NULL;

		function_2225a0(effect->overrides, sound_effect_tag_get_components(effect->definition_tag_index), inputs,
			effects->effect.data, &effects->effect.size);
	}
	else
	{
		effects->effect.size = 0;
	}
}

/* the effect data of a platform playback */
// @retail 0x2228d0
void function_2228d0(long handle, s_sound_effect_buffer *buffer, s_effect_inputs const *inputs)
{
	s_platform_playback_view *playback = (s_platform_playback_view *)function_18d090(NONE, handle);

	if (playback)
	{
		s_playback_sound_effect *effect = (s_playback_sound_effect *)tag_block_get_first(playback->effect_count, playback->effects);

		if (playback_sound_effect_valid(effect))
		{
			function_2225a0(effect->overrides, sound_effect_tag_get_components(effect->definition_tag_index), inputs,
				buffer->data, &buffer->size);
		}
	}
}

/* ---- the sound environment a point hears ---- */

struct s_14b240_owner;
struct s_bsp3d_disk;
long function_18cfd0(long cluster_index, real_point3d const *point, real *distance);
real function_14b240(s_14b240_owner const *owner, s_bsp3d_disk const *disk, real_point3d const *point);

/* the structure's sound environments, as their disks (0x24 bytes) */
struct s_sound_environment_disk_view
{
	byte unknown00[0x24];
};

struct s_sound_environment_bsp_view
{
	byte unknown00[0x60];
	s_sound_environment_disk_view *environments;
};

/* how much a point is inside the sound environment nearest to a source in
   a cluster: 0 at its disk, approaching 1 far away; and a value of the
   source */
// @retail 0x222150
void function_222150(long cluster_index, real_point3d const *point, real const *values, real_point3d const *listener, long index, real *result)
{
	s_sound_environment_bsp_view *bsp = (s_sound_environment_bsp_view *)g_4e0348;
	real distance;
	long environment_index = function_18cfd0(cluster_index, point, &distance);

	if (environment_index != NONE)
	{
		real value = values[index];

		distance = function_14b240((s_14b240_owner const *)bsp, (s_bsp3d_disk const *)&bsp->environments[environment_index], listener);
		result[1] = value;
		result[0] = (real)(1.0 - 1.0f / (distance + 1.0f));
	}
	else
	{
		result[0] = 1.0f;
		result[1] = 0.0f;
	}
}
