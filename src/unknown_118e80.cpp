// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_118E80.CPP: an object's forward vector in the world (lane I's
   outside function; the command scripts' 0x25a130 calls it). The position's
   counterpart is 0xb9dd0 (unknown_0b58c0.cpp). */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include <string.h>
#include "object_markers.h"
#include "unknown_1cafc0.h"
#include "object_queries.h"
#include <math.h>

/* the object as this reads it */
struct s_forward_flags
{
	union
	{
		word value;
		struct
		{
			word flag0 : 1;
			word flag1 : 1;
			word flag2 : 1;
			word flag3 : 1;
			word flag4 : 1;
			word flag5 : 1;
			word flag6 : 1;
			word : 9;
		};
	};
};

struct s_forward_object
{
	long definition_index;
	byte unknown04[0x14 - 4];
	long parent_index;
	char parent_node;
	byte unknown19[0x30 - 0x19];
	point3f center;
	byte unknown3c[0x64 - 0x3c];
	point3f position;
	vector3f forward;
	vector3f up;
	byte unknown88[0x90 - 0x88];
	real value90;
	vector3f angular_velocity;
	byte unknowna0[0xf0 - 0xa0];
	real valuef0;
	byte unknownf4[0x10a - 0xf4];
	word : 2;
	word frozen : 1;
	word : 13;
	byte unknown10c[0x116 - 0x10c];
	short nodes_offset;
	byte unknown118[0x12a - 0x118];
	short animation_state_offset;
	s_forward_flags flags;
	byte unknown12e[0x146 - 0x12e];
	char speed_mode;
	byte unknown147;
	vector3f control;
	vector3f requested_forward;
	vector3f requested_up;
	vector3f turn_velocity;
	real turn;
	char mode;
};

struct s_forward_object_header
{
	byte unknown0[8];
	s_forward_object *object;
};

// @retail 0x118f70
byte function_118f70(long object_index)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	byte flags = (byte)object->flags.value;
	flags >>= 1;
	flags &= 1;
	return flags;
}

// @retail 0x119250
void function_119250(long object_index)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_forward_flags *flags = &object->flags;
	flags->flag6 = true;
}

PRIVATE __forceinline void object_flag_set_118fe0(s_forward_flags *flags, bool enabled)
{
	if (enabled)
		flags->flag0 = true;
	else
		flags->flag0 = false;
}

// @retail 0x118fe0
void function_118fe0(long object_index, bool enabled)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	if (TEST_FIELD_BIT(object->frozen))
		enabled = false;
	object_flag_set_118fe0(&object->flags, enabled);
}

// @retail 0x119bc0
bool function_119bc0(long object_index)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	bool result = false;
	if (!TEST_FIELD_BIT(object->frozen) && (TEST_FIELD_BIT(object->flags.flag5) || TEST_FIELD_BIT(object->flags.flag4)))
		result = true;
	return result;
}

struct s_orientation_request_118f90
{
	long name0;
	long name4;
	bool enabled;
	byte unknown09[2];
	bool force;
	byte unknown0c[0xc];
	vector3f forward;
	vector3f up;
};

// @retail 0x118f90
void function_118f90(s_orientation_request_118f90 *request)
{
	memset(request, 0, sizeof(*request));
	request->name0 = 0x6000086;
	request->name4 = 0x400000c;
	request->forward = *g_4687a8;
	request->up = *g_4687b0;
}

// @retail 0x119500
void function_119500(long object_index, long state, s_orientation_request_118f90 *request)
{
	if (request->enabled)
	{
		s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		if (TEST_FIELD_BIT(object->flags.flag5) && (object->mode == 1 || object->mode == 3) && object->parent_index == NONE)
		{
			if (object->value90 > 0.0f)
				request->name4 = 0xd00003f;
			else
				request->name4 = 0x800001e;
			return;
		}
		switch (state)
		{
		case 1: request->name4 = 0x400000c; break;
		case 2: request->name4 = 0xa000017; break;
		case 3: request->name4 = 0x9000016; break;
		case 4: request->name4 = 0xa000014; break;
		case 5: request->name4 = 0x9000015; break;
		}
	}
}

