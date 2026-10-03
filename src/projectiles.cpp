// @flags /O2 /Ob1 /arch:SSE /Gr
/* PROJECTILES.CPP: projectiles

The functions follow projectiles.obj in Bungie's May 2003 builds
(halo-symbol-atlas): the projectile object type's callbacks (its
definition at 0x467f28), and its flight, collision, attachment and
detonation. */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "real_math.h"
#include "object_markers.h"
#include "object_iterator.h"
#include "effects.h"
#include "sound_sources.h"
#include <math.h>
#include <string.h>

#ifndef PIN
#define PIN(n,floor,ceiling) ((n)<(floor) ? (floor) : ((n)>(ceiling)?(ceiling):(n)))
#endif

/* a projectile's target: an object, and the model node aimed at */
struct s_projectile_target
{
	long object_index;
	short node_index;
	byte unknown06[2];
};

/* the projectile object (the object fields, then the projectile's own from
   +0x12c) */
struct s_projectile
{
	long tag_index;
	struct
	{
		dword unknown0 : 3;
		dword unknown3 : 1;
		dword : 28;
	} object_flags;
	byte unknown08[0x88 - 0x8];
	real_vector3d linear_velocity;
	real_vector3d angular_velocity;
	byte unknowna0[0xc1 - 0xa0];
	byte unknownc1;
	byte unknownc2[0x11c - 0xc2];
	short attachment_states_size;
	short attachment_states_offset;
	byte unknown120[0x12c - 0x120];
	struct
	{
		dword rotating : 1;
		dword unknown1 : 1;
		dword unknown2 : 1;
		dword unknown3 : 1;
		dword unknown4 : 4;
		dword unknown8 : 1;
		dword unknown9 : 5;
		dword unknown14 : 1;
		dword unknown15 : 1;
		dword : 16;
	} flags;
	short action;
	byte unknown132[0x144 - 0x132];
	s_projectile_target target;
	long attachment_index;
	byte unknown150[0x158 - 0x150];
	real unknown158;
	byte unknown15c[4];
	real unknown160;
	byte unknown164[0x168 - 0x164];
	real distance_traveled;
	byte unknown16c[0x170 - 0x16c];
	real speed;
	real_vector3d rotation_axis;
	real rotation_sine;
	real rotation_cosine;
};

/* an object's attachment state (8 bytes) */
struct s_projectile_attachment_state
{
	byte type;
	byte unknown01[3];
	long value;
};

/* the projectile definition tag (proj) fields read here */
struct s_projectile_definition
{
	byte unknown00[0xbc];
	union
	{
		dword flags;
		struct
		{
			dword unknown0 : 5;
			dword drifts : 1;
			dword unknown6 : 4;
			dword difficulty_scaled : 1;
			dword : 21;
		} flag_bits;
	};
	byte unknownc0[0xe0 - 0xc0];
	real maximum_range;
	byte unknowne4[0x164 - 0xe4];
	real gravity_scale;
	byte unknown168[4];
	real initial_speed;
	byte unknown170[0x178 - 0x170];
	real final_speed;
	real unknown17c;
	real unknown180;
	byte unknown184[0x18c - 0x184];
	real unknown18c;
	byte unknown190[4];
	real unknown194;
	byte unknown198[4];
	long material_response_count;
	byte *material_responses;
};

struct s_projectile_header
{
	byte unknown00[8];
	s_projectile *object;
};

#define PROJECTILE_GET(index) (((s_projectile_header *)g_4e0300->data)[(index) & 0xffff].object)
#define PROJECTILE_DEFINITION_GET(tag) ((s_projectile_definition *)g_4e3b44[(tag) & 0xffff].bytes)

struct s_unknown_1eb550;
extern s_unknown_1eb550 *g_51e9c4;
real function_1e96a0(short column, short row);
void function_b9a90(long object_index);
void function_f8eb0(long projectile_index, real_vector3d const *displacement);
bool function_109a00(long projectile_index, real_vector3d *delta, bool unknown, real_vector3d *velocity);

/* the difficulty scale of a projectile row (row 10) */
PRIVATE inline real projectile_difficulty_scale()
{
	return function_1e96a0(g_4e6948->state == 1 ? g_4e6948->difficulty : 1, 10);
}

/* a vector's length, and the vector made unit length (left alone when it's
   too short) */
PRIVATE inline real projectile_normalize(real_vector3d *v)
{
	real magnitude = (real)sqrt(v->k * v->k + v->j * v->j + v->i * v->i);

	if (!(fabs(magnitude) < 0.0001f))
	{
		real inverse = 1.0f / magnitude;

		v->i = inverse * v->i;
		v->j = v->j * inverse;
		v->k = v->k * inverse;
		return magnitude;
	}
	return 0.0f;
}

