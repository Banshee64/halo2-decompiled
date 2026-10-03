// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1C62F0.CPP: the animation channels (0x1c62f0..0x1c7450) */

#include "cseries.h"
#include "globals.h"
#include "unknown_1c62f0.h"

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
		return function_1dafc0(graph, channel->animation_id.graph_index);
	}
	return graph;
}

// @retail 0x1c7380
void c_animation_channel_data_get(c_animation_channel const *channel, s_animation_data *data)
{
	function_1ddb40(data, graph_tag_get(channel->graph_tag_index), channel->animation_id);
}

#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

/* in this file, not decompiled yet (stubbed in src/stubs/lane_c.cpp) */
void function_1c69b0(c_animation_channel *channel);
void __stdcall function_1c66a0(c_animation_channel *channel, real frame, long a, long b, long c);

// @retail 0x1c6440
s_animation *c_animation_channel::get_animation() const
{
	s_animation *animation = NULL;

	if (animation_id.index != NONE)
	{
		animation = function_1daea0(graph_tag_get(graph_tag_index), animation_id);
	}
	return animation;
}

// @retail 0x1c6500
void c_animation_channel::set_frame_last()
{
	s_animation *animation = get_animation();
	real frame = (real)(animation->frame_count - 1) + 0.0001f;

	if (0.0f > frame)
	{
		frame = 0.0f;
	}
	frame_position = frame;
	unknown11 |= 0xa;
	unknown10 = 0;
}

// @retail 0x1c6560
void c_animation_channel::set_frame_position(real frame)
{
	s_animation *animation = get_animation();

	frame_position = PIN(frame, 0.0f, (real)animation->frame_count - 0.0001f);
	if ((flags & 1) && !(unknown11 & 9))
	{
		if (g_510c54->rate * rate * 30.0f + frame_position >= (real)animation->frame_count - 0.0001f)
		{
			unknown11 |= 2;
		}
		else
		{
			unknown11 &= ~2;
		}
	}
	function_1c69b0(this);
}

// @retail 0x1c6620
void c_animation_channel::set_frame_ratio(real ratio)
{
	s_animation *animation = get_animation();
	real last_frame = (real)(animation->frame_count - 1) + 0.0001f;
	real frame = last_frame * ratio;

	set_frame_position(PIN(frame, 0.0f, last_frame));
}

// @retail 0x1c68c0
void c_animation_channel::update(long a, long b, long c)
{
	unknown14 = 0;
	unknown16 = 0;
	if (unknown10 > 0)
	{
		unknown10--;
	}
	else if ((flags & 1) && !(unknown11 & 9))
	{
		function_1c66a0(this, g_510c54->rate * rate * 30.0f + frame_position, a, b, c);
	}
}

// @retail 0x1c6920
void c_animation_channel::set_frame_ratio_and_advance(real ratio, long a, long b, long c)
{
	s_animation *animation = get_animation();
	real last_frame;
	real frame;

	unknown10 = 0;
	last_frame = (real)(animation->frame_count - 1) + 0.0001f;
	frame = last_frame * ratio;
	function_1c66a0(this, PIN(frame, 0.0f, last_frame), a, b, c);
}

// @retail 0x1c6e30
real c_animation_channel::get_frame_ratio() const
{
	real result = 0.0f;

	if (animation_id.index != NONE)
	{
		real frame_count = (real)get_animation()->frame_count;

		if (frame_count > 0.0f)
		{
			real ratio = frame_position / frame_count;

			result = PIN(ratio, 0.0f, 1.0f);
		}
	}
	return result;
}

// @retail 0x1c6ea0
real c_animation_channel::get_duration() const
{
	real result = 0.0f;

	if (animation_id.index != NONE)
	{
		result = (real)get_animation()->frame_count * (1.0f / 30.0f);
	}
	return result;
}

// @retail 0x1c6ee0
real c_animation_channel::get_event_time() const
{
	real result = -1.0f;

	if (animation_id.index != NONE)
	{
		long frame = function_1dae20(get_animation());

		if (frame != NONE)
		{
			result = (real)frame * (1.0f / 30.0f);
		}
	}
	return result;
}

// @retail 0x1c7300
bool c_animation_channel::is_unflagged0() const
{
	bool result = true;

	if (graph_tag_index != NONE && animation_id.index != NONE)
	{
		result = !TEST_FIELD_BIT(get_animation()->flag0);
	}
	return result;
}

// @retail 0x1c7340
bool c_animation_channel::is_unflagged6() const
{
	bool result = true;

	if (graph_tag_index != NONE && animation_id.index != NONE)
	{
		result = !TEST_FIELD_BIT(get_animation()->flag6);
	}
	return result;
}
