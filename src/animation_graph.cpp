// @flags /O2 /Ob1 /arch:SSE /Gr
/* ANIMATION_GRAPH.CPP: the animation graph tag's lookups (0x1dacb0..0x1ddea0) */

#include "cseries.h"
#include "globals.h"
#include "animation_graph.h"
#include "unknown_123680.h"
#include "real_math.h"

/* a binary search of a sorted block (unknown_1dd560.cpp) */
struct s_sorted_array;
void *function_1dd560(s_sorted_array *array, long key, long element_size);

// @retail 0x1dafc0
s_graph_tag *function_1dafc0(s_graph_tag *graph, long graph_index)
{
	return graph_inherited_get(graph, graph_index);
}

/* requests the resource of an animation (inlined copies; the out-of-line one
   is function_1dd840) */
inline void graph_animation_request(s_graph_tag *graph, long animation_index)
{
	s_animation *animation = graph_animation_get(graph, animation_index);

	if (animation->internal_flags & 0x10)
	{
		s_cache_resource *resource = &graph->resources[animation->resource_index];

		if (resource->streamed)
		{
			function_1236f0(resource, true);
		}
	}
}

// @retail 0x1dd840
void function_1dd840(s_graph_tag *graph, long animation_index)
{
	graph_animation_request(graph, animation_index);
}

// @retail 0x1dd9d0
void function_1dd9d0(s_graph_tag *graph, c_animation_id animation_id)
{
	if (g_510c20 && g_510c21 && animation_id.index != NONE)
	{
		s_graph_tag *animation_graph = graph;
		s_animation *animation;

		if (animation_id.graph_index != NONE)
		{
			animation_graph = function_1dafc0(graph, animation_id.graph_index);
		}
		animation = graph_animation_get(animation_graph, animation_id.index);
		if (animation->parent_animation != NONE)
		{
			animation = graph_animation_get(animation_graph, animation->parent_animation);
		}
		function_1dd840(animation_graph, animation_id.index);
		while (animation->next_animation != NONE)
		{
			graph_animation_request(animation_graph, animation->next_animation);
			animation = graph_animation_get(animation_graph, animation->next_animation);
		}
	}
}

// @retail 0x1daea0
s_animation *function_1daea0(s_graph_tag *graph, c_animation_id animation_id)
{
	s_animation *animation = NULL;

	if (animation_id.index != NONE)
	{
		if (animation_id.graph_index == NONE)
		{
			animation = graph_animation_get(graph, animation_id.index);
		}
		else
		{
			animation = graph_animation_get(graph_inherited_get(graph, animation_id.graph_index), animation_id.index);
		}
		if (animation)
		{
			function_1dd9d0(graph, animation_id);
		}
	}
	return animation;
}

// @retail 0x1daff0
s_graph_inheritance *function_1daff0(s_graph_tag *graph, c_animation_id animation_id)
{
	s_graph_inheritance *result = NULL;

	if (animation_id.index != NONE && animation_id.graph_index >= 0 && animation_id.graph_index < graph->inheritance_count)
	{
		s_graph_inheritance *inheritance;

		function_1dd9d0(graph, animation_id);
		inheritance = &graph->inheritance[animation_id.graph_index];
		if (inheritance->graph_tag_index != NONE && inheritance->node_map_flag_count > 0 && inheritance->node_map_count > 0 &&
			inheritance->node_map_flags && inheritance->node_map)
		{
			result = inheritance;
		}
		else if (inheritance->graph_tag_index != NONE)
		{
			function_1daea0(graph, animation_id);
		}
	}
	return result;
}

// @retail 0x1dadb0
short function_1dadb0(s_animation const *animation, long type)
{
	long i;

	for (i = 0; i < animation->event_count; i++)
	{
		s_animation_event const *event = &animation->events[i];

		if (event->type == type)
		{
			return event->frame;
		}
	}
	return NONE;
}

