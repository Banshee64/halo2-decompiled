// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_11C380.CPP: the scenario's trigger volumes, as boxes placed in the
   world or on an object's node (outside functions lane A's script functions
   need) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "object_queries.h"

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
	real radius;
	byte unknown40[0x44 - 0x40];
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
	byte unknown004[0x30 - 4];
	real_point3d center;
	byte unknown03c[0x116 - 0x3c];
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
long render_model_find_named_entry(long render_model_index, long name);
void matrix4x3_from_point_and_vectors(real_matrix4x3 *out, real_point3d const *position, real_vector3d const *forward, real_vector3d const *up);
int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);

/* the matrix of a trigger volume: placed in the world, or on a node of the
   object it names */
// @retail 0x11c380
bool function_11c380(long trigger_volume_index, real_matrix4x3 *matrix)
{
	s_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_11c380 *)g_4e0350)->trigger_volumes[trigger_volume_index];
	bool result = false;

	if (trigger_volume->object_name_index != NONE)
	{
		long object_index = function_bb760(trigger_volume->object_name_index);
		if (object_index != NONE)
		{
			s_object_11c380 *object = OBJECT_GET_11C380(object_index);
			s_object_definition_11c380 *definition = (s_object_definition_11c380 *)g_4e3b44[object->definition_index & 0xffff].bytes;
			long node_index = render_model_find_named_entry(definition->model_index, trigger_volume->node_name);
			if (node_index == NONE)
				return false;

			matrix4x3_from_point_and_vectors(matrix, &trigger_volume->position, &trigger_volume->forward, &trigger_volume->up);
			object = OBJECT_GET_11C380(object_index);
			real_matrix4x3 *node_matrices = (real_matrix4x3 *)((byte *)object + object->node_matrices_offset);
			function_142a60(&node_matrices[(short)node_index], matrix, matrix);
			return true;
		}
	}
	else
	{
		matrix4x3_from_point_and_vectors(matrix, &trigger_volume->position, &trigger_volume->forward, &trigger_volume->up);
		return true;
	}
	return result;
}

/* whether a point is inside a trigger volume */
// @retail 0x11c470
bool function_11c470(long trigger_volume_index, real_point3d const *point)
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

long object_list_new(void);
void __stdcall object_list_add(long list_index, long object_index);
void function_11bed0(real_point3d const *point, s_location *location);
short __stdcall function_bb050(long a, unsigned long type_mask, void const *location, real_point3d const *position, float radius, long *objects, short maximum_count);

/* real_math's inline point transform */
static inline real_point3d *matrix4x3_transform_point(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *out)
{
	real x = point->x;
	real y = point->y;
	real z = point->z;

	if (matrix->scale != 1.0f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	out->x = matrix->up.i * z + matrix->left.i * y + matrix->forward.i * x + matrix->position.x;
	out->y = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	out->z = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x + matrix->position.z;
	return out;
}

/* a new object list of the objects (of the types of a mask) inside a trigger
   volume */
// @retail 0x11c5f0
long function_11c5f0(long trigger_volume_index, long type_mask)
{
	long list_index = object_list_new();
	real_matrix4x3 matrix;

	if (function_11c380(trigger_volume_index, &matrix))
	{
		s_trigger_volume *trigger_volume = &((s_scenario_trigger_volumes_11c380 *)g_4e0350)->trigger_volumes[trigger_volume_index];
		real_point3d center;
		s_location location;

		center.x = trigger_volume->extents.i * 0.5f;
		center.y = trigger_volume->extents.j * 0.5f;
		center.z = trigger_volume->extents.k * 0.5f;
		matrix4x3_transform_point(&matrix, &center, &center);
		function_11bed0(&center, &location);
		if (location.cluster_index != NONE)
		{
			long objects[128];
			long count = function_bb050(0, type_mask, &location, &center, trigger_volume->radius, objects, 128);
			for (long i = 0; i < count; i++)
			{
				long object_index = objects[i];
				if (object_index != NONE &&
					function_11c470(trigger_volume_index, &OBJECT_GET_11C380(object_index)->center))
				{
					object_list_add(list_index, object_index);
				}
			}
		}
	}
	return list_index;
}