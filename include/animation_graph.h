/* ANIMATION_GRAPH.H: the animation graph tag and the lookups of its
   animations (src/animation_graph.cpp, 0x1dacb0..0x1ddea0).

   An animation is named by a c_animation_id: the graph it is in (NONE for the
   graph itself, else an index into the graph's inheritance list) and its index
   in that graph's animation block. An animation's data may sit in a streamed
   resource of the graph (the cache of unknown_123680.cpp); the lookups request
   the resources of the animations they touch. */

#ifndef ANIMATION_GRAPH_H
#define ANIMATION_GRAPH_H

#include "cseries.h"
#include "globals.h"

struct s_cache_resource;

struct c_animation_id
{
	short graph_index;
	short index;

	c_animation_id() : graph_index(NONE), index(NONE) {}
};

/* a frame event of an animation (type 0 and 1 are the feet) */
struct s_animation_event
{
	short type;
	short frame;
};

/* the sizes of the parts of an animation's data (0x10 bytes) */
struct s_animation_data_sizes
{
	char static_node_flags_size;
	char animated_node_flags_size;
	short movement_data_size;
	short unknown04;
	short static_data_size;
	long unknown08;
	long animated_data_size;
};

/* an animation (0x6c bytes) */
struct s_animation
{
	long name;
	byte unknown04[0xc];
	byte type;
	char frame_info_type;
	char blend_screen;
	byte node_count;
	short frame_count;
	byte internal_flags;
	byte unknown17;
	byte flag0 : 1;
	byte unknown18_1 : 5;
	byte flag6 : 1;
	byte unknown18_7 : 1;
	byte unknown19[3];
	real weight;
	byte unknown20[4];
	long resource_index;
	long resource_offset;
	byte unknown2c[4];
	short parent_animation;
	short next_animation;
	long data_size;
	byte *data;
	s_animation_data_sizes sizes;
	long event_count;
	s_animation_event *events;
	long sound_event_count;
	short *sound_events;
	byte unknown5c[0x10];
};

/* a graph the graph inherits from (0x20 bytes) */
struct s_graph_inheritance
{
	long group_tag;
	long graph_tag_index;
	long node_map_count;
	void *node_map;
	long node_map_flag_count;
	void *node_map_flags;
	byte unknown18[8];
};

/* a blend screen (0x1c bytes) */
struct s_blend_screen
{
	long name;
	real right_yaw_per_frame;
	real left_yaw_per_frame;
	short right_frame_count;
	short left_frame_count;
	real down_pitch_per_frame;
	real up_pitch_per_frame;
	short down_frame_count;
	short up_frame_count;
};

/* a node of the graph's skeleton (0x20 bytes) */
struct s_graph_node
{
	long name;
	short next_sibling_index;
	short first_child_index;
	short parent_index;
	byte flags;
	byte joint_flags;
	byte unknown0c[0x14];
};

/* the data of an animation as the codecs read it */
struct s_animation_data
{
	byte *data;
	s_animation_data_sizes *sizes;
	byte node_count;
	char frame_info_type;
	short frame_count;

	s_animation_data() : data(NULL), sizes(NULL), node_count(0), frame_info_type(0) {}
};

/* the graph tag */
struct s_graph_tag
{
	byte unknown00[0xc];
	long node_count;
	s_graph_node *nodes;
	byte unknown14[0x28 - 0x14];
	s_blend_screen *blend_screens;
	long animation_count;
	s_animation *animations;
	long mode_count;
	void *modes;
	byte unknown3c[0x4c - 0x3c];
	long inheritance_count;
	s_graph_inheritance *inheritance;
	byte unknown54[0xac - 0x54];
	long resource_count;
	s_cache_resource *resources;
};

inline s_graph_tag *graph_tag_get(long tag_index)
{
	return (s_graph_tag *)g_4e3b44[tag_index & 0xffff].bytes;
}

inline s_animation *graph_animation_get(s_graph_tag *graph, long index)
{
	s_animation *animation = NULL;

	if (index != NONE)
	{
		animation = &graph->animations[index];
	}
	return animation;
}

/* the graph an animation id's graph index names (inlined copies; the
   out-of-line one is function_1dafc0) */
inline s_graph_tag *graph_inherited_get(s_graph_tag *graph, long graph_index)
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

/* the frame of an animation's first event of a type, or NONE (inlined
   copies; the out-of-line one is function_1dadb0) */
inline short animation_event_frame_get(s_animation const *animation, long type)
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

s_animation *function_1daea0(s_graph_tag *graph, c_animation_id animation_id);
s_graph_inheritance *function_1daff0(s_graph_tag *graph, c_animation_id animation_id);
s_graph_tag *function_1dafc0(s_graph_tag *graph, long graph_index);
void function_1dd840(s_graph_tag *graph, long animation_index);
void function_1dd9d0(s_graph_tag *graph, c_animation_id animation_id);
short function_1dadb0(s_animation const *animation, long type);
short function_1dade0(s_animation const *animation, long type, long frame);
short function_1dae20(s_animation const *animation);
short function_1dae50(s_animation const *animation);
long function_1dae80(s_animation const *animation);
real *function_1daf30(s_graph_tag *graph, c_animation_id animation_id);
void *function_1db120(s_graph_tag *graph, long mode, long weapon_class, long weapon_type);
long function_1dd490(s_graph_tag *graph, long flags);
bool function_1dd4c0(long render_model_tag_index, s_graph_tag *graph, long *node_count, long *node_map);
c_animation_id function_1dd5d0(s_graph_tag *graph, c_animation_id animation_id);
c_animation_id *function_1dd630(s_graph_tag *graph, c_animation_id *result, c_animation_id animation_id, bool first_seed);
byte *function_1dd7c0(s_graph_tag *graph, c_animation_id animation_id);
void function_1dd880(s_graph_tag *graph, c_animation_id animation_id, s_graph_tag **animation_graph, s_animation **animation);
void function_1ddab0(s_graph_tag *graph);
void function_1ddaf0(s_graph_tag *graph);
void function_1ddb40(s_animation_data *data, s_graph_tag *graph, c_animation_id animation_id);
void function_1ddd00(s_graph_tag *graph, long mode, long weapon_class, long weapon_type, bool urgent, bool other);

#endif
