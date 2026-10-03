// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_11C380.CPP: the scenario's trigger volumes, as boxes placed in the
   world or on an object's node (outside functions lane A's script functions
   need) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"

/* a trigger volume of the scenario (0x44 bytes) */
struct s_trigger_volume
{
	byte unknown00[4];
	short object_name_index;
	byte unknown06[2];
	long node_name;
	real_vector3d forward;
	real_vector3d up;
	real_point3d position;
	real_vector3d extents;
	byte unknown3c[0x44 - 0x3c];
};

struct s_scenario_trigger_volumes_11c380
{
	byte unknown000[0x108];
	long trigger_volume_count;
	s_trigger_volume *trigger_volumes;
};

struct s_object_11c380
{
	long definition_index;
	byte unknown004[0x116 - 4];
	short node_matrices_offset;
};

struct s_object_header_11c380
{
	byte unknown00[8];
	s_object_11c380 *object;
};

struct s_object_definition_11c380
{
	byte unknown00[0x38];
	long model_index;
};

#define OBJECT_GET_11C380(index) (((s_object_header_11c380 *)g_4e0300->data)[(index) & 0xffff].object)

long function_bb760(short index);
short function_16da90(long model_index, long node_name);
void function_1420f0(real_vector3d const *forward, real_vector3d const *up, real_point3d const *position, real_matrix4x3 *out);
int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);

/* the matrix of a trigger volume: placed in the world, or on a node of the
   object it names */
// @retail 0x11c380
bool function_11c380(long trigger_volume_index, real_matrix4x3 *matrix)
{
	s_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_11c380 *)g_4e0350)->trigger_volumes[trigger_volume_index];
	bool result = false;

	if (trigger_volume->object_name_index == NONE)
	{
		function_1420f0(&trigger_volume->forward, &trigger_volume->up, &trigger_volume->position, matrix);
		return true;
	}

	long object_index = function_bb760(trigger_volume->object_name_index);
	if (object_index != NONE)
	{
		s_object_11c380 *object = OBJECT_GET_11C380(object_index);
		s_object_definition_11c380 *definition = (s_object_definition_11c380 *)g_4e3b44[object->definition_index & 0xffff].bytes;
		short node_index = function_16da90(definition->model_index, trigger_volume->node_name);
		if (node_index == NONE)
			return false;

		function_1420f0(&trigger_volume->forward, &trigger_volume->up, &trigger_volume->position, matrix);
		object = OBJECT_GET_11C380(object_index);
		real_matrix4x3 *node_matrices = (real_matrix4x3 *)((byte *)object + object->node_matrices_offset);
		function_142a60(&node_matrices[node_index], matrix, matrix);
		return true;
	}
	return result;
}

/* whether a point is inside a trigger volume */
// @retail 0x11c470
bool function_11c470(short trigger_volume_index, real_point3d const *point)
{
	real_matrix4x3 matrix;
	bool result = false;

	if (function_11c380(trigger_volume_index, &matrix))
	{
		s_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_11c380 *)g_4e0350)->trigger_volumes[trigger_volume_index];
		if (matrix.scale != 0.0f)
		{
			real_vector3d vector;
			vector3d_from_points3d(&matrix.position, point, &vector);
			if (matrix.scale != 1.0f)
			{
				real inverse_scale = 1.0f / matrix.scale;
				vector.i *= inverse_scale;
				vector.j *= inverse_scale;
				vector.k *= inverse_scale;
			}
			real x = dot_product3d(&vector, &matrix.forward);
			real y = dot_product3d(&vector, &matrix.left);
			real z = dot_product3d(&vector, &matrix.up);
			if (x > 0.0f && y > 0.0f && z > 0.0f &&
				trigger_volume->extents.i > x && trigger_volume->extents.j > y && trigger_volume->extents.k > z)
			{
				return true;
			}
		}
	}
	return result;
}
