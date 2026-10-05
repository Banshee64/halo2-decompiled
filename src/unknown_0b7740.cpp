#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "effects.h"
#include <string.h>
#include <xmmintrin.h>

// @flags /O2 /Ob1 /arch:SSE /Gr

struct s_object_transform_view
{
	long definition_index;
	byte unknown004[0x14 - 4];
	long parent_index;
	char parent_node;
	byte unknown019[0x64 - 0x19];
	point3f position;
	vector3f forward;
	vector3f up;
	byte unknown088[0x116 - 0x88];
	short node_matrices_offset;
};

struct s_object_transform_header
{
	byte unknown00[8];
	s_object_transform_view *object;
};

void function_1420f0(transform4x3f *out, point3f const *position,
	vector3f const *forward, vector3f const *up);
int __fastcall function_142a60(transform4x3f const *a, transform4x3f const *b,
	transform4x3f *result);

// @retail 0xba160
transform4x3f *function_ba160(long object_index, transform4x3f *matrix)
{
	transform4x3f *const *matrix_reference = &matrix;
	s_record_pool *objects = g_4e0300;
	s_object_transform_view *object = ((s_object_transform_header *)objects->data)[object_index & 0xffff].object;

	function_1420f0(*matrix_reference, &object->position, &object->forward, &object->up);
	if (object->parent_index != NONE)
	{
		s_object_transform_view *parent = ((s_object_transform_header *)objects->data)[object->parent_index & 0xffff].object;
		transform4x3f *nodes = (transform4x3f *)((byte *)parent + parent->node_matrices_offset);

		function_142a60(&nodes[object->parent_node], matrix, matrix);
	}
	return matrix;
}

struct s_velocity_object
{
	byte unknown000[0x88];
	vector3f linear_velocity;
	vector3f angular_velocity;
	byte unknown0a0[0xd4 - 0xa0];
	long synchronization_index;
};

struct s_velocity_object_header
{
	byte unknown00[8];
	s_velocity_object *object;
};

void function_b58c0(long index, dword mask);

// @retail 0xb7740
void function_b7740(long object_index, vector3f const *linear_velocity,
	vector3f const *angular_velocity, bool skip_update)
{
	dword update_mask = 0;
	object_index &= 0xffff;
	s_record_pool *objects = g_4e0300;
	s_velocity_object *object = ((s_velocity_object_header *)objects->data)[object_index].object;
	if (linear_velocity)
	{
		object->linear_velocity = *linear_velocity;
		update_mask |= 0x10;
	}
	if (angular_velocity)
	{
		object->angular_velocity = *angular_velocity;
		update_mask |= 0x20;
	}
	if (!skip_update && update_mask)
	{
		long index = ((s_velocity_object_header *)objects->data)[object_index].object->synchronization_index;
		if (index != NONE)
			function_b58c0(index, update_mask);
	}
}

struct s_object_list;
extern s_object_list *g_4de2f4;

// @retail 0xb8820
long function_b8820()
{
	if (g_4de2f4 && *(byte *)g_4de2f4)
		return 1;
	return 0;
}

struct s_object_named_value
{
	byte unknown00[8];
	long value;
	byte unknown0c[0x18 - 0xc];
};

struct s_object_named_values
{
	byte unknown00[0x94];
	long count;
	s_object_named_value *entries;
};

