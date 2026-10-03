// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0CC2B0.CPP: whether a point is within a cone around a unit's
   facing, seen from its head (units.cpp; an outside function lane A's script
   functions need) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "object_markers.h"
#include <math.h>

/* the unit's facing vector (+0x18c) */
struct s_unit_0cc2b0
{
	byte unknown000[0x18c];
	real_vector3d facing;
};

struct s_object_header_0cc2b0
{
	byte unknown00[8];
	s_unit_0cc2b0 *object;
};

real function_30bf0(real_vector3d *v);

// @retail 0xcc2b0
bool unit_can_see_point(long unit_index, real_point3d const *point, real angle)
{
	volatile bool result = false;

	if (unit_index != NONE)
	{
		s_unit_0cc2b0 *unit = ((s_object_header_0cc2b0 *)g_4e0300->data)[unit_index & 0xffff].object;
		s_object_marker marker;
		real_point3d head;
		real_vector3d vector;

		function_b8d30(unit_index, 0x4000095, &marker, 1, false);
		head = marker.matrix.position;
		vector.i = point->x - head.x;
		vector.j = point->y - head.y;
		vector.k = point->z - head.z;
		function_30bf0(&vector);
		if (vector.k * unit->facing.k + vector.j * unit->facing.j + vector.i * unit->facing.i > cos(angle))
			return true;
	}
	return result;
}
