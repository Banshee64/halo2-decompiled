// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_279D80.CPP: sampling an animation's nodes, and 256-bit node masks.

   function_2798a0 fills the sampling state (0x504450..0x5044b0) the codecs'
   channel decoders read, then runs the decoders of the animation's static
   and animated data through function_279860. */

#include "cseries.h"
#include "animation_graph.h"
#include "real_math.h"
#include "unknown_11cb00.h"
#include <math.h>
#include <string.h>
#include <xmmintrin.h>

/* the channel decoders of a codec: rotation, translation and scale */
struct s_animation_samplers
{
	void (*rotation)(void);
	void (*translation)(void);
	void (*scale)(void);
};

/* an animation codec (0x28 bytes); its decoders sample a frame, or
   interpolate between two */
struct s_animation_codec
{
	char const *name;
	long unknown04;
	long unknown08;
	s_animation_samplers samplers[2];
	char (__stdcall *unknown24)(long a, long b, long c, long d);
};

/* the codecs' channel decoders (unknown_28c470.cpp, unknown_28c510.cpp,
   unknown_28cdb0.cpp, unknown_2c4d60.cpp) */
void function_28c470(void);
void function_28c4e0(void);
void function_28c510(void);
void function_28c530(void);
void function_28c5d0(void);
void function_28c6b0(void);
void function_28c710(void);
void function_28c750(void);
void function_28c790(void);
void function_28c880(void);
void function_28c960(void);
void function_28c9e0(void);
void function_28cb70(void);
void function_28ccc0(void);
void function_28cdb0(void);
void function_28cf40(void);
void function_28d090(void);
void function_28d180(void);
void function_28d320(void);
void function_28d470(void);
void function_28d560(void);
void function_28d6f0(void);
void function_28d840(void);
void function_2c4d60(void);
void function_2c4da0(void);
void function_2c4de0(void);
char __stdcall function_28d170(long a, long b, long c, long d);

s_animation_codec const g_47fb18[9] =
{
	{ "_no_compression_codec", 1, 0, { { NULL, NULL, NULL }, { NULL, NULL, NULL } }, function_28d170 },
	{ "_uncompressed_static_data_codec", 0, 0,
		{ { function_28c470, function_28c4e0, function_28c510 }, { function_28c470, function_28c4e0, function_28c510 } },
		function_28d170 },
	{ "_uncompressed_animated_data_codec", 1, 0,
		{ { function_2c4d60, function_2c4da0, function_2c4de0 }, { function_2c4d60, function_2c4da0, function_2c4de0 } },
		function_28d170 },
	{ "_8byte_quantized_rotation_only_codec", 1, 0,
		{ { function_28c960, function_28c750, function_2c4de0 }, { function_28c790, function_28c880, function_28c6b0 } },
		function_28d170 },
	{ "byte_keyframe_lightly_quantized", 2, 1,
		{ { function_28cdb0, function_28cf40, function_28d090 }, { function_28c9e0, function_28cb70, function_28ccc0 } },
		function_28d170 },
	{ "word_keyframe_lightly_quantized", 2, 2,
		{ { function_28d560, function_28d6f0, function_28d840 }, { function_28d180, function_28d320, function_28d470 } },
		function_28d170 },
	{ "reverse_byte_keyframe_lightly_quantized", 2, 1,
		{ { function_28cdb0, function_28cf40, function_28d090 }, { function_28c9e0, function_28cb70, function_28ccc0 } },
		function_28d170 },
	{ "reverse_word_keyframe_lightly_quantized", 2, 2,
		{ { function_28d560, function_28d6f0, function_28d840 }, { function_28d180, function_28d320, function_28d470 } },
		function_28d170 },
	{ "_blend_screen_codec", 1, 0,
		{ { function_28c710, function_28c750, function_2c4de0 }, { function_28c530, function_28c5d0, function_28c6b0 } },
		function_28d170 },
};

