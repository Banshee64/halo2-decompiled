#ifndef __UNKNOWN_20FE20_H__
#define __UNKNOWN_20FE20_H__

#include "real_math.h"

/* a point, either in the world or (output_index not NONE) relative to the
   node of an object that an output entry of g_4f93a0 names */
struct s_node_point
{
	real_point3d point;
	short output_index;
};

bool function_2104b0(short output_index, real_point3d const *point, real_point3d *out);
real_point3d *function_210850(s_node_point const *point, real_point3d *out);
real function_210a30(s_node_point const *a, s_node_point const *b);

#endif
