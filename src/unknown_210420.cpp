// @flags /O2 /Ob1 /Gr
/* UNKNOWN_210420.CPP: a node point from an object's marker (lane I's outside
   function; 0x26bfa0's only callee among the node point helpers of
   unknown_20fe20.cpp) */

#include "cseries.h"
#include "slot_handler.h"

short function_210310(long object_index, long a); /* unknown_20fe20.cpp */
bool function_210690(short output_index, point3f const *point, point3f *out); /* unknown_20fe20.cpp */

/* the point relative to the object's output when it has one, otherwise the
   point itself */
// @retail 0x210420
bool function_210420(point3f const *point, long object_index, long marker, s_type_c3b527 *node_point)
{
	if (object_index == NONE)
	{
		node_point->point = *point;
		node_point->output_index = NONE;
		return true;
	}

	short output_index = function_210310(object_index, marker);

	if (output_index == NONE)
	{
		node_point->point = *point;
		node_point->output_index = output_index;
		return true;
	}
	if (function_210690(output_index, point, &node_point->point))
	{
		node_point->output_index = output_index;
		return true;
	}
	node_point->point = *point;
	node_point->output_index = NONE;
	return true;
}
