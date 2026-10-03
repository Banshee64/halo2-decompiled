// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_0B8BD0.CPP: object queries: a node matrix, the object whose model
   carries an object's markers, its markers by name, whether an object or one
   of its parents is hidden, the location of an object's ultimate parent.
   Decompiled by lane F: 0x18c3b0 calls them with register arguments. */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "object_queries.h"

struct s_object_query_view
{
	byte unknown00[4];
	byte flags;
	byte unknown05[0x14 - 0x5];
	long parent_index;
	byte unknown18[0x28 - 0x18];
	s_location location;
	byte unknown30[0x88 - 0x30];
	real_vector3d linear_velocity;
	real_vector3d angular_velocity;
	byte unknowna0[0xb4 - 0xa0];
	long havok_component_index;
	byte unknownb8[0xc1 - 0xb8];
	byte stationary : 1;
	byte unknownc1 : 7;
	byte unknownc2[0x116 - 0xc2];
	short node_matrices_offset;
};

struct s_object_query_header
{
	byte unknown00[2];
	byte flags;
	byte type;
	byte unknown04[4];
	s_object_query_view *object;
};

static inline s_object_query_header *object_query_header(long object_index)
{
	return (s_object_query_header *)g_4e0300->data + (object_index & 0xffff);
}

// @retail 0xb8bd0
real_matrix4x3 *object_get_node_matrix(long object_index, short node_index)
{
	s_object_query_view *object = object_query_header(object_index)->object;
	return (real_matrix4x3 *)((byte *)object + object->node_matrices_offset) + node_index;
}

/* an object as the marker queries read it */
struct s_object_marker_source
{
	long definition_index;
	dword flag0 : 1;
	dword unknown04_1 : 9;
	dword mirrored : 1;
	dword unknown04_11 : 21;
	byte unknown08[0x14 - 0x8];
	long parent_index;
	byte unknown18[0xaa - 0x18];
	byte type;
	byte unknownab[0x114 - 0xab];
	short node_matrices_size;
	short node_matrices_offset;
	byte unknown118[0x11a - 0x118];
	short original_node_matrices_offset;
	byte unknown11c[0x154 - 0x11c];
	long marker_parent_index;
};

struct s_object_marker_header
{
	byte unknown00[8];
	s_object_marker_source *object;
};

struct s_object_marker_definition
{
	byte unknown00[2];
	byte unknown02_0 : 4;
	byte markers_from_parent : 1;
	byte unknown02_5 : 3;
	byte unknown03[0x38 - 0x3];
	long model_index;
};

struct s_object_marker_model
{
	byte unknown00[4];
	long render_model_index;
};

/* a marker of an object (0x70 bytes): where it is relative to its node, and
   in the world */
struct s_object_marker
{
	short node_index;
	short unknown02;
	real_matrix4x3 node_relative;
	real_matrix4x3 matrix;
	real unknown6c;
};

bool function_10cf50(long object_index);
short function_1d8f00(long render_model_index, long marker_name);
short function_1d8f50(short marker_index, long render_model_index, real_matrix4x3 const *original_node_matrices, long a4, real_matrix4x3 const *node_matrices, bool mirrored, s_object_marker *markers, short maximum_count);

static inline s_object_marker_source *object_marker_source_get(long object_index)
{
	return ((s_object_marker_header *)g_4e0300->data)[object_index & 0xffff].object;
}

static inline byte *object_marker_tag_get(long tag_index)
{
	return g_4e3b44[tag_index & 0xffff].bytes;
}

static inline real_matrix4x3 const *object_marker_root_node_matrix(long object_index)
{
	s_object_marker_source *object = object_marker_source_get(object_index);
	return (real_matrix4x3 const *)((byte *)object + object->node_matrices_offset);
}

// @retail 0xb8ca0
long function_b8ca0(long object_index)
{
	s_object_marker_source *object = object_marker_source_get(object_index);

	if (object->parent_index != NONE &&
		TEST_FIELD_BIT(((s_object_marker_definition *)object_marker_tag_get(object->definition_index))->markers_from_parent))
	{
		return function_b8ca0(object->parent_index);
	}
	if (TEST_FIELD_BIT(object->flag0) && ((1 << object->type) & 0x1c) && function_10cf50(object_index))
	{
		return object->marker_parent_index;
	}
	return object_index;
}

