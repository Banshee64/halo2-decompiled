// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_18C250.CPP: the sound source callbacks built with /arch:SSE
   (the tables are in unknown_18c810.cpp) */

#include "cseries.h"
#include "globals.h"
#include "sound_sources.h"
#include <math.h>

static inline real_vector3d *cross_product3d(real_vector3d const *a, real_vector3d const *b, real_vector3d *result)
{
	result->i = b->k * a->j - a->k * b->j;
	result->j = a->k * b->i - a->i * b->k;
	result->k = a->i * b->j - a->j * b->i;
	return result;
}

static inline real_vector3d *rotate_vector_about_axis(real_vector3d *vector, real_vector3d const *axis, real sine, real cosine)
{
	real_vector3d cross;

	cross_product3d(axis, vector, &cross);
	vector->i = cosine * vector->i + sine * cross.i;
	vector->j = cosine * vector->j + sine * cross.j;
	vector->k = cosine * vector->k + sine * cross.k;
	return vector;
}

// @retail 0x18c250
bool __stdcall function_18c250(long object_index, long tag_index, long a, s_sound_location *location)
{
	bool result = object_index == NONE || function_18c3b0(object_index, tag_index, a, location);

	if (object_index == NONE || result)
	{
		s_sound_tag *sound = (s_sound_tag *)((s_tag_instance_view *)g_4e3b44)[tag_index & 0xffff].data;
		s_sound_class_view *sound_class = &((s_sound_globals_view *)g_51ebd4)->classes[sound->class_index];
		s_sound_class_spatialization *class_spatialization = sound_class_get_spatialization(sound_class);

		if (class_spatialization && (class_spatialization->flags & 1))
		{
			location->flag0 = true;
			location->up = *g_4687a4;
			location->forward = *g_4687a8;
			rotate_vector_about_axis(&location->forward, g_4687b0, (real)sin(class_spatialization->angle), (real)cos(class_spatialization->angle));
		}
	}
	return result;
}