// @retail 0x1dade0
short function_1dade0(s_animation const *animation, long type, long frame)
{
	long i;

	for (i = 0; i < animation->event_count; i++)
	{
		s_animation_event const *event = &animation->events[i];

		if (event->type == type && event->frame > frame)
		{
			return event->frame;
		}
	}
	return NONE;
}

// @retail 0x1dae20
short function_1dae20(s_animation const *animation)
{
	return animation_event_frame_get(animation, 0);
}

// @retail 0x1dae50
short function_1dae50(s_animation const *animation)
{
	return animation_event_frame_get(animation, 1);
}

// @retail 0x1dae80
long function_1dae80(s_animation const *animation)
{
	long result = NONE;

	if (animation->sound_event_count > 0)
	{
		result = animation->sound_events[0];
	}
	return result;
}

// @retail 0x1daf30
real *function_1daf30(s_graph_tag *graph, c_animation_id animation_id)
{
	real *result = NULL;

	if (animation_id.index != NONE)
	{
		s_animation *animation;

		function_1dd9d0(graph, animation_id);
		if (animation_id.graph_index != NONE)
		{
			graph = graph_inherited_get(graph, animation_id.graph_index);
		}
		animation = graph_animation_get(graph, animation_id.index);
		if (animation->blend_screen != NONE)
		{
			result = &graph->blend_screens[animation->blend_screen].right_yaw_per_frame;
		}
	}
	return result;
}

/* the animations of a mode, weapon class and weapon type (inlined copies;
   the out-of-line one is function_1db120) */
inline void *graph_weapon_type_get(s_graph_tag *graph, long mode, long weapon_class, long weapon_type)
{
	void *result = NULL;
	byte *mode_entry = (byte *)function_1dd560((s_sorted_array *)&graph->mode_count, mode, 0x14);

	if (mode_entry)
	{
		byte *weapon_class_entry = (byte *)function_1dd560((s_sorted_array *)(mode_entry + 4), weapon_class, 0x14);

		if (weapon_class_entry)
		{
			result = function_1dd560((s_sorted_array *)(weapon_class_entry + 4), weapon_type, 0x34);
		}
	}
	return result;
}

// @retail 0x1db120
void *function_1db120(s_graph_tag *graph, long mode, long weapon_class, long weapon_type)
{
	return graph_weapon_type_get(graph, mode, weapon_class, weapon_type);
}

// @retail 0x1dd490
long function_1dd490(s_graph_tag *graph, long flags)
{
	long i;

	for (i = 0; i < graph->node_count; i++)
	{
		if ((graph->nodes[i].flags & flags) == flags)
		{
			return i;
		}
	}
	return NONE;
}

/* a node of a render model (0x60 bytes) */
struct s_render_model_node
{
	long name;
	byte unknown04[0x5c];
};

struct s_render_model_nodes
{
	byte unknown00[0x48];
	long node_count;
	s_render_model_node *nodes;
};

// @retail 0x1dd4c0
bool function_1dd4c0(long render_model_tag_index, s_graph_tag *graph, long *node_count, long *node_map)
{
	s_render_model_nodes *render_model = (s_render_model_nodes *)g_4e3b44[render_model_tag_index & 0xffff].bytes;
	bool result = true;
	long i;

	*node_count = render_model->node_count;
	for (i = 0; i < render_model->node_count; i++)
	{
		s_render_model_node *node = &render_model->nodes[i];
		long node_index = NONE;
		long j;

		for (j = 0; j < graph->node_count; j++)
		{
			if (node->name == graph->nodes[j].name)
			{
				node_index = j;
				break;
			}
		}
		node_map[i] = node_index;
		if (node_index == NONE)
		{
			result = false;
		}
	}
	return result;
}

// @retail 0x1dd5d0
c_animation_id function_1dd5d0(s_graph_tag *graph, c_animation_id animation_id)
{
	c_animation_id parent_id = animation_id;

	if (animation_id.index != NONE)
	{
		s_animation *animation = function_1daea0(graph, animation_id);

		if (animation->parent_animation != NONE || animation->next_animation != NONE)
		{
			short parent_index = function_1daea0(graph, animation_id)->parent_animation;

			if (parent_index != NONE)
			{
				parent_id.index = parent_index;
				return parent_id;
			}
		}
	}
	return animation_id;
}

