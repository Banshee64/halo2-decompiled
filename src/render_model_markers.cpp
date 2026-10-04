// @flags /O2 /arch:SSE /Gr
/* RENDER_MODEL_MARKERS.CPP: a render model's named marker groups, the markers
   of a group placed on an object's node matrices, and pulling a node chain
   toward a marker */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "object_markers.h"
#include <math.h>

#define k_real_epsilon 0.0001f

/* one marker of a group (0x24 bytes) */
struct s_render_model_marker
{
	char region_index;
	char permutation_index;
	byte node_index;
	byte unknown03;
	real_point3d translation;
	real_quaternion rotation;
	real scale;
};

/* a named group of markers (0xc bytes) */
struct s_render_model_marker_group
{
	long name;
	long marker_count;
	s_render_model_marker *markers;
};

/* a node (0x60 bytes) */
struct s_render_model_node_view
{
	long name;
	short parent_node_index;
	byte unknown06[0x60 - 6];
};

struct s_render_model_view
{
	byte unknown00[0x48];
	long node_count;
	s_render_model_node_view *nodes;
	byte unknown50[0x58 - 0x50];
	long marker_group_count;
	s_render_model_marker_group *marker_groups;
};

struct s_first_person_marker;

matrix3x3 *function_141e10(matrix3x3 *out, real_quaternion const *q);
void function_141590(real_matrix4x3 const *in, real_matrix4x3 *out);
void matrix4x3_from_point_and_quaternion(real_matrix4x3 *out, real_point3d const *position, real_quaternion const *rotation);
int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);
void function_120220(real_matrix4x3 *mid, real_matrix4x3 *root, real_matrix4x3 *target, real_matrix4x3 *end);
struct s_render_model_definition;
void render_model_build_child_node_matrices(s_render_model_definition const *definition, real_matrix4x3 const *parent_matrix,
	long node_index, long node_count, real_matrix4x3 *node_matrices);

PRIVATE inline s_render_model_view *render_model_view_get(long render_model_index)
{
	return (s_render_model_view *)g_4e3b44[render_model_index & 0xffff].bytes;
}

/* the index of the render model's marker group with the name, or NONE */
// @retail 0x1d8f00
long function_1d8f00(long render_model_index, long marker_name)
{
	long result = NONE;

	if (render_model_index != NONE && marker_name != NONE && marker_name != 0)
	{
		s_render_model_view *definition = render_model_view_get(render_model_index);
		long i;

		for (i = 0; i < definition->marker_group_count; i++)
		{
			if (definition->marker_groups[i].name == marker_name)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

/* whether the marker's region shows the marker's permutation (always without
   a region permutation table, or for markers in no region) */
PRIVATE __forceinline bool marker_permutation_visible(byte const *region_permutations, s_render_model_marker const *marker)
{
	bool result = true;

	if (region_permutations && marker->region_index != NONE)
	{
		result = region_permutations[marker->region_index] == (byte)marker->permutation_index;
	}
	return result;
}

/* fills up to count markers of the group, each placed on its node's matrix;
   returns how many it filled */
// @retail 0x1d8f50
short function_1d8f50(long marker_group_index, long render_model_index, byte const *region_permutations,
	long const *node_remapping, real_matrix4x3 const *node_matrices, bool mirrored, s_object_marker *markers, long count)
{
	long result = 0;

	if (marker_group_index != NONE)
	{
		s_render_model_marker_group *group = &render_model_view_get(render_model_index)->marker_groups[marker_group_index];
		long i;

		for (i = 0; i < group->marker_count; i++)
		{
			s_render_model_marker *marker = &group->markers[i];

			if (marker_permutation_visible(region_permutations, marker))
			{
				s_object_marker *out;
				long node_index;

				if (result >= count)
				{
					break;
				}
				node_index = marker->node_index;
				out = markers++;
				result++;
				if (node_remapping)
				{
					node_index = node_remapping[node_index];
				}
				out->node_index = (short)node_index;
				matrix4x3_from_point_and_quaternion(&out->node_matrix, &marker->translation, &marker->rotation);
				function_142a60(&node_matrices[out->node_index], &out->node_matrix, &out->matrix);
				out->unknown6c = marker->scale;
				if (mirrored)
				{
					out->matrix.left.i = 0.0f - out->matrix.left.i;
					out->matrix.left.j = 0.0f - out->matrix.left.j;
					out->matrix.left.k = 0.0f - out->matrix.left.k;
				}
			}
		}
	}
	return result;
}

/* function_1d8f50 for the group with the name */
// @retail 0x1d90b0
short function_1d90b0(long render_model_index, long marker_name, byte const *region_permutations, long model_index,
	long const *node_remapping, long node_count, real_matrix4x3 const *node_matrices, bool mirrored, s_first_person_marker *markers,
	long count)
{
	return function_1d8f50(function_1d8f00(render_model_index, marker_name), render_model_index, region_permutations,
		node_remapping, node_matrices, mirrored, (s_object_marker *)markers, count);
}

/* moves the node and its parent and grandparent so the marker on the node
   reaches the target (weighted toward the node's own position), then carries
   the node's children along */
// @retail 0x1d90e0
void function_1d90e0(long render_model_index, real_matrix4x3 *nodes, long node_index, real_matrix4x3 const *marker_matrix,
	real_matrix4x3 const *target_matrix, real weight, long node_count)
{
	s_render_model_view *definition = render_model_view_get(render_model_index);
	short parent_node_index = definition->nodes[node_index].parent_node_index;

	if (parent_node_index != NONE)
	{
		short grandparent_node_index = definition->nodes[parent_node_index].parent_node_index;

		if (grandparent_node_index != NONE)
		{
			real_matrix4x3 target;
			real_matrix4x3 node_inverse;
			real_matrix4x3 delta;
			real_matrix4x3 *node = &nodes[node_index];

			function_141590(marker_matrix, &target);
			function_142a60(target_matrix, &target, &target);
			function_141590(node, &node_inverse);
			if (!(fabs(weight - 1.0f) < k_real_epsilon))
			{
				target.position.x = node->position.x * (1.0f - weight) + target.position.x * weight;
				target.position.y = node->position.y * (1.0f - weight) + target.position.y * weight;
				target.position.z = node->position.z * (1.0f - weight) + target.position.z * weight;
			}
			function_120220(&nodes[parent_node_index], &nodes[grandparent_node_index], &target, node);
			function_142a60(node, &node_inverse, &delta);
			render_model_build_child_node_matrices((s_render_model_definition const *)definition, &delta, node_index, node_count,
				nodes);
		}
	}
}