/* sets the object and node a projectile is guided to */
// @retail 0xf8db0
void projectile_set_target(long projectile_index, s_projectile_target const *target)
{
	PROJECTILE_GET(projectile_index)->target = *target;
}

// @retail 0xfa170
real projectile_get_ballistic_acceleration(long definition_index)
{
	return 0.0f - PROJECTILE_DEFINITION_GET(definition_index)->gravity_scale * *(real *)g_51e9c4;
}

/* forgets a target that is going away */
// @retail 0xfa9f0
void __stdcall projectile_clear_target(long projectile_index, long object_index)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);

	if (projectile->target.object_index == object_index)
	{
		projectile->target.object_index = NONE;
		projectile->target.node_index = NONE;
	}
}

// @retail 0xfaa30
void projectile_set_action(long projectile_index, short action)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);

	if (action > projectile->action)
		projectile->action = action;
}

/* a projectile's speed: its definition's final speed once it has one */
// @retail 0xfbfd0
void function_fbfd0(long projectile_index)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(projectile->tag_index);

	projectile->speed = TEST_FIELD_BIT(projectile->object_flags.unknown3) ? definition->final_speed :
		definition->initial_speed;
}

/* the object a projectile is attached to (its attachment state of type 3) */
// @retail 0xfd410
long function_fd410(long projectile_index)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	long attachment_index = projectile->attachment_index;
	long result = NONE;

	if (attachment_index != NONE)
	{
		s_projectile_attachment_state *states =
			(s_projectile_attachment_state *)((byte *)projectile + projectile->attachment_states_offset);

		if (attachment_index >= 0 &&
			attachment_index < (long)(projectile->attachment_states_size / sizeof(s_projectile_attachment_state)) &&
			states[attachment_index].type == 3)
		{
			result = states[attachment_index].value;
		}
	}
	return result;
}

// @retail 0xfd460
void function_fd460(long projectile_index, long value)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	long attachment_index = projectile->attachment_index;

	if (attachment_index != NONE)
	{
		s_projectile_attachment_state *states =
			(s_projectile_attachment_state *)((byte *)projectile + projectile->attachment_states_offset);

		if (attachment_index >= 0 &&
			attachment_index < (long)(projectile->attachment_states_size / sizeof(s_projectile_attachment_state)))
		{
			states[attachment_index].type = 3;
			states[projectile->attachment_index].value = value;
		}
	}
}

/* the difficulty scale of a projectile's speed */
// @retail 0xfd8b0
real function_fd8b0(long definition_index, long owner_index)
{
	if (TEST_FIELD_BIT(PROJECTILE_DEFINITION_GET(definition_index)->flag_bits.difficulty_scaled))
	{
		if (owner_index == NONE)
			return projectile_difficulty_scale();
		return 1.5f;
	}
	return 1.0f;
}

// @retail 0xfa7b0
real projectile_estimate_time_to_target(long definition_index, real distance)
{
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(definition_index);
	real scale = TEST_FIELD_BIT(definition->flag_bits.difficulty_scaled) ? projectile_difficulty_scale() : 1.0f;
	real speed = definition->unknown17c * scale;

	if (speed > 0.0f)
		return distance / speed;
	return 0.0f;
}

// @retail 0xfd3c0
bool __stdcall function_fd3c0(long projectile_index)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);

	projectile->unknown160 = 1.0f;
	projectile->unknown158 = 1.0f;
	*(dword *)&projectile->flags &= ~8;
	function_b9a90(projectile_index);
	return true;
}

/* an iterator over the projectiles (c_object_iterator<projectile_datum>):
   the current one, then the object iterator */
struct s_projectile_iterator
{
	s_projectile *datum;
	s_object_iterator iterator;
};

/* the first projectile there is */
// @retail 0xfa9a0
bool __stdcall function_fa9a0(long *value)
{
	s_projectile_iterator iterator;

	function_bae80(&iterator.iterator, 0x20, 0);
	if (iterator.datum = (s_projectile *)function_baeb0(&iterator.iterator))
	{
		*value = iterator.iterator.object_index;
		return true;
	}
	return false;
}

/* the direction, distance and time to a target in a straight line */
// @retail 0xfa580
bool projectile_aim_linear(real speed, real_point3d const *origin, real_point3d const *target, real_vector3d *direction,
	real *distance, real *speed_out, real *time)
{
	real_vector3d vector;

	vector.i = target->x - origin->x;
	vector.j = target->y - origin->y;
	vector.k = target->z - origin->z;

	real length = projectile_normalize(&vector);
	real seconds = 0.0f;

	if (length == 0.0f)
		vector = *g_4687b0;
	if (speed > 0.0001f)
		seconds = length / speed;
	*direction = vector;
	if (distance)
		*distance = length;
	if (speed_out)
		*speed_out = speed;
	if (time)
		*time = seconds;
	return true;
}

