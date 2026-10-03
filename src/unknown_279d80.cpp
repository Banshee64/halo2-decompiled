// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_279D80.CPP: sampling an animation's nodes, and 256-bit node masks */

#include "cseries.h"
#include "animation_graph.h"
#include <xmmintrin.h>

/* not decompiled yet (src/stubs/lane_c.cpp) */
void function_2798a0(s_animation_data *data, real frame, real a, s_graph_tag *graph, s_animation *animation, long b,
	s_graph_inheritance *inheritance, dword const *node_mask, long c, bool interpolate, long d, long e, long f);

PRIVATE inline long animation_data_size(s_animation_data_sizes const *sizes)
{
	return sizes->static_node_flags_size + sizes->animated_node_flags_size + sizes->movement_data_size +
		sizes->unknown04 + sizes->static_data_size + sizes->unknown08 + sizes->animated_data_size;
}

// @retail 0x279d80
void function_279d80(s_graph_tag *graph, c_animation_id animation_id, long b, real frame, real a,
	s_graph_inheritance *inheritance, dword const *node_mask, long c, bool interpolate)
{
	s_animation *animation = function_1daea0(graph, animation_id);
	s_animation_data data;
	long size;
	byte *address;

	function_1ddb40(&data, graph, animation_id);
	size = animation_data_size(data.sizes);
	if (size > 0x400)
	{
		size = 0x400;
	}
	for (address = data.data; address < data.data + size; address += 0x20)
	{
		_mm_prefetch((char const *)address, _MM_HINT_T0);
	}
	function_2798a0(&data, frame, a, graph, animation, b, inheritance, node_mask, c, interpolate, 0, 0, 0);
}

// @retail 0x27a380
void node_mask_and(dword *mask, dword const *other)
{
	long i;

	for (i = 0; i < 8; i++)
	{
		mask[i] &= other[i];
	}
}
