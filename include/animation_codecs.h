/* ANIMATION_CODECS.H: the animation codecs (their table g_47fb18 is in
   src/unknown_279d80.cpp) */

#ifndef ANIMATION_CODECS_H
#define ANIMATION_CODECS_H

#include "cseries.h"
#include "real_math.h"

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

extern s_animation_codec const g_47fb18[9];

/* a decoded node orientation (0x20 bytes; the decoders write it through
   g_5044c0) */
struct s_animation_output
{
	real_quaternion rotation;
	real_vector3d vector;
	real scale;
};

#endif