/* the rotation a spinning projectile makes each tick */
// @retail 0xfbd40
void projectile_adjust_for_angular_velocity_change(long projectile_index)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	real_vector3d axis = projectile->angular_velocity;
	real speed = projectile_normalize(&axis);

	if (speed > 0.0001f)
	{
		real angle = speed * g_510c54->rate;

		projectile->flags.rotating = true;
		projectile->rotation_axis = axis;
		projectile->rotation_sine = (real)sin(angle);
		projectile->rotation_cosine = (real)cos(angle);
	}
	else
	{
		projectile->flags.rotating = false;
		projectile->rotation_sine = 0.0f;
		projectile->rotation_cosine = 1.0f;
	}
}

/* a projectile's exported function values */
// @retail 0xfbe70
bool __stdcall function_fbe70(long projectile_index, long name, real *value, bool *active)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(projectile->tag_index);
	bool result = true;
	real output;

	switch (name)
	{
	case 0xe000562:
		output = projectile->unknown158;
		break;
	case 0x6000564:
		output = TEST_FIELD_BIT(projectile->flags.unknown1) ? 1.0f : 0.0f;
		break;
	case 0xf000563:
		if (definition->maximum_range != 0.0f)
			output = projectile->distance_traveled / definition->maximum_range;
		else
			output = 0.0f;
		break;
	case 0x12000565:
		if (TEST_FIELD_BIT(projectile->flags.unknown8))
			output = 1.0f;
		else if (definition->unknown17c == 0.0f || definition->unknown180 == 0.0f)
			output = 1.0f;
		else
			output = PIN((projectile->distance_traveled - definition->unknown18c) * definition->unknown194, 0.0f, 1.0f);
		break;
	case 0x11000566:
		output = TEST_FIELD_BIT(projectile->flags.unknown3) ? 1.0f : 0.0f;
		break;
	default:
		result = false;
		break;
	}
	if (result)
	{
		*value = output;
		*active = output > 0.0f;
	}
	return result;
}

/* the point a projectile's target is aimed at: its node's marker, or the
   object's default marker */
// @retail 0xf86f0
void function_f86f0(s_projectile_target const *target, real_point3d *point)
{
	long object_index = target->object_index;
	s_projectile *object = PROJECTILE_GET(object_index);

	if (target->node_index == NONE)
	{
		s_object_marker marker;

		function_b8d30(object_index, 0x40000bd, &marker, 1, false);
		*point = marker.matrix.position;
		return;
	}

	byte *model = g_4e3b44[*(long *)(g_4e3b44[object->tag_index & 0xffff].bytes + 0x38) & 0xffff].bytes;
	long marker_name = *(long *)(*(byte **)(model + 0x6c) + target->node_index * 0x1c);
	s_object_marker markers[2];

	if (function_b8d30(object_index, marker_name, markers, 2, false) > 0)
	{
		*point = markers[0].matrix.position;
		return;
	}

	s_object_marker marker;

	function_b8d30(object_index, 0x40000bd, &marker, 1, false);
	*point = marker.matrix.position;
}

/* moves a projectile that drifts with its velocity */
// @retail 0xfa100
void function_fa100(long projectile_index)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);

	if (TEST_FIELD_BIT(PROJECTILE_DEFINITION_GET(projectile->tag_index)->flag_bits.drifts))
	{
		real_vector3d displacement = projectile->linear_velocity;

		function_f8eb0(projectile_index, &displacement);
	}
}

/* the projectile type's per-tick update: its movement, plus what moved it */
// @retail 0xf8de0
bool __stdcall function_f8de0(long projectile_index)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	real_vector3d *velocity = &projectile->linear_velocity;
	real_vector3d displacement = *velocity;
	real_vector3d delta;
	bool moved;

	if ((!(projectile->unknownc1 & 1) || TEST_FIELD_BIT(projectile->flags.unknown14)) &&
		function_109a00(projectile_index, &delta, TEST_FIELD_BIT(projectile->flags.unknown15), &displacement))
	{
		moved = true;
	}
	else
	{
		moved = false;
	}
	function_f8eb0(projectile_index, &displacement);
	if (moved)
	{
		velocity->i += delta.i;
		velocity->j += delta.j;
		velocity->k += delta.k;
	}
	return true;
}

/* the response of a projectile to a material, or its parents' */
struct s_projectile_material_response
{
	byte unknown00[0x10];
	short material_index;
	byte unknown12[0x58 - 0x12];
};

s_projectile_material_response g_547660;

