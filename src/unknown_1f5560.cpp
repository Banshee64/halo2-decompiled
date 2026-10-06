// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F5560.CPP: small accessors of an actor's movement state */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1e3920.h"
#include "slot_handler.h"
#include "unknown_2626b0.h"
#include "unit_requests.h"
#include "props.h"
#include "unknown_0259d0.h"
#include <string.h>
#include <math.h>

long function_1e4a50(long index);
real function_26f290(short type);

struct s_movement_speed_definition
{
	byte unknown00[0x1a];
	short type;
};

// @retail 0x1f8050
real function_1f8050(long actor_index, bool *fast)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	s_moving_object *object = moving_object_get(actor->unit_index);
	s_movement_speed_definition *definition = (s_movement_speed_definition *)function_1e4a50(actor->tag_index);
	real result = 1.5f;
	if (fast)
		*fast = false;
	if (definition && definition->type > 0)
	{
		short type = definition->type;
		result = function_26f290((type < 6 ? type : 6) - 1);
		if (fast)
			*fast = type >= 7;
	}
	else if (*(byte *)((byte *)object + 0xaa) == 0)
	{
		result = *(real *)(g_4e3b44[object->tag_index & 0xffff].bytes + 0x1f8);
	}
	return result;
}

static inline void vector3d_set(vector3f *vector, real i, real j, real k)
{
	vector->i = i;
	vector->j = j;
	vector->k = k;
}

/* takes the pending facing (+0x622) unless the actor rides a vehicle */
// @retail 0x1f5560
bool function_1f5560(long actor_index, vector3f *facing)
{
	s_actor_moving *actor = actor_moving_get(actor_index);

	if (actor->unknown26c == NONE)
	{
		if (actor->unknown622)
			vector3d_set(facing, actor->unknown624 * actor->unknown62c, actor->unknown628 * actor->unknown62c, actor->unknown630);
		actor->unknown622 = false;
	}
	else
	{
		actor->unknown622 = false;
	}
	return true;
}

// @retail 0x1f9450
void function_1f9450(long actor_index, long value)
{
	s_actor_moving *actor = actor_moving_get(actor_index);

	actor->unknown300 = value;
	actor->unknown304 = g_510c54->field_2_3 * 5;
}

/* asks the actor's unit (unless it rides a vehicle) to play an animation,
   request 0x19, aimed at a target when one is given */
// @retail 0x1f57f0
bool function_1f57f0(long actor_index, long animation, long const *target)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	if (actor->unknown26c == NONE)
	{
		s_unit_request request;

		memset(&request, 0, sizeof(request));
		request.type = 0x19;
		request.type19.animation = animation;
		if (target)
		{
			request.type19.target[0] = target[0];
			request.type19.target[1] = target[1];
			request.type19.field_x4d3867 = true;
		}

		switch (animation)
		{
		case 0x9000011:
		case 0x9000012:
		case 0xa000010:
		case 0xa000013:
		case 0xd00002b:
		case 0xe00002a:
			request.type19.mode = 1;
			break;
		default:
			request.type19.mode = 2;
			break;
		}

		result = function_e6900(actor->unit_index, &request);
	}

	return result;
}
/* the parts of the actor's unit that say where it is heading */
struct s_heading_unit
{
	byte unknown000[0x70];
	vector3f forward;
	byte unknown07c[0xaa - 0x7c];
	byte type;
	byte unknown0ab[0x344 - 0xab];
	short unknown344;
	short state_offset;
	byte unknown348[0x3dc - 0x348];
	byte movement_type;
	byte unknown3dd[0x3ec - 0x3dd];
	point3f unknown3ec;
};

struct s_heading_unit_header
{
	byte unknown00[8];
	s_heading_unit *object;
};

extern vector3f *g_4687bc;
real function_30bf0(vector3f *v);
bool function_10f630(long object_index, long *first, long *second);
point3f *function_210850(s_type_c3b527 const *point, point3f *out);

static inline short heading_unit_get_state(s_heading_unit *unit)
{
	return *(short *)((byte *)unit + unit->state_offset + 0x36);
}

/* where the actor's unit (a biped) is heading: backwards out of its current
   animation, or toward the actor's target point */
// @retail 0x1f55e0
bool function_1f55e0(long actor_index, point3f *point, vector3f *direction)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	s_heading_unit *unit = ((s_heading_unit_header *)g_4e0300->data)[actor->unit_index & 0xffff].object;
	volatile bool result = false;

	if (unit->type == 0)
	{
		short state = heading_unit_get_state(unit);

		if (state == 6 || state == 7)
		{
			if (unit->movement_type == 4)
			{
				point3f *position = &unit->unknown3ec;

				if (position)
				{
					*point = *position;
					*direction = *g_4687bc;
				}
			}
		}
		else
		{
			long first;
			long second;

			function_10f630(actor->unit_index, &first, &second);
			if (unit->movement_type == 5 || state == 5 || first == 0x50000cb)
			{
				direction->i = unit->forward.i * -1.0f;
				direction->j = unit->forward.j * -1.0f;
				direction->k = unit->forward.k * -1.0f;
				result = true;
				return result;
			}
			if (actor->unknown264 && g_510c54->game_time < actor->unknown634)
			{
				point3f target;

				function_210850(&actor->unknown638, &target);
				direction->i = target.x - actor->position.x;
				direction->j = target.y - actor->position.y;
				direction->k = target.z - actor->position.z;
				if (function_30bf0(direction) > 0.0f)
				{
					result = true;
					return result;
				}
			}
		}
	}

	return result;
}
bool __stdcall function_110ab0(long unit_index);

