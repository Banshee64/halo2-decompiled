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

static inline void identity_transform_ab(transform4x3f *matrix)
{
    matrix->scale = 1.0f;
    matrix->forward.i = 1.0f;
    matrix->forward.j = 0.0f;
    matrix->forward.k = 0.0f;
    matrix->left.i = 0.0f;
    matrix->left.j = 1.0f;
    matrix->left.k = 0.0f;
    matrix->up.i = 0.0f;
    matrix->up.j = 0.0f;
    matrix->up.k = 1.0f;
    matrix->position.x = 0.0f;
    matrix->position.y = 0.0f;
    matrix->position.z = 0.0f;
}

// @retail 0xbdc40
void function_bdc40(point3f const *position, vector3f const *forward, vector3f const *up, real scale, bool mirrored, transform4x3f const *parent, bool parent_mirrored, transform4x3f *out)
{
    transform4x3f translation;
    transform4x3f rotation;
    transform4x3f temporary;
    identity_transform_ab(&translation);
    translation.position = *position;
    rotation.scale = 1.0f;
    rotation.forward = *forward;
    rotation.left.i = up->j * forward->k - forward->j * up->k;
    rotation.left.j = forward->i * up->k - up->i * forward->k;
    rotation.left.k = forward->j * up->i - forward->i * up->j;
    rotation.up = *up;
    rotation.position.x = 0.0f;
    rotation.position.y = 0.0f;
    rotation.position.z = 0.0f;
    if (mirrored)
    {
        rotation.left.i = 0.0f - rotation.left.i;
        rotation.left.j = 0.0f - rotation.left.j;
        rotation.left.k = 0.0f - rotation.left.k;
    }
    if (scale != 1.0f)
    {
        identity_transform_ab(&temporary);
        temporary.scale = scale;
        function_142a60(&rotation, &temporary, &rotation);
    }
    transform4x3f const *base;
    if (parent)
    {
        if (parent->scale != 1.0f || parent_mirrored)
        {
            temporary = *parent;
            if (temporary.scale != 1.0f)
            {
                translation.position.x *= temporary.scale;
                translation.position.y *= temporary.scale;
                translation.position.z *= temporary.scale;
                temporary.scale = 1.0f;
            }
            if (parent_mirrored)
            {
                temporary.left.i = 0.0f - temporary.left.i;
                temporary.left.j = 0.0f - temporary.left.j;
                temporary.left.k = 0.0f - temporary.left.k;
            }
            parent = &temporary;
        }
        function_142a60(parent, &translation, out);
        base = out;
    }
    else
        base = &translation;
    function_142a60(base, &rotation, out);
}

extern long g_4e7414;
extern bool g_4e7411;
extern long g_4de2fc;
extern bool g_4de2f8;
extern long g_4de300[0x800];
short __stdcall function_14a5b0(short cluster_index, point3f const *point, real radius, long maximum_count, short *clusters);

static inline bool cluster_sphere_accept_ab(long object, s_object_cluster_reference const *record, dword type_mask, point3f const *position, real radius)
{
    if (type_mask & (1 << record->type))
    {
        long index = object & 0xffff;
        if (g_4de300[index] != g_4de2fc)
        {
            g_4de300[index] = g_4de2fc;
            real z = record->center.z - position->z;
            real y = record->center.y - position->y;
            real x = record->center.x - position->x;
            real combined = record->radius + radius;
            return combined * combined >= z * z + y * y + x * x;
        }
    }
    return false;
}

