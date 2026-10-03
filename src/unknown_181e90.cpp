// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_181E90.CPP: whether the ratio of two lengths lies outside the
   allowed range */

#include "cseries.h"

/* the smallest allowed ratio (set at run time) */
real g_54e870;

// @retail 0x181e90
bool function_181e90(real numerator, real denominator)
{
	if (!(numerator < 0.001f) && !(denominator < 0.001f))
	{
		real ratio = numerator / denominator;
		real pinned = ratio < g_54e870 ? g_54e870 : (ratio > 16.0f ? 16.0f : ratio);
		if (pinned != ratio)
		{
			return true;
		}
	}
	return false;
}
