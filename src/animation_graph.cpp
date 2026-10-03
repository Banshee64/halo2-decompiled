// @flags /O2 /arch:SSE /Gr
/* ANIMATION_GRAPH.CPP: the animation graph tag's lookups (0x1dacb0..0x1ddea0) */

#include "cseries.h"
#include "globals.h"
#include "animation_graph.h"
#include "unknown_123680.h"

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