// @retail 0x1dd630
c_animation_id *function_1dd630(s_graph_tag *graph, c_animation_id *result, c_animation_id animation_id, bool first_seed)
{
	if (animation_id.index != NONE)
	{
		s_animation *animation = function_1daea0(graph, animation_id);

		if (animation->parent_animation != NONE || animation->next_animation != NONE)
		{
			s_graph_tag *animation_graph = graph;

			if (animation_id.graph_index != NONE)
			{
				animation_graph = function_1dafc0(graph, animation_id.graph_index);
			}
			animation_id = function_1dd5d0(graph, animation_id);
			animation = graph_animation_get(animation_graph, animation_id.index);
			if (1.0f > animation->weight)
			{
				real random = _real_random(first_seed ? &g_4e7408->unknown0 : &g_4e7408->seed, NULL, 0);

				while (animation->next_animation != NONE && animation->weight < random)
				{
					animation_id.index = animation->next_animation;
					animation = graph_animation_get(animation_graph, animation_id.index);
				}
			}
		}
		if (animation_id.index != NONE)
		{
			function_1dd9d0(graph, animation_id);
		}
	}
	*result = animation_id;
	return result;
}

// @retail 0x1dd7a0
void function_1dd7a0(long *reference)
{
	reference[0] = NONE;
	reference[1] = NONE;
	((short *)reference)[4] = NONE;
	((short *)reference)[5] = 0;
	reference[3] = NONE;
	reference[4] = NONE;
}

// @retail 0x1dd7c0
byte *function_1dd7c0(s_graph_tag *graph, c_animation_id animation_id)
{
	s_animation *animation;

	if (animation_id.graph_index != NONE)
	{
		graph = graph_inherited_get(graph, animation_id.graph_index);
	}
	animation = graph_animation_get(graph, animation_id.index);
	if (animation->internal_flags & 0x10)
	{
		s_cache_resource *resource = &graph->resources[animation->resource_index];

		return (byte *)function_1237e0(resource, animation->name) + animation->resource_offset;
	}
	return animation->data;
}

// @retail 0x1dd880
void function_1dd880(s_graph_tag *graph, c_animation_id animation_id, s_graph_tag **animation_graph, s_animation **animation)
{
	*animation_graph = NULL;
	*animation = NULL;
	if (animation_id.graph_index == NONE)
	{
		*animation_graph = graph;
	}
	else
	{
		*animation_graph = graph_inherited_get(graph, animation_id.graph_index);
	}
	*animation = graph_animation_get(*animation_graph, animation_id.index);
}

// @retail 0x1ddab0
void function_1ddab0(s_graph_tag *graph)
{
	long i;

	for (i = 0; i < graph->resource_count; i++)
	{
		s_cache_resource *resource = &graph->resources[i];

		if (resource->streamed)
		{
			function_1236f0(resource, false);
		}
	}
}

// @retail 0x1ddaf0
void function_1ddaf0(s_graph_tag *graph)
{
	long i;

	function_1ddab0(graph);
	for (i = 0; i < graph->inheritance_count; i++)
	{
		function_1ddab0(graph_inherited_get(graph, i));
	}
}

// @retail 0x1ddb40
void function_1ddb40(s_animation_data *data, s_graph_tag *graph, c_animation_id animation_id)
{
	if (animation_id.index != NONE)
	{
		s_animation *animation = function_1daea0(graph, animation_id);

		data->data = function_1dd7c0(graph, animation_id);
		data->sizes = &animation->sizes;
		data->node_count = animation->node_count;
		data->frame_info_type = animation->frame_info_type;
		data->frame_count = animation->frame_count;
	}
}

/* the animations of a weapon type (0x34 bytes): the resources they need
   first and the rest */