void function_ba350(long object_index, real seconds);

void function_b9b90(long object_index, bool disable);
void function_b7740(long object_index, vector3f const *linear_velocity, vector3f const *angular_velocity, bool skip_update);
void function_1c4b00(long object_index, void *linear, void *angular, long force);
void function_b7360(long object_index);
void function_bba20(long object_index);
real function_30bf0(vector3f *vector);
void function_118e80(long object_index, vector3f *forward);
bool function_1faf80(vector3f *facing, long object_index, void const *settings_pointer,
	vector3f const *control, real threshold, real *turn);
void function_11f0d0(vector3f *position, vector3f *forward, vector3f const *target, real rate, real max_angle, real scale);
void function_11d180(vector3f *left, vector3f const *in, vector3f *out, vector3f const *up, vector3f *forward);
real function_11ce20(vector3f const *a, vector3f const *b);
void function_109290(long object_index, long a, long b, long c);

struct s_facing_definition_1197b0
{
	byte unknown00[0xc4];
	real max_angle;
	real acceleration;
	real speed_scale;
	byte unknownd0[4];
	dword flags;
	byte unknownd8[0x13c - 0xd8];
	byte settings[1];
};

PRIVATE __forceinline void world_up_1197b0(long object_index, vector3f *up)
{
	s_forward_object_header *headers = (s_forward_object_header *)g_4e0300->data;
	s_forward_object *object = headers[object_index & 0xffff].object;
	if (object->parent_index == NONE)
		*up = object->up;
	else
	{
		s_forward_object *parent = headers[object->parent_index & 0xffff].object;
		transform4x3f *matrix = (transform4x3f *)((byte *)parent + parent->nodes_offset + object->parent_node * 0x34);
		real i = object->up.i, j = object->up.j, k = object->up.k;
		up->i = matrix->up.i * k + matrix->left.i * j + matrix->forward.i * i;
		up->j = matrix->up.j * k + matrix->left.j * j + matrix->forward.j * i;
		up->k = matrix->up.k * k + matrix->left.k * j + matrix->forward.k * i;
	}
}

// @retail 0x1197b0
void function_1197b0(long object_index)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_facing_definition_1197b0 *definition = (s_facing_definition_1197b0 *)g_4e3b44[object->definition_index & 0xffff].bytes;
	if ((bool)((definition->flags >> 4) & 1))
	{
		function_1faf80(&object->requested_forward, object_index, definition->settings, &object->control, 0.9f, &object->turn);
		return;
	}
	real max_angle = definition->max_angle;
	real acceleration = definition->acceleration;
	if (object->speed_mode == 1)
	{
		max_angle *= definition->speed_scale;
		acceleration *= definition->speed_scale;
	}
	vector3f forward, turn_velocity;
	if (max_angle == 0.0f && acceleration == 0.0f)
	{
		forward = object->requested_forward;
		turn_velocity = *g_4687a4;
	}
	else
	{
		function_118e80(object_index, &forward);
		turn_velocity = object->turn_velocity;
		function_11f0d0(&turn_velocity, &forward, &object->requested_forward, g_510c54->rate, max_angle, acceleration);
	}
	vector3f temporary_velocity = *g_4687a4;
	vector3f up;
	world_up_1197b0(object_index, &up);
	vector3f new_forward, target_up, left1, left2, forward2;
	function_11d180(&left1, &forward, &target_up, &object->requested_up, &new_forward);
	function_11d180(&left2, &forward, &up, &up, &forward2);
	if (function_11ce20(&up, &target_up) > max_angle * g_510c54->rate)
	{
		function_11f0d0(&temporary_velocity, &up, &target_up, g_510c54->rate, max_angle, acceleration * 2.0f);
		function_11d180(&left2, &forward, &up, &up, &forward2);
	}
	else
		up = target_up;
	point3f position = object->position;
	function_109290(object_index, (long)&position, (long)&new_forward, (long)&up);
	if (dot3f(&object->up, &up) < 0.9999f || dot3f(&object->forward, &new_forward) < 0.9999f)
	{
		object->up = up;
		object->forward = new_forward;
		function_bba20(object_index);
	}
	object->turn_velocity = turn_velocity;
}

