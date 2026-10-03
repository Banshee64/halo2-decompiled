/* OBJECT_MARKERS.H: the 0x70 byte object marker that function_b8d30 fills
   (offsets checked against retail 0x10da60, 0x10ae60) */

#ifndef OBJECT_MARKERS_H
#define OBJECT_MARKERS_H

#include "cseries.h"
#include "real_math.h"

struct s_object_marker
{
	short node_index;
	short unknown02;
	real_matrix4x3 node_matrix;
	real_matrix4x3 matrix;
	real unknown6c;
};

/* the flag comes last: retail loads cl after ebx (the markers) at every call */
short function_b8d30(long object_index, long marker_name, s_object_marker *markers, short count, bool flag);

#endif