/* the sampling state (the decoders read it; g_504464, g_50446c, g_504480 are
   in globals.h, g_504468 in unknown_28c510.cpp) */
extern long g_504468;
long g_504450;
long g_504454;
bool g_504458;
long g_50445c;
long g_504460;
real g_504470;
s_animation *g_504474;
s_graph_inheritance *g_504478;
dword const *g_50447c;
s_animation_samplers g_504484;
byte *g_504490;
byte *g_504494;
byte *g_504498;
real_quaternion_transform *g_50449c;
bool g_5044a0;
long g_5044a4;
long g_5044a8;
real g_5044ac;
real g_5044b0;

/* the nodes the object-space parent nodes set: rotation, translation and
   scale */
dword g_55e530[8];
dword g_55e550[8];
dword g_55e570[8];

/* the combination of two node masks */
dword g_55e590[8];

/* not decompiled yet (src/stubs/lane_c.cpp) */
void function_279860(void);
bool __stdcall function_27a100(s_graph_tag *graph, s_animation *animation, s_graph_inheritance *inheritance,
	long node_count, real_quaternion_transform *transforms);

void node_mask_and(dword *mask, dword const *other);

PRIVATE inline long animation_data_size(s_animation_data_sizes const *sizes)
{
	return sizes->static_node_flags_size + sizes->animated_node_flags_size + sizes->movement_data_size +
		sizes->unknown04 + sizes->static_data_size + sizes->unknown08 + sizes->animated_data_size;
}

PRIVATE inline void node_mask_clear(dword *mask)
{
	long i;

	for (i = 0; i < 8; i++)
	{
		mask[i] = 0;
	}
}

#define PIN(value, minimum, maximum) ((value) < (minimum) ? (minimum) : ((value) > (maximum) ? (maximum) : (value)))