struct s_impulse_definition_119020
{
	byte unknown00[0xd4];
	dword flags;
};

// @retail 0x119020
void function_119020(long object_index, vector3f const *impulse)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_impulse_definition_119020 *definition = (s_impulse_definition_119020 *)g_4e3b44[object->definition_index & 0xffff].bytes;
	function_b9b90(object_index, false);
	vector3f velocity_before_impulse;
	function_ba1d0(object_index, &velocity_before_impulse, NULL);
	vector3f velocity;
	velocity.i = impulse->i + velocity_before_impulse.i;
	velocity.j = impulse->j + velocity_before_impulse.j;
	velocity.k = impulse->k + velocity_before_impulse.k;
	if (TEST_FIELD_BIT(object->frozen) || (bool)((definition->flags >> 3) & 1) || object->mode == 2)
	{
		vector3f axis;
		axis.i = g_4687b0->j * impulse->k - g_4687b0->k * impulse->j;
		axis.j = g_4687b0->k * impulse->i - g_4687b0->i * impulse->k;
		axis.k = g_4687b0->i * impulse->j - g_4687b0->j * impulse->i;
		function_30bf0(&axis);
		real magnitude = (real)sqrt(impulse->i * impulse->i + impulse->j * impulse->j + impulse->k * impulse->k);
		real scale = function_x82e52f(&g_4e7408->unknown0, NULL, 0) * magnitude * 1.57079637f;
		object->angular_velocity.i += scale * axis.i;
		object->angular_velocity.j += scale * axis.j;
		object->angular_velocity.k += scale * axis.k;
	}
	function_b7740(object_index, &velocity, NULL, false);
	function_1c4b00(object_index, &velocity, NULL, 1);
	if (length_sq3f(&velocity) > 0.0001f)
	{
		function_b9b90(object_index, false);
		function_b7360(object_index);
		function_bba20(object_index);
	}
}

PRIVATE __forceinline bool direction_state_119650(long name)
{
	return name == 0xa000014 || name == 0x9000015 || name == 0x9000016 || name == 0xa000017;
}

// @retail 0x119650
bool function_119650(long object_index, long mode, long set, real seconds, long flags)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_animation_state *state = (s_animation_state *)((byte *)object + object->animation_state_offset);
	long old_mode = state->unknown70;
	long old_set = state->unknown7c;
	bool result = true;
	if (mode != 0x7000101 || set != 0x7000101)
		result = state->animation_set(mode, 0x7000101, 0x7000101, set, flags, 0x3f);
	if (result)
	{
		bool mode_changed = old_mode != mode && mode != 0x7000101;
		bool set_changed = old_set != set && set != 0x7000101;
		if (mode_changed || set_changed)
		{
			volatile real blend = 0.267f;
			if (mode_changed)
				blend = 0.267f;
			if ((old_set == 0x400000c && direction_state_119650(set)) ||
				(direction_state_119650(old_set) && (direction_state_119650(set) || set == 0x400000c)))
				blend = 0.267f;
			if (blend <= seconds)
				blend = seconds;
			if (blend > 0.0f)
				function_ba350(object_index, blend);
		}
	}
	return result;
}

// @retail 0x1195c0
void function_1195c0(long object_index, s_orientation_request_118f90 const *request)
{
	s_forward_object *object = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_animation_state *state = (s_animation_state *)((byte *)object + object->animation_state_offset);
	bool changed = false;
	if (request->name0 != state->unknown70 && request->name0 != 0x7000101)
		changed = true;
	if (request->name4 != state->unknown7c && request->name4 != 0x7000101)
		changed = true;
	if (request->force || state->channels[0].graph_tag_index == NONE || state->channels[0].animation_id.index == NONE || changed)
	{
		if (!function_119650(object_index, request->name0, request->name4, 0.0f, 0) && request->name4 == 0x400000c)
			state->channels_clear();
	}
}

