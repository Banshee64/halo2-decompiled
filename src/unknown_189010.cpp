// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_189010.CPP: sound sources: whether a sound plays from an object
   of the vehicle type, and the source description a sound starts from */

#include "cseries.h"
#include "globals.h"
#include "sound_sources.h"
#include "local_cameras.h"
#include <math.h>

struct s_sound_tag_flags
{
	byte flags;
};

struct s_source_object
{
	byte unknown00[0x12c];
	byte flags12c;
	byte unknown12d[0x154 - 0x12d];
	long value154;
};

struct s_source_object_header
{
	byte unknown00[3];
	byte type;
	byte unknown04[4];
	s_source_object *object;
};

struct s_sound_source_description
{
	char value0;
	byte flags;
	byte unknown02[2];
	real_point3d position;
	real_vector3d direction;
	byte unknown1c[8];
	long value24;
};

static inline s_source_object_header *source_object_header(long object_index)
{
	return (s_source_object_header *)g_4e0300->data + (object_index & 0xffff);
}

// @retail 0x189010
bool function_189010(long object_index, long tag_index)
{
	bool result = false;

	if (tag_index != NONE && (((s_sound_tag_flags *)g_4e3b44[tag_index & 0xffff].bytes)->flags & 0x40) && object_index != NONE)
	{
		if ((1 << source_object_header(object_index)->type) & 4)
		{
			result = true;
		}
	}
	return result;
}

// @retail 0x1892a0
void function_1892a0(s_sound_source_description *description, real_point3d const *position, real_vector3d const *direction, short value, long tag_index, long object_index)
{
	description->position = *position;
	description->direction = *direction;
	description->value0 = value == NONE ? 0 : (char)value;
	description->value24 = NONE;

	if (object_index != NONE && function_189010(object_index, tag_index))
	{
		s_source_object_header *header = source_object_header(object_index);
		if ((1 << header->type) & 4)
		{
			s_source_object *object = header->object;
			if (object->flags12c & 1)
			{
				long value154 = object->value154;
				if (value154 != NONE)
				{
					description->value24 = value154;
				}
			}
		}
	}
}

/* what 0x21d110 starts a sound from (0xa0 bytes) */
struct s_sound_play_state
{
	dword flags;
	long priority;
	byte unknown08[4];
	s_sound_location location;
	long object_index;
	byte unknown54[4];
	s_sound_source_callbacks const *source;
	s_sound_marker marker;
	byte unknown84[0x8c - 0x84];
	short marker_size;
	byte unknown8e[0x98 - 0x8e];
	long platform_playback;
	short variant0;
	short variant1;
};

struct s_sound_class_play_flags
{
	byte unknown00[8];
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word flag8 : 1;
	word flag9 : 1;
	word flag10 : 1;
	word flag11 : 1;
	word flag12 : 1;
	word flag13 : 1;
	word flag14 : 1;
	word flag15 : 1;
};

struct s_sound_play_tag
{
	byte unknown00;
	byte flags;
	char class_index;
	byte unknown03[0x10 - 3];
	long duration;
};

struct s_unknown_5c;
s_unknown_5c *function_221810(short index);
long function_21d110(s_sound_play_state *state, long tag_index);
long game_sound_find_platform_playback_by_label(long label);
void function_18d4f0(long object_index, char *audible, long *local_player_index);

static __forceinline long real_to_long_round(real value)
{
	long result;

	__asm
	{
		fld value
		fistp result
	}
	return result;
}

// @retail 0x189fe0
long function_189fe0(s_sound_request const *request, long tag_index)
{
	long result = NONE;

	if (tag_index != NONE)
	{
		s_sound_play_tag *sound = (s_sound_play_tag *)g_4e3b44[tag_index & 0xffff].bytes;
		s_sound_play_state state;
		dword flags = (request->source ? 0x20 : 0) + 5;

		if (request->object_index != NONE)
		{
			flags |= 8;
		}
		else
		{
			flags &= ~8;
		}
		if (request->platform_playback != NONE)
		{
			flags |= 0x100;
		}
		else
		{
			flags &= ~0x100;
		}
		state.priority = request->location.audible ? 3 : 1;
		state.flags = flags;
		state.location = request->location;
		state.object_index = request->object_index;
		state.source = request->source;
		state.platform_playback = request->platform_playback;

		if (TEST_FIELD_BIT(((s_sound_class_play_flags *)function_221810(sound->class_index))->flag15) && !(sound->flags & 1))
		{
			state.location.flag11 = true;
		}

		if (request->marker)
		{
			state.marker = *request->marker;
			state.marker_size = sizeof(s_sound_marker);
		}
		else
		{
			state.marker_size = 0;
		}

		char const *variant = request->variant;
		if (variant && variant[1] != NONE && variant[0] != NONE)
		{
			state.flags |= 0x400;
			state.variant0 = request->variant[0];
			state.variant1 = request->variant[1];
		}

		result = function_21d110(&state, tag_index);
		if (result != NONE)
		{
			s_sound_class_play_flags *sound_class = (s_sound_class_play_flags *)function_221810(sound->class_index);
			if (TEST_FIELD_BIT(sound_class->flag1) && TEST_FIELD_BIT(sound_class->flag2))
			{
				s_game_time_globals *game_time = g_510c54;
				long end_time = game_time->game_time + real_to_long_round((real)sound->duration * 0.001f * (real)game_time->ticks_per_second) + 10;
				s_looping_sound_globals *globals = g_4ed288;

				globals->value20 = end_time > globals->value20 ? end_time : globals->value20;
				if (state.location.flag8 || TEST_FIELD_BIT(state.location.flag11) || !state.location.audible)
				{
					globals->value24 = end_time > globals->value24 ? end_time : globals->value24;
				}
			}
		}
	}
	return result;
}