// @retail 0xfd4c0
s_projectile_material_response *__stdcall projectile_get_material_response(s_projectile_definition const *definition,
	short material_index)
{
	long response_count = definition->material_response_count;
	s_projectile_material_response *result = NULL;

	while (material_index != NONE && material_index >= 0 &&
		material_index < *(long *)((byte *)g_4e034c + 0x150))
	{
		byte *material = *(byte **)((byte *)g_4e034c + 0x154) + material_index * 0xb4;

		if (!material)
			break;

		s_projectile_material_response *response = (s_projectile_material_response *)definition->material_responses;

		for (long i = 0; i < response_count; i++, response++)
		{
			if (response->material_index == material_index)
			{
				result = response;
				break;
			}
		}
		material_index = *(short *)(material + 8);
		if (result)
			return result;
	}
	if (!result)
		result = &g_547660;
	return result;
}

/* the projectile object type (0x467f28): its name, group tag, datum size,
   its callbacks, the base object type (0x4678e8) and itself. The four slots
   that hold 0x175f40 (an empty function folded with c_game_engine::v10) and
   the base type are left NULL. */
struct s_object_type_definition_view
{
	char const *name;
	long group_tag;
	short datum_size;
	short unknown0a;
	long unknown0c;
	void *functions[29];
	void *parent;
	void *self;
	byte unknown8c[0xc8 - 0x8c];
};

extern s_object_type_definition_view g_467f28;

s_object_type_definition_view g_467f28 =
{
	"projectile",
	'proj',
	0x1ac,
	NONE,
	NONE,
	{
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL,
		(void *)function_f8de0, NULL, NULL, (void *)function_fbe70,
		NULL, NULL, (void *)projectile_clear_target, NULL,
		NULL, (void *)function_fd3c0, NULL, NULL,
		NULL, NULL, NULL, NULL,
		NULL
	},
	NULL,
	&g_467f28
};

extern real_vector3d *g_4687bc;
extern real_vector3d *g_4687a4;
extern short g_47d8e0;
long __stdcall effect_new_from_parameters(s_effect_parameters *parameters);
void function_b9b90(long object_index, bool disable);
void function_1883e0(long tag_index, bool ignore_distance, real_point3d const *point, short element_index, long unused,
	long index, long variant, long *first_value04, long *second_value04, long *first_value, long *second_value,
	long *first_value0c, long *second_value0c);
long function_1895f0(s_sound_position const *position, real scale, long tag_index);
void function_11bed0(real_point3d const *point, s_location *location);
dword vector3d_compress(real_vector3d const *vector);
void function_b75a0(long object_index, real_point3d const *point, real_vector3d const *forward, long a, long b);
void __stdcall function_b77d0(long object_index, real_vector3d const *velocity);
void __stdcall function_b93b0(long parent_index, long object_index, long node_index);
void __stdcall function_1e2930(long object_index, long actor_index);
void function_a83e0(long object_index, long parent_index, real_point3d const *point, long node_index,
	real_vector3d const *forward);

/* a copy of the effect parameter defaults (unknown_175bd0.cpp keeps its
   own inline) */
PRIVATE inline void projectile_effect_parameters_initialize(s_effect_parameters *parameters)
{
	memset(parameters, 0, sizeof(*parameters));
	parameters->tag_index = NONE;
	parameters->unknown18 = NONE;
	parameters->object_index = NONE;
	parameters->owner.unknown4 = NONE;
	parameters->owner.unknown0 = NONE;
	parameters->owner.unknown8 = NONE;
	parameters->unknown34 = 0;
	parameters->unknown38 = 0;
	parameters->scale_a = 1.0f;
	parameters->scale_b = 1.0f;
	parameters->unknown3c = 0;
	parameters->unknown30 = 0;
	parameters->color_a = 0xff808080;
	parameters->color_b = 0xff808080;
	parameters->source = 0;
}

/* pushes a projectile, and spins it at random by the push's strength */
// @retail 0xfa820
void projectile_accelerate(long projectile_index, real_vector3d const *impulse)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);

	if (*(long *)((byte *)projectile + 0x14) == NONE)
	{
		projectile->linear_velocity.i += impulse->i;
		projectile->linear_velocity.j += impulse->j;
		projectile->linear_velocity.k += impulse->k;

		real_vector3d axis = g_4417f0[random_index(&g_4e7408->unknown0, 1026)];
		real spin = (real)sqrt(magnitude_squared3d(impulse)) * _real_random(&g_4e7408->unknown0, __FILE__, __LINE__) *
			1.5707964f;

		projectile->angular_velocity.i += spin * axis.i;
		projectile->angular_velocity.j += spin * axis.j;
		projectile->angular_velocity.k += axis.k * spin;
		projectile_adjust_for_angular_velocity_change(projectile_index);
		function_b9b90(projectile_index, false);
	}
}

