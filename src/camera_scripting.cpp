// @flags /O2 /arch:SSE /Gr
/* CAMERA_SCRIPTING.CPP: the scripted camera (0x16bd10..0x16c6ea): the
   velocity profile of a camera pan, cutscene camera points, animated
   cameras. The state lives at g_510c6c (unknown_1552e0.cpp); its mode
   picks which view of the bytes at +0x40 is current. unknown_16bc40.cpp
   holds three more functions of this file (0x16c2b0, 0x16c6f0, 0x16c740). */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "animation_graph.h"
#include "unknown_1cafc0.h"

/* a camera pan's velocity profile: it accelerates from the start rate to
   the peak, holds it, then moves toward the end rate (0x18 bytes) */
struct s_camera_velocity_profile
{
	real start_rate;
	real peak_rate;
	real acceleration;
	real deceleration;
	short acceleration_ticks;
	short deceleration_ticks;
	short total_ticks;
	bool valid;
};

enum
{
	_camera_scripting_mode_none = 0,
	_camera_scripting_mode_point,
	_camera_scripting_mode_pan,
	_camera_scripting_mode_animation,
	_camera_scripting_mode_first_person
};

/* the state of each mode, at +0x40 of the scripted camera */
struct s_camera_point_state
{
	short point_index;
	short type;
	real_vector3d offset;
	real_vector3d left;
};

struct s_camera_pan_state
{
	real_point3d position;
	real_quaternion rotation;
	long start_time;
	s_camera_velocity_profile profile;
};

struct s_camera_animation_state
{
	long start_time;
	long graph_tag_index;
	long animation_name;
	short cutscene_flag_index;
};

/* the scripted camera (g_510c6c, 0x78 bytes) */
struct s_camera_scripting_state
{
	byte unknown00;
	bool active;
	short mode;
	byte unknown04[0x14 - 0x4];
	long ticks;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	long object_index;
	union
	{
		s_camera_point_state point;
		s_camera_pan_state pan;
		s_camera_animation_state animation;
	};
};

struct s_unknown_78;
extern s_unknown_78 *g_510c6c;
#define camera_scripting_state ((s_camera_scripting_state *)g_510c6c)

/* the scenario's cutscene camera points (0x40 bytes) and flags (0x38 bytes) */
struct s_cutscene_camera_point
{
	short flags;
	short type;
	byte unknown04[0x28 - 0x4];
	real_point3d position;
	real orientation[3];
};

struct s_cutscene_flag
{
	byte unknown00[0x24];
	real_point3d position;
	real facing[2];
};

struct s_camera_scenario_view
{
	byte unknown000[0x1e0];
	long cutscene_flag_count;
	s_cutscene_flag *cutscene_flags;
	long camera_point_count;
	s_cutscene_camera_point *camera_points;
};

#define camera_scenario ((s_camera_scenario_view *)g_4e0350)

void function_141ce0(real a, real b, real c, real_matrix4x3 *out);
real_quaternion *function_141f60(matrix3x3 const *matrix, real_quaternion *out);
real_point3d *matrix4x3_transform_point(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *out);
real_vector3d *function_142640(real_matrix4x3 const *matrix, real_vector3d const *vector, real_vector3d *out);
void matrix4x3_from_point_and_vectors(real_matrix4x3 *out, real_point3d const *position, real_vector3d const *forward,
	real_vector3d const *up);
int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);
real_vector3d *function_11d090(real_vector3d const *v, real_vector3d *out);
void function_11df60(real_vector3d const *rotation, real_vector3d *forward, real_vector3d *up);
real function_30bf0(real_vector3d *v);
struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
bool function_11b930(long unit_index);
void function_16c6f0(long object_index, real_matrix4x3 *matrix);
c_animation_id function_1dd0b0(s_graph_tag *graph, long name);
void function_3f660(real_matrix4x3 const *matrix);
void function_1554b0(long unknown);
void __stdcall function_16f280(real unknown);

static inline long camera_seconds_to_ticks_round(real seconds)
{
	real ticks_real = (real)g_510c54->ticks_per_second * seconds;
	long ticks;
	__asm
	{
		fld ticks_real
		fistp ticks
	}
	return ticks;
}