// @retail 0xbb050
short __stdcall function_bb050(long mask, dword type_mask, void const *location, point3f const *position, real radius, long *objects, short maximum_count)
{
    short count = 0;
    if (!type_mask) type_mask = 0xffffffff;
    if (!mask) mask = NONE;
    short cluster = *(short *)((byte const *)location + 4);
    short cluster_count = 0;
    short clusters[0x200];
    if (cluster != NONE)
    {
        if (radius > 0.0f)
        {
            ++g_4e7414;
            g_4e7411 = true;
            cluster_count = function_14a5b0(cluster, position, radius, 0x200, clusters);
            g_4e7411 = false;
            if (cluster_count > 0x200) cluster_count = 0x200;
        }
        else
        {
            cluster_count = 1;
            clusters[0] = cluster;
        }
    }
    ++g_4de2fc;
    g_4de2f8 = true;
    for (short i = 0; i < cluster_count; ++i)
    {
        short cluster = clusters[i];
        s_object_cluster_reference *record = 0;
        if (mask & 1)
        {
            s_object_cluster_iterator iterator;
            for (long object = function_b8940(cluster, &record, &iterator); object != NONE;
                 object = object_cluster_next_ab((s_record_pool *)g_4de2e4, &iterator, &record))
            {
                if (cluster_sphere_accept_ab(object, record, type_mask, position, radius))
                {
                    if (count >= maximum_count) goto done;
                    objects[count++] = object;
                }
            }
        }
        record = 0;
        if (mask & 2)
        {
            s_object_cluster_iterator iterator;
            for (long object = function_b8a10(cluster, &record, &iterator); object != NONE;
                 object = object_cluster_next_ab((s_record_pool *)g_4de2d8, &iterator, &record))
            {
                if (cluster_sphere_accept_ab(object, record, type_mask, position, radius))
                {
                    if (count >= maximum_count) goto done;
                    objects[count++] = object;
                }
            }
        }
    }
done:
    g_4de2f8 = false;
    return count;
}

#include "unknown_1cafc0.h"
#include <math.h>
struct s_16760c_render_model;
bool __stdcall function_bab40(long object_index, long name, real *value);

// @retail 0xbd970
void function_bd970(long object_index, s_16760c_render_model *render_model, s_animation_state *state, long node_mask, long node_count, byte *orientations)
{
    short index = NONE;
    c_animation_channel channel;
    for (;;)
    {
        s_graph_tag *graph = graph_tag_get(state->graph_tag_index);
        short next = index + 1;
        if (next >= graph->unknown44_count)
            break;
        index = next;
        s_graph_element44 *entry = &graph->unknown44[index];
        c_type_709360 animation_id = entry->animation_id;
        long name = *(long *)(entry->unknown08 + 4);
        short kind = *(short *)(entry->unknown08 + 2);
        if (animation_id.index != NONE)
            function_1dd9d0(graph, animation_id);
        if (name && state->graph_tag_index != NONE && state->channel_start(&channel, animation_id, NONE, NONE, NONE, NONE, 0x7f))
        {
            s_animation *animation = 0;
            if (channel.animation_id.index != NONE)
                animation = function_1daea0(graph_tag_get(channel.graph_tag_index), channel.animation_id);
            real value;
            function_bab40(object_index, name, &value);
            if (kind == 0)
            {
                channel.set_frame_position((animation->frame_count - 1) * value);
                channel.sample(1.0f, (dword const *)node_mask, node_count, (real_quaternion_transform *)orientations);
            }
            else if (kind == 1)
            {
                real frame = (real)fmod((double)((dword)(g_510c54->game_time + object_index)) * g_510c54->rate * 0.03333333507180214f, (double)animation->frame_count);
                channel.set_frame_position(frame);
                channel.sample(value, (dword const *)node_mask, node_count, (real_quaternion_transform *)orientations);
            }
        }
    }
}

struct rigid_transform_scaled
{
    quaternionf rotation;
    point3f position;
    real scale;
};
transform4x3f *function_b8bd0(long object_index, short node_index);
void function_141590(transform4x3f const *in, transform4x3f *out);
quaternionf *function_141f60(matrix3x3 const *matrix, quaternionf *out);
void orientation_from_matrix4x3(transform4x3f const *matrix, rigid_transform_scaled *out);