/* the sound of a projectile's impact */
// @retail 0xfcdd0
void function_fcdd0(long definition_index, real_point3d const *point, real_vector3d const *forward)
{
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(definition_index);
	long sound_index = *(long *)((byte *)definition + 0x124);

	if (sound_index != NONE)
	{
		s_location location = { 0 };
		s_sound_position position = { 0 };

		function_11bed0(point, &location);
		position.position = *point;
		position.compressed_forward = vector3d_compress(forward);
		position.velocity = *g_4687a4;
		position.location = location;
		function_1895f0(&position, 1.0f, sound_index);
	}
}

/* a projectile's detonation (or impact) effect at a point, facing out of
   the surface it hit */
// @retail 0xfcbc0
void function_fcbc0(real_point3d const *point, real_vector3d const *normal, long definition_index,
	real_vector3d const *velocity, bool airburst, bool impact)
{
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(definition_index);
	s_effect_marker markers[4];
	s_effect_parameters parameters;
	long effect_index;

	markers[0].position = *point;
	markers[1].position = *point;
	markers[2].position = *point;
	markers[3].position = *point;
	markers[0].forward = *normal;
	markers[1].forward = *g_4687bc;
	markers[2].forward = *g_4687b0;
	markers[3].forward.i = 0.0f - normal->i;
	markers[3].forward.j = 0.0f - normal->j;
	markers[3].forward.k = 0.0f - normal->k;
	markers[0].name = 0x700054c;
	markers[1].name = 0x70000c0;
	markers[2].name = 0x20000ca;
	markers[3].name = 0x8000550;
	if (airburst)
		effect_index = *(long *)((byte *)definition + 0x114);
	else if (impact)
		effect_index = *(long *)((byte *)definition + 0xf4);
	else
		effect_index = *(long *)((byte *)definition + 0xfc);

	projectile_effect_parameters_initialize(&parameters);
	parameters.tag_index = effect_index;
	parameters.markers = markers;
	parameters.marker_count = 4;
	if (velocity)
		parameters.velocity = *velocity;
	effect_new_from_parameters(&parameters);
}

/* the effect values of an effects block's effect (+0x58), NONE without one */
// @retail 0xfd740
void function_fd740(real_point3d const *point, byte const *effects, long unused, long index,
	long *first_value04, long *second_value04, long *first_value, long *second_value, long *first_value0c,
	long *second_value0c)
{
	long effect_index = *(long const *)(effects + 0x58);

	if (effect_index != NONE)
	{
		function_1883e0(effect_index, false, point, g_47d8e0, unused, index, 0, first_value04, second_value04,
			first_value, second_value, first_value0c, second_value0c);
		return;
	}
	if (first_value04)
		*first_value04 = NONE;
	if (second_value04)
		*second_value04 = NONE;
	if (first_value)
		*first_value = NONE;
	if (second_value)
		*second_value = NONE;
	if (first_value0c)
		*first_value0c = NONE;
	if (second_value0c)
		*second_value0c = NONE;
}

/* the same for what a projectile hit: ignoring distance for multiplayer
   objects the network owns */
// @retail 0xfd7d0
void function_fd7d0(long object_index, real_point3d const *point, byte const *effects,
	long unused, long index, long *first_value04, long *second_value04, long *first_value, long *second_value,
	long *first_value0c, long *second_value0c)
{
	long effect_index = *(long const *)(effects + 0x58);

	if (effect_index != NONE)
	{
		bool ignore_distance = false;

		if (object_index != NONE)
		{
			s_projectile *object = PROJECTILE_GET(object_index);

			if (g_4e6948->state == 2 && g_4e6948->mode != 4 && *(long *)((byte *)object + 0xd4) != NONE)
				ignore_distance = true;
		}
		function_1883e0(effect_index, ignore_distance, point, g_47d8e0, unused, index, 0, first_value04, second_value04,
			first_value, second_value, first_value0c, second_value0c);
		return;
	}
	if (first_value04)
		*first_value04 = NONE;
	if (second_value04)
		*second_value04 = NONE;
	if (first_value)
		*first_value = NONE;
	if (second_value)
		*second_value = NONE;
	if (first_value0c)
		*first_value0c = NONE;
	if (second_value0c)
		*second_value0c = NONE;
}