// @retail 0x2798a0
void function_2798a0(s_animation_data *data, real frame, real weight, s_graph_tag *graph, s_animation *animation,
	long node_count, s_graph_inheritance *inheritance, dword const *node_mask, real_quaternion_transform *transforms,
	bool interpolate, bool blend, real blend_frame, real blend_weight)
{
	if (weight > 0.0001f)
	{
		long frame_index = real_truncate(frame);
		long last_frame = animation->frame_count - 1;

		node_mask_clear(g_55e530);
		node_mask_clear(g_55e550);
		node_mask_clear(g_55e570);
		g_50445c = node_count;
		g_504460 = node_count;
		frame_index = PIN(frame_index, 0, last_frame);
		g_504478 = inheritance;
		g_50447c = node_mask;
		g_504484.rotation = NULL;
		g_504484.translation = NULL;
		g_504484.scale = NULL;
		g_504470 = 1.0f;
		g_504464 = frame_index;
		g_504468 = frame_index;
		g_50446c = 0.0f;
		g_504474 = animation;
		g_504480 = NULL;
		g_504490 = NULL;
		g_504494 = NULL;
		g_504498 = NULL;
		g_50449c = transforms;
		g_5044a0 = blend;
		if (blend)
		{
			long blend_frame_index = real_truncate(blend_frame);

			g_5044a4 = PIN(blend_frame_index, 0, last_frame);
			g_5044a8 = PIN(blend_frame_index + 1, 0, last_frame);
			g_5044ac = blend_frame - (real)g_5044a4;
			g_504468 = PIN(g_504464 + 1, 0, last_frame);
			g_50446c = frame - (real)g_504464;
			g_5044b0 = blend_weight;
		}
		else if (interpolate)
		{
			if (frame_index != last_frame)
			{
				long next_frame_index = frame_index + 1;
				real fraction;

				g_504468 = PIN(next_frame_index, 0, last_frame);
				fraction = frame - (real)frame_index;
				g_50446c = fraction;
				if (fraction < 0.0001f)
				{
					g_50446c = 0.0f;
					interpolate = false;
				}
				else if (fraction > 0.9999f)
				{
					g_504464 = next_frame_index;
					g_50446c = 0.0f;
					interpolate = false;
				}
			}
			else
			{
				interpolate = false;
			}
		}
		g_504454 = 0;
		if (g_504478)
		{
			g_504454 = (inheritance->flags & 1) ? 2 : 1;
			g_50445c = g_504474->node_count;
		}
		else
		{
			g_50445c = g_504474->node_count;
			if (g_504460 <= g_504474->node_count)
			{
				g_50445c = g_504460;
			}
		}
		g_504450 = 0;
		{
			bool full = weight > 0.9999f ? true : false;

			if (animation->type == 1)
			{
				g_504450 = full ? 2 : 3;
				g_504470 = weight;
			}
			else if (!full)
			{
				g_504450 = 1;
				g_504470 = weight;
			}
		}
		if (animation->type == 0 && data->sizes->static_data_size != 0)
		{
			long flags_size;

			g_504480 = (struct s_animation_data *)data->data;
			g_504484 = g_47fb18[*data->data].samplers[0];
			g_504490 = data->data + data->sizes->static_data_size + data->sizes->animated_data_size;
			flags_size = ((data->node_count + 31) >> 3) & ~3;
			g_504494 = data->data + data->sizes->static_data_size + data->sizes->animated_data_size + flags_size;
			flags_size = ((data->node_count + 31) >> 3) & ~3;
			g_504498 = data->data + data->sizes->static_data_size + data->sizes->animated_data_size + flags_size * 2;
			g_504458 = false;
			function_279860();
		}
		if (data->sizes->animated_data_size != 0)
		{
			byte *animated_data;
			long flags_size;

			if (g_504450 == 0 && function_27a100(graph, animation, inheritance, node_count, transforms))
			{
				g_504450 = 4;
			}
			animated_data = data->data + data->sizes->static_data_size;
			g_504480 = (struct s_animation_data *)animated_data;
			g_504484 = g_47fb18[*animated_data].samplers[interpolate ? 1 : 0];
			g_504490 = data->data + data->sizes->static_data_size + data->sizes->animated_data_size +
				data->sizes->static_node_flags_size;
			flags_size = ((data->node_count + 31) >> 3) & ~3;
			g_504494 = data->data + data->sizes->static_data_size + data->sizes->animated_data_size +
				data->sizes->static_node_flags_size + flags_size;
			flags_size = ((data->node_count + 31) >> 3) & ~3;
			g_504498 = data->data + data->sizes->static_data_size + data->sizes->animated_data_size +
				data->sizes->static_node_flags_size + flags_size * 2;
			g_504458 = interpolate;
			function_279860();
		}
	}
}

// @retail 0x279d40
dword const *node_masks_combine(dword const *mask, dword const *other)
{
	dword const *result = NULL;

	if (mask)
	{
		if (other && mask != other)
		{
			memcpy(g_55e590, mask, sizeof(g_55e590));
			node_mask_and(g_55e590, other);
			result = g_55e590;
		}
		else
		{
			result = mask;
		}
	}
	else if (other)
	{
		result = other;
	}
	return result;
}

// @retail 0x279d80
void function_279d80(s_graph_tag *graph, c_animation_id animation_id, long node_count, real frame, real weight,
	s_graph_inheritance *inheritance, dword const *node_mask, real_quaternion_transform *transforms, bool interpolate)
{
	s_animation *animation = function_1daea0(graph, animation_id);
	s_animation_data data;
	long size;
	byte *address;

	function_1ddb40(&data, graph, animation_id);
	size = animation_data_size(data.sizes);
	if (size > 0x400)
	{
		size = 0x400;
	}
	for (address = data.data; address < data.data + size; address += 0x20)
	{
		_mm_prefetch((char const *)address, _MM_HINT_T0);
	}
	function_2798a0(&data, frame, weight, graph, animation, node_count, inheritance, node_mask, transforms, interpolate,
		false, 0.0f, 0.0f);
}