// @retail 0x1895f0
long function_1895f0(s_sound_position const *position, real scale, long tag_index)
{
	s_sound_request request;

	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.spatial = *position;
	request.location.unknown08 = 0;
	request.marker = NULL;
	request.source = NULL;
	request.variant = NULL;
	request.location.scale = scale;
	request.location.audible = 1;
	request.location.requested_audible = 1;
	request.object_index = NONE;
	request.platform_playback = NONE;
	return function_189fe0(&request, tag_index);
}

struct s_sound_label_play
{
	long label;
	long tag_index;
	real scale;
	char const *variant;
};

// @retail 0x189650
long function_189650(s_sound_position const *position, s_sound_label_play const *play)
{
	s_sound_request request;

	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.spatial = *position;
	request.location.scale = play->scale;
	request.location.audible = 1;
	request.location.requested_audible = 1;
	request.location.unknown08 = 0;
	request.object_index = NONE;
	request.platform_playback = game_sound_find_platform_playback_by_label(play->label);
	request.marker = NULL;
	request.source = NULL;
	request.variant = NULL;
	return function_189fe0(&request, play->tag_index);
}

// @retail 0x1896c0
long function_1896c0(real scale, long tag_index)
{
	s_sound_request request;

	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.audible = 0;
	request.location.requested_audible = 0;
	request.location.unknown08 = 0;
	request.marker = NULL;
	request.source = NULL;
	request.variant = NULL;
	request.object_index = NONE;
	request.platform_playback = NONE;
	request.location.scale = scale;
	return function_189fe0(&request, tag_index);
}

extern s_sound_source_callbacks const g_444b7c;

// @retail 0x189710
long function_189710(real scale, long tag_index)
{
	s_sound_request request;

	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.audible = 0;
	request.location.requested_audible = 0;
	request.location.unknown08 = 0;
	request.marker = NULL;
	request.variant = NULL;
	request.location.scale = scale;
	request.object_index = NONE;
	request.platform_playback = NONE;
	request.source = &g_444b7c;
	return function_189fe0(&request, tag_index);
}

// @retail 0x189760
long function_189760(s_sound_label_play const *play)
{
	s_sound_request request;

	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.audible = 0;
	request.location.requested_audible = 0;
	request.location.scale = play->scale;
	request.location.unknown08 = 0;
	request.object_index = NONE;
	request.platform_playback = game_sound_find_platform_playback_by_label(play->label);
	request.marker = NULL;
	request.source = NULL;
	request.variant = NULL;
	return function_189fe0(&request, play->tag_index);
}

// @retail 0x189340
long function_189340(long object_index, s_sound_source_callbacks const *source, long tag_index, char audible, s_sound_marker const *marker, real scale, long platform_playback, char const *variant)
{
	s_sound_request request;

	request.location.flags = 0;
	function_18d4f0(object_index, &audible, NULL);
	request.location.audible = audible;
	request.location.requested_audible = audible;
	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.unknown08 = 0;
	request.location.spatial.location.leaf_index = NONE;
	request.location.spatial.location.cluster_index = NONE;
	request.location.spatial.location.bsp_index = g_4686c4;
	request.location.scale = scale;
	request.variant = variant;

	if (source->update(object_index, tag_index, marker, &request.location))
	{
		request.platform_playback = platform_playback;
		request.object_index = object_index;
		request.marker = marker;
		request.source = source;
		return function_189fe0(&request, tag_index);
	}
	return NONE;
}

void function_1892a0(s_sound_source_description *description, real_point3d const *position, real_vector3d const *direction, short value, long tag_index, long object_index);
extern real_point3d *g_468788;
extern s_sound_source_callbacks const g_444b5c;

