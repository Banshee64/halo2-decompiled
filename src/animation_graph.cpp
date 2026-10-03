// @flags /O2 /arch:SSE /Gr
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
	s_graph_tag *result = NULL;

	if (graph_index < graph->inheritance_count)
	{
		s_graph_inheritance *inheritance = &graph->inheritance[graph_index];

		if (inheritance->graph_tag_index != NONE)
		{
			result = graph_tag_get(inheritance->graph_tag_index);
		}
	}
	return result;
}

// @retail 0x1dd840
void function_1dd840(s_graph_tag *graph, long animation_index)
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
			function_1dd840(animation_graph, animation->next_animation);
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
			animation = graph_animation_get(function_1dafc0(graph, animation_id.graph_index), animation_id.index);
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
	return function_1dadb0(animation, 0);
}

// @retail 0x1dae50
short function_1dae50(s_animation const *animation)
{
	return function_1dadb0(animation, 1);
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
			graph = function_1dafc0(graph, animation_id.graph_index);
		}
		animation = graph_animation_get(graph, animation_id.index);
		if (animation->blend_screen != NONE)
		{
			result = &graph->blend_screens[animation->blend_screen].right_yaw_per_frame;
		}
	}
	return result;
}

// @retail 0x1db120
void *function_1db120(s_graph_tag *graph, long mode, long weapon_class, long weapon_type)
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
c_animation_id *function_1dd5d0(s_graph_tag *graph, c_animation_id *result, c_animation_id animation_id)
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
				*result = parent_id;
				return result;
			}
		}
	}
	*result = animation_id;
	return result;
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
			c_animation_id parent_id;

			if (animation_id.graph_index != NONE)
			{
				animation_graph = function_1dafc0(graph, animation_id.graph_index);
			}
			animation_id = *function_1dd5d0(graph, &parent_id, animation_id);
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
		graph = function_1dafc0(graph, animation_id.graph_index);
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
		*animation_graph = function_1dafc0(graph, animation_id.graph_index);
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
		function_1ddab0(function_1dafc0(graph, i));
	}
}

// @retail 0x1ddb40
void function_1ddb40(s_animation_data *data, s_graph_tag *graph, c_animation_id animation_id)
{
	if (animation_id.index != NONE)
	{
		s_animation *animation = function_1daea0(graph, animation_id);

		data->data = function_1dd7c0(graph, animation_id);
		data->sizes = animation->sizes;
		data->node_count = animation->node_count;
		data->frame_info_type = animation->frame_info_type;
		data->frame_count = animation->frame_count;
	}
}