// @retail 0xb8d30
short function_b8d30(bool flag, long object_index, long marker_name, short count, s_object_marker *markers)
{
	short result = 0;
	long marker_object_index = object_index;

	if (!flag)
	{
		marker_object_index = function_b8ca0(object_index);
	}

	if (marker_object_index != NONE)
	{
		s_object_marker_source *object = object_marker_source_get(marker_object_index);
		long model_index = ((s_object_marker_definition *)object_marker_tag_get(object->definition_index))->model_index;

		if (model_index != NONE)
		{
			real_matrix4x3 const *original_node_matrices = (real_matrix4x3 const *)((byte *)object + object->original_node_matrices_offset);
			real_matrix4x3 const *node_matrices = (real_matrix4x3 const *)((byte *)object + object->node_matrices_offset);
			bool mirrored = TEST_FIELD_BIT(object->mirrored);
			long node_count = object->node_matrices_size / sizeof(real_matrix4x3);
			long render_model_index = ((s_object_marker_model *)object_marker_tag_get(model_index))->render_model_index;
			short marker_index = function_1d8f00(render_model_index, marker_name);

			result = function_1d8f50(marker_index, render_model_index, original_node_matrices, 0, node_matrices, mirrored, markers, count);
			if (result)
			{
				return result;
			}
		}
	}

	s_object_marker_source *object = object_marker_source_get(object_index);

	markers->node_index = 0;
	markers->node_relative.scale = 1.0f;
	markers->node_relative.forward.i = 1.0f;
	markers->node_relative.forward.j = 0.0f;
	markers->node_relative.forward.k = 0.0f;
	markers->node_relative.left.i = 0.0f;
	markers->node_relative.left.j = 1.0f;
	markers->node_relative.left.k = 0.0f;
	markers->node_relative.up.i = 0.0f;
	markers->node_relative.up.j = 0.0f;
	markers->node_relative.up.k = 1.0f;
	markers->node_relative.position.x = 0.0f;
	markers->node_relative.position.y = 0.0f;
	markers->node_relative.position.z = 0.0f;
	markers->matrix = *object_marker_root_node_matrix(object_index);
	markers->unknown6c = 0.0f;
	if (TEST_FIELD_BIT(object->mirrored))
	{
		markers->matrix.left.i = 0.0f - markers->matrix.left.i;
		markers->matrix.left.j = 0.0f - markers->matrix.left.j;
		markers->matrix.left.k = 0.0f - markers->matrix.left.k;
	}

	if (!marker_name)
	{
		result = 1;
	}
	return result;
}

// @retail 0xb9ce0
bool object_or_parent_hidden(long object_index)
{
	do
	{
		s_object_query_header *header = object_query_header(object_index);
		s_object_query_view *object = header->object;
		if ((header->flags & 0x10) || (object->flags & 1))
		{
			return true;
		}
		object_index = object->parent_index;
	} while (object_index != NONE);

	return false;
}

// @retail 0xba300
void object_get_root_location(long object_index, s_location *location)
{
	long root_index = NONE;

	while (object_index != NONE)
	{
		root_index = object_index;
		object_index = object_query_header(object_index)->object->parent_index;
	}
	*location = object_query_header(root_index)->object->location;
}

/* an element (0xa0 bytes) of the havok components, g_51e9b8 */
struct s_object_query_havok_component
{
	byte unknown00[0x18];
	char rigid_body_index;
	byte unknown19[0x74 - 0x19];
	long rigid_body_count;
	byte unknown78[0xa0 - 0x78];
};

struct s_havok_component;
void havok_component_rigid_body_linear_velocity_get(long rigid_body_index, s_havok_component *component, real_vector3d *velocity);
void havok_component_rigid_body_angular_velocity_get(long rigid_body_index, s_havok_component *component, real_vector3d *velocity);

// @retail 0xba1d0
void object_get_velocities(long object_index, real_vector3d *linear_velocity, real_vector3d *angular_velocity)
{
	long root_index = NONE;

	while (object_index != NONE)
	{
		root_index = object_index;
		object_index = object_query_header(object_index)->object->parent_index;
	}

	s_object_query_view *object = object_query_header(root_index)->object;
	if (object->havok_component_index != NONE)
	{
		s_object_query_havok_component *component = &((s_object_query_havok_component *)g_51e9b8->data)[object->havok_component_index & 0xffff];
		long rigid_body_index = (short)(component->rigid_body_index >= 0 && component->rigid_body_index < component->rigid_body_count ? component->rigid_body_index : NONE);

		if (rigid_body_index != NONE)
		{
			bool stationary = TEST_FIELD_BIT(object->stationary);

			if (linear_velocity)
			{
				if (stationary)
				{
					*linear_velocity = *g_4687a4;
				}
				else
				{
					havok_component_rigid_body_linear_velocity_get(rigid_body_index, (s_havok_component *)component, linear_velocity);
				}
			}
			if (angular_velocity)
			{
				if (stationary)
				{
					*angular_velocity = *g_4687a4;
				}
				else
				{
					havok_component_rigid_body_angular_velocity_get(rigid_body_index, (s_havok_component *)component, angular_velocity);
				}
			}
			return;
		}
	}

	if (linear_velocity)
	{
		*linear_velocity = object->linear_velocity;
	}
	if (angular_velocity)
	{
		*angular_velocity = object->angular_velocity;
	}
}