/* sticks a projectile to what it hit */
// @retail 0xfd560
void function_fd560(long projectile_index, long object_index, long node_index, real_point3d const *point,
	real_vector3d const *forward)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(projectile->tag_index);

	if (object_index != NONE && (definition->flags & 8))
	{
		short attached_count = 0;

		for (long child_index = *(long *)((byte *)PROJECTILE_GET(object_index) + 0x10); child_index != NONE; )
		{
			s_projectile *child = PROJECTILE_GET(child_index);

			if (child->tag_index == projectile->tag_index && !((*(dword *)&child->flags >> 6) & 1))
			{
				child->unknown160 = 0.0f;
				child->unknown158 = 0.0f;
				attached_count++;
			}

			short maximum = *(short *)((byte *)definition + 0xe6);

			if (maximum && attached_count >= maximum)
			{
				*(dword *)&projectile->flags |= 0x80;
				break;
			}
			child_index = *(long *)((byte *)child + 0xc);
		}
	}

	function_b75a0(projectile_index, point, forward, 0, 0);
	function_b77d0(projectile_index, g_4687a4);
	*(dword *)&projectile->flags |= 8;
	function_b9b90(projectile_index, true);
	if (object_index != NONE)
	{
		function_b93b0(object_index, projectile_index, node_index);

		s_projectile *parent = PROJECTILE_GET(object_index);

		if (((1 << *((byte *)parent + 0xaa)) & 3) && *(long *)((byte *)parent + 0x12c) != NONE && g_4f55d0->active)
			function_1e2930(projectile_index, *(long *)((byte *)parent + 0x12c));
	}

	real seconds = 0.0f;

	if ((*(dword *)&projectile->flags >> 10) & 1)
		seconds = *(real *)((byte *)definition + 0x150);
	else if (definition->flags & 4)
		seconds = *(real *)((byte *)definition + 0xd8);

	real ticks = g_510c54->ticks_per_second * seconds;

	if (ticks >= 1.0f)
		*(real *)((byte *)projectile + 0x15c) = 1.0f / ticks;
	function_a83e0(projectile_index, object_index, point, node_index, forward);
}

real function_30bf0(real_vector3d *v);

/* aims a falling projectile along an arc: the slowest speed that reaches
   the target, then the time to it at the speed used (high or low arc),
   falling back to the slowest arc when the speed can't reach it */
// @retail 0xfa1a0
bool projectile_aim_ballistic(real speed, real gravity_scale, real_point3d const *origin, real_point3d const *target,
	real *minimum_speed, real const *time_scale, real const *forced_speed, bool high_arc, real_vector3d *direction,
	real *speed_out, real *time_out, real *distance, real *vertical_speed, real *horizontal_speed)
{
	real_vector3d delta;

	delta.i = target->x - origin->x;
	delta.j = target->y - origin->y;

	real horizontal_squared = delta.j * delta.j + delta.i * delta.i;
	real horizontal = (real)sqrt(horizontal_squared);
	real gravity = *(real *)g_51e9c4 * gravity_scale;
	bool result = true;

	delta.k = target->z - origin->z;
	if (!(0.0f > gravity) && gravity > 0.0f && (horizontal > 0.0001f || delta.k > 0.0001f))
	{
		real distance_squared = delta.k * delta.k + horizontal_squared;
		real a = gravity * gravity * 0.25f;
		real discriminant_base = distance_squared * a * 4.0f;
		real two_a = a * 2.0f;
		real b = gravity * delta.k;
		real root = -(real)sqrt(discriminant_base);
		real optimal_time = (real)sqrt(-1.0f / two_a * root);
		real minimum = b - root;
		real minimum_speed_value = 0.0f > minimum ? 0.0f : (real)sqrt(minimum);
		real chosen_speed;
		real time;
		bool found = false;

		if (minimum_speed)
			*minimum_speed = minimum_speed_value;
		if (forced_speed)
		{
			chosen_speed = *forced_speed;
		}
		else
		{
			chosen_speed = speed;
			if (time_scale && *time_scale > 0.0f)
			{
				real t = PIN(*time_scale * optimal_time, 0.001f, optimal_time);
				real x = -(distance_squared / (t * t) + t * t * a);
				real needed = (real)sqrt(b - x);

				if (speed > needed)
					chosen_speed = needed;
			}
		}

		if (chosen_speed >= minimum_speed_value)
		{
			real c = b - chosen_speed * chosen_speed;
			real discriminant = c * c - discriminant_base;

			if (0.0f > c && discriminant >= 0.0f)
			{
				real t2 = ((real)sqrt(discriminant) * (high_arc ? 1 : -1) - c) / two_a;

				if (t2 > 0.0f)
				{
					time = (real)sqrt(t2);
					found = true;
				}
			}
		}
		if (!found)
		{
			time = optimal_time;
			result = false;
			chosen_speed = minimum_speed_value;
		}

		real inverse_time = 1.0f / time;
		real_vector3d velocity;

		velocity.i = inverse_time * delta.i;
		velocity.j = inverse_time * delta.j;

		real horizontal_speed_value = (real)sqrt(velocity.j * velocity.j + velocity.i * velocity.i);

		velocity.k = inverse_time * delta.k + time * gravity * 0.5f;

		real vertical = velocity.k;
		real path_distance = time * chosen_speed;

		if (function_30bf0(&velocity) == 0.0f)
		{
			result = false;
			velocity = delta;
			if (function_30bf0(&velocity) == 0.0f)
				velocity = *g_4687b0;
		}
		*direction = velocity;
		if (distance)
			*distance = path_distance;
		if (speed_out)
			*speed_out = chosen_speed;
		if (vertical_speed)
			*vertical_speed = vertical;
		if (horizontal_speed)
			*horizontal_speed = horizontal_speed_value;
		if (time_out)
			*time_out = time;
		return result;
	}
	return projectile_aim_linear(speed, origin, target, direction, distance, speed_out, time_out);
}