static inline void cross_product3d(real_vector3d const *a, real_vector3d const *b, real_vector3d *out)
{
	out->i = a->j * b->k - a->k * b->j;
	out->j = a->k * b->i - a->i * b->k;
	out->k = a->i * b->j - a->j * b->i;
}

// @retail 0x16bd10
bool camera_velocity_profile_new(s_camera_velocity_profile *profile, real total_seconds, real acceleration_seconds,
	real deceleration_seconds, real start_rate, real end_rate)
{
	short total_ticks = (short)camera_seconds_to_ticks_round(total_seconds);
	short acceleration_ticks = (short)camera_seconds_to_ticks_round(acceleration_seconds);
	short deceleration_ticks = (short)camera_seconds_to_ticks_round(deceleration_seconds);
	bool result = false;

	profile->valid = false;
	if (total_ticks > 0)
	{
		real distance = (start_rate - 1.0f) * (real)acceleration_ticks + (real)(total_ticks * 2) +
			(real)deceleration_ticks * (end_rate - 1.0f);

		if (distance > 0.001f)
		{
			real peak_rate = 2.0f / distance;

			profile->peak_rate = peak_rate;
			profile->start_rate = peak_rate * start_rate;
			if (acceleration_ticks > 0)
			{
				profile->acceleration = (peak_rate - profile->start_rate) / ((real)acceleration_ticks * 2.0f);
			}
			else
			{
				profile->acceleration = 0.0f;
			}
			if (deceleration_ticks > 0)
			{
				profile->deceleration = peak_rate * (end_rate - 1.0f) / ((real)deceleration_ticks * 2.0f);
			}
			else
			{
				profile->deceleration = 0.0f;
			}
			result = true;
			profile->acceleration_ticks = acceleration_ticks;
			profile->deceleration_ticks = deceleration_ticks;
			profile->total_ticks = total_ticks;
			profile->valid = result;
		}
	}

	return result;
}

// @retail 0x16be50
real camera_velocity_profile_evaluate(s_camera_velocity_profile const *profile, long ticks)
{
	real result = 1.0f;

	if (profile->valid)
	{
		real rates[3];
		real accelerations[3];
		long acceleration_ticks;
		long hold_ticks;
		long deceleration_ticks;

		rates[0] = profile->start_rate;
		rates[1] = profile->peak_rate;
		rates[2] = profile->peak_rate;
		accelerations[0] = profile->acceleration;
		accelerations[1] = 0.0f;
		accelerations[2] = profile->deceleration;

		if (ticks < 0)
		{
			acceleration_ticks = 0;
		}
		else if (ticks > profile->acceleration_ticks)
		{
			acceleration_ticks = profile->acceleration_ticks;
		}
		else
		{
			acceleration_ticks = ticks;
		}

		if (ticks - profile->acceleration_ticks < 0)
		{
			hold_ticks = 0;
		}
		else if (ticks - profile->acceleration_ticks >
			profile->total_ticks - profile->deceleration_ticks - profile->acceleration_ticks)
		{
			hold_ticks = profile->total_ticks - profile->deceleration_ticks - profile->acceleration_ticks;
		}
		else
		{
			hold_ticks = ticks - profile->acceleration_ticks;
		}

		if (ticks - (profile->total_ticks - profile->deceleration_ticks) < 0)
		{
			deceleration_ticks = 0;
		}
		else if (ticks - (profile->total_ticks - profile->deceleration_ticks) > profile->deceleration_ticks)
		{
			deceleration_ticks = profile->deceleration_ticks;
		}
		else
		{
			deceleration_ticks = ticks - (profile->total_ticks - profile->deceleration_ticks);
		}

		{
			real a = (real)acceleration_ticks;
			real b = (real)hold_ticks;
			real c = (real)deceleration_ticks;

			result = c * c * accelerations[2] + a * a * accelerations[0] + c * rates[2] + b * rates[1] + a * rates[0] +
				b * b * accelerations[1];
		}
		if (0.0f > result)
		{
			result = 0.0f;
		}
		else if (result > 1.0f)
		{
			result = 1.0f;
		}
	}

	return result;
}

