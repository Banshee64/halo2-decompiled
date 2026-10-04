// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_221DA0.CPP: sound transmission helpers: whether a listener hears
   a sound through the structure's clusters, the effect parameters of a
   sound's environment, and the conversions of its DSP parameters */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "local_cameras.h"

#define PIN(n, floor, ceiling) ((n) < (floor) ? (floor) : ((n) > (ceiling) ? (ceiling) : (n)))

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

/* a real in [-1, 1] to the DSP's 24 bit fixed point */
// @retail 0x2221d0
long function_2221d0(real value)
{
	value = PIN(value, -1.0f, 1.0f);
	if (value < 0.0f)
	{
		return 0x1000000 - (long)(value * -8388608.0);
	}
	return (long)(value * 8388607.0);
}

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