/* aims a projectile of a definition at a target: along an arc for
   projectiles that fall, in a line otherwise */
// @retail 0xfa6a0
bool projectile_aim(long definition_index, real const *speed_override, real_point3d const *origin, real_point3d const *target,
	real *unknown2, real const *unknown3, real const *unknown4, bool unknown5, real_vector3d *direction, real *speed_out,
	real *time, real *distance, bool *linear)
{
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(definition_index);
	real speed;
	bool result;

	if (speed_override)
		speed = *speed_override;
	else
		speed = definition->unknown17c *
			(TEST_FIELD_BIT(definition->flag_bits.difficulty_scaled) ? projectile_difficulty_scale() : 1.0f);

	if ((definition->flags & 2) && definition->gravity_scale > 0.0f)
	{
		result = projectile_aim_ballistic(speed, definition->gravity_scale, origin, target, unknown2, unknown3, unknown4,
			unknown5, direction, speed_out, time, distance, NULL, NULL);
		if (linear)
			*linear = false;
		return result;
	}
	result = projectile_aim_linear(speed, origin, target, direction, distance, speed_out, time);
	if (linear)
		*linear = true;
	return result;
}

struct s_collision_result_1697c0
{
	byte unknown00[8];
	real_point3d point;
	byte unknown14[0x24 - 0x14];
	short unknown24;
	byte unknown26[0x4c - 0x26];
};

bool __stdcall function_1697c0(long flags, real_point3d const *point, real_vector3d const *vector,
	long ignore_object_index, long ignore_unit_index, s_collision_result_1697c0 *result);
real function_30bf0(real_vector3d *v);
extern real_vector3d *g_4687ac;

/* whether a projectile's path to a point hits anything: the line from it,
   and for projectiles with a radius the two lines either side of it */
// @retail 0xfc030
bool projectile_collision_test_line(long projectile_index, real_point3d const *point, s_collision_result_1697c0 *collision)
{
	s_projectile *projectile = PROJECTILE_GET(projectile_index);
	s_projectile_definition *definition = PROJECTILE_DEFINITION_GET(projectile->tag_index);
	real_point3d *origin = (real_point3d *)((byte *)projectile + 0x64);
	long ignore_object_index = *(long *)((byte *)projectile + 0x140);
	real_vector3d vector;

	vector.i = point->x - origin->x;
	vector.j = point->y - origin->y;
	vector.k = point->z - origin->z;
	if (function_1697c0(0x2480000f, origin, &vector, ignore_object_index, projectile_index, collision))
		return true;

	real radius = *(real *)((byte *)definition + 0xc8);

	if (!(0.0001f > radius) && *(long *)((byte *)projectile + 0xc4) != NONE)
	{
		real_vector3d side;
		real_vector3d const *up = g_4687b0;
		real_vector3d delta;

		delta.i = point->x - origin->x;
		delta.j = point->y - origin->y;
		delta.k = point->z - origin->z;
		side.i = delta.k * up->j - up->k * delta.j;
		side.j = up->k * delta.i - up->i * delta.k;
		side.k = up->i * delta.j - delta.i * up->j;
		if (function_30bf0(&side) == 0.0f)
			side = *g_4687ac;

		real_point3d start;
		real_vector3d offset_vector;

		start.x = side.i * radius + origin->x;
		start.y = side.j * radius + origin->y;
		start.z = side.k * radius + origin->z;
		offset_vector.i = side.i * radius + point->x - start.x;
		offset_vector.j = side.j * radius + point->y - start.y;
		offset_vector.k = side.k * radius + point->z - start.z;
		if (function_1697c0(0x4800008, &start, &offset_vector, ignore_object_index, NONE, collision))
			return true;

		start.x = (0.0f - radius) * side.i + origin->x;
		start.y = side.j * (0.0f - radius) + origin->y;
		start.z = side.k * (0.0f - radius) + origin->z;
		offset_vector.i = (0.0f - radius) * side.i + point->x - start.x;
		offset_vector.j = side.j * (0.0f - radius) + point->y - start.y;
		offset_vector.k = side.k * (0.0f - radius) + point->z - start.z;
		if (function_1697c0(0x4800008, &start, &offset_vector, ignore_object_index, NONE, collision))
			return true;
	}
	return false;
}

void function_1763a0(real_point3d const *point, real_vector3d const *direction, s_effect_marker *markers,
	real_vector3d const *normal);
long function_189fe0(s_sound_request const *request, long tag_index);