// @retail 0x16bf70
void function_16bf70(long animation_graph_index, long animation_name, long unit_index, short cutscene_flag_index)
{
	if (animation_graph_index != NONE)
	{
		s_animation_state state;
		c_animation_id animation_id;

		state.initialize(animation_graph_index, NONE, true);
		animation_id = function_1dd0b0(graph_tag_get(state.graph_tag_index), animation_name);
		if (animation_id.index != NONE)
		{
			s_animation *animation = function_1daea0(graph_tag_get(state.graph_tag_index), animation_id);

			if (animation)
			{
				s_camera_scripting_state *camera = camera_scripting_state;

				camera->mode = _camera_scripting_mode_animation;
				camera->active = true;
				camera->animation.cutscene_flag_index = cutscene_flag_index;
				camera->animation.graph_tag_index = animation_graph_index;
				camera->animation.animation_name = animation_name;
				camera->animation.start_time = g_510c54->game_time;
				camera->ticks = camera_seconds_to_ticks_round(((real)animation->frame_count - 1.0f) * (1.0f / 30.0f));
				if (unit_index != NONE && function_badc0(unit_index, 3) && function_11b930(unit_index))
				{
					camera->object_index = unit_index;
				}
				else
				{
					camera->object_index = NONE;
				}
			}
		}
		state.channels_clear_partial();
	}
}

// @retail 0x16c0b0
void function_16c0b0(short camera_point_index)
{
	s_camera_scenario_view *scenario = camera_scenario;

	if (camera_point_index >= 0 && camera_point_index < scenario->camera_point_count)
	{
		s_cutscene_camera_point *point = &scenario->camera_points[camera_point_index];
		real_matrix4x3 matrix;

		function_141ce0(point->orientation[0], point->orientation[1], point->orientation[2], &matrix);
		matrix.position = point->position;
		function_3f660(&matrix);
	}
}

// @retail 0x16c2f0
void function_16c2f0(short camera_point_index, short ticks, long object_index)
{
	s_camera_scenario_view *scenario = camera_scenario;

	if (camera_point_index >= 0 && camera_point_index < scenario->camera_point_count)
	{
		s_cutscene_camera_point *point = &scenario->camera_points[camera_point_index];
		s_camera_scripting_state *camera = camera_scripting_state;
		real seconds;

		camera->mode = _camera_scripting_mode_point;
		camera->active = true;
		seconds = (real)ticks * (1.0f / 30.0f);
		camera->point.type = point->type;
		camera->point.point_index = camera_point_index;
		camera->position = point->position;
		function_11df60((real_vector3d const *)point->orientation, &camera->forward, &camera->up);
		camera->ticks = camera_seconds_to_ticks_round(seconds);
		if (object_index != NONE)
		{
			real_matrix4x3 matrix;

			switch (camera->point.type)
			{
			case 0:
				break;
			case 1:
				function_16c6f0(object_index, &matrix);
				function_142640(&matrix, &camera->forward, &camera->forward);
				function_142640(&matrix, &camera->up, &camera->up);
				function_142640(&matrix, (real_vector3d *)&camera->position, (real_vector3d *)&camera->position);
				break;
			case 2:
				function_16c6f0(object_index, &matrix);
				function_142640(&matrix, &camera->forward, &camera->forward);
				function_142640(&matrix, &camera->up, &camera->up);
				function_142640(&matrix, (real_vector3d *)&camera->position, (real_vector3d *)&camera->position);
				camera->point.offset = *(real_vector3d *)&matrix.position;
				cross_product3d(&camera->forward, &camera->up, &camera->point.left);
				function_30bf0(&camera->point.left);
				break;
			case 3:
				function_16c6f0(object_index, &matrix);
				matrix4x3_transform_point(&matrix, &camera->position, &camera->position);
				function_142640(&matrix, &camera->forward, &camera->forward);
				function_142640(&matrix, &camera->up, &camera->up);
				object_index = NONE;
				break;
			default:
				__assume(0);
			}
		}
		camera->object_index = object_index;
		function_1554b0(0);
		function_16f280(0.0001f);
	}
}

