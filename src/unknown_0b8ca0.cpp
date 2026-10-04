// @flags /O2 /arch:SSE /Gr
/* object markers and object physics state (objects.obj, 0xb8ca0-0xb9c60) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include "object_markers.h"

struct s_object_definition_view
{
	byte unknown00[2];
	byte flag0 : 1;
	byte flag1 : 1;
	byte flag2 : 1;
	byte flag3 : 1;
	byte flag4 : 1;
	byte : 3;
	byte unknown03[0x38 - 3];
	long model_index;
};

struct s_model_definition_view
{
	byte unknown00[4];
	long render_model_index;
};

/* the object (the object data) */
struct s_object_view
{
	long definition_index;
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	dword flag5 : 1;
	dword flag6 : 1;
	dword flag7 : 1;
	dword flag8 : 1;
	dword flag9 : 1;
	dword mirrored : 1;
	dword : 21;
	byte unknown08[0x14 - 8];
	long parent_index;
	byte unknown18[0xa0 - 0x18];
	real scale;
	byte unknown0a4[0xaa - 0xa4];
	byte type;
	byte unknown0ab[0xb4 - 0xab];
	long havok_component_index;
	byte unknown0b8[0xc0 - 0xb8];
	byte unknown0c0 : 6;
	byte physics_active : 1;
	byte : 1;
	byte physics_disabled : 1;
	byte : 7;
	byte unknown0c2[0xd4 - 0xc2];
	long field_x10a40f;
	byte unknown0d8[0x10e - 0xd8];
	short root_node_offset;
	short unknown110;
	short unknown112;
	short node_matrices_size;
	short node_matrices_offset;
	byte unknown118[0x11a - 0x118];
	short region_permutations_offset;
	byte unknown11c[0x154 - 0x11c];
	long unit_index;
};

struct s_object_header_view
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_object_view *object;
};

#define OBJECT_HEADER_GET(index) (&((s_object_header_view *)g_4e0300->data)[(index) & 0xffff])
#define OBJECT_GET(index) (OBJECT_HEADER_GET(index)->object)
#define TAG_DATA(type, index) ((type *)g_4e3b44[(index) & 0xffff].bytes)

bool function_10cf50(long item_index);
long function_1d8f00(long render_model_index, long marker_name);
short function_1d8f50(long marker_group_index, long render_model_index, byte const *region_permutations,
	long const *node_remapping, transform4x3f const *field_50, bool mirrored, s_object_marker *markers, short count);
void function_b58c0(long index, dword mask);
void function_b7360(long object_index);
void havok_component_rigid_bodies_activate(s_havok_component *component);

// @retail 0xb8ca0
long function_b8ca0(long object_index)
{
	s_object_view *object = OBJECT_GET(object_index);

	if (object->parent_index != NONE &&
		TEST_FIELD_BIT(TAG_DATA(s_object_definition_view, object->definition_index)->flag4))
	{
		return function_b8ca0(object->parent_index);
	}
	if (TEST_FIELD_BIT(object->flag0) && ((1 << object->type) & 0x1c) && function_10cf50(object_index))
		return object->unit_index;
	return object_index;
}