/* the effects and sounds of an effects block (a material's response) at
   a point */
// @retail 0xfcea0
void function_fcea0(long effects_index, real_point3d const *point, real_vector3d const *direction,
	real_vector3d const *velocity, long index, real_vector3d const *normal)
{
	byte *effects = g_4e3b44[effects_index & 0xffff].bytes;
	s_location location;
	long sounds[3];
	long effect_indices[3];
	long i;

	function_11bed0(point, &location);
	function_fd740(point, effects, 0xf, index, &effect_indices[0], &sounds[0], &effect_indices[1], &sounds[1],
		&effect_indices[2], &sounds[2]);
	for (i = 0; i < 3; i++)
	{
		long effect_index = effect_indices[i];

		if (effect_index != NONE)
		{
			s_effect_marker markers[6];
			s_effect_parameters parameters;

			function_1763a0(point, direction, markers, normal);
			projectile_effect_parameters_initialize(&parameters);
			parameters.markers = markers;
			parameters.tag_index = effect_index;
			parameters.marker_count = 6;
			parameters.flags = 4;
			if (velocity)
				parameters.velocity = *velocity;
			effect_new_from_parameters(&parameters);
		}
	}
	for (i = 0; i < 3; i++)
	{
		long sound_index = sounds[i];

		if (sound_index != NONE)
		{
			s_sound_position position;
			s_sound_request request;

			position.position = *point;
			position.compressed_forward = vector3d_compress(direction);
			position.velocity = *g_4687a4;
			position.location = location;
			request.location.unknown02 = 0;
			request.location.flags = 0;
			request.location.scale = 1.0f;
			request.location.spatial = position;
			request.location.audible = 1;
			request.location.requested_audible = 1;
			request.location.unknown08 = 0;
			request.platform_playback = NONE;
			request.object_index = NONE;
			request.source = NULL;
			request.marker = NULL;
			request.variant = NULL;
			function_189fe0(&request, sound_index);
		}
	}
}

/* a projectile's detonation effects and sounds at a point, attached to an
   object's node when it stuck to one */
// @retail 0xfd0e0
void function_fd0e0(long definition_index, real scale_a, real scale_b, real_vector3d const *direction,
	real_point3d const *point, real_vector3d const *normal, long index, bool attached, long object_index,
	short node_index, bool alternate)
{
	byte *definition = g_4e3b44[definition_index & 0xffff].bytes;
	long sounds[3];
	long effect_indices[3];
	s_location location;
	s_effect_marker markers[6];
	s_effect_parameters parameters;
	long i;

	effect_indices[0] = NONE;
	effect_indices[1] = NONE;
	effect_indices[2] = NONE;
	sounds[0] = NONE;
	sounds[1] = NONE;
	sounds[2] = NONE;
	function_11bed0(point, &location);
	function_1763a0(point, direction, markers, normal);
	projectile_effect_parameters_initialize(&parameters);
	if (attached)
	{
		parameters.flags = 1;
		parameters.object_index = object_index;
		parameters.unknown18 = node_index;
	}
	parameters.scale_a = scale_a;
	parameters.markers = markers;
	parameters.scale_b = scale_b;
	parameters.marker_count = 6;
	function_fd740(point, definition, 0xf, index, &effect_indices[0], &sounds[0], &effect_indices[1], &sounds[1],
		&effect_indices[2], &sounds[2]);
	if (alternate && *(long *)(definition + 0xec) != NONE)
	{
		parameters.flags |= 4;
		parameters.tag_index = *(long *)(definition + 0xec);
		effect_new_from_parameters(&parameters);
		return;
	}
	for (i = 0; i < 3; i++)
	{
		if (effect_indices[i] != NONE)
		{
			parameters.tag_index = effect_indices[i];
			effect_new_from_parameters(&parameters);
		}
	}
	parameters.flags |= 4;
	if (*(long *)(definition + 0x144) != NONE)
	{
		parameters.tag_index = *(long *)(definition + 0x144);
		effect_new_from_parameters(&parameters);
	}
	for (i = 0; i < 3; i++)
	{
		long sound_index = sounds[i];

		if (sound_index != NONE)
		{
			s_sound_position position;
			s_sound_request request;

			position.position = *point;
			position.compressed_forward = vector3d_compress(direction);
			position.velocity = *g_4687a4;
			position.location = location;
			request.location.unknown02 = 0;
			request.location.flags = 0;
			request.location.spatial = position;
			request.location.scale = 1.0f;
			request.location.audible = 1;
			request.location.requested_audible = 1;
			request.location.unknown08 = 0;
			request.object_index = NONE;
			request.platform_playback = NONE;
			request.marker = NULL;
			request.source = NULL;
			request.variant = NULL;
			function_189fe0(&request, sound_index);
		}
	}
}
