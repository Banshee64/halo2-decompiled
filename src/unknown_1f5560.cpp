// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F5560.CPP: small accessors of an actor's movement state */

#include "cseries.h"
#include "globals.h"
#include "actor_moving.h"
#include "unit_requests.h"
#include "props.h"
#include "real_math.h"
#include <string.h>
#include <math.h>

static inline void vector3d_set(real_vector3d *vector, real i, real j, real k)
{
	vector->i = i;
	vector->j = j;
	vector->k = k;
}

/* takes the pending facing (+0x622) unless the actor rides a vehicle */
// @retail 0x1f5560
bool function_1f5560(long actor_index, real_vector3d *facing)
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
	actor->unknown304 = g_510c54->ticks_per_second * 5;
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
			request.type19.has_target = true;
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
	real_vector3d forward;
	byte unknown07c[0xaa - 0x7c];
	byte type;
	byte unknown0ab[0x344 - 0xab];
	short unknown344;
	short state_offset;
	byte unknown348[0x3dc - 0x348];
	byte movement_type;
	byte unknown3dd[0x3ec - 0x3dd];
	real_point3d unknown3ec;
};

struct s_heading_unit_header
{
	byte unknown00[8];
	s_heading_unit *object;
};

extern real_vector3d *g_4687bc;
real function_30bf0(real_vector3d *v);
bool function_10f630(long object_index, long *first, long *second);
real_point3d *function_210850(s_node_point const *point, real_point3d *out);

static inline short heading_unit_get_state(s_heading_unit *unit)
{
	return *(short *)((byte *)unit + unit->state_offset + 0x36);
}

/* where the actor's unit (a biped) is heading: backwards out of its current
   animation, or toward the actor's target point */
// @retail 0x1f55e0
bool function_1f55e0(long actor_index, real_point3d *point, real_vector3d *direction)
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
				real_point3d *position = &unit->unknown3ec;

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
				real_point3d target;

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
bool function_1f4f40(long actor_index, real_vector3d const *facing, short unknown, s_node_point const *point, bool face_prop)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	if (!function_110ab0(actor->unit_index))
	{
		real_point3d target;
		real_vector3d offset;

		function_210850(point, &target);
		vector3d_from_points3d(&target, &actor->position, &offset);
		if (!(magnitude_squared3d(&offset) > 0.5f))
		{
			if (face_prop)
			{
				prop_state *state = prop_state_get(prop_ref_get(actor->prop_index));
				real_vector3d direction;

				direction.i = state->position.x - actor->position.x;
				direction.j = state->position.y - actor->position.y;
				direction.k = 0.0f;
				if (!(function_30bf0(&direction) > 0.0f) || dot_product3d(facing, &direction) < 0.70710677f)
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

bool function_2105b0(short output_index, real_vector3d const *vector, real_vector3d *out);

static inline real heading_distance3d(real_point3d const *a, real_point3d const *b)
{
	real_vector3d v;

	v.i = b->x - a->x;
	v.j = b->y - a->y;
	v.k = b->z - a->z;
	return (real)sqrt(v.k * v.k + v.j * v.j + v.i * v.i);
}

/* while the actor's target point (+0x638) is fresh: faces it, and once
   within half a world unit of it asks the unit to stop there (request 0x2d) */
// @retail 0x1f58c0
bool function_1f58c0(long actor_index, real_vector3d *facing, short *unknown)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;

	if (!function_110ab0(actor->unit_index) && g_510c54->game_time < actor->unknown634)
	{
		real_point3d target;
		real_vector3d direction;

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
