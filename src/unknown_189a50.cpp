// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_189A50.CPP: plays a sound a number of times, on an object or
   without a position (an outside function lane A's script functions need;
   the sound sources of unknown_189010.cpp) */

#include "cseries.h"
#include "globals.h"
#include "sound_sources.h"
#include "sound_records.h"
#include "object_markers.h"
#include "sound_definitions.h"
#include <math.h>

struct s_sound_label_play
{
	long label;
	long tag_index;
	real scale;
	char const *variant;
};

long function_1890c0(s_sound_label_play const *play, long object_index, short value, point3f const *position, vector3f const *direction);
long function_189fe0(s_sound_request const *request, long tag_index);
long function_18d5b0(long label);

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
	request.platform_playback = function_18d5b0(play->label);
	request.marker = NULL;
	request.source = NULL;
	request.variant = NULL;
	return function_189fe0(&request, play->tag_index);
}

/* a local player's camera (0x48 bytes, local_cameras.h) with its matrix */
struct s_local_camera_matrix_view
{
	byte unknown00[6];
	bool active;
	byte unknown07;
	transform4x3f matrix;
	byte unknown3c[0x48 - 0x3c];
};

struct s_local_cameras_matrix_view
{
	byte unknown00[0x88];
	s_local_camera_matrix_view cameras[4];
};

struct s_4e6380;
extern s_4e6380 *g_4e6380;

struct s_sound_class_distance_view
{
	real minimum_distance;
	byte unknown04[0x38 - 4];
};

struct s_sound_globals_class_distance_view
{
	byte unknown00[4];
	s_sound_class_distance_view *classes;
};

struct s_sound_promotion_distance_view
{
	byte unknown00[0x18];
	real minimum_distance;
};

struct s_unknown_5c;
s_unknown_5c *function_221810(short index);
dword vector3d_compress(vector3f const *vector);
void function_11bed0(s_location *location, point3f const *point);
long function_1895f0(s_sound_position const *position, real scale, long tag_index);

static inline long local_player_first_index(void)
{
	long result = NONE;

	for (long i = 0; i < 4; i++)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

/* plays a sound around the first local player's camera, at an angle from its
   forward vector and at its minimum distance */
// @retail 0x189b20
void function_189b20(long tag_index, real angle, real scale)
{
	s_local_camera_matrix_view *camera = &((s_local_cameras_matrix_view *)g_4e6380)->cameras[local_player_first_index()];

	if (camera->active)
	{
		real radians = angle * 0.017453292f;
		vector3f direction;
		s_sound_definition *definition = sound_definition_get(tag_index);
		real distance;
		point3f point;
		s_sound_position position;

		direction.i = (real)cos(radians);
		direction.j = (real)sin(radians);
		direction.k = 0.0f;

		if (definition->flags & 0x400)
			distance = ((s_sound_promotion_distance_view *)function_221810(definition->promotion_index))->minimum_distance;
		else
			distance = ((s_sound_globals_class_distance_view *)g_51ebd4)->classes[definition->class_index].minimum_distance;

		real camera_scale = camera->matrix.scale;
		point.x = distance * direction.i;
		point.y = distance * direction.j;
		point.z = distance * direction.k;
		if (camera_scale != 1.0f)
		{
			point.x *= camera_scale;
			point.y *= camera_scale;
			point.z *= camera_scale;
		}
		position.position.x = camera->matrix.up.i * point.z + camera->matrix.left.i * point.y + camera->matrix.forward.i * point.x + camera->matrix.position.x;
		position.position.y = camera->matrix.up.j * point.z + camera->matrix.left.j * point.y + camera->matrix.forward.j * point.x + camera->matrix.position.y;
		position.position.z = camera->matrix.up.k * point.z + camera->matrix.left.k * point.y + camera->matrix.forward.k * point.x + camera->matrix.position.z;
		position.compressed_forward = vector3d_compress(g_4687a8);
		position.velocity = *g_4687a4;
		function_11bed0(&position.location, &position.position);
		function_1895f0(&position, scale, tag_index);
	}
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