struct s_graph_weapon_type
{
	byte unknown00[0x24];
	long urgent_resource_count;
	long *urgent_resources;
	long resource_count;
	long *resources;
};

// @retail 0x1ddb90
void function_1ddb90(s_graph_tag *graph, long mode, long weapon_class, long weapon_type, bool urgent, bool other)
{
	if (urgent || other)
	{
		s_graph_weapon_type *animations = (s_graph_weapon_type *)graph_weapon_type_get(graph, mode, weapon_class, weapon_type);

		if (animations)
		{
			long i;

			if (urgent)
			{
				for (i = 0; i < animations->urgent_resource_count; i++)
				{
					s_cache_resource *resource = &graph->resources[animations->urgent_resources[i]];

					if (resource->streamed)
					{
						function_1236f0(resource, true);
					}
				}
			}
			if (other)
			{
				for (i = 0; i < animations->resource_count; i++)
				{
					s_cache_resource *resource = &graph->resources[animations->resources[i]];

					if (resource->streamed)
					{
						function_1236f0(resource, false);
					}
				}
			}
		}
	}
}

// @retail 0x1ddc70
void function_1ddc70(s_graph_tag *graph, long mode, long weapon_class, long weapon_type, bool urgent, bool other)
{
	if (urgent || other)
	{
		function_1ddb90(graph, mode, weapon_class, weapon_type, urgent, other);
		function_1ddb90(graph, mode, weapon_class, 0x30000d9, urgent, other);
		function_1ddb90(graph, mode, 0x30000d9, weapon_type, urgent, other);
		function_1ddb90(graph, mode, 0x30000d9, 0x30000d9, urgent, other);
		function_1ddb90(graph, 0x30000d9, 0x30000d9, 0x30000d9, urgent, other);
	}
}

// @retail 0x1ddd00
void function_1ddd00(s_graph_tag *graph, long mode, long weapon_class, long weapon_type, bool urgent, bool other)
{
	if (urgent || other)
	{
		long i;

		function_1ddc70(graph, mode, weapon_class, weapon_type, urgent, other);
		for (i = 0; i < graph->inheritance_count; i++)
		{
			function_1ddc70(graph_inherited_get(graph, i), mode, weapon_class, weapon_type, urgent, other);
		}
	}
}

// @retail 0x1dacb0
void function_1dacb0(s_graph_tag *graph, c_animation_id animation_id, real *distance, real *event_distance)
{
	real total = 0.0f;
	real total_at_event = 0.0f;
	s_animation *animation = function_1daea0(graph, animation_id);
	s_animation_data data;
	s_animation_data_sizes *sizes;

	function_1ddb40(&data, graph, animation_id);
	sizes = data.sizes;
	if (sizes->movement_data_size != 0)
	{
		real *movement = (real *)(data.data + sizes->static_node_flags_size + sizes->animated_node_flags_size +
			sizes->static_data_size + sizes->animated_data_size);
		short event_frame = animation_event_frame_get(animation, 0);
		short frame;

		for (frame = 0; frame < animation->frame_count; frame++)
		{
			switch (animation->frame_info_type)
			{
			case 1:
			case 2:
			case 3:
				total += *movement++;
				break;
			}
			if (frame == event_frame)
			{
				total_at_event = total;
			}
		}
	}
	if (distance)
	{
		*distance = total;
	}
	if (event_distance)
	{
		*event_distance = total_at_event;
	}
}

// @retail 0x1db070
c_animation_id *s_graph_tag::variant_find(c_animation_id *result, long name, char a, char b, long c, long d, char e, char f, char g)
{
	c_animation_id animation_id;
	long i;

	for (i = 0; i < sound_reference_count; i++)
	{
		s_graph_sound_reference *reference = &sound_references[i];

		if (reference->name == name && reference->unknown0a == a && reference->unknown0b == b)
		{
			long j;

			for (j = 0; j < reference->variant_count; j++)
			{
				s_graph_sound_variant *variant = &reference->variants[j];

				if (variant->unknown04 == c && variant->unknown08 == d && variant->unknown0e == e &&
					variant->unknown0f == f && variant->unknown0c == g)
				{
					*result = variant->animation_id;
					return result;
				}
			}
		}
	}
	*result = animation_id;
	return result;
}

