// @flags /O2 /Gr
/* UNKNOWN_26BFA0.CPP: the locations of objects and actors (lane I's outside
   functions; 0x26c180 has about 30 callers among the slot handlers) */

#include "cseries.h"
#include "slot_handler.h"
#include "lane_c_callees.h"

/* the object as 0x26bfa0 reads it */
struct s_location_object
{
	byte unknown00[0x14];
	long parent_index;
	byte unknown18[0x30 - 0x18];
	real_point3d center;
	byte unknown3c[0xaa - 0x3c];
	char type;
};

struct s_location_object_header
{
	byte unknown0[8];
	s_location_object *object;
};

/* the structure bsp (g_4e0348) and its pathfinding data at +0xc4 */
struct s_location_structure_view
{
	byte unknown00[0xc4];
	long pathfinding_count;
	void *pathfinding;
};

#define LOCATION_OBJECT(index) (((s_location_object_header *)g_4e0300->data)[(index) & 0xffff].object)

void function_dfdb0(long object_index, long unknown, long *location_index, real_point3d *point, long *a, long *b);
void function_f1070(long object_index, long unknown, long *location_index, real_point3d *point, long *a, long *b);
void function_210420(s_location_view *location, long a, long b, real_point3d const *point);
long function_26d100(real_vector3d const *up, s_collision_result_1697c0 *collision, long *unknown, real_point3d const *point);

/* finds the location of the object's root: a unit's (0), a vehicle's (1),
   otherwise the pathfinding location under its center */
// @retail 0x26bfa0
void function_26bfa0(long object_index, long *location_index, s_location_view *location)
{
	long result = NONE;
	long root = NONE;

	while (object_index != NONE)
	{
		root = object_index;
		object_index = LOCATION_OBJECT(object_index)->parent_index;
	}

	s_location_object *object = LOCATION_OBJECT(root);
	long index = NONE;
	long a = NONE;
	long b = NONE;
	real_point3d point;

	switch (object->type)
	{
	case 0:
		function_dfdb0(root, 0, &index, &point, &a, &b);
		result = index;
		if (location)
		{
			if (result != NONE)
			{
				function_210420(location, a, b, &point);
			}
			else
			{
				location->point.point = point;
				location->point.output_index = NONE;
			}
		}
		break;
	case 1:
		function_f1070(root, 0, &index, &point, &a, &b);
		result = index;
		if (location)
		{
			if (result != NONE)
			{
				function_210420(location, a, b, &point);
			}
			else
			{
				location->point.point = point;
				location->point.output_index = NONE;
			}
		}
		break;
	default:
		{
			s_location_structure_view *structure = (s_location_structure_view *)g_4e0348;

			if (structure->pathfinding_count > 0 && structure->pathfinding)
			{
				s_collision_result_1697c0 collision;

				point = object->center;
				collision.unknown24 = NONE;
				result = function_26d100(g_4687b0, &collision, (long *)location, &point);
			}
		}
		break;
	}
	if (location_index)
	{
		*location_index = result;
	}
}

/* the actor (0x888 bytes) as 0x26c180 reads it */
struct s_actor_location_view
{
	byte unknown000[0x18];
	long unit_index;
	byte unknown01c[0x229 - 0x1c];
	bool unknown229;
	byte unknown22a[0x238 - 0x22a];
	real_point3d position;
	byte unknown244[0x26c - 0x244];
	long unknown26c;
	byte unknown270[0x278 - 0x270];
	bool location_valid;
	byte unknown279[0x27c - 0x279];
	s_location_view location;
};

void function_1caa40(long object_index, real_point3d *position);
bool function_26be90(long object_index);

/* computes the actor's location once per update */
// @retail 0x26c180
void function_26c180(long actor_index)
{
	s_actor_location_view *actor = (s_actor_location_view *)actor_get(actor_index);

	if (!actor->location_valid)
	{
		actor->location.point.point = actor->position;
		actor->location_valid = true;
		actor->location.point.output_index = NONE;
		actor->location.unknown10 = NONE;
		if (actor->unknown229)
		{
			long object_index = actor->unknown26c;

			if (object_index == NONE)
			{
				object_index = actor->unit_index;
			}
			function_1caa40(object_index, &actor->location.point.point);
		}
		else if (actor->unknown26c != NONE)
		{
			function_26bfa0(actor->unknown26c, &actor->location.unknown10, &actor->location);
		}
		else
		{
			long unit_index = actor->unit_index;

			if (unit_index != NONE && function_26be90(unit_index))
			{
				function_26bfa0(unit_index, &actor->location.unknown10, &actor->location);
			}
		}
	}
}