// @retail 0x118e10
void function_118e10(long object_index, point3f *point)
{
	s_object_marker marker;
	if (function_b8d30(object_index, 0x4000095, &marker, 1, false) > 0)
		*point = marker.matrix.position;
	else
		*point = ((s_forward_object_header *)g_4e0300->data)[object_index & 0xffff].object->center;
}

struct s_damage_owner
{
	long player_index;
	long object_index;
	short team;
};

struct s_type_1e6529
{
	long definition_index;
	union { byte unknown04[4]; dword flags; };
	s_damage_owner owner;
	long unknown14;
	long unknown18;
	long unknown1c;
	short unknown20;
	short unknown22;
	point3f position;
	point3f origin;
	vector3f direction;
	vector3f node_direction;
	real unknown54;
	real unknown58;
	real unknown5c;
	real distance;
	real distance_scale;
	bool in_unknown_radius;
	byte unknown69[3];
	vector3f cone_direction;
	real unknown78;
	short unknown7c;
	short unknown7e;
	byte unknown80[4];
	bool unknown84;
	byte unknown85[3];
};

void function_d6660(s_type_1e6529 *data, long definition_index);
void object_get_damage_owner(long object_index, s_damage_owner *owner);
void function_d7b80(s_type_1e6529 *data, long object_index, short node_index, short unknown0c, short region_entry_index, vector3f const *unknown14);

struct s_parent_effect_definition_11a140
{
	byte unknown00[0x16c];
	long effect_index;
	byte unknown170[4];
	long alternate_effect_index;
};

// @retail 0x11a140
void function_11a140(long object_index)
{
	s_forward_object_header *headers = (s_forward_object_header *)g_4e0300->data;
	s_forward_object *object = headers[object_index & 0xffff].object;
	s_parent_effect_definition_11a140 *definition = (s_parent_effect_definition_11a140 *)g_4e3b44[object->definition_index & 0xffff].bytes;
	s_forward_object *parent = headers[object->parent_index & 0xffff].object;
	long effect_index = definition->effect_index;
	if (parent->valuef0 > 0.0f && definition->alternate_effect_index != NONE)
		effect_index = definition->alternate_effect_index;
	if (effect_index != NONE)
	{
		s_type_1e6529 data;
		data.unknown7c = NONE;
		function_d6660(&data, effect_index);
		object_get_damage_owner(object_index, &data.owner);
		data.origin = object->center;
		data.position = object->center;
		function_d7b80(&data, object->parent_index, NONE, NONE, NONE, NULL);
	}
}

/* the object's forward vector, turned by its parent's node when it has a
   parent */
// @retail 0x118e80
void function_118e80(long object_index, vector3f *forward)
{
	s_forward_object_header *headers = (s_forward_object_header *)g_4e0300->data;
	s_forward_object *object = headers[object_index & 0xffff].object;

	if (object->parent_index == NONE)
	{
		if (forward)
		{
			*forward = object->forward;
		}
		return;
	}

	s_forward_object *parent = headers[object->parent_index & 0xffff].object;
	transform4x3f *matrix = (transform4x3f *)((byte *)parent + parent->nodes_offset + object->parent_node * 0x34);

	if (forward)
	{
		real i = object->forward.i;
		real j = object->forward.j;
		real k = object->forward.k;

		forward->i = matrix->rotation.up.i * k + matrix->rotation.left.i * j + matrix->rotation.forward.i * i;
		forward->j = matrix->rotation.up.j * k + matrix->rotation.left.j * j + matrix->rotation.forward.j * i;
		forward->k = matrix->rotation.up.k * k + matrix->rotation.left.k * j + matrix->rotation.forward.k * i;
	}
}