// @retail 0xb8c40
long function_b8c40(long object_index, short entry_index)
{
	long result = NONE;
	s_object_transform_view *object = ((s_object_transform_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_object_named_values *definition = (s_object_named_values *)g_4e3b44[object->definition_index & 0xffff].bytes;
	if (entry_index >= 0 && entry_index < definition->count)
		result = definition->entries[entry_index].value;
	return result;
}

struct s_object_link_owner
{
	byte unknown00[8];
	s_record_pool *references;
};

struct s_object_link_iterator
{
	s_object_link_owner *owner;
	long index;
};

struct s_object_link_entry
{
	byte unknown00[4];
	short value;
	byte unknown06[2];
	long next;
};

// @retail 0xb8b20
short function_b8b20(s_object_link_iterator *iterator)
{
	short result;
	s_record_pool *references = iterator->owner->references;
	if (iterator->index != NONE)
	{
		long size = references->size;
		byte *data = references->data;
		s_object_link_entry *entry = (s_object_link_entry *)(data + (iterator->index & 0xffff) * size);
		long next = entry->next;
		if (next != NONE)
			_mm_prefetch((char const *)(data + (next & 0xffff) * size), _MM_HINT_T0);
		iterator->index = next;
		result = entry->value;
	}
	else
	{
		result = NONE;
	}
	return result;
}

struct s_object_partition_record_ab
{
	short type;
	word flags;
	point3f centre;
	real radius;
};

struct s_object_partition_view_ab
{
	long tag_index;
	dword flags;
	byte unknown08[0x40 - 8];
	point3f centre;
	real radius;
	byte unknown50[0xaa - 0x50];
	char type;
};

struct s_object_partition_header_ab
{
	byte unknown00[8];
	s_object_partition_view_ab *object;
};

word collision_object_flags(long object_index);

// @retail 0xb88e0
void function_b88e0(long object_index, s_object_partition_record_ab *record)
{
	s_object_partition_view_ab *object = ((s_object_partition_header_ab *)g_4e0300->data)[object_index & 0xffff].object;
	word flags = 0;
	if ((bool)((object->flags >> 9) & 1))
		flags = collision_object_flags(object_index);
	record->type = object->type;
	record->flags = flags;
	record->centre = object->centre;
	record->radius = object->radius;
}

extern void *g_4de2e0;
extern void *g_4de2e4;
extern void *g_4de2d4;
extern void *g_4de2d8;

struct s_partition_object_link_ab
{
	long salt;
	long object_index;
	long next;
	s_object_partition_record_ab record;
};

struct s_object_cluster_reference
{
	byte type;
	byte unknown01;
	word flags;
	point3f center;
	real radius;
};

struct s_object_cluster_iterator
{
	long next;
};

static __forceinline long object_cluster_next_ab(s_record_pool *array, s_object_cluster_iterator *iterator, s_object_cluster_reference **record)
{
	long result;
	if (iterator->next != NONE)
	{
		long *size = &array->size;
		byte **data = &array->data;
		s_partition_object_link_ab *link = (s_partition_object_link_ab *)(*data + (iterator->next & 0xffff) * *size);
		long next = link->next;
		*record = (s_object_cluster_reference *)&link->record;
		if (next != NONE)
			_mm_prefetch((char const *)(*data + (next & 0xffff) * *size), _MM_HINT_T0);
		iterator->next = next;
		result = link->object_index;
	}
	else
	{
		result = NONE;
	}
	return result;
}

// @retail 0xb8940
long function_b8940(short cluster_index, s_object_cluster_reference **record, s_object_cluster_iterator *iterator)
{
	short const *cluster_reference = &cluster_index;
	iterator->next = ((long *)g_4de2e0)[*cluster_reference];
	return object_cluster_next_ab((s_record_pool *)g_4de2e4, iterator, record);
}

// @retail 0xb89b0
long function_b89b0(s_object_cluster_reference **record, s_object_cluster_iterator *iterator)
{
	return object_cluster_next_ab((s_record_pool *)g_4de2e4, iterator, record);
}

// @retail 0xb8a10
long function_b8a10(short cluster_index, s_object_cluster_reference **record, s_object_cluster_iterator *iterator)
{
	short const *cluster_reference = &cluster_index;
	iterator->next = ((long *)g_4de2d4)[*cluster_reference];
	return object_cluster_next_ab((s_record_pool *)g_4de2d8, iterator, record);
}

struct s_placement_defaults_ab
{
	long tag_index;
	long unique_id;
	short bsp_index;
	char type;
	char source;
	long field_0c;
	long scenario_index;
	byte bsp_policy;
	byte unknown15[3];
	dword flags;
	point3f position;
	vector3f forward;
	vector3f up;
	vector3f linear_velocity;
	vector3f angular_velocity;
	real scale;
	long player_index;
	long object_index;
	long team;
	s_effect_owner owner;
	long field_74;
	byte unknown78[0xb4 - 0x78];
	short field_b4;
	byte unknownb6[2];
	byte field_b8;
	byte unknownb9[0xc4 - 0xb9];
};

struct s_placement_source_ab
{
	byte unknown00[0xaa];
	byte type;
	byte unknownab[0xc0 - 0xab];
	byte flags_c0;
	byte unknownc1[0x138 - 0xc1];
	short team;
	byte unknown13a[2];
	long player_index;
};

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);

// @retail 0xb7930
void function_b7930(void *data, long tag_index, long object_index, s_effect_owner const *owner)
{
	s_placement_defaults_ab *placement = (s_placement_defaults_ab *)data;
	memset(placement, 0, sizeof(*placement));
	placement->tag_index = tag_index;
	placement->field_0c = 0;
	placement->flags = 0;
	placement->forward = *g_4687a8;
	placement->up = *g_4687b0;
	placement->scale = 1.0f;
	placement->field_74 = 0;
	placement->field_b4 = NONE;
	placement->bsp_policy = 0;
	s_placement_source_ab *object = (s_placement_source_ab *)function_badc0(object_index, NONE);
	placement->team = NONE;
	placement->player_index = NONE;
	if (object)
	{
		dword flags = *(volatile dword *)&placement->flags;
		placement->object_index = object_index;
		if ((bool)(((dword)object->flags_c0 >> 2) & 1))
			flags |= 0x20;
		else
			flags &= ~0x20;
		placement->flags = flags;
		if ((1 << object->type) & 3)
		{
			placement->player_index = object->player_index;
			placement->team = object->team;
		}
	}
	else
	{
		placement->object_index = NONE;
	}
	if (owner)
		placement->owner = *owner;
	else
	{
		placement->owner.unknown4 = NONE;
		placement->owner.unknown0 = NONE;
		placement->owner.unknown8 = NONE;
	}
	placement->type = NONE;
	placement->source = NONE;
	placement->bsp_index = NONE;
	placement->unique_id = NONE;
	placement->scenario_index = NONE;
	placement->field_b8 = 0;
}

struct s_scenario_identifier_ab
{
	long unique_id;
	short origin_bsp;
	char type;
	char source;
};

struct s_scenario_type_ab
{
	byte unknown00[0xa];
	short block_offset;
	short palette_offset;
	short element_size;
};

struct s_scenario_block_ab
{
	long count;
	byte *elements;
};

// @retail 0xb7a40
void *__stdcall function_b7a40(s_scenario_identifier_ab const *identifier, long *index_out)
{
	void *result = 0;
	char source = identifier->source;
	if (source == 1 || source == 0)
	{
		char type = identifier->type;
		s_scenario_type_ab *definition = (s_scenario_type_ab *)g_468630[type];
		if (definition->block_offset != NONE)
		{
			long size = definition->element_size;
			s_scenario_block_ab *block = (s_scenario_block_ab *)((byte *)g_4e0350 + definition->block_offset);
			long count = block->count;
			byte *element = block->elements;
			for (long i = 0; i < count; i++, element += size)
			{
				s_scenario_identifier_ab *current = (s_scenario_identifier_ab *)(element + 0x28);
				bool match = (current->unique_id == identifier->unique_id) & (current->type == type) & (current->source == source);
				if (match && current->source == 0)
					match &= current->origin_bsp == identifier->origin_bsp;
				if (match)
				{
					if (index_out)
						*index_out = i;
					return element;
				}
			}
		}
	}
	return result;
}
