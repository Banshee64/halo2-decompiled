/* UNKNOWN_2BA3E0.CPP: filling a placement (a position, a unit direction and
   a second vector) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

// @flags /O2 /arch:SSE /Gr

/* a placement (0x28 bytes) */
struct s_placement
{
	point3f position;
	vector3f direction;
	vector3f up;
	short value24;
};

/* the direction used when the given one cannot be normalized */
extern vector3f *g_4687bc;

real function_1201a0(vector3f *v, vector3f const *fallback);

// @retail 0x2ba3e0
void placement_set(s_placement *placement, vector3f const *direction, point3f const *position, vector3f const *up, short value)
{
	vector3f normal = *direction;

	function_1201a0(&normal, g_4687bc);
	placement->value24 = value;
	placement->position = *position;
	placement->direction = normal;
	placement->up = *up;
}
