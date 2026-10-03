// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1C62F0.CPP: the animation channels (0x1c62f0..0x1c7450) */

#include "cseries.h"
#include "globals.h"
#include "unknown_1c62f0.h"

/* a graph tag: the graphs it inherits from (0x20 byte entries, the tag
   index at +4) */
struct s_graph_inheritance
{
	byte unknown00[4];
	long graph_tag_index;
	byte unknown08[0x18];
};

struct s_graph_tag
{
	byte unknown00[0x4c];
	long inheritance_count;
	s_graph_inheritance *inheritance;
};

inline s_graph_tag *graph_tag_get(long tag_index)
{
	return (s_graph_tag *)g_4e3b44[tag_index & 0xffff].bytes;
}

// @retail 0x1c62f0
c_animation_channel::c_animation_channel()
{
	reset();
}

// @retail 0x1c6340
void c_animation_channel::reset()
{
	graph_tag_index = NONE;
	animation_id.graph_index = NONE;
	animation_id.index = NONE;
	frame_position = 0.0f;
	unknown10 = 0;
	unknown11 = 0;
	flags = 0;
	rate = 1.0f;
	unknown14 = 0;
	unknown16 = 0;
	unknown08 = NONE;
	unknown0e = NONE;
	unknown0c = NONE;
	unknown0d = NONE;
}

// @retail 0x1c6390
c_animation_channel *c_animation_channel::copy_from(c_animation_channel const *other)
{
	graph_tag_index = other->graph_tag_index;
	animation_id = other->animation_id;
	unknown08 = other->unknown08;
	unknown0c = other->unknown0c;
	unknown0d = other->unknown0d;
	unknown0e = other->unknown0e;
	unknown10 = other->unknown10;
	unknown11 = other->unknown11;
	flags = other->flags;
	unknown14 = other->unknown14;
	unknown16 = other->unknown16;
	rate = other->rate;
	frame_position = other->frame_position;
	return this;
}

// @retail 0x1c63f0
void c_animation_channel::clear()
{
	graph_tag_index = NONE;
	animation_id.graph_index = NONE;
	animation_id.index = NONE;
	frame_position = 0.0f;
	unknown10 = 0;
	unknown11 = 0;
	flags = 0;
	rate = 1.0f;
	unknown14 = 0;
	unknown16 = 0;
	unknown08 = NONE;
	unknown0c = NONE;
	unknown0d = NONE;
	unknown0e = NONE;
}

// @retail 0x1c6470
bool c_animation_channel::set(long graph_tag_index, word flags, c_animation_id animation_id, long unknown08,
	byte unknown0c, byte unknown0d, char unknown0e)
{
	if (graph_tag_index != NONE && (short)(unknown08 >> 16) != NONE)
	{
		this->animation_id.graph_index = NONE;
		this->animation_id.index = NONE;
		this->unknown10 = 0;
		this->unknown11 = 0;
		this->unknown14 = 0;
		this->unknown16 = 0;
		this->unknown08 = unknown08;
		this->unknown0c = unknown0c;
		this->flags = flags;
		this->unknown0d = unknown0d;
		this->unknown0e = unknown0e;
		this->graph_tag_index = graph_tag_index;
		this->frame_position = 0.0f;
		this->rate = 1.0f;
		this->animation_id = animation_id;
		this->unknown10 = (flags >> 14) & 2;
		return true;
	}
	return false;
}

// @retail 0x1c6bb0
s_graph_tag *c_animation_channel_get_graph(c_animation_channel const *channel)
{
	s_graph_tag *graph = graph_tag_get(channel->graph_tag_index);

	if (channel->animation_id.graph_index != NONE)
	{
		s_graph_tag *inherited = NULL;

		if (channel->animation_id.graph_index < graph->inheritance_count)
		{
			long tag_index = graph->inheritance[channel->animation_id.graph_index].graph_tag_index;

			if (tag_index != NONE)
			{
				inherited = graph_tag_get(tag_index);
			}
		}
		return inherited;
	}
	return graph;
}

void *__stdcall function_1ddb40(void *graph, c_animation_id animation_id);

// @retail 0x1c7380
void *c_animation_channel_animation_get(c_animation_channel const *channel)
{
	c_animation_id animation_id = channel->animation_id;

	return function_1ddb40(g_4e3b44[channel->graph_tag_index & 0xffff].bytes, animation_id);
}
