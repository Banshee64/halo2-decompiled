// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1FB350.CPP: the recorded animations of objects (entries 55.. of the
   subsystem table at 0x441594) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "recorded_animations.h"

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

struct animation_playback_controller;
struct playback_unit_control_view;

/* the event-stream codecs (recorded_animation_playback.cpp and
   recorded_animation_playback_v1.cpp) */
void __stdcall recorded_animation_initialize_event_stream(animation_playback_controller *controller,
	playback_unit_control_view *control, byte const **cursor, byte version);
bool __stdcall recorded_animation_apply_event_stream(animation_playback_controller *controller,
	playback_unit_control_view *control, long *remaining_ticks, byte const **cursor);
void __stdcall recorded_animation_initialize_event_stream_v1(animation_playback_controller *controller,
	playback_unit_control_view *control, byte const **cursor, byte version);
bool __stdcall recorded_animation_apply_event_stream_v1(animation_playback_controller *controller,
	playback_unit_control_view *control, long *remaining_ticks, byte const **cursor);

struct s_recorded_animation_version
{
	void (__stdcall *initialize)(animation_playback_controller *controller,
		playback_unit_control_view *control, byte const **cursor, byte version);
	bool (__stdcall *apply)(animation_playback_controller *controller,
		playback_unit_control_view *control, long *remaining_ticks, byte const **cursor);
};

/* retail's g_46fd54 and g_46fd4c: recorded_animation_playback_v1.cpp and
   recorded_animation_playback.cpp define these pairs as const objects with
   internal linkage, so this file keeps its own copies until they are shared */
static s_recorded_animation_version const recorded_animation_version_v1 =
{
	recorded_animation_initialize_event_stream_v1,
	recorded_animation_apply_event_stream_v1
};

static s_recorded_animation_version const recorded_animation_version_current =
{
	recorded_animation_initialize_event_stream,
	recorded_animation_apply_event_stream
};

/* the codecs of each recording version (1..4) */
s_recorded_animation_version const *const g_46fd5c[4] =
{
	&recorded_animation_version_v1,
	&recorded_animation_version_v1,
	&recorded_animation_version_v1,
	&recorded_animation_version_current
};

/* a cutscene recording of the scenario (0x34 bytes) */
struct s_scenario_recorded_animation
{
	byte unknown00[0x20];
	byte version;
	byte unknown21;
	byte control_version;
	byte unknown23;
	short length_ticks;
	byte unknown26[0x30 - 0x26];
	byte const *event_stream;
};

struct s_scenario_recorded_animations_view
{
	byte unknown000[0x110];
	long recorded_animation_count;
	s_scenario_recorded_animation *recorded_animations;
};

struct s_recorded_animation_object
{
	byte unknown000[0x134];
	dword flags;
};

struct s_recorded_animation_object_header
{
	byte unknown00[8];
	s_recorded_animation_object *object;
};

#define RECORDED_ANIMATION_OBJECT(index) (((s_recorded_animation_object_header *)g_4e0300->data)[(index) & 0xffff].object)
#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

bool function_cbf40(long unit_index);
void __stdcall function_cbf60(long unit_index, bool active);

inline void recorded_animation_object_set_flag(long object_index, long bit, bool value)
{
	dword *flags = &RECORDED_ANIMATION_OBJECT(object_index)->flags;
	*flags = value ? (*flags | FLAG(bit)) : (*flags & ~FLAG(bit));
}

