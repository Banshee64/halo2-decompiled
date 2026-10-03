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

class c_animation_channel
{
public:
	c_animation_channel();
	void reset();
	void clear();
	c_animation_channel *copy_from(c_animation_channel const *other);
	bool set(long graph_tag_index, word flags, c_animation_id animation_id, long unknown08, byte unknown0c,
		byte unknown0d, char unknown0e);

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
