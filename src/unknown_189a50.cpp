// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_189A50.CPP: plays a sound a number of times, on an object or
   without a position (an outside function lane A's script functions need;
   the sound sources of unknown_189010.cpp) */

#include "cseries.h"
#include "globals.h"
#include "sound_sources.h"
#include "sound_records.h"
#include "object_markers.h"

struct s_sound_label_play
{
	long label;
	long tag_index;
	real scale;
	char const *variant;
};

long function_1890c0(s_sound_label_play const *play, long object_index, short value, real_point3d const *position, real_vector3d const *direction);
long function_189fe0(s_sound_request const *request, long tag_index);
long game_sound_find_platform_playback_by_label(long label);

/* retail inlines these two of unknown_189010.cpp here (0x1891d0, 0x189760) */
static inline long sound_play_on_object_marker(long object_index, long marker_name, s_sound_label_play const *play)
{
	s_object_marker marker;

	function_b8d30(object_index, marker_name, &marker, 1, false);
	return function_1890c0(play, object_index, marker.node_index, &marker.node_matrix.position, &marker.node_matrix.forward);
}

static inline long sound_play_unpositioned(s_sound_label_play const *play)
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

// @retail 0x189a50
void function_189a50(long tag_index, long object_index, real scale, long count)
{
	if (count)
	{
		s_sound_label_play play;

		play.label = NONE;
		play.tag_index = tag_index;
		play.scale = scale;
		play.variant = NULL;
		for (; count; count--)
		{
			if (object_index != NONE)
				sound_play_on_object_marker(object_index, 0, &play);
			else
				sound_play_unpositioned(&play);
		}
	}
}