// @retail 0x16c4f0
void function_16c4f0(short camera_point_index, short ticks)
{
	if (ticks > 0)
	{
		s_cutscene_camera_point *point = &camera_scenario->camera_points[camera_point_index];
		s_camera_scripting_state *camera = camera_scripting_state;
		s_camera_pan_state *pan;
		real_matrix4x3 matrix;
		matrix3x3 rotation;

		camera->mode = _camera_scripting_mode_pan;
		camera->active = true;
		pan = &camera->pan;
		pan->position = point->position;
		function_141ce0(point->orientation[0], point->orientation[1], point->orientation[2], &matrix);
		rotation.forward = matrix.forward;
		rotation.left = matrix.left;
		rotation.up = matrix.up;
		function_141f60(&rotation, &pan->rotation);
		pan->start_time = g_510c54->game_time;
		camera_velocity_profile_new(&pan->profile, (real)ticks * (1.0f / 30.0f), 0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		function_16c2f0(camera_point_index, ticks, NONE);
	}
}

// @retail 0x16c5f0
void function_16c5f0(short camera_point_index, short target_point_index, short ticks, short acceleration_ticks,
	real start_rate, short deceleration_ticks, real end_rate)
{
	s_camera_scripting_state *camera;

	function_16c2f0(camera_point_index, 0, NONE);
	camera = camera_scripting_state;
	camera->mode = _camera_scripting_mode_pan;
	camera->active = true;
	if (camera_velocity_profile_new(&camera->pan.profile, (real)ticks * (1.0f / 30.0f),
		(real)acceleration_ticks * (1.0f / 30.0f), (real)deceleration_ticks * (1.0f / 30.0f), start_rate, end_rate))
	{
		s_cutscene_camera_point *point = &camera_scenario->camera_points[target_point_index];
		real_matrix4x3 matrix;

		camera->mode = _camera_scripting_mode_pan;
		camera->active = true;
		s_camera_pan_state *pan = &camera->pan;

		pan->position = point->position;
		function_141ce0(point->orientation[0], point->orientation[1], point->orientation[2], &matrix);
		function_141f60(&matrix.rotation, &pan->rotation);
		pan->start_time = g_510c54->game_time;
	}
	else
	{
		function_16c4f0(target_point_index, ticks);
	}
}

/* the objects' header data and the tags, as 0x16c110 reads them */
struct s_camera_object
{
	long definition_index;
	byte unknown004[0x116 - 0x4];
	short node_matrices_offset;
};

struct s_camera_object_header
{
	byte unknown00[8];
	s_camera_object *object;
};

static inline void *camera_tag_get(long tag_index)
{
	return g_4e3b44[tag_index & 0xffff].bytes;
}

// @retail 0x16c110
void function_16c110(long animation_graph_index, long animation_name, long object_index, short cutscene_flag_index,
	long frame)
{
	if (animation_graph_index != NONE)
	{
		s_animation_state state;
		c_animation_id animation_id;

		state.initialize(animation_graph_index, NONE, true);
		animation_id = function_1dd0b0(graph_tag_get(state.graph_tag_index), animation_name);
		if (animation_id.index != NONE)
		{
			real_matrix4x3 matrix;
			real_matrix4x3 transform;

			state.animation_matrix_get(animation_id, (real)frame * (1.0f / 30.0f), 0, &matrix);
			if (object_index != NONE)
			{
				s_camera_object *object = ((s_camera_object_header *)g_4e0300->data)[object_index & 0xffff].object;
				byte *object_definition = (byte *)camera_tag_get(object->definition_index);
				byte *model = (byte *)camera_tag_get(*(long *)(object_definition + 0x38));
				byte *render_model = (byte *)camera_tag_get(*(long *)(model + 0x4));
				byte *nodes = *(byte **)(render_model + 0x4c);

				function_142a60((real_matrix4x3 *)((byte *)object + object->node_matrices_offset),
					(real_matrix4x3 *)(nodes + 0x28), &transform);
				function_142a60(&transform, &matrix, &matrix);
			}
			if (cutscene_flag_index != NONE)
			{
				s_cutscene_flag *flag = &camera_scenario->cutscene_flags[cutscene_flag_index];
				real_vector3d forward;
				real_vector3d up;

				forward.i = (real)cos(flag->facing[0]) * (real)cos(flag->facing[1]);
				forward.j = (real)sin(flag->facing[0]) * (real)cos(flag->facing[1]);
				forward.k = (real)sin(flag->facing[1]);
				matrix4x3_from_point_and_vectors(&transform, &flag->position, &forward, function_11d090(&forward, &up));
				function_142a60(&transform, &matrix, &matrix);
			}
			function_3f660(&matrix);
		}
		state.channels_clear_partial();
	}
}