// @retail 0xbfa40
void function_bfa40(long object_index, long node_mask)
{
    byte *object = (byte *)((s_object_transform_header *)g_4e0300->data)[object_index & 0xffff].object;
    if (*(short *)(object + 0x112) == NONE)
        return;
    byte *definition = g_4e3b44[*(long *)object & 0xffff].bytes;
    long model_index = *(long *)(definition + 0x38);
    if (model_index == NONE)
        return;
    byte *model = g_4e3b44[model_index & 0xffff].bytes;
    if (*(long *)(model + 4) == NONE)
        return;
    transform4x3f *matrices = (transform4x3f *)(object + *(short *)(object + 0x116));
    rigid_transform_scaled *orientations = (rigid_transform_scaled *)(object + *(short *)(object + 0x112));
    long count = (dword)(long)*(short *)(object + 0x110) / sizeof(rigid_transform_scaled);
    long parent_index = *(long *)(object + 0x14);
    transform4x3f *parent = 0;
    bool parent_mirrored = false;
    if (parent_index != NONE)
    {
        parent = function_b8bd0(parent_index, *(char *)(object + 0x18));
        byte *local_be682a = (byte *)((s_object_transform_header *)g_4e0300->data)[parent_index & 0xffff].object;
        parent_mirrored = (*(dword *)(local_be682a + 4) >> 10) & 1;
    }
    transform4x3f root, inverse_root, inverse_parent, relative;
    function_bdc40((point3f *)(object + 0x64), (vector3f *)(object + 0x70), (vector3f *)(object + 0x7c),
        *(real *)(object + 0xa0), (*(dword *)(object + 4) >> 10) & 1, parent, parent_mirrored, &root);
    function_141590(&root, &inverse_root);
    for (long i = 0; i < count; ++i)
    {
        if (!node_mask || (((dword *)node_mask)[i >> 5] & (1 << (i & 31))))
        {
            short node_parent = *(short *)(*(byte **)(model + 0x7c) + i * 0x5c + 4);
            if (node_parent != NONE)
            {
                function_141590(&matrices[node_parent], &inverse_parent);
                function_142a60(&inverse_parent, &matrices[i], &relative);
                function_141f60(&relative.rotation, &orientations[i].rotation);
                orientations[i].position = relative.position;
                orientations[i].scale = relative.scale;
            }
            else
            {
                function_142a60(&inverse_root, &matrices[i], &relative);
                orientation_from_matrix4x3(&relative, &orientations[i]);
            }
        }
    }
}

long bit_vector_highest_set_bit(dword const *bits, long bit_count);
void function_109050(long object_index, long a, long b, long c);
struct s_1d9240;
void function_1d9470(s_1d9240 const *p, s_blend_orientation const *targets, long count, dword const *mask,
    s_blend_orientation *orientations);

// @retail 0xbdb60
void function_bdb60(long object_index, s_16760c_render_model *render_model, s_animation_state *state,
    long node_mask, long node_count, byte *orientations)
{
    if (node_mask)
    {
        long highest = bit_vector_highest_set_bit((dword const *)node_mask, node_count);
        if (highest >= 0 && highest + 1 < node_count)
            node_count = highest + 1;
    }
    if (node_count)
    {
        c_animation_channel *channel = &state->channels[0];
        if (state->graph_tag_index != NONE && channel->graph_tag_index != NONE && channel->animation_id.index != NONE)
        {
            function_1daea0(graph_tag_get(channel->graph_tag_index), channel->animation_id);
            state->sample((long)render_model, 1.0f, (dword const *)node_mask,
                (real_quaternion_transform *)orientations, 0, object_index, node_count);
        }
        function_bd970(object_index, render_model, state, node_mask, node_count, orientations);
        function_109050(object_index, node_mask, node_count, (long)orientations);
        if (state->unknown64.unknown1 && !(state->unknown64.unknown3 & 2))
        {
            byte *object = (byte *)((s_object_transform_header *)g_4e0300->data)[object_index & 0xffff].object;
            s_blend_orientation *targets = (s_blend_orientation *)(object + *(short *)(object + 0x10e));
            function_1d9470((s_1d9240 const *)&state->unknown64, targets, (short)node_count,
                (dword const *)node_mask, (s_blend_orientation *)orientations);
        }
    }
}


bool __stdcall function_1c4c50(long object_index, long node_index, point3f const *point,
    vector3f const *change_a, vector3f const *change_b, long *result_object,
    vector3f *linear, vector3f *angular);
void function_b7360(long object_index);
void function_bba20(long object_index);

// @retail 0xb7880
void __stdcall function_b7880(long object_index, long node_index, point3f const *point,
    vector3f const *impulse, vector3f const *angular_impulse)
{
    long result_object;
    vector3f linear, angular;
    if (function_1c4c50(object_index, node_index, point, impulse, angular_impulse,
        &result_object, &linear, &angular))
    {
        s_velocity_object *object = ((s_velocity_object_header *)g_4e0300->data)[result_object & 0xffff].object;
        *((byte *)object + 0xc1) &= ~1;
        function_b7360(result_object);
        function_bba20(result_object);
        object->linear_velocity = linear;
        object->angular_velocity = angular;
    }
}
