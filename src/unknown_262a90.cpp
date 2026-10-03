// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_262A90.CPP: where a reference (function_262b40) stands, and the
   way it faces */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "unknown_20fe20.h"
#include "unknown_2626b0.h"

real function_30bf0(real_vector3d *v);
bool function_29d6c0(real_vector3d *vector, s_reference reference);

// @retail 0x262a90
bool function_262a90(s_reference reference, real_point3d *point, real_vector3d *vector)
{
	bool result = false;
	s_node_point *node = (s_node_point *)function_262b40(reference);

	if (node)
	{
		function_210850(node, point);
		if (function_29d6c0(vector, reference))
		{
			vector->k = 0.0f;
			if (function_30bf0(vector) > 0.0f)
				result = true;
		}
	}

	return result;
}

// @retail 0x262af0
bool function_262af0(s_reference reference, real_point3d *point, real_vector3d *facing)
{
	bool result = false;
	s_node_point *node = (s_node_point *)function_262b40(reference);

	if (node)
	{
		function_210850(node, point);
		if (facing)
			*facing = *g_4687b0;
		result = true;
	}

	return result;
}