struct s_sound_globals_side_view
{
	long front[2];
	long side[2];
	long back[2];
};

struct s_tag_header_alt_sound_view
{
	byte unknown00[0x14];
	long sound_globals_tag_index;
};

struct s_tag_header_globals_sound_view
{
	byte unknown00[0xc0];
	void *header;
	s_tag_header_alt_sound_view *header_alt;
};

struct s_local_camera_view
{
	byte unknown00[0x18];
	real_vector3d forward;
	byte unknown24[0x30 - 0x24];
	real_point3d position;
};

// @retail 0x189400
long function_189400(s_sound_position const *position, long object_index, long tag_index, real scale)
{
	long local_player_index = NONE;

	if (object_index != NONE)
	{
		char audible = 1;
		if (!(((s_sound_play_tag *)g_4e3b44[tag_index & 0xffff].bytes)->unknown00 & 4))
		{
			function_18d4f0(object_index, &audible, &local_player_index);
		}

		bool audible_everywhere = audible == 1;
		if (!audible_everywhere)
		{
			bool camera_inactive = local_player_index != NONE && !local_camera_get(local_player_index)->active;
			if (!camera_inactive)
			{
				s_sound_marker marker;

				marker.flag0 = false;
				marker.flag1 = false;
				function_1892a0((s_sound_source_description *)&marker, g_468788, (real_vector3d *)g_4687a8, NONE, tag_index, object_index);

				s_tag_header_globals_sound_view *globals = (s_tag_header_globals_sound_view *)g_4e034c;
				s_tag_header_alt_sound_view *header = globals->header ? globals->header_alt : NULL;
				long sound_globals_tag_index = header->sound_globals_tag_index;
				if (sound_globals_tag_index != NONE)
				{
					s_sound_globals_side_view *sides = (s_sound_globals_side_view *)g_4e3b44[sound_globals_tag_index & 0xffff].bytes;
					s_local_camera_view *camera = (s_local_camera_view *)local_camera_get(local_player_index);
					real_vector3d forward = camera->forward;
					real distance = (position->position.y * forward.j + position->position.z * forward.k + position->position.x * forward.i)
						- (camera->position.y * forward.j + camera->position.z * forward.k + camera->position.x * forward.i);

					if ((real)fabs(distance) <= 0.01f)
					{
						marker.value1c = sides->side[0];
						marker.value20 = sides->side[1];
					}
					else if (distance > 0.0f)
					{
						marker.value1c = sides->front[0];
						marker.value20 = sides->front[1];
					}
					else
					{
						marker.value1c = sides->back[0];
						marker.value20 = sides->back[1];
					}
					marker.flag1 = true;
				}
				return function_189340(object_index, &g_444b5c, tag_index, 0, &marker, scale, NONE, NULL);
			}
		}
	}
	return function_1895f0(position, scale, tag_index);
}

extern s_sound_source_callbacks const g_444afc;
extern s_sound_source_callbacks const g_444b1c;

// @retail 0x189060
long function_189060(long object_index, short value, real scale, real_point3d const *position, real_vector3d const *direction, long tag_index)
{
	s_sound_source_description description;

	description.flags = 0;
	function_1892a0(&description, position, direction, value, tag_index, object_index);
	return function_189340(object_index, function_189010(object_index, tag_index) ? &g_444b1c : &g_444afc, tag_index, 1, (s_sound_marker *)&description, scale, NONE, NULL);
}

// @retail 0x1890c0
long function_1890c0(s_sound_label_play const *play, long object_index, short value, real_point3d const *position, real_vector3d const *direction)
{
	s_sound_source_description description;
	s_sound_request request;
	char audible = 1;

	description.flags = 0;
	function_18d4f0(object_index, &audible, NULL);
	function_1892a0(&description, position, direction, value, play->tag_index, object_index);
	request.location.audible = audible;
	request.location.requested_audible = audible;
	request.location.spatial.location.leaf_index = NONE;
	request.location.spatial.location.cluster_index = NONE;
	request.location.spatial.location.bsp_index = g_4686c4;
	request.variant = play->variant;
	request.location.unknown02 = 0;
	request.location.flags = 0;
	request.location.unknown08 = 0;
	request.location.scale = play->scale;

	if (function_18c3b0(object_index, play->tag_index, (s_sound_marker *)&description, &request.location))
	{
		request.object_index = object_index;
		request.marker = (s_sound_marker *)&description;
		request.platform_playback = game_sound_find_platform_playback_by_label(play->label);
		request.source = function_189010(object_index, play->tag_index) ? &g_444b1c : &g_444afc;
		return function_189fe0(&request, play->tag_index);
	}
	return NONE;
}