/* asks the actor's unit to move to the point facing the given way (request
   0x25), once it is within half a world unit of it; when asked, only while
   it faces within 45 degrees of its prop */
// @retail 0x1f4f40
bool function_1f4f40(long actor_index, vector3f const *facing, short unknown, s_type_c3b527 const *point, bool face_prop)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	if (!function_110ab0(actor->unit_index))
	{
		point3f target;
		vector3f offset;

		function_210850(point, &target);
		vector3d_from_points3d(&target, &actor->position, &offset);
		if (!(length_sq3f(&offset) > 0.5f))
		{
			if (face_prop)
			{
				s_type_5cfb45 *state = function_25d690(prop_ref_get(actor->prop_index));
				vector3f direction;

				direction.i = state->position.x - actor->position.x;
				direction.j = state->position.y - actor->position.y;
				direction.k = 0.0f;
				if (!(function_30bf0(&direction) > 0.0f) || dot3f(facing, &direction) < 0.70710677f)
					return result;
			}

			{
				s_unit_request request;

				request.type = 0x25;
				request.type25.point = target;
				request.type25.facing = *facing;
				request.type25.unknown1c = unknown;
				result = function_e6900(actor->unit_index, &request);
			}
		}
	}

	return result;
}

bool function_2105b0(short output_index, vector3f const *vector, vector3f *out);

static inline real heading_distance3d(point3f const *a, point3f const *b)
{
	vector3f v;

	v.i = b->x - a->x;
	v.j = b->y - a->y;
	v.k = b->z - a->z;
	return (real)sqrt(v.k * v.k + v.j * v.j + v.i * v.i);
}

/* while the actor's target point (+0x638) is fresh: faces it, and once
   within half a world unit of it asks the unit to stop there (request 0x2d) */
// @retail 0x1f58c0
bool function_1f58c0(long actor_index, vector3f *facing, short *unknown)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	if (!function_110ab0(actor->unit_index) && g_510c54->game_time < actor->unknown634)
	{
		point3f target;
		vector3f direction;

		result = true;
		function_210850(&actor->unknown638, &target);
		vector3d_from_points3d(&actor->position, &target, &direction);
		if (function_30bf0(&direction) > 0.0f)
		{
			*facing = direction;
			*unknown = 0;
			actor->unknown6d1 = false;
		}

		if (heading_distance3d(&actor->position, &target) < 0.5f)
		{
			s_unit_request request;

			request.type = 0x2d;
			request.type2d.unknown4 = 2;
			request.type2d.point = target;
			if (!function_2105b0(actor->unknown638.output_index, &actor->unknown648, &request.type2d.vector))
				request.type2d.vector = actor->unknown648;
			if (function_e6900(actor->unit_index, &request))
				actor->unknown634 = NONE;
		}
	}

	return result;
}

bool function_e7020(long unit_index, bool *alternate, long name);
bool function_10f340(long unit_index, long mode, long set);
bool function_10f9b0(long unit_index, long mode, long set, long lookup_flags, transform4x3f *matrix, bool any_weapon);

// @retail 0x1f57a0
bool function_1f57a0(long actor_index, long name)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;
	/* Retail keeps the animation name in its argument slot. */
	long const *name_reference = &name;

	if (actor->unknown26c == NONE)
	{
		bool alternate;
		result = function_e7020(actor->unit_index, &alternate, *name_reference);
	}
	return result;
}

// @retail 0x1f5a60
real function_1f5a60(long unit_index)
{
	real result = 0.92f;
	transform4x3f matrix;

	if (function_10f9b0(unit_index, 0x50000cb, 0x60000cd, 3, &matrix, false))
		result = 0.0f - matrix.position.z;
	return result;
}

// @retail 0x1f5ef0
bool function_1f5ef0(long actor_index, long mode)
{
	bool result = false;
	s_actor_moving *actor = actor_moving_get(actor_index);

	if (actor->unit_index != NONE)
		result = function_10f340(actor->unit_index, mode, 0x400000c);
	return result;
}

