// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_137650.CPP: render model node matrices */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include <xmmintrin.h>

/* a render model's node (0x60 bytes): the inverse of its default matrix at
   +0x28 */
struct s_render_model_node
{
	byte unknown00[0x28];
	transform4x3f inverse;
	byte unknown5c[4];
};

/* a region permutation (0x5c bytes) */
struct s_render_model_permutation
{
	byte unknown00[0x25];
	byte node_count;
	byte unknown26[0x36];
};

/* a level of detail (12 bytes) */
struct s_render_model_lod
{
	byte unknown00[4];
	long node_count;
	byte unknown08[4];
};

struct s_render_model_view
{
	byte unknown00[4];
	byte flags;
	byte unknown05[0x17];
	long region_count;
	byte unknown20[8];
	s_render_model_permutation *permutations;
	byte unknown2c[0xc];
	s_render_model_lod *lods;
	char lod_indices[0xc];
	long node_count;
	s_render_model_node *nodes;
};

/* a node's matrix as the vertex shaders take it: three rows of four */
struct s_node_matrix
{
	real rows[3][4];
};

int __fastcall function_142a60(transform4x3f const *a, transform4x3f const *b, transform4x3f *result);

// @retail 0x137650
long function_137650(long model_index, long lod, byte const *permutations)
{
	s_render_model_view *model = (s_render_model_view *)g_4e3b44[model_index & 0xffff].bytes;
	long count;

	if (model->flags & 4)
	{
		count = 1;
	}
	else
	{
		count = model->lods[model->lod_indices[lod]].node_count + model->node_count;
	}

	for (long i = 0; i < model->region_count; i++)
	{
		byte permutation = permutations[i];
		if (permutation != 0xff)
		{
			count += model->permutations[permutation].node_count;
		}
	}

	return count;
}

// @retail 0x1376c0
long function_1376c0(long model_index, s_node_matrix *matrices, transform4x3f const *nodes)
{
	s_render_model_view *model = (s_render_model_view *)g_4e3b44[model_index & 0xffff].bytes;

	for (long node_index = 0; node_index < model->node_count; node_index++)
	{
		s_render_model_node *definition = &model->nodes[node_index];
		s_node_matrix *out = &matrices[node_index];
		_mm_prefetch((char const *)&nodes[node_index + 1], _MM_HINT_T0);
		_mm_prefetch((char const *)&nodes[node_index + 1] + 0x20, _MM_HINT_T0);
		_mm_prefetch((char const *)&definition[1].inverse, _MM_HINT_T0);
		_mm_prefetch((char const *)&definition[1].inverse + 0x20, _MM_HINT_T0);

		transform4x3f matrix;
		function_142a60(&nodes[node_index], &definition->inverse, &matrix);

		out->rows[0][0] = matrix.forward.i * matrix.scale;
		out->rows[0][1] = matrix.left.i * matrix.scale;
		out->rows[0][2] = matrix.up.i * matrix.scale;
		out->rows[1][0] = matrix.forward.j * matrix.scale;
		out->rows[1][1] = matrix.left.j * matrix.scale;
		out->rows[1][2] = matrix.up.j * matrix.scale;
		out->rows[2][0] = matrix.forward.k * matrix.scale;
		out->rows[2][1] = matrix.left.k * matrix.scale;
		out->rows[2][2] = matrix.up.k * matrix.scale;
		out->rows[0][3] = matrix.position.x;
		out->rows[1][3] = matrix.position.y;
		out->rows[2][3] = matrix.position.z;
	}

	return model->node_count;
}