// @retail 0xb8d30
short function_b8d30(long object_index, long marker_name, s_object_marker *markers, short count, bool flag)
{
	long marker_object_index = object_index;
	short result = 0;

	if (!flag)
		marker_object_index = function_b8ca0(object_index);
	if (marker_object_index != NONE)
	{
		s_object_view *object = OBJECT_GET(marker_object_index);
		long model_index = TAG_DATA(s_object_definition_view, object->definition_index)->model_index;

		if (model_index != NONE)
		{
			byte const *region_permutations = (byte *)object + object->region_permutations_offset;
			transform4x3f const *field_50 = (transform4x3f *)((byte *)object + object->node_matrices_offset);
			bool mirrored = TEST_FIELD_BIT(object->mirrored);
			volatile long node_count = object->node_matrices_size / sizeof(transform4x3f);
			long render_model_index = TAG_DATA(s_model_definition_view, model_index)->render_model_index;

			result = function_1d8f50(function_1d8f00(render_model_index, marker_name), render_model_index,
				region_permutations, NULL, field_50, mirrored, markers, count);
			if (result)
				return result;
		}
	}

	{
		s_object_view *object = OBJECT_GET(object_index);

		markers->node_index = 0;
		markers->node_matrix.scale = 1.0f;
		markers->node_matrix.rotation.forward.i = 1.0f;
		markers->node_matrix.rotation.forward.j = 0.0f;
		markers->node_matrix.rotation.forward.k = 0.0f;
		markers->node_matrix.rotation.left.i = 0.0f;
		markers->node_matrix.rotation.left.j = 1.0f;
		markers->node_matrix.rotation.left.k = 0.0f;
		markers->node_matrix.rotation.up.i = 0.0f;
		markers->node_matrix.rotation.up.j = 0.0f;
		markers->node_matrix.rotation.up.k = 1.0f;
		markers->node_matrix.position.x = 0.0f;
		markers->node_matrix.position.y = 0.0f;
		markers->node_matrix.position.z = 0.0f;
		s_object_view *node_object = OBJECT_GET(object_index);
		markers->matrix = *(transform4x3f *)((byte *)node_object + node_object->node_matrices_offset);
		markers->unknown6c = 0.0f;
		if (TEST_FIELD_BIT(object->mirrored))
		{
			markers->matrix.rotation.left.i = 0.0f - markers->matrix.rotation.left.i;
			markers->matrix.rotation.left.j = 0.0f - markers->matrix.rotation.left.j;
			markers->matrix.rotation.left.k = 0.0f - markers->matrix.rotation.left.k;
		}
		if (!marker_name)
			result = 1;
	}
	return result;
}

// @retail 0xb9b90
void function_b9b90(long object_index, bool disable)
{
	s_object_header_view *header = OBJECT_HEADER_GET(object_index);
	s_object_view *object = header->object;
	dword type_mask = 1 << header->type;

	if ((type_mask & 0x1883) && TEST_FIELD_BIT(object->physics_active) && object->havok_component_index != NONE)
	{
		if (!disable)
		{
			havok_component_rigid_bodies_activate(havok_component_get(object->havok_component_index));
			object->physics_disabled = false;
			function_b7360(object_index);
		}
	}
	else if (disable)
	{
		object->physics_disabled = true;
	}
	else
	{
		object->physics_disabled = false;
		function_b7360(object_index);
	}

	if (type_mask & 0x1c)
	{
		s_object_view *item = OBJECT_GET(object_index);
		if (item->field_x10a40f != NONE)
			function_b58c0(item->field_x10a40f, 0x400);
	}
	else if (type_mask & 0x20)
	{
		s_object_view *projectile = OBJECT_GET(object_index);
		if (projectile->field_x10a40f != NONE)
			function_b58c0(projectile->field_x10a40f, 0x400);
	}
}

void function_ba350(long object_index, long a);
void __stdcall function_bd020(long object_index);

/* the root node's state at the object's offset +0x10e */
struct s_object_root_node
{
	byte unknown00[0x10];
	vector3f vector;
	real value_1c;
};

// @retail 0xb7680
void function_b7680(long object_index, real scale, long a)
{
	if (object_index != NONE && scale > 0.0f)
	{
		s_object_view *object = OBJECT_GET(object_index);
		real old_scale = object->scale;

		object->scale = scale;
		if (OBJECT_GET(object_index)->unknown112 != NONE)
		{
			s_object_root_node *node;
			real ratio;

			function_ba350(object_index, a);
			node = (s_object_root_node *)((byte *)object + object->root_node_offset);
			ratio = old_scale / scale;
			node->value_1c *= ratio;
			node->vector.i *= ratio;
			node->vector.j *= ratio;
			node->vector.k *= ratio;
		}
		function_bd020(object_index);
	}
}

// @retail 0xb9d20
bool function_b9d20(long object_index)
{
	long root_index = NONE;

	while (object_index != NONE)
	{
		root_index = object_index;
		object_index = OBJECT_GET(object_index)->parent_index;
	}
	return (OBJECT_HEADER_GET(root_index)->flags >> 6) & 1;
}