/* Chooses the unit animation mode from the actor's requested mode and state. */
// @retail 0x1f5f30
long function_1f5f30(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	long result;

	if (actor->unknown450 != NONE)
		result = actor->unknown450;
	else
	{
		result = 0x6000086;
		if (actor->unknown44b[1])
			result = 0x7000039;
		else if (actor->unknown44b[0])
			result = 0x4000089;
		else if (actor->unknown225 &&
			(*(long *)((byte *)actor + 0x7fc) == 0x700002c || function_1f5ef0(actor_index, 0x700002c)))
			result = 0x700002c;
		else
		{
			switch (actor->unknown084)
			{
			case 1: result = 0x6000084; break;
			case 2: break;
			case 3: result = 0x6000085; break;
			case 4: result = 0x6000086; break;
			case 5: result = 0x4000089; break;
			}
		}
	}
	if (result == 0x4000089 && !actor->unknown5d0 && actor->unknown024 != 1 && team_is_enemy(actor->unknown024, 1))
		result = 0x6000086;
	return result;
}

struct s_actor_move_request
{
	bool active;
	byte unknown01[3];
	s_path_point target;
	long target_index;
	real distance;
	s_path_point start;
	bool flag2c;
	bool flag2d;
	bool flag2e;
	byte unknown2f;
	short mode;
	short value32;
	long next_index;
	short value38;
	byte unknown3a[2];
	s_path_point next_target;
	byte unknown4c[0xa0 - 0x4c];
	long valuea0;
	short valuea4;
	byte unknowna6[0xc4 - 0xa6];
};

struct s_actor_move_target
{
	s_path_point point;
	long unknown10;
	long index;
};

/* Seeds a movement request from an actor location and a valid target. */
// @retail 0x1f9490
bool function_1f9490(long actor_index, s_reference reference, s_actor_move_request *request)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	s_actor_move_target *target = (s_actor_move_target *)function_262b40(reference);
	bool result = false;

	if (target)
	{
		function_26c180(actor_index);
		memset(request, 0, sizeof(*request));
		request->start = actor->location;
		request->target = target->point;
		request->target_index = target->index;
		request->distance = 0.0f;
		request->flag2c = true;
		request->flag2d = true;
		request->flag2e = false;
		request->value32 = 0;
		request->value38 = NONE;
		request->next_target = target->point;
		request->next_index = target->index;
		request->mode = 6;
		request->valuea0 = NONE;
		request->valuea4 = NONE;
		request->active = true;
		result = true;
	}
	return result;
}

// @retail 0x1f9760
bool function_1f9760(long actor_index, long object_index, point3f const *point, real distance,
	bool *last_out, bool *direction_valid_out, vector3f *direction_out)
{
	s_actor_view *actor = actor_get(actor_index);
	byte *definition = g_4e3b44[moving_object_get(object_index)->tag_index & 0xffff].bytes;
	bool reached = false;
	bool last = actor->unknown53a + 1 >= actor->unknown539;
	bool direction_valid = false;
	vector3f direction;
	if (actor->unknown456)
	{
		direction_valid = false;
		last = true;
	}
	else if (!last)
	{
		s_type_c3b527 *current = &actor->unknown53c[actor->unknown53a].point;
		s_type_c3b527 *next = &actor->unknown53c[actor->unknown53a + 1].point;
		function_210be0(current, next, &direction);
		if (function_30bf0(&direction) > 0.0001f)
		{
			direction_valid = true;
			s_type_c3b527 *previous = actor->unknown53a > 0 ? &actor->unknown53c[actor->unknown53a - 1].point : (s_type_c3b527 *)actor->unknown528;
			vector3f approach;
			function_210be0(previous, current, &approach);
			if (function_30bf0(&approach) > 0.0f)
			{
				real remaining = dot3f((vector3f *)&current->point, &approach) - dot3f((vector3f const *)point, &approach);
				if (*(real *)(definition + 4) * 1.2f > remaining)
					reached = true;
			}
		}
	}
	else
	{
		if (actor->unknown4d5 || actor->unknown4d4)
		{
			direction = *(vector3f *)((byte *)actor + 0x4d8);
			if (function_30bf0(&direction) != 0.0f)
				direction_valid = true;
		}
		if (function_1e3920(actor_index) > distance)
			reached = true;
		else if (direction_valid && actor->unknown4d4)
		{
			s_type_c3b527 *current = &actor->unknown53c[actor->unknown53a].point;
			real remaining = dot3f((vector3f *)&current->point, &direction) - dot3f((vector3f const *)point, &direction);
			if (*(real *)(definition + 4) * 1.2f > remaining)
				reached = true;
		}
	}
	*last_out = last;
	if (last && !actor->unknown4d5)
		*direction_valid_out = false;
	else
	{
		*direction_valid_out = direction_valid;
		*direction_out = direction;
	}
	return reached;
}

bool function_fa1a0(real speed, real gravity_scale, point3f const *origin, point3f const *target,
	real *minimum_speed, real const *time_scale, real const *forced_speed, bool high_arc, vector3f *direction,
	real *speed_out, real *time_out, real *distance, real *vertical_speed, real *horizontal_speed);

// @retail 0x1f8100
bool function_1f8100(long actor_index, point3f const *origin, point3f const *target)
{
	bool fast;
	real speed = function_1f8050(actor_index, &fast);
	if (fast)
		return true;
	vector3f direction;
	return function_fa1a0(speed, 1.0f, origin, target, NULL, NULL, NULL, false, &direction,
		NULL, NULL, NULL, NULL, NULL);
}
