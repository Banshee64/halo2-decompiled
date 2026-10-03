// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_16D280.CPP: render model queries (0x16d280..0x16e2b3), the rest of
   the models file that unknown_16d180.cpp starts: node lookups, default
   orientations and node matrices built down the node hierarchy */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

/* a render model node (0x60 bytes) */
struct s_render_model_node
{
	long name;
	short parent_node_index;
	short first_child_node_index;
	short next_sibling_node_index;
	short unknown0a;
	real_point3d default_translation;
	real_quaternion default_rotation;
	byte unknown28[0x60 - 0x28];
};

/* a render model's marker group (16 bytes) */
struct s_render_model_marker_group
{
	byte unknown00[5];
	char index;
	byte unknown06[0x10 - 6];
};

/* an entry of the block at +0x78 (0x5c bytes) */
struct s_render_model_named_entry
{
	long name;
	byte unknown04[0x5c - 4];
};

struct s_render_model_definition
{
	byte unknown00[4];
	long model_index;
	byte unknown08[0x48 - 0x8];
	long node_count;
	s_render_model_node *nodes;
	byte unknown50[0x70 - 0x50];
	long marker_group_count;
	s_render_model_marker_group *marker_groups;
	long named_entry_count;
	s_render_model_named_entry *named_entries;
};

/* an orientation: a quaternion, a translation and a scale (0x20 bytes;
   unknown_141590.cpp) */
struct real_orientation
{
	real_quaternion rotation;
	real_point3d position;
	real scale;
};

int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);

static inline s_render_model_definition *render_model_get(long render_model_index)
{
	return (s_render_model_definition *)g_4e3b44[render_model_index & 0xffff].bytes;
}

// @retail 0x16d890
long render_model_find_marker_group(long render_model_index, long index)
{
	s_render_model_definition *definition = render_model_get(render_model_index);
	long result = NONE;
	long i;

	for (i = 0; i < definition->marker_group_count; i++)
	{
		if (definition->marker_groups[i].index == index)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x16d8d0
void render_model_build_child_node_matrices(s_render_model_definition const *definition, real_matrix4x3 const *parent_matrix,
	long node_index, long node_count, real_matrix4x3 *node_matrices)
{
	long child_index = definition->nodes[node_index].first_child_node_index;

	while (child_index >= 0 && child_index < definition->node_count && child_index < node_count)
	{
		s_render_model_node const *child = &definition->nodes[child_index];

		function_142a60(parent_matrix, &node_matrices[child_index], &node_matrices[child_index]);
		if (child->first_child_node_index != NONE)
		{
			render_model_build_child_node_matrices(definition, &node_matrices[child_index], child_index, node_count, node_matrices);
		}
		child_index = child->next_sibling_node_index;
	}
}

// @retail 0x16d940
void render_model_get_default_orientations(s_render_model_definition const *definition, real_orientation *orientations)
{
	long i;

	for (i = 0; i < definition->node_count; i++)
	{
		s_render_model_node const *node = &definition->nodes[i];

		orientations[i].rotation = node->default_rotation;
		orientations[i].position = node->default_translation;
		orientations[i].scale = 1.0f;
	}
}

// @retail 0x16da90
long render_model_find_named_entry(long render_model_index, long name)
{
	long result = NONE;

	if (render_model_index != NONE)
	{
		s_render_model_definition *definition = render_model_get(render_model_index);
		long i;

		for (i = 0; i < definition->named_entry_count; i++)
		{
			if (definition->named_entries[i].name == name)
			{
				result = i;
				break;
			}
		}
	}
	return result;
}

// @retail 0x16ddf0
void *render_model_get_model_definition(long render_model_index)
{
	return g_4e3b44[render_model_get(render_model_index)->model_index & 0xffff].bytes;
}

void function_1420f0(real_vector3d const *forward, real_vector3d const *up, real_point3d const *position, real_matrix4x3 *out);
void __stdcall function_1421f0(real_matrix4x3 *out, real_orientation const *orientation);

#define MAXIMUM_NODES_PER_MODEL 253

// @retail 0x16d9c0
void render_model_build_node_matrices(real_vector3d const *forward, real_vector3d const *up, real_point3d const *position,
	s_render_model_definition const *definition, real_matrix4x3 *node_matrices, real_orientation const *orientations)
{
	real_matrix4x3 local_matrix;
	real_matrix4x3 root_matrix;
	long node_stack[MAXIMUM_NODES_PER_MODEL];

	function_1420f0(forward, up, position, &root_matrix);
	if (definition->node_count > 0)
	{
		long read_index = 0;
		long write_index = 1;

		node_stack[0] = 0;
		do
		{
			long node_index = node_stack[read_index++];
			s_render_model_node const *node = &definition->nodes[node_index];
			real_matrix4x3 const *parent_matrix = node_index == 0 ? &root_matrix : &node_matrices[node->parent_node_index];

			function_1421f0(&local_matrix, &orientations[node_index]);
			function_142a60(parent_matrix, &local_matrix, &node_matrices[node_index]);
			if (node->next_sibling_node_index != NONE)
			{
				node_stack[write_index++] = node->next_sibling_node_index;
			}
			if (node->first_child_node_index != NONE)
			{
				node_stack[write_index++] = node->first_child_node_index;
			}
		}
		while (read_index != write_index);
	}
}