// @retail 0x1dceb0
bool function_1dceb0(s_graph_iterator3c *iterator, s_graph_tag *graph)
{
	short index = iterator->next_index + 1;

	if (index < graph->unknown3c_count)
	{
		s_graph_element3c *element;

		iterator->next_index = index;
		element = &graph->unknown3c[index];
		iterator->animation_id = element->animation_id;
		iterator->unknown10 = element->unknown14;
		iterator->unknown0c = element->unknown10;
		iterator->unknown00 = element->unknown08;
		iterator->unknown08 = element->unknown0c;
		iterator->index = index;
		iterator->unknown04 = element->unknown18;
		iterator->unknown1c = element->unknown24;
		iterator->unknown18 = element->unknown20;
		iterator->unknown14 = element->unknown1c;
		if (iterator->animation_id.index != NONE)
		{
			function_1dd9d0(graph, iterator->animation_id);
		}
		return true;
	}
	return false;
}

/* the animation of the given name in the graph or the graphs it inherits
   from, the graph first */
// @retail 0x1dd0b0
c_animation_id *function_1dd0b0(s_graph_tag *graph, c_animation_id *result, long name)
{
	c_animation_id animation_id;

	if (graph)
	{
		s_graph_tag *current = graph;
		long graph_index = NONE;

		do
		{
			long index;

			if (animation_id.index != NONE)
			{
				break;
			}
			for (index = 0; index < current->animation_count; index++)
			{
				s_animation *animation = NULL;

				if (index != NONE)
				{
					animation = &current->animations[index];
				}
				if (animation->name == name)
				{
					animation_id.graph_index = (short)graph_index;
					animation_id.index = (short)index;
					break;
				}
			}
			if (animation_id.index == NONE)
			{
				s_graph_inheritance *inheritance;

				graph_index++;
				if (graph_index >= graph->inheritance_count)
				{
					break;
				}
				inheritance = &graph->inheritance[graph_index];
				current = NULL;
				if (inheritance->graph_tag_index != NONE)
				{
					current = graph_tag_get(inheritance->graph_tag_index);
				}
			}
		}
		while (current);
		if (animation_id.index != NONE)
		{
			function_1dd9d0(graph, animation_id);
		}
	}
	*result = animation_id;
	return result;
}

/* an orientation (as unknown_141590.cpp declares it) */
struct real_orientation
{
	real_quaternion rotation;
	real_point3d position;
	real scale;
};

void __stdcall function_1421f0(real_matrix4x3 *out, real_orientation const *orientation);
int __fastcall function_142a60(real_matrix4x3 const *a, real_matrix4x3 const *b, real_matrix4x3 *result);

/* the node matrices of the graph's skeleton from the nodes' orientations,
   the root node's relative to the given matrix */
// @retail 0x1dd1c0
void function_1dd1c0(s_graph_tag *graph, real_matrix4x3 *matrices, real_orientation const *orientations, real_matrix4x3 const *root)
{
	long node_indices[255];
	real_matrix4x3 matrix;
	long count;
	long i = 0;

	if (graph->node_count > 0)
	{
		count = 1;
		node_indices[0] = 0;
		do
		{
			long node_index = node_indices[i++];
			s_graph_node *node = &graph->nodes[node_index];
			real_matrix4x3 const *parent;

			if (node_index == 0)
			{
				parent = root;
			}
			else
			{
				parent = &matrices[node->parent_index];
			}
			function_1421f0(&matrix, &orientations[node_index]);
			function_142a60(parent, &matrix, &matrices[node_index]);
			if (node->next_sibling_index != NONE)
			{
				node_indices[count++] = node->next_sibling_index;
			}
			if (node->first_child_index != NONE)
			{
				node_indices[count++] = node->first_child_index;
			}
		}
		while (i != count);
	}
}