// @retail 0x279e40
void function_279e40(s_aiming_screen const *screen, s_graph_tag *graph, c_animation_id animation_id, long node_count,
	real yaw, real pitch, real weight, s_graph_inheritance *inheritance, dword const *node_mask,
	real_quaternion_transform *transforms)
{
	s_animation *animation = function_1daea0(graph, animation_id);
	s_animation_data data;
	long right_frame_count;
	long left_frame_count;
	long yaw_frame_count;
	long down_frame_count;
	long up_frame_count;
	real yaw_per_frame;
	real yaw_frames;
	real yaw_frame;
	real pitch_per_frame;
	real pitch_frames;
	real pitch_frame;
	real lower_row;
	real upper_row;
	real lower_frame;
	real upper_frame;
	real fraction;

	function_1ddb40(&data, graph, animation_id);
	right_frame_count = screen->right_frame_count;
	left_frame_count = screen->left_frame_count;
	yaw_frame_count = right_frame_count + left_frame_count + 1;
	down_frame_count = screen->down_frame_count;
	up_frame_count = screen->up_frame_count;

	yaw_per_frame = yaw < 0.0f ? screen->right_yaw_per_frame : screen->left_yaw_per_frame;
	yaw_frames = yaw_per_frame < 0.0001f ? 0.0f : yaw / yaw_per_frame;
	yaw_frames = PIN(yaw_frames, (real)-right_frame_count, (real)left_frame_count);
	yaw_frame = (real)right_frame_count + yaw_frames;

	pitch_per_frame = pitch < 0.0f ? screen->down_pitch_per_frame : screen->up_pitch_per_frame;
	pitch_frames = pitch_per_frame < 0.0001f ? 0.0f : pitch / pitch_per_frame;
	pitch_frames = PIN(pitch_frames, (real)-down_frame_count, (real)up_frame_count);
	pitch_frame = (real)down_frame_count + pitch_frames;

	lower_row = (real)floor(pitch_frame);
	upper_row = (real)ceil(pitch_frame);
	lower_row = PIN(lower_row, 0.0f, (real)(down_frame_count + up_frame_count));
	upper_row = PIN(upper_row, 0.0f, (real)(down_frame_count + up_frame_count));
	lower_frame = (real)yaw_frame_count * lower_row + yaw_frame;
	upper_frame = (real)yaw_frame_count * upper_row + yaw_frame;
	fraction = pitch_frame - lower_row;
	if (fraction < 0.0001f)
	{
		function_2798a0(&data, lower_frame, weight, graph, animation, node_count, inheritance, node_mask, transforms,
			true, false, 0.0f, 0.0f);
	}
	else if (upper_row - pitch_frame < 0.0001f)
	{
		function_2798a0(&data, upper_frame, weight, graph, animation, node_count, inheritance, node_mask, transforms,
			true, false, 0.0f, 0.0f);
	}
	else
	{
		function_2798a0(&data, lower_frame, weight, graph, animation, node_count, inheritance, node_mask, transforms,
			true, true, upper_frame, fraction);
	}
}

// @retail 0x27a060
void function_27a060(s_graph_tag *graph, c_animation_id animation_id, long node_count, real ratio, real weight,
	s_graph_inheritance *inheritance, dword const *node_mask, real_quaternion_transform *transforms)
{
	s_animation *animation = function_1daea0(graph, animation_id);
	s_animation_data data;
	real last_frame;
	real frame;

	function_1ddb40(&data, graph, animation_id);
	last_frame = (real)(animation->frame_count - 1) + 0.0001f;
	frame = last_frame * ratio;
	frame = PIN(frame, 0.0f, last_frame);
	function_2798a0(&data, frame, weight, graph, animation, node_count, inheritance, node_mask, transforms, true,
		false, 0.0f, 0.0f);
}

// @retail 0x27a380
void node_mask_and(dword *mask, dword const *other)
{
	long i;

	for (i = 0; i < 8; i++)
	{
		mask[i] &= other[i];
	}
}
