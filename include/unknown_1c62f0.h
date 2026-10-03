/* UNKNOWN_1C62F0.H: the animation channels (src/unknown_1c62f0.cpp) and the
   object that holds three of them with the graph's tag index (src/unknown_1cafc0.cpp).

   A channel plays one animation of a graph tag (the tag instance's data has
   the animation block at +0x4c; 0x1daea0 returns an animation, whose frame
   count is the short at +0x14). */

#ifndef UNKNOWN_1C62F0_H
#define UNKNOWN_1C62F0_H

#include "cseries.h"

/* an animation of a graph: graph_index (-1 unset) and the animation's index */
struct c_animation_id
{
	short graph_index;
	short index;

	c_animation_id() : graph_index(NONE), index(NONE) {}
};

/* an animation of a graph (0x6c bytes): its frame count, flags and the
   frames of its events */
struct s_animation_event
{
	short type;
	short frame;
};

struct s_animation
{
	byte unknown00[0x14];
	short frame_count;
	byte unknown16[2];
	byte flag0 : 1;
	byte unknown18_1 : 5;
	byte flag6 : 1;
	byte unknown18_7 : 1;
	byte unknown19[0x4c - 0x19];
	long event_count;
	s_animation_event *events;
	byte unknown54[0x6c - 0x54];
};

struct s_graph_tag;

/* the animation of a graph tag (0x1daea0, not decompiled yet) */
s_animation *function_1daea0(s_graph_tag *graph, c_animation_id animation_id);

class c_animation_channel
{
public:
	c_animation_channel();
	void reset();
	void clear();
	c_animation_channel *copy_from(c_animation_channel const *other);
	bool set(long graph_tag_index, word flags, c_animation_id animation_id, long unknown08, byte unknown0c,
		byte unknown0d, char unknown0e);
	s_animation *get_animation() const;
	void set_frame_last();
	void set_frame_position(real frame);
	void set_frame_ratio(real ratio);
	void update(long a, long b, long c);
	void set_frame_ratio_and_advance(real ratio, long a, long b, long c);
	real get_frame_ratio() const;
	real get_duration() const;
	real get_event_time() const;
	bool is_unflagged0() const;
	bool is_unflagged6() const;

	long graph_tag_index;
	c_animation_id animation_id;
	long unknown08;
	char unknown0c;
	char unknown0d;
	short unknown0e;
	byte unknown10;
	byte unknown11;
	word flags;
	short unknown14;
	short unknown16;
	real rate;
	real frame_position;
};

#endif