// @retail 0x1fb360
bool function_1fb360(long unit_index, short recording_index, long flags)
{
	bool result = false;

	if (unit_index != NONE && recording_index != NONE)
	{
		s_scenario_recorded_animations_view *scenario = (s_scenario_recorded_animations_view *)g_4e0350;

		if (recording_index < scenario->recorded_animation_count)
		{
			long datum_index;
			s_recorded_animation *animation = recorded_animation_find(unit_index, &datum_index);
			s_scenario_recorded_animation *recording = &scenario->recorded_animations[recording_index];

			if (!recorded_animation_playing(unit_index))
			{
				if (!animation)
				{
					long index = datum_new(g_4f5724);

					if (index != NONE)
						animation = &((s_recorded_animation *)g_4f5724->data)[index & 0xffff];
				}

				if (animation)
				{
					animation->object_index = unit_index;
					animation->remaining_ticks = 0;
					animation->ticks = recording->length_ticks;
					animation->cursor = recording->event_stream;
					animation->version = recording->version - 1;
					animation->flags &= ~1;
					g_46fd5c[animation->version]->initialize((animation_playback_controller *)animation->controller,
						(playback_unit_control_view *)animation->control, &animation->cursor, recording->control_version);
					function_cbf60(unit_index, true);
					SET_FLAG(animation->flags, 2, function_cbf40(unit_index));
					recorded_animation_object_set_flag(unit_index, 0, false);
					recorded_animation_object_set_flag(unit_index, 22, true);
					animation->flags |= (word)flags;
					result = true;
				}
			}
		}
	}

	return result;
}

struct s_recorded_animation_object_datum
{
	short salt;
	byte unknown02;
	byte type;
	byte unknown04[4];
	s_recorded_animation_object *object;
};

static inline byte *recorded_animation_datum_try_and_get(s_data_array *data, long datum_index)
{
	byte *result = 0;

	if (datum_index != NONE)
	{
		long index = datum_index & 0xffff;

		if (index < data->high_water_index)
		{
			byte *datum = data->data + data->size * index;

			if (*(short *)datum != 0 && *(short *)datum == (datum_index >> 16))
				result = datum;
		}
	}

	return result;
}

/* the object, if it is a unit (types 0 and 1) */
static inline s_recorded_animation_object *recorded_animation_unit_try_and_get(long object_index)
{
	s_recorded_animation_object_datum *datum = (s_recorded_animation_object_datum *)recorded_animation_datum_try_and_get(g_4e0300, object_index);
	s_recorded_animation_object *result = 0;

	if (datum && ((1 << datum->type) & 3))
		result = datum->object;

	return result;
}

bool __stdcall function_beb30(long object_index);
void __stdcall function_b8540(long a);
void function_c6de0(long object_index, void *control);
void function_f0fd0(long vehicle_index, bool set);

/* advances every recorded animation by a tick; finished ones restore the
   unit's flags and are deleted */
// @retail 0x1fb510
void recorded_animations_update(void)
{
	s_data_iterator iterator;
	s_recorded_animation *animation;

	iterator.data = g_4f5724;
	iterator.index = NONE;
	iterator.datum_index = NONE;
	while ((animation = (s_recorded_animation *)data_iterator_next_inlined(&iterator)) != 0)
	{
		if (recorded_animation_unit_try_and_get(animation->object_index))
		{
			if (!(animation->flags & 1))
			{
				bool finished;

				animation->ticks--;
				finished = !g_46fd5c[animation->version]->apply((animation_playback_controller *)animation->controller,
					(playback_unit_control_view *)animation->control, &animation->remaining_ticks, &animation->cursor);
				animation->remaining_ticks++;
				function_c6de0(animation->object_index, animation->control);
				SET_FLAG(animation->flags, 0, finished);
				continue;
			}

			recorded_animation_object_set_flag(animation->object_index, 0, (animation->flags & 4) != 0);
			recorded_animation_object_set_flag(animation->object_index, 22, false);
			function_cbf60(animation->object_index, false);
			if (animation->flags & 8)
			{
				long object_index = animation->object_index;

				if (object_index != NONE && !function_beb30(object_index))
					function_b8540(object_index);
			}
			if (animation->flags & 0x10)
				function_f0fd0(animation->object_index, true);
		}

		datum_delete(g_4f5724, iterator.datum_index);
	}
}
