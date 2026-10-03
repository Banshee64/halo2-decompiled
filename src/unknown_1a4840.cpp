// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1A4840.CPP: a buffer window that advances with a position, and a
   two-term falloff */

#include "cseries.h"

/* a circular window over a stream: entries start..start+count (mod size)
   cover the stream up to position */
struct s_stream_window
{
	byte unknown00[0xc];
	long position;
	long size;
	long start;
	long count;
};

// @retail 0x1a4840
void stream_window_advance(s_stream_window *window, long position)
{
	long delta = position - window->position;

	if (delta > 0)
	{
		if (window->count >= delta)
		{
			window->start = (window->start + delta) % window->size;
			window->count -= delta;
		}
		window->position = position;
	}
}

/* 1 up to half of maximum, falling linearly to 0 at maximum */
static inline real falloff(real value, real maximum)
{
	real minimum = maximum * 0.5f;

	if (value >= maximum)
		return 0.0f;
	if (minimum >= value)
		return 1.0f;
	return (maximum - value) / (maximum - minimum);
}

// @retail 0x1a4870
real function_1a4870(real distance, real maximum_distance, real angle, real maximum_angle)
{
	real distance_scale = falloff(distance, maximum_distance);

	return falloff(angle, maximum_angle) * distance_scale;
}
