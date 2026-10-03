// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FB350.CPP: the recorded animations of objects (entries 55.. of the
   subsystem table at 0x441594) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"

/* one recorded animation playing on an object (0xa0 bytes) */
struct s_recorded_animation
{
	short salt;
	byte unknown02[2];
	long object_index;
	word ticks;
	word flags;
	long unknown0c;
	long unknown10;
	byte unknown14[0x9c - 0x14];
	short unknown9c;
	byte unknown9e[2];
};

s_data_array *g_4f5724;

// @retail 0x1fb2f0
void recorded_animations_initialize(void)
{
	g_4f5724 = data_new_inlined("recorded animations", 1, sizeof(s_recorded_animation), 0, g_510c2c);
}

// @retail 0x1fb330
void recorded_animations_initialize_for_new_map(void)
{
	g_4f5724->valid = true;
	data_delete_all(g_4f5724);
}

// @retail 0x1fb350
void recorded_animations_dispose_from_old_map(void)
{
	g_4f5724->valid = false;
}

/* whether a recorded animation that has not finished plays on the object */
// @retail 0x1fb6f0
bool recorded_animation_playing(long object_index)
{
	s_data_iterator iterator;
	s_recorded_animation *animation;
	bool result = false;

	iterator.data = g_4f5724;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while ((animation = (s_recorded_animation *)data_iterator_next_inlined(&iterator)) != 0)
	{
		if (animation->object_index == object_index && !(animation->flags & 1))
		{
			result = true;
			break;
		}
	}

	return result;
}

/* the recorded animation of an object, and its datum index */
// @retail 0x1fb760
s_recorded_animation *recorded_animation_find(long object_index, long *datum_index)
{
	s_data_iterator iterator;
	s_recorded_animation *animation;
	s_recorded_animation *result = 0;
	long found_index = NONE;

	iterator.data = g_4f5724;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while ((animation = (s_recorded_animation *)data_iterator_next_inlined(&iterator)) != 0)
	{
		if (animation->object_index == object_index)
		{
			result = animation;
			found_index = iterator.datum_index;
			break;
		}
	}

	if (datum_index)
		*datum_index = found_index;

	return result;
}

/* the frames (at 30 a second) the object's recorded animation has played */
// @retail 0x1fb4c0
long recorded_animation_get_frames(long object_index)
{
	real frames;
	long result = 0;
	s_recorded_animation *animation = recorded_animation_find(object_index, 0);

	if (animation && animation->object_index == object_index)
	{
		frames = (real)animation->ticks * g_510c54->rate * 30.0f;

		__asm
		{
			fld frames
			fistp result
		}
	}

	return result;
}
