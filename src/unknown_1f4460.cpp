// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1F4460.CPP: an actor's movement goal (actor +0x4ac..+0x4e8) */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_20fe20.h"
#include "unknown_1f4460.h"
#include "real_math.h"

void function_1f86a0(long index);
bool __stdcall function_1f8a70(long actor_index, long unknown);
void __stdcall function_2628f0(long actor_index, s_reference reference);

// @retail 0x1f4460
bool function_1f4460(long actor_index, s_type_c3b527 const *point, long target_index, long unknown, bool unknown2)
{
	s_actor_view *actor = actor_get(actor_index);

	if (!actor->unknown229 && target_index == NONE)
	{
		function_1f86a0(actor_index);
		return false;
	}

	function_2628f0(actor_index, g_470fa0);
	if (actor->unknown4ac == 2 && actor->unknown4c8 == target_index &&
		function_210a30(point, &actor->unknown4b8) <= 0.01f)
	{
		if (actor->unknown040 && !actor->unknown506)
			return function_1f8a70(actor_index, 0);
		return true;
	}

	function_1f86a0(actor_index);
	actor->unknown4b0 = 0.0f;
	actor->unknown4b4 = 0.0f;
	actor->unknown4cc = 0.0f;
	actor->unknown4d0 = 0.0f;
	actor->unknown4e4 = NONE;
	actor->unknown4ac = 0;
	actor->unknown4ae = false;
	actor->unknown4d4 = false;
	actor->unknown4d5 = false;
	actor->unknown4e8 = false;
	actor->unknown4ae = unknown2;
	actor->unknown4ac = 2;
	actor->unknown4b8 = *point;
	actor->unknown4e4 = unknown;
	actor->unknown4c8 = target_index;
	if (actor->unknown229)
		actor->unknown656 = 0;

	return function_1f8a70(actor_index, 0);
}

/* the parts of a unit or vehicle that set how fast its actor can stop */
struct s_stopping_object
{
	long definition_index;
	byte unknown004[0x70 - 0x4];
	vector3f forward;
	byte unknown07c[0x88 - 0x7c];
	vector3f velocity;
	byte unknown094[0x3dc - 0x94];
	byte movement_type;
};

struct s_stopping_object_header
{
	byte unknown00[8];
	s_stopping_object *object;
};

struct s_stopping_vehicle_definition
{
	byte unknown000[0x1f4];
	real maximum_speed;
	byte unknown1f8[4];
	real acceleration;
};

struct s_stopping_unit_definition
{
	byte unknown000[0x2dc];
	real maximum_speed;
	byte unknown2e0[4];
	real acceleration;
	real deceleration;
	byte unknown2ec[0x2f4 - 0x2ec];
	real speed_scale;
};

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);

static inline byte stopping_object_get_movement_type(s_stopping_object *object)
{
	return object->movement_type;
}

/* how far the actor's unit (or vehicle) travels before it stops, and how far
   it travels while it speeds up to its top speed and stops again */
// @retail 0x1f40b0
void function_1f40b0(long actor_index, real *stopping_distance, real *round_trip_distance)
{
	s_actor_view *actor = actor_get(actor_index);
	real maximum_speed = 2.5f;
	real speed = 0.0f;
	real acceleration = 15.0f;
	real deceleration = 24.0f;

	if (actor->unknown26c != NONE)
	{
		if (actor->unknown270 == 3)
		{
			s_stopping_object *vehicle = ((s_stopping_object_header *)g_4e0300->data)[actor->unknown26c & 0xffff].object;
			s_stopping_vehicle_definition *definition = (s_stopping_vehicle_definition *)g_4e3b44[vehicle->definition_index & 0xffff].bytes;

			speed = dot3f(&vehicle->velocity, &vehicle->forward);
			maximum_speed = definition->maximum_speed;
			acceleration = definition->acceleration;
			deceleration = definition->acceleration;
		}
	}
	else if (actor->unknown018 != NONE)
	{
		s_stopping_object *unit = (s_stopping_object *)function_badc0(actor->unknown018, 1);

		if (unit)
		{
			s_stopping_unit_definition *definition = (s_stopping_unit_definition *)g_4e3b44[unit->definition_index & 0xffff].bytes;

			speed = dot3f(&unit->velocity, &unit->forward);
			if (stopping_object_get_movement_type(unit) == 2)
			{
				maximum_speed = definition->maximum_speed;
				acceleration = definition->acceleration;
				deceleration = definition->deceleration;
				if (actor->unknown5d4 && definition->speed_scale > 0.0f)
				{
					maximum_speed = definition->speed_scale * maximum_speed;
					acceleration = definition->speed_scale * acceleration;
					deceleration = definition->speed_scale * deceleration;
				}
			}
		}
	}

	if (stopping_distance)
		*stopping_distance = speed * speed / (deceleration * 2.0f);
	if (round_trip_distance)
	{
		if (speed > maximum_speed)
			maximum_speed = speed;
		*round_trip_distance = (maximum_speed * maximum_speed - speed * speed) / (acceleration * 2.0f) +
			maximum_speed * maximum_speed / (deceleration * 2.0f);
	}
}