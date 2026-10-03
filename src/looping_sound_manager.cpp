// @flags /O2 /Ob1 /arch:SSE /Gr
/* LOOPING_SOUND_MANAGER.CPP: controllers and looping playback. */

#include "cseries.h"
#include "data_array.h"
#include "local_cameras.h"
#include "sound_sources.h"
#include "crc.h"
#include <string.h>
#include <math.h>

class c_looping_sound_controller
{
public:
	short salt;
	byte reference_count : 7;
	byte source_updated : 1;
	byte synch_count : 7;
	byte synch_playback : 1;
	long definition_index;
	dword random_seed;
	real listener_values[4];

	void initialize(long definition_index, dword random_seed, bool source_updated);
	void add_synch_count(long count);
	void update_controller_source(s_sound_location const *source, bool source_updated);
	inline char add_reference(void) { return ++reference_count; }
};

struct looping_sound_datum
{
	short salt;
	short sound_class;
	long definition_index;
	long identifier;
	s_sound_location source;
	byte flags;
	byte playing_sound_count;
	byte state;
	byte update_phase;
	long controller_index;
	long detail_sound_times[12];
	byte track_storage[0xc4 - 0x88];
	real listener_gains[4];
};

/* These globals are owned by sound_manager.cpp. */
extern void *g_51ebd8;
extern void *g_51ebdc;
extern void *g_502110;

#define CONTROLLERS ((s_data_array *)g_51ebd8)
#define LOOPING_SOUNDS ((s_data_array *)g_502110)

struct s_looping_sound_system
{
	byte unknown00[0x78];
	bool initialized;
	bool hardware_available;
	bool enabled;
	byte unknown7b[4];
	byte source_update_phase : 1;
	byte unknown7f : 7;
	byte unknown80[4];
	long time;
	byte unknown88[0x1e0 - 0x88];
	real elapsed_time;
};

static inline bool looping_sound_system_available(void)
{
	s_looping_sound_system *system = (s_looping_sound_system *)g_4e6380;
	return system->initialized && system->hardware_available && system->enabled;
}

static inline c_looping_sound_controller *controller_get(long index)
{
	c_looping_sound_controller *result = (c_looping_sound_controller *)g_51ebdc;
	if (index != NONE)
		result = (c_looping_sound_controller *)CONTROLLERS->data + (index & 0xffff);
	return result;
}

static inline long next_sound_datum(s_data_array *data, long index)
{
	long start = index == NONE ? 0 : (index & 0xffff) + 1;
	return data_datum_index(data, data_next_absolute_index(data, start));
}

// @retail 0x2198f0
c_looping_sound_controller *looping_sound_controller_get(long index)
{
	return controller_get(index);
}

// @retail 0x219910
c_looping_sound_controller *looping_sound_controller_try_and_get(long index)
{
	c_looping_sound_controller *result = (c_looping_sound_controller *)g_51ebdc;
	if (index != NONE)
		result = (c_looping_sound_controller *)datum_get_inlined(CONTROLLERS, index);
	return result;
}

// @retail 0x219960
long looping_sound_controller_find_existing(long definition_index)
{
	long result = NONE;
	if (looping_sound_system_available() && definition_index != NONE)
	{
		s_data_array *data = CONTROLLERS;
		for (result = next_sound_datum(data, NONE); result != NONE; result = next_sound_datum(data, result))
		{
			if (controller_get(result)->definition_index == definition_index)
				break;
		}
	}
	return result;
}

// @retail 0x219a30
long looping_sound_controller_find_and_reference(long definition_index)
{
	long result = NONE;
	if (looping_sound_system_available() && definition_index != NONE)
	{
		result = looping_sound_controller_find_existing(definition_index);
		if (result != NONE)
			((c_looping_sound_controller *)CONTROLLERS->data)[result & 0xffff].add_reference();
	}
	return result;
}

// @retail 0x219c80
inline void looping_sound_controller_release(long index)
{
	if (index != NONE)
	{
		c_looping_sound_controller *controller = (c_looping_sound_controller *)CONTROLLERS->data + (index & 0xffff);
		if (--controller->reference_count == 0)
			datum_delete(CONTROLLERS, index);
	}
}

// @retail 0x219cc0
void looping_sound_controllers_update(void)
{
	s_data_array *data = CONTROLLERS;
	long index = next_sound_datum(data, NONE);
	while (index != NONE)
	{
		c_looping_sound_controller *controller = controller_get(index);
		controller->random_seed = controller->random_seed * 1664525 + 1013904223;
		long start = index == NONE ? 0 : (index & 0xffff) + 1;
		index = data_datum_index(data, data_next_absolute_index_inlined(data, start));
	}
}

static inline void controller_initialize(c_looping_sound_controller *controller, long definition_index, dword random_seed, bool source_updated)
{
	controller->definition_index = definition_index;
	controller->random_seed = random_seed;
	controller->source_updated = !source_updated;
	controller->synch_count = 0;
	controller->synch_playback = true;
	memset(controller->listener_values, 0, sizeof(controller->listener_values));
}

// @retail 0x219e90
void c_looping_sound_controller::initialize(long definition_index, dword random_seed, bool source_updated)
{
	controller_initialize(this, definition_index, random_seed, source_updated);
}

// @retail 0x219f60
void c_looping_sound_controller::add_synch_count(long count)
{
	if (definition_index != NONE)
		synch_count += count;
}

s_data_array *function_11cc20(long maximum_count, const char *name, long size);

// @retail 0x21a0c0
bool looping_sound_initialize(void)
{
	g_502110 = function_11cc20(128, "looping sounds", sizeof(looping_sound_datum));
	if (g_502110)
	{
		LOOPING_SOUNDS->valid = true;
		data_delete_all(LOOPING_SOUNDS);
	}
	return g_502110 != NULL;
}

// @retail 0x21b070
void looping_sound_delete(long index)
{
	looping_sound_datum *sound = (looping_sound_datum *)LOOPING_SOUNDS->data + (index & 0xffff);
	looping_sound_controller_release(sound->controller_index);
	datum_delete(LOOPING_SOUNDS, index);
}

/* Dispose the looping sounds from the old map; retain the shared declaration. */
// @retail 0x21a1e0
void function_21a1e0(void)
{
	s_data_array *data = LOOPING_SOUNDS;
	if (data)
	{
		s_data_iterator iterator = { data, NONE, NONE };
		while (data_iterator_next_inlined(&iterator))
		{
			looping_sound_delete(iterator.datum_index);
		}
	}
}

/* Existing helpers retain their upstream declarations. */
dword random_seed_generate(void);
long players_first_active_local_player(void);
long players_next_active_local_player(long index);
real function_12a9d0(long listener_index, s_sound_position const *position);
void __stdcall function_21f430(long controller_index);

extern dword g_510800_pool_base;
extern long g_510804_pool_size;
extern dword g_510808_pool_checksum;

// @retail 0x219a90
long looping_sound_controller_find_or_create(long definition_index)
{
	s_looping_sound_system *system = (s_looping_sound_system *)g_4e6380;
	long result = NONE;
	if (system->initialized && system->hardware_available && system->enabled && definition_index != NONE)
	{
		result = looping_sound_controller_find_existing(definition_index);
		if (result == NONE)
			result = datum_new(CONTROLLERS);
		if (result != NONE)
		{
			c_looping_sound_controller *controller = (c_looping_sound_controller *)g_51ebdc;
			if (result != NONE)
				controller = (c_looping_sound_controller *)CONTROLLERS->data + (result & 0xffff);
			if (controller->add_reference() == 1)
				controller->initialize(definition_index, random_seed_generate(), system->source_update_phase);
		}
	}
	return result;
}

// @retail 0x219b40
bool looping_sound_controllers_initialize(void)
{
	g_51ebd8 = function_11cc20(64, "sound playback controllers", sizeof(c_looping_sound_controller));
	byte *top = (byte *)(g_510804_pool_size + g_510800_pool_base);
	byte *memory = (byte *)(((dword)top + 3) & ~3);
	long aligned_size = (memory - top) + sizeof(c_looping_sound_controller);
	g_510804_pool_size += aligned_size;
	crc_checksum_buffer(&g_510808_pool_checksum, &aligned_size, sizeof(aligned_size));
	g_51ebdc = memory;
	if (g_51ebd8 && g_51ebdc)
	{
		CONTROLLERS->valid = true;
		data_delete_all(CONTROLLERS);
		byte phase = ((s_looping_sound_system *)g_4e6380)->source_update_phase;
		dword seed = random_seed_generate();
		c_looping_sound_controller *controller = (c_looping_sound_controller *)g_51ebdc;
		controller->definition_index = NONE;
		controller->random_seed = seed;
		controller->source_updated = ~phase;
		controller->synch_count = 0;
		controller->synch_playback = true;
		memset(controller->listener_values, 0, sizeof(controller->listener_values));
	}
	return g_51ebd8 && g_51ebdc;
}

// @retail 0x219ed0
void c_looping_sound_controller::update_controller_source(s_sound_location const *source, bool source_updated)
{
	if (definition_index != NONE && this->source_updated != source_updated)
	{
		if (source->audible == 1)
		{
			for (long index = players_first_active_local_player(); index != NONE; index = players_next_active_local_player(index))
				listener_values[index] = function_12a9d0(index, &source->spatial);
		}
		else
		{
			memset(listener_values, 0, sizeof(listener_values));
		}
		this->source_updated = source_updated;
	}
}

// @retail 0x219c30
void sound_refresh_looping_controller(long definition_index, s_sound_location const *source)
{
	s_looping_sound_system *system = (s_looping_sound_system *)g_4e6380;
	if (system->initialized && system->hardware_available && system->enabled && definition_index != NONE)
	{
		long index = looping_sound_controller_find_existing(definition_index);
		if (index != NONE)
		{
			c_looping_sound_controller *controller = looping_sound_controller_try_and_get(index);
			if (controller)
				controller->update_controller_source(source, system->source_update_phase);
		}
	}
}

// @retail 0x219d90
void looping_sound_controllers_synchronize(bool initial_playback)
{
	long index = next_sound_datum(CONTROLLERS, NONE);
	while (index != NONE)
	{
		c_looping_sound_controller *controller = controller_get(index);
		if (initial_playback ? (controller->synch_playback && controller->definition_index != NONE && controller->synch_count == 0) :
			(!controller->synch_playback && controller->definition_index != NONE && controller->synch_count == 0))
		{
			function_21f430(index);
			if (!initial_playback)
				controller->synch_playback = true;
		}
		long start = index == NONE ? 0 : (index & 0xffff) + 1;
		index = data_datum_index(CONTROLLERS, data_next_absolute_index_inlined(CONTROLLERS, start));
	}
}

void function_11bed0(real_point3d const *point, s_location *location);

// @retail 0x21a100
void looping_sound_update_locations(void)
{
	s_data_array *data = LOOPING_SOUNDS;
	long index = next_sound_datum(data, NONE);
	while (index != NONE)
	{
		looping_sound_datum *sound = (looping_sound_datum *)data->data + (index & 0xffff);
		if (sound->source.audible == 1)
			function_11bed0(&sound->source.spatial.position, &sound->source.spatial.location);
		long start = index == NONE ? 0 : (index & 0xffff) + 1;
		index = data_datum_index(data, data_next_absolute_index_inlined(data, start));
	}
}

/* Runtime track storage starts at +0x88. Keep the tail as a byte view until
   all of the retail array bounds have been recovered. */
struct looping_sound_track_datum
{
	long sound_index;
	dword permutations;
	dword random_seed;
	char current_state;
	char next_state;
	byte completed_count;
	byte unknown0f;
	real gain;
	inline byte notify_completion(void) { return ++completed_count; }
};

struct looping_sound_detail
{
	byte unknown00[8];
	long sound_definition_index;
	real period_lower;
	real period_upper;
	byte unknown14[4];
	dword flags;
	real yaw_lower;
	real yaw_upper;
	real pitch_lower;
	real pitch_upper;
	real distance_lower;
	real distance_upper;
};

struct looping_sound_track_definition
{
	long unknown00;
	dword flags;
	dword value08;
	real fade_in_duration;
	real fade_out_duration;
	long start_group;
	long start_sound_index;
	long loop_group;
	long loop_sound_index;
	long end_group;
	long end_sound_index;
	long alternate_loop_group;
	long alternate_loop_sound_index;
	long alternate_end_group;
	long alternate_end_sound_index;
	short effect;
	short unknown3e;
	long alternate_start_group;
	long alternate_start_sound_index;
	long alternate_transition_group;
	long alternate_transition_sound_index;
	real crossfade_duration;
	real alternate_fade_out_duration;
};

struct looping_sound_definition
{
	dword flags;
	byte unknown04[8];
	real minimum_distance;
	real maximum_distance;
	byte unknown14[8];
	long track_count;
	looping_sound_track_definition *tracks;
	long detail_sound_count;
	looping_sound_detail *detail_sounds;
};

/* A playing sound's list links are inside its loop state at +0x5c. */
struct s_looping_track_links
{
	long controller_index;
	char track_index;
	byte flags;
	char synch_state;
	byte unknown07;
	dword random_seed;
	dword permutations;
	long next_sound_index;
	long previous_sound_index;
};

struct s_looping_track_sound
{
	short salt;
	char state;
	byte unknown03;
	union
	{
		word flags;
		struct
		{
			word unknown04 : 3;
			word transition_pending : 1;
			word unknown04mid : 2;
			word stop_requested : 1;
			word looping : 1;
			word unknown04bit8 : 1;
			word channel_initialized : 1;
			word unknown04hi : 6;
		};
	};
	short sound_class;
	byte unknown08[4];
	long definition_index;
	long looping_sound_index;
	s_sound_source_callbacks const *callbacks;
	s_sound_location source;
	s_looping_track_links loop;
	byte unknown74[0x8c - 0x74];
	long time;
	real pitch;
	dword track_value;
	dword random_seed;
	char pitch_range_index;
	char permutation_index;
	short current_chunk_index;
	byte priority;
	char listener_index;
	short chunk_index;
	char unknowna4;
	byte unknowna5[3];
	long handle;
	short channel_index;
	short fade_curve;
	long fade_gain_bits;
	long fade_start_time;
	long fade_end_time;
};

static inline looping_sound_track_datum *looping_track_get(long index, short track_index)
{
	looping_sound_datum *sound = (looping_sound_datum *)LOOPING_SOUNDS->data + (index & 0xffff);
	return (looping_sound_track_datum *)sound->track_storage + track_index;
}

static inline s_looping_track_sound *track_sound_get(s_data_array *data, long index)
{
	return (s_looping_track_sound *)data->data + (index & 0xffff);
}

// @retail 0x21aeb0
long looping_sound_new(long definition_index, short sound_class, long identifier, long controller_definition_index)
{
	long result = NONE;
	if (looping_sound_system_available())
	{
		result = datum_new(LOOPING_SOUNDS);
		if (result != NONE)
		{
			looping_sound_datum *sound = (looping_sound_datum *)LOOPING_SOUNDS->data + (result & 0xffff);
			sound->sound_class = sound_class;
			sound->definition_index = definition_index;
			sound->identifier = identifier;
			sound->controller_index = looping_sound_controller_find_or_create(controller_definition_index);
			sound->playing_sound_count = 0;
			sound->flags = 0;
			looping_sound_definition *definition = (looping_sound_definition *)g_4e3b44[definition_index & 0xffff].bytes;
			for (long i = 0; i < definition->detail_sound_count; i++)
			{
				looping_sound_detail *detail = &definition->detail_sounds[i];
				real delay;
				if (detail->flags & 4)
					delay = 0;
				else
					delay = _real_random_range(&g_4e7408->seed, __FILE__, __LINE__, detail->period_lower, detail->period_upper);
				sound->detail_sound_times[i] = (long)(delay * 1000.f + (real)((s_looping_sound_system *)g_4e6380)->time);
			}
			if (definition->flags & 0x30)
			{
				c_looping_sound_controller *controller = controller_get(sound->controller_index);
				looping_sound_track_datum *track = (looping_sound_track_datum *)sound->track_storage;
				for (long i = 0; i < definition->track_count; i++, track++)
				{
					if (controller->definition_index == NONE)
						random_next(&controller->random_seed);
					track->random_seed = controller->random_seed;
				}
			}
		}
	}
	return result;
}

static inline long track_sound_next(s_data_array *data, long first, long index, long *count)
{
	if (*count > 0 && index != NONE)
	{
		s_looping_track_links *links = &track_sound_get(data, index)->loop;
		index = links->next_sound_index;
		if (index == first)
			index = NONE;
	}
	++*count;
	return index;
}

// @retail 0x21b870
long looping_sound_find_sound(long looping_sound_index, long definition_index, long pitch_range_index, short track_index)
{
	long first = looping_track_get(looping_sound_index, track_index)->sound_index;
	s_data_array *data = g_4e637c;
	long index = first;
	long count = 0;
	while ((index = track_sound_next(data, first, index, &count)) != NONE)
	{
		s_looping_track_sound *sound = track_sound_get(data, index);
		if (sound->definition_index == definition_index &&
			(pitch_range_index == NONE || sound->pitch_range_index == pitch_range_index))
			return index;
	}
	return NONE;
}

// @retail 0x21bc80
void looping_sound_link_sound(long looping_sound_index, short track_index, long sound_index)
{
	looping_sound_track_datum *track = looping_track_get(looping_sound_index, track_index);
	s_looping_track_links *sound = &track_sound_get(g_4e637c, sound_index)->loop;
	if (track->sound_index != NONE)
	{
		s_looping_track_links *first = &track_sound_get(g_4e637c, track->sound_index)->loop;
		sound->next_sound_index = first->next_sound_index;
		sound->previous_sound_index = track->sound_index;
		first->next_sound_index = sound_index;
		track_sound_get(g_4e637c, sound->next_sound_index)->loop.previous_sound_index = sound_index;
	}
}

// @retail 0x21cf80
long looping_sound_find(long definition_index, long identifier)
{
	s_data_array *data = LOOPING_SOUNDS;
	long index = next_sound_datum(data, NONE);
	while (index != NONE)
	{
		looping_sound_datum *sound = (looping_sound_datum *)data->data + (index & 0xffff);
		if (sound->definition_index == definition_index && sound->identifier == identifier)
			break;
		long start = index == NONE ? 0 : (index & 0xffff) + 1;
		index = data_datum_index(data, data_next_absolute_index_inlined(data, start));
	}
	return index;
}

// @retail 0x21bf00
bool __stdcall track_loop_track_sound(long looping_sound_index, long definition_index, s_sound_marker const *marker, s_sound_location *source)
{
	bool result = false;
	looping_sound_datum *sound = (looping_sound_datum *)datum_get_inlined(LOOPING_SOUNDS, looping_sound_index);
	if (sound)
	{
		long controller_index = sound->controller_index;
		c_looping_sound_controller *controller = controller_get(controller_index);
		if (controller_index == NONE || controller->synch_playback)
		{
			*source = sound->source;
			result = true;
			if (sound->identifier == (sound->definition_index | 0x6000))
				source->audible = 0;
		}
	}
	return result;
}

long const g_44a150[7] = { 1, 1, 2, 5, 5, 1, 6 };
long const g_44a16c[7] = { 3, 3, 2, 4, 4, 3, 6 };
long const g_44a188[7] = { 20, 28, 36, 64, 44, 72, 52 };

// @retail 0x21a050
long looping_sound_track_has_next_sound(looping_sound_track_datum const *track, looping_sound_track_definition const *definition)
{
	if (track->current_state != track->next_state)
	{
		long offset = g_44a188[track->next_state];
		if (*(long const *)((byte const *)definition + offset + 4) != NONE)
			return true;
	}
	return false;
}

static inline bool looping_sound_track_transition_is_immediate(looping_sound_track_datum const *track)
{
	if ((1 << track->current_state) & 0x29)
	{
		if (g_44a16c[track->current_state] == track->next_state ||
			g_44a150[track->current_state] == track->next_state)
			return true;
	}
	return false;
}

// @retail 0x21a080
bool looping_sound_track_transition_requires_stop(looping_sound_track_datum const *track)
{
	return !looping_sound_track_transition_is_immediate(track);
}

// @retail 0x21c140
bool __stdcall looping_sound_positions_equal(real_point3d const *a, real_point3d const *b)
{
	return fabs(a->x - b->x) < 0.0001f && fabs(a->y - b->y) < 0.0001f && fabs(a->z - b->z) < 0.0001f;
}

struct s_looping_source_flags
{
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word unknown : 8;
};

struct s_looping_source_tail
{
	long unknown30;
	real value34;
	real value38;
	short value3c;
	short value3e;
	long value40;
};

struct s_looping_spatialization
{
	dword flags;
	byte unknown04[0x24];
	real listener_gains[4];
};

long function_2197f0(real gain);
long function_1251e0(void const *definition, long gain_bits, real scale);
dword vector3d_compress(real_vector3d const *vector);

// @retail 0x21ac30
void __stdcall looping_sound_track_notify(long looping_sound_index, long definition_index, long sound_index, long tracking_data)
{
	s_looping_track_sound *playing = track_sound_get(g_4e637c, sound_index);
	if (!TEST_FIELD_BIT(playing->looping) || playing->fade_start_time <= playing->fade_end_time)
	{
		looping_sound_datum *sound = (looping_sound_datum *)datum_get_inlined(LOOPING_SOUNDS, looping_sound_index);
		if (sound)
		{
			s_looping_track_links const *tracking = (s_looping_track_links const *)tracking_data;
			looping_sound_track_datum *track = (looping_sound_track_datum *)sound->track_storage + tracking->track_index;
			track->notify_completion();
		}
	}
}

// @retail 0x21acc0
bool __stdcall looping_sound_track_spatialize(long looping_sound_index, long definition_index, s_sound_source_view const *source, s_sound_spatialization_view *spatialization)
{
	bool result = false;
	looping_sound_datum *sound = (looping_sound_datum *)datum_get_inlined(LOOPING_SOUNDS, looping_sound_index);
	if (sound && sound->identifier == (sound->definition_index | 0x6000))
	{
		void const *definition = g_4e3b44[definition_index & 0xffff].bytes;
		s_looping_spatialization *output = (s_looping_spatialization *)spatialization;
		for (short i = 0; i < 4; ++i)
		{
			long gain_bits = function_2197f0(sound->listener_gains[i]);
			long value_bits = function_1251e0(definition, gain_bits, sound->source.scale);
			output->listener_gains[i] = *(real *)&value_bits + *(real *)&sound->source.unknown08;
		}
		output->flags |= 0x10;
		result = true;
	}
	return result;
}

// @retail 0x21adb0
void __stdcall looping_sound_track_stopped(long looping_sound_index, long sound_index, long unused)
{
	looping_sound_datum *sound = (looping_sound_datum *)datum_get_inlined(LOOPING_SOUNDS, looping_sound_index);
	s_looping_track_links *tracking = &track_sound_get(g_4e637c, sound_index)->loop;
	long track_index = tracking->track_index;
	long next = NONE;
	if (tracking->next_sound_index != sound_index)
	{
		s_looping_track_links *previous = &track_sound_get(g_4e637c, tracking->previous_sound_index)->loop;
		s_looping_track_links *following = &track_sound_get(g_4e637c, tracking->next_sound_index)->loop;
		previous->next_sound_index = tracking->next_sound_index;
		following->previous_sound_index = tracking->previous_sound_index;
		next = tracking->next_sound_index;
	}
	if (sound)
	{
		--sound->playing_sound_count;
		looping_sound_track_datum *track = (looping_sound_track_datum *)sound->track_storage + track_index;
		if (track->sound_index == sound_index)
			track->sound_index = next;
	}
	if (tracking->controller_index != NONE)
	{
		c_looping_sound_controller *controller = looping_sound_controller_try_and_get(tracking->controller_index);
		if (controller && tracking->synch_state == NONE && controller->definition_index != NONE)
		{
			controller->synch_count--;
			controller->synch_playback = false;
		}
	}
}

// @retail 0x21bfb0
bool __stdcall track_loop_impulse_sound(long looping_sound_index, long definition_index, s_sound_marker const *tracking_data, s_sound_location *source)
{
	looping_sound_datum *sound = (looping_sound_datum *)datum_get_inlined(LOOPING_SOUNDS, looping_sound_index);
	bool result = false;
	if (sound)
	{
		if (sound->source.audible)
		{
			source->spatial.velocity = sound->source.spatial.velocity;
			source->spatial.compressed_forward = sound->source.spatial.compressed_forward;
			source->spatial.location = sound->source.spatial.location;
			if (TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag2))
				((s_looping_source_flags *)source)->flag2 = true;
			else
				((s_looping_source_flags *)source)->flag2 = false;
			s_looping_source_tail const *input = (s_looping_source_tail const *)sound->source.unknown30;
			s_looping_source_tail *output = (s_looping_source_tail *)source->unknown30;
			output->value34 = input->value34;
			if (TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag3))
				((s_looping_source_flags *)source)->flag3 = true;
			else
				((s_looping_source_flags *)source)->flag3 = false;
			output->value38 = input->value38;
			if (TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag4))
				((s_looping_source_flags *)source)->flag4 = true;
			else
				((s_looping_source_flags *)source)->flag4 = false;
			output->value3c = input->value3c;
			if (TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag5))
				((s_looping_source_flags *)source)->flag5 = true;
			else
				((s_looping_source_flags *)source)->flag5 = false;
			output->value3e = input->value3e;
			if (TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag6))
				((s_looping_source_flags *)source)->flag6 = true;
			else
				((s_looping_source_flags *)source)->flag6 = false;
			output->value40 = input->value40;
		}
		else
		{
			source->spatial.compressed_forward = vector3d_compress(g_4687a8);
			source->spatial.velocity = *g_4687a4;
		}
		source->spatial.position = *(real_point3d const *)tracking_data;
		if (source->audible == 1)
		{
			source->spatial.position.x += sound->source.spatial.position.x;
			source->spatial.position.y += sound->source.spatial.position.y;
			source->spatial.position.z += sound->source.spatial.position.z;
		}
		result = true;
	}
	return result;
}

/* Retail folds this always-true callback with other identical functions
   at 0xa5750, already marked in unknown_09a9f0.cpp. */
static bool __stdcall looping_sound_tracking_data_equal(void const *, void const *)
{
	return true;
}

extern s_sound_source_callbacks const g_44a110 = {
	track_loop_track_sound, looping_sound_track_notify, NULL,
	(bool (__stdcall *)(long, long, s_sound_source_view const *, s_sound_spatialization_view *))looping_sound_track_spatialize,
	looping_sound_track_stopped, NULL, looping_sound_tracking_data_equal, NULL
};

extern s_sound_source_callbacks const g_44a130 = {
	track_loop_impulse_sound, NULL, NULL, NULL, NULL, NULL,
	(bool (__stdcall *)(void const *, void const *))looping_sound_positions_equal, NULL
};

struct s_animation_ref;
struct s_animation_state;
struct s_packed_value;
struct s_set_ref;
struct s_sound;

struct s_looping_playback_definition
{
	word flags;
	byte unknown02[4];
	short sound_class;
	short pitch_range_base;
	char pitch_range_count;
	char playback_index;
};

struct s_looping_pitch_modifier
{
	byte unknown00[8];
	short lower;
	short upper;
	byte unknown0c[8];
};

struct s_looping_sound_class
{
	byte unknown00[0xc];
	real gain_change_rate;
	byte unknown10[8];
	short pitch;
	byte unknown1a[0x38 - 0x1a];
};

struct s_looping_sound_tables
{
	byte unknown00[4];
	s_looping_sound_class *classes;
	byte unknown08[4];
	s_looping_pitch_modifier *pitch_modifiers;
};

dword function_2194c0(s_packed_value *value, s_animation_ref *ref);
void function_219560(s_packed_value *value, s_animation_ref *ref, dword data);
void function_2192b0(s_animation_state *state, s_animation_ref *ref, byte value);
long function_219370(s_animation_state *state, s_animation_ref *ref);

// @retail 0x21cb90
dword looping_sound_get_permutations(looping_sound_datum *sound, looping_sound_definition *definition, s_looping_playback_definition *playback, long track_index, short pitch_range, bool clear)
{
	if ((definition->flags & 0x30) && sound->controller_index != NONE)
	{
		dword *permutations = &((looping_sound_track_datum *)sound->track_storage)[track_index].permutations;
		dword result = *permutations;
		if (clear)
			*permutations = 0;
		return result;
	}
	return function_2194c0((s_packed_value *)&g_51ebd4->sets[playback->pitch_range_base + pitch_range], (s_animation_ref *)playback);
}

// @retail 0x21cbe0
void looping_sound_set_permutations(looping_sound_datum *sound, looping_sound_definition *definition, s_looping_playback_definition *playback, long track_index, short pitch_range, dword permutations, short permutation)
{
	if ((definition->flags & 0x30) && sound->controller_index != NONE)
		((looping_sound_track_datum *)sound->track_storage)[track_index].permutations = permutations;
	else
	{
		s_permutation_set *set = &g_51ebd4->sets[playback->pitch_range_base + pitch_range];
		function_219560((s_packed_value *)set, (s_animation_ref *)playback, permutations);
		function_2192b0((s_animation_state *)set, (s_animation_ref *)playback, (byte)permutation);
	}
}

// @retail 0x21cc30
long looping_sound_get_previous_permutation(looping_sound_definition *definition, s_looping_playback_definition *playback, long pitch_range)
{
	if (!(definition->flags & 0x30))
		return (short)function_219370((s_animation_state *)&g_51ebd4->sets[playback->pitch_range_base + pitch_range], (s_animation_ref *)playback);
	return NONE;
}

long function_126bd0(long definition_index);
real sound_get_maximum_distance(s_sound const *sound, long definition_index);
long __stdcall function_127d00(s_sound_location const *source, real maximum_distance, real *distance);
short function_218f50(s_looping_playback_definition *definition, short previous, real pitch);
short function_219110(s_set_ref *ref, short offset, dword *used_mask, short previous, dword *seed, bool *all_used, bool mirror);
void sound_playback_set_chunk(long sound_index, long definition_index, char pitch_range_index, char permutation_index, short chunk_index);
long __stdcall function_125e60(s_looping_track_sound *sound);

// @retail 0x21b940
long looping_sound_new_sound(long looping_sound_index, long definition_index, long pitch_range_index, short track_index, short state, dword *seed)
{
	looping_sound_datum *sound = (looping_sound_datum *)LOOPING_SOUNDS->data + (looping_sound_index & 0xffff);
	looping_sound_definition *definition = (looping_sound_definition *)g_4e3b44[sound->definition_index & 0xffff].bytes;
	looping_sound_track_definition *track_definition = &definition->tracks[track_index];
	s_sound_location *source = &sound->source;
	if ((byte)function_126bd0(definition_index))
	{
		s_looping_playback_definition *playback = (s_looping_playback_definition *)g_4e3b44[definition_index & 0xffff].bytes;
		real maximum_distance = sound_get_maximum_distance((s_sound const *)source, definition_index);
		long listener_index = function_127d00(source, maximum_distance, NULL);
		if (listener_index != NONE)
		{
			long result = datum_new(g_4e637c);
			if (result != NONE)
			{
				s_looping_sound_tables *tables = (s_looping_sound_tables *)g_51ebd4;
				s_looping_sound_class *sound_class = &tables->classes[playback->sound_class];
				looping_sound_track_datum *track = (looping_sound_track_datum *)sound->track_storage + track_index;
				s_looping_track_sound *playing = track_sound_get(g_4e637c, result);
				playing->listener_index = (char)listener_index;
				playing->channel_index = NONE;
				playing->flags = 0;
				playing->pitch = _real_random_range(seed, __FILE__, __LINE__, (real)sound_class->pitch, (real)sound_class->pitch);
				playing->track_value = track_definition->value08;
				playing->random_seed = *seed;
				playing->looping_sound_index = looping_sound_index;
				playing->source = *source;
				playing->state = (char)state;
				playing->sound_class = sound->sound_class;
				playing->time = ((s_looping_sound_system *)g_4e6380)->time;
				playing->callbacks = &g_44a110;
				playing->fade_end_time = 0;
				playing->fade_start_time = 0;
				playing->chunk_index = NONE;
				playing->unknowna4 = NONE;
				playing->handle = NONE;
				playing->loop.track_index = (char)track_index;
				playing->loop.flags = 0;
				playing->loop.random_seed = track->random_seed;
				playing->loop.permutations = track->permutations;
				playing->loop.next_sound_index = result;
				playing->loop.previous_sound_index = result;
				if (playing->state == 2 && (playback->flags & 0x2000))
					playing->loop.flags |= 1;
				if (definition->flags & 0x20)
				{
					looping_sound_controller_get(sound->controller_index)->add_synch_count(1);
					playing->loop.controller_index = sound->controller_index;
					playing->loop.synch_state = NONE;
				}
				else
					playing->loop.controller_index = NONE;
				if (pitch_range_index == NONE)
				{
					s_looping_pitch_modifier *modifier = &tables->pitch_modifiers[playback->playback_index];
					real pitch = ((real)modifier->upper - (real)modifier->lower) * track->gain + playing->pitch + (real)modifier->lower;
					pitch_range_index = function_218f50(playback, NONE, pitch);
				}
				dword permutations = looping_sound_get_permutations(sound, definition, playback, track_index, pitch_range_index, state == 2);
				short previous = (short)looping_sound_get_previous_permutation(definition, playback, pitch_range_index);
				short permutation = function_219110((s_set_ref *)playback, pitch_range_index, &permutations, previous, seed, NULL, false);
				looping_sound_set_permutations(sound, definition, playback, track_index, pitch_range_index, permutations, permutation);
				sound_playback_set_chunk(result, definition_index, (char)pitch_range_index, (char)permutation, 0);
				function_125e60(playing);
				looping_sound_link_sound(looping_sound_index, track_index, result);
				++sound->playing_sound_count;
			}
			return result;
		}
	}
	return NONE;
}

// @retail 0x21b910
long looping_sound_find_or_create_sound(long looping_sound_index, long definition_index, long pitch_range_index, short track_index, short state, dword *seed)
{
	long result = looping_sound_find_sound(looping_sound_index, definition_index, pitch_range_index, track_index);
	if (result == NONE)
		result = looping_sound_new_sound(looping_sound_index, definition_index, pitch_range_index, track_index, state, seed);
	return result;
}

struct s_looping_pitch_bounds
{
	short unknown00;
	short minimum;
	short maximum;
	short natural_minimum;
	short natural_maximum;
};

struct s_looping_pitch_tables
{
	byte unknown00[0x1c];
	s_looping_pitch_bounds *bounds;
};

real function_12aff0(real a, real b, real c, bool flag);
long __stdcall function_2197b0(short curve, real gain, real scale);
long function_12a810(long sound_index);
real g_44a0f0 = 0.0f;

// @retail 0x219f80
long looping_sound_pitch_attenuation(s_looping_playback_definition const *definition, s_permutation_set const *pitch_range, real pitch)
{
	long result = 0;
	if (definition->flags & 0x2000)
	{
		s_looping_pitch_bounds *bounds = &((s_looping_pitch_tables *)g_51ebd4)->bounds[*(short const *)((byte const *)pitch_range + 2)];
		real lower = (real)bounds->natural_minimum;
		if (!(fabs(pitch - lower) < 0.0001f) && pitch < lower)
			result = function_2197b0(2, g_44a0f0, function_12aff0((real)bounds->minimum, lower, pitch, true));
		else
		{
			real upper = (real)bounds->natural_maximum;
			if (!(fabs(pitch - upper) < 0.0001f) && pitch > upper)
				result = function_2197b0(2, g_44a0f0, function_12aff0((real)bounds->maximum, upper, pitch, false));
		}
	}
	return result;
}

struct s_looping_promotion
{
	byte unknown00[0xa];
	word flags;
	byte unknown0c[0x5c - 0xc];
};

struct s_looping_promotion_table
{
	long count;
	s_looping_promotion *entries;
};

// @retail 0x21bd00
void looping_sound_fade_out(real duration, long first_sound_index)
{
	if (first_sound_index != NONE)
	{
		long index = first_sound_index;
		long count = 0;
		while ((index = track_sound_next(g_4e637c, first_sound_index, index, &count)) != NONE)
		{
			s_looping_track_sound *playing = track_sound_get(g_4e637c, index);
			s_looping_playback_definition *definition = (s_looping_playback_definition *)g_4e3b44[playing->definition_index & 0xffff].bytes;
			s_tag_header *header = g_4e034c->header ? g_4e034c->header_alt : NULL;
			s_looping_promotion_table *promotions = (s_looping_promotion_table *)g_4e3b44[header->datum_index & 0xffff].bytes;
			s_looping_promotion *promotion = &promotions->entries[(short)(char)definition->unknown02[0]];
			short curve = !(promotion->flags & 0x400);
			playing->fade_gain_bits = function_12a810(index);
			playing->fade_curve = curve;
			playing->fade_start_time = (long)(duration * 1000.f);
			playing->fade_end_time = NONE;
			playing->looping = true;
		}
	}
}

// @retail 0x21be20
void looping_sound_fade_in(long first_sound_index, short curve, real duration)
{
	if (first_sound_index != NONE)
	{
		s_data_array *data = g_4e637c;
		long index = first_sound_index;
		long count = 0;
		while ((index = track_sound_next(data, first_sound_index, index, &count)) != NONE)
		{
			s_looping_track_sound *playing = track_sound_get(data, index);
			if (playing->state == 2)
			{
				if (TEST_FIELD_BIT(playing->looping))
					playing->fade_gain_bits = function_12a810(index);
				else
					playing->fade_gain_bits = 0xc2800000;
				playing->fade_curve = curve;
				playing->fade_start_time = NONE;
				playing->fade_end_time = (long)(duration * 1000.f);
				playing->looping = true;
			}
		}
	}
}

// @retail 0x21ce60
void detail_sound_random_offset(looping_sound_detail const *detail, real_vector3d *offset)
{
	real distance = _real_random_range(&g_4e7408->seed, __FILE__, __LINE__, detail->distance_lower, detail->distance_upper);
	if (distance != 0.f)
	{
		real pitch = _real_random_range(&g_4e7408->seed, __FILE__, __LINE__, detail->pitch_lower, detail->pitch_upper);
		real yaw = _real_random_range(&g_4e7408->seed, __FILE__, __LINE__, detail->yaw_lower, detail->yaw_upper);
		real cosine_pitch = (real)cos(pitch);
		offset->i = (real)cos(yaw) * cosine_pitch * distance;
		offset->j = (real)sin(yaw) * cosine_pitch * distance;
		offset->k = (real)sin(pitch) * distance;
	}
	else
		*offset = *g_4687a4;
}

struct s_looping_detail_request
{
	long flags;
	long mode;
	long type;
	s_sound_location source;
	long looping_sound_index;
	long unknown54;
	s_sound_source_callbacks const *callbacks;
	real_vector3d offset;
	byte unknown68[0x8c - 0x68];
	short tracking_data_size;
};

// @retail 0x21cc60
bool looping_sound_prepare_detail(looping_sound_detail const *detail, long looping_sound_index, s_looping_detail_request *request)
{
	looping_sound_datum *sound = (looping_sound_datum *)LOOPING_SOUNDS->data + (looping_sound_index & 0xffff);
	byte const *definition = g_4e3b44[detail->sound_definition_index & 0xffff].bytes;
	s_sound_location *source = &request->source;
	*source = sound->source;
	bool result = false;
	source->unknown02 = 0;
	request->type = 0x10;
	request->mode = definition[4] ? 0 : 2;
	if (sound->source.audible == 1)
	{
		source->audible = 1;
		real_vector3d offset;
		detail_sound_random_offset(detail, &offset);
		track_loop_impulse_sound(looping_sound_index, detail->sound_definition_index, (s_sound_marker const *)&offset, source);
		request->offset = offset;
		request->flags = 0x2f;
		request->looping_sound_index = looping_sound_index;
		request->callbacks = &g_44a130;
		request->tracking_data_size = sizeof(offset);
		result = true;
	}
	else
	{
		long player_count = *(short *)&g_4e8c20->unknown00[8];
		if (player_count > 0)
		{
			long skip = random_index(&g_4e7408->seed, (short)player_count);
			long player = players_first_active_local_player();
			while (skip > 0)
			{
				player = players_next_active_local_player(player);
				--skip;
			}
			source->audible = 1;
			s_local_camera *camera = local_camera_get(player);
			real_vector3d offset;
			detail_sound_random_offset(detail, &offset);
			source->spatial.position.x = camera->position.x + offset.i;
			source->spatial.position.y = camera->position.y + offset.j;
			source->spatial.position.z = camera->position.z + offset.k;
			function_11bed0(&source->spatial.position, &source->spatial.location);
			source->spatial.compressed_forward = vector3d_compress(g_4687a8);
			source->spatial.velocity = *g_4687a4;
			request->flags = 7;
			result = true;
		}
	}
	source->requested_audible = source->audible;
	((s_looping_source_flags *)source)->flag7 = true;
	return result;
}

bool g_4e636d;
real sound_get_minimum_distance(s_sound const *sound, long definition_index);

// @retail 0x21d060
void render_debug_looping_sound(long definition_index, s_sound_location const *source)
{
	if (g_4e636d && source->audible == 1)
	{
		looping_sound_definition *definition = (looping_sound_definition *)g_4e3b44[definition_index & 0xffff].bytes;
		/* Retail repeatedly tests the first track; no track-pointer advance survives. */
		for (short i = 0; i < definition->track_count; ++i)
		{
			long track_sound = definition->tracks->loop_sound_index;
			if (track_sound != NONE)
			{
				real minimum = sound_get_minimum_distance((s_sound const *)source, track_sound);
				sound_get_maximum_distance((s_sound const *)source, track_sound);
				if (minimum != 0.f)
					return;
				break;
			}
		}
		for (long i = 0; i < definition->detail_sound_count; ++i)
		{
			if (definition->detail_sounds[i].sound_definition_index != NONE)
				break;
		}
	}
}

struct s_looping_voice_group
{
	short count;
	short voice_indices[16];
	short limit;
	bool started_this_tick;
	byte unknown25;
};

struct s_looping_voice_counts
{
	s_looping_voice_group definition;
	s_looping_voice_group source;
};

struct s_sound_playback;
struct s_sound_voice;
extern s_sound_voice *g_4e6378;

struct s_looping_voice_view
{
	long sound_index;
	byte unknown04[8];
	short channel_index;
	byte unknown0e[0x24 - 0xe];
};

void sound_playback_release_reference(s_sound_playback *sound);
void __stdcall function_127320(long datum, long count);
void function_128500(long sound_index, s_looping_voice_counts *counts);
short function_128a60(long sound_index, short count, short const *voice_indices);

// @retail 0x21c190
bool sound_commit_looping_definition(looping_sound_datum *sound, looping_sound_track_datum *track, long sound_index, dword *seed)
{
	looping_sound_definition *definition = (looping_sound_definition *)g_4e3b44[sound->definition_index & 0xffff].bytes;
	s_looping_track_sound *playing = track_sound_get(g_4e637c, sound_index);
	long offset = g_44a188[track->next_state];
	looping_sound_track_definition *track_definition = &definition->tracks[playing->loop.track_index];
	long definition_index = *(long *)((byte *)track_definition + offset + 4);
	s_looping_playback_definition *playback = (s_looping_playback_definition *)g_4e3b44[definition_index & 0xffff].bytes;
	playing->transition_pending = true;
	bool result = false;
	short pitch_range = function_218f50(playback, playing->pitch_range_index, playing->pitch);

	/* Retail expands the permutation helpers here, but calls them from
	   looping_sound_new_sound. Keep the same shared/local mask rules. */
	dword permutations;
	if ((definition->flags & 0x30) && sound->controller_index != NONE)
	{
		dword *mask = &((looping_sound_track_datum *)sound->track_storage)[playing->loop.track_index].permutations;
		permutations = *mask;
		*mask = 0;
	}
	else
		permutations = function_2194c0((s_packed_value *)&g_51ebd4->sets[playback->pitch_range_base + pitch_range], (s_animation_ref *)playback);

	long previous = NONE;
	if (!(definition->flags & 0x30))
		previous = (short)function_219370((s_animation_state *)&g_51ebd4->sets[playback->pitch_range_base + playing->pitch_range_index], (s_animation_ref *)playback);
	short permutation = function_219110((s_set_ref *)playback, pitch_range, &permutations, (short)previous, seed, NULL, false);
	if ((definition->flags & 0x30) && sound->controller_index != NONE)
		((looping_sound_track_datum *)sound->track_storage)[playing->loop.track_index].permutations = permutations;
	else
	{
		s_permutation_set *set = &g_51ebd4->sets[playback->pitch_range_base + pitch_range];
		function_219560((s_packed_value *)set, (s_animation_ref *)playback, permutations);
		function_2192b0((s_animation_state *)set, (s_animation_ref *)playback, (byte)permutation);
	}

	s_looping_track_sound *updated = track_sound_get(g_4e637c, sound_index);
	sound_playback_release_reference((s_sound_playback *)updated);
	updated->definition_index = definition_index;
	updated->pitch_range_index = (char)pitch_range;
	updated->permutation_index = (char)permutation;
	updated->current_chunk_index = 0;
	if (playing->channel_index != NONE)
	{
		s_looping_voice_counts counts;
		function_128500(sound_index, &counts);
		short count;
		short const *indices;
		long reason;
		if (counts.source.count >= counts.source.limit)
		{
			count = counts.source.count;
			indices = counts.source.voice_indices;
			reason = 5;
		}
		else if (counts.definition.count >= counts.definition.limit)
		{
			count = counts.definition.count;
			indices = counts.definition.voice_indices;
			reason = 6;
		}
		else
			count = 0;
		if (count > 0)
		{
			short voice_index = function_128a60(sound_index, count, indices);
			long stopped_sound = voice_index == NONE ? sound_index : ((s_looping_voice_view *)g_4e6378)[voice_index].sound_index;
			if (stopped_sound != NONE)
			{
				function_127320(stopped_sound, reason);
				result = stopped_sound == sound_index;
			}
		}
	}
	track->current_state = track->next_state;
	return result;
}

long function_125f70(long definition_index, s_looping_detail_request *request, long *reason);
void function_126df0(long new_sound, long old_sound, short curve, real duration);

// @retail 0x21b0e0
void process_looping_sounds(bool update)
{
	s_data_array *data = LOOPING_SOUNDS;
	long index = next_sound_datum(data, NONE);
	while (index != NONE)
	{
		looping_sound_datum *sound = (looping_sound_datum *)data->data + (index & 0xffff);
		if (update && sound->update_phase != *((byte *)g_4e6380 + 0x7f) &&
			(!(sound->flags & 2) || sound->playing_sound_count == 0))
		{
			looping_sound_delete(index);
		}
		else
		{
			looping_sound_definition *definition = (looping_sound_definition *)g_4e3b44[sound->definition_index & 0xffff].bytes;
			looping_sound_track_datum *track = (looping_sound_track_datum *)sound->track_storage;
			for (long track_index = 0; track_index < definition->track_count; ++track_index, ++track)
			{
				looping_sound_track_definition *track_definition = &definition->tracks[track_index];
				track->completed_count = 0;
				if (sound->state == 2)
					continue;
				if (track->sound_index != NONE)
				{
					s_looping_track_sound *playing = track_sound_get(g_4e637c, track->sound_index);
					s_looping_playback_definition *playback = (s_looping_playback_definition *)g_4e3b44[playing->definition_index & 0xffff].bytes;
					s_looping_sound_class *sound_class = &((s_looping_sound_tables *)g_51ebd4)->classes[playback->sound_class];
					if (!(fabs(sound_class->gain_change_rate) < 0.0001f))
					{
						real step = ((s_looping_sound_system *)g_4e6380)->elapsed_time * sound_class->gain_change_rate;
						real delta = sound->source.scale - track->gain;
						real lower = 0.f - step;
						track->gain += delta < lower ? lower : delta > step ? step : delta;
					}
					else
						track->gain = sound->source.scale;
				}
				if (update && track->sound_index != NONE)
				{
					s_looping_track_sound *playing = track_sound_get(g_4e637c, track->sound_index);
					long definition_index = playing->definition_index;
					s_looping_playback_definition *playback = (s_looping_playback_definition *)g_4e3b44[definition_index & 0xffff].bytes;
					s_looping_pitch_modifier *modifier = &((s_looping_sound_tables *)g_51ebd4)->pitch_modifiers[playback->playback_index];
					real pitch = ((real)modifier->upper - (real)modifier->lower) * track->gain + playing->pitch + (real)modifier->lower;
					c_looping_sound_controller *controller = controller_get(sound->controller_index);
					if (controller->definition_index == NONE)
						random_next(&controller->random_seed);
					dword seed = controller->random_seed;
					short previous_pitch_range = playing->pitch_range_index;
					short selected = function_218f50(playback, previous_pitch_range, pitch);
					if (playback->flags & 0x2000)
					{
						long sounds[9] = { NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE };
						long gains[9] = { 0xc2800000, 0xc2800000, 0xc2800000, 0xc2800000, 0xc2800000, 0xc2800000, 0xc2800000, 0xc2800000, 0xc2800000 };
						dword active_mask = 0;
						long first = track->sound_index;
						long current = first;
						long count = 0;
						s_data_array *playing_sounds = g_4e637c;
						while ((current = track_sound_next(playing_sounds, first, current, &count)) != NONE)
						{
							s_looping_track_sound *entry = track_sound_get(playing_sounds, current);
							if (entry->definition_index == definition_index && entry->state == 2)
								sounds[entry->pitch_range_index] = current;
						}
						long pitch_range_count = playback->pitch_range_count;
						for (long i = 0; i < pitch_range_count; ++i)
						{
							s_permutation_set *set = &g_51ebd4->sets[playback->pitch_range_base + i];
							s_looping_pitch_bounds *bounds = &((s_looping_pitch_tables *)g_51ebd4)->bounds[*(short *)((byte *)set + 2)];
							gains[i] = looping_sound_pitch_attenuation(playback, set, pitch);
							real clamped = pitch < (real)bounds->minimum ? (real)bounds->minimum :
								pitch > (real)bounds->maximum ? (real)bounds->maximum : pitch;
							if (clamped == pitch)
								active_mask |= 1 << i;
						}
						pitch_range_count = playback->pitch_range_count;
						for (long i = 0; i < pitch_range_count; ++i)
						{
							dword active = active_mask & (1 << i);
							if (!active && sounds[i] != NONE)
							{
								function_127320(sounds[i], 8);
								sounds[i] = NONE;
							}
							if (active && sounds[i] == NONE)
							{
								long created = looping_sound_new_sound(index, definition_index, i, (short)track_index, 2, &seed);
								if (created != NONE)
								{
									if (selected == i)
										track->sound_index = created;
									track_sound_get(g_4e637c, created)->track_value = 0xc2800000;
									sounds[i] = created;
								}
							}
							if (sounds[i] != NONE)
							{
								s_looping_track_sound *entry = track_sound_get(g_4e637c, sounds[i]);
								real gain = *(real *)&track_definition->value08 + *(real *)&gains[i];
								entry->track_value = *(dword *)&gain;
								entry->loop.unknown07 = 0xff;
							}
						}
					}
					else if (selected != previous_pitch_range)
					{
						long replacement = looping_sound_find_sound(playing->looping_sound_index, playing->definition_index, selected, (short)track_index);
						if (replacement == NONE)
							replacement = looping_sound_new_sound(playing->looping_sound_index, playing->definition_index, selected, (short)track_index, 2, &seed);
						if (replacement != NONE)
						{
							track_sound_get(g_4e637c, replacement)->pitch = playing->pitch;
							function_126df0(replacement, track->sound_index, 1, 0.75f);
							track->sound_index = replacement;
						}
					}
				}
			}
			if (sound->state != 2 && !(sound->flags & 4))
			{
				for (long i = 0; i < definition->detail_sound_count; ++i)
				{
					looping_sound_detail *detail = &definition->detail_sounds[i];
					if (detail->sound_definition_index != NONE && sound->detail_sound_times[i] < ((s_looping_sound_system *)g_4e6380)->time &&
						(!(detail->flags & 1) || !(sound->flags & 1)) && (!(detail->flags & 2) || (sound->flags & 1)))
					{
						s_looping_detail_request request;
						*(word *)&request.source = 0;
						if (looping_sound_prepare_detail(detail, index, &request))
						{
							long reason;
							if (function_125f70(detail->sound_definition_index, &request, &reason) != NONE || reason == 1)
							{
								real delay = _real_random_range(&g_4e7408->seed, __FILE__, __LINE__, detail->period_lower, detail->period_upper);
								sound->detail_sound_times[i] = (long)((double)(delay * 1000.f) + ((s_looping_sound_system *)g_4e6380)->time);
							}
						}
					}
				}
			}
		}
		long start = index == NONE ? 0 : (index & 0xffff) + 1;
		index = data_datum_index(LOOPING_SOUNDS, data_next_absolute_index_inlined(LOOPING_SOUNDS, start));
		data = LOOPING_SOUNDS;
	}
}

/* Channel properties consist of a 0x30-byte base and 0x448 bytes of
   effect/impulse state. Retail clears those two parts separately. */
struct s_looping_effect_playback
{
	byte data[0x420];
};

struct s_looping_impulse_parameters
{
	short type;
	short index;
	real lower;
	real upper;
	long identifier;
};

struct s_looping_channel_effects
{
	dword flags;
	short effect;
	byte unknown06[0x18 - 6];
	s_looping_effect_playback playback;
	s_looping_impulse_parameters impulse;
};

struct s_looping_channel_properties
{
	dword flags;
	long format;
	real pitch;
	long gain_bits;
	byte unknown10[0x2c - 0x10];
	long controller_index;
	s_looping_channel_effects effects;
};

struct s_looping_channel_spatialization
{
	byte unknown00[0x14];
	real doppler_scale;
	byte unknown18[0x38 - 0x18];
};

struct s_looping_driver_channel
{
	byte unknown00[0xd];
	signed char state : 3;
	byte unknown0d : 5;
	byte unknown0e[0x34 - 0xe];
};

struct s_looping_permutation
{
	byte unknown00[4];
	char gain;
	byte unknown05[7];
	short first_chunk;
	short chunk_count;
};

struct s_looping_chunk
{
	byte unknown00[4];
	dword unknown04 : 31;
	dword transition_boundary : 1;
	byte unknown08[4];
};

struct s_looping_chunk_tables
{
	byte unknown00[0x44];
	s_looping_chunk *chunks;
};

struct s_unknown_5c;
struct s_sound_permutation;
struct s_permutation_group;
struct s_bink_sound_settings;
extern s_bink_sound_settings *g_51ebe4;

s_unknown_5c *function_221810(short index);
long function_219290(short index, s_permutation_group *group);
void sound_voice_mark_channel(short voice_index);
void sound_voice_queue_chunk(short voice_index, s_sound_permutation const *permutation, short chunk_index);
void function_12a1b0(short voice_index, s_looping_track_sound *sound, s_looping_channel_spatialization const *spatialization, s_looping_channel_properties *properties);
void function_21f8a0(short channel_index, s_looping_channel_properties const *properties, s_looping_effect_playback const *effects);
void __stdcall function_21f720(s_looping_impulse_parameters const *parameters);
void function_21fa80(short channel_index, s_looping_channel_properties const *properties, s_looping_effect_playback const *effects, bool force);

// @retail 0x21c400
void update_channel_for_looping_sound(short voice_index, real gain, s_looping_channel_spatialization const *spatialization)
{
	s_looping_voice_view *voice = &((s_looping_voice_view *)g_4e6378)[voice_index];
	long sound_index = voice->sound_index;
	s_looping_track_sound *playing = track_sound_get(g_4e637c, sound_index);
	s_looping_playback_definition *playback = (s_looping_playback_definition *)g_4e3b44[playing->definition_index & 0xffff].bytes;
	s_permutation_set *pitch_range = &g_51ebd4->sets[playback->pitch_range_base + playing->pitch_range_index];
	looping_sound_datum *sound = (looping_sound_datum *)LOOPING_SOUNDS->data + (playing->looping_sound_index & 0xffff);
	looping_sound_track_datum *track = (looping_sound_track_datum *)sound->track_storage + playing->loop.track_index;
	looping_sound_definition *definition = (looping_sound_definition *)g_4e3b44[sound->definition_index & 0xffff].bytes;
	looping_sound_track_definition *track_definition = &definition->tracks[playing->loop.track_index];
	c_looping_sound_controller *controller = controller_get(sound->controller_index);
	s_looping_pitch_modifier *modifier = &((s_looping_sound_tables *)g_51ebd4)->pitch_modifiers[playback->playback_index];
	real scale = playing->source.scale;
	real track_gain = *(real *)&playing->track_value;
	real pitch = ((real)modifier->upper - (real)modifier->lower) * track->gain + playing->pitch + (real)modifier->lower;
	bool primary = track->sound_index == sound_index;
	s_looping_channel_properties properties;
	properties.flags = 0;
	memset((byte *)&properties + 4, 0, 0x30 - 4);
	properties.effects.flags = 0;
	memset((byte *)&properties.effects + 4, 0, sizeof(properties.effects) - 4);
	real total_gain = *(real *)&playing->source.unknown08 + (track_gain + gain);
	properties.gain_bits = function_1251e0(playback, *(long *)&total_gain, scale);
	properties.effects.effect = *(short *)((byte *)track_definition + 0x3c);
	if (properties.effects.effect)
		properties.effects.flags |= 1;
	properties.controller_index = definition->flags & 0x20 ? sound->controller_index : NONE;
	s_looping_pitch_bounds *bounds = &((s_looping_pitch_tables *)g_51ebd4)->bounds[*(short *)((byte *)pitch_range + 2)];
	properties.pitch = pitch - (real)bounds->unknown00;
	if (playing->sound_class & 2)
	{
		byte const *promotion = (byte const *)function_221810((char)playback->unknown02[0]);
		real doppler;
		if (sound->controller_index != NONE)
		{
			c_looping_sound_controller *source_controller = controller_get(sound->controller_index);
			doppler = source_controller->definition_index != NONE ? source_controller->listener_values[playing->listener_index] : 0.f;
		}
		else
			doppler = function_12a9d0(playing->listener_index, &playing->source.spatial);
		properties.pitch += *(real const *)(promotion + 0x48) * spatialization->doppler_scale * doppler;
	}
	function_12a1b0(voice_index, playing, spatialization, &properties);
	if (!TEST_FIELD_BIT(playing->channel_initialized))
	{
		s_looping_permutation *permutation = &((s_looping_permutation *)g_51ebd4->chances)[pitch_range->first_index + playing->permutation_index];
		*(real *)&properties.gain_bits += (real)permutation->gain;
		function_21f8a0(voice->channel_index, &properties, &properties.effects.playback);
		function_21f720(&properties.effects.impulse);
		sound_voice_queue_chunk(voice_index, (s_sound_permutation const *)permutation, playing->current_chunk_index);
		if (definition->flags & 0x20)
		{
			char synch_count = NONE;
			if (controller->definition_index != NONE)
			{
				synch_count = controller->synch_count;
				--controller->synch_count;
				controller->synch_playback = false;
			}
			playing->loop.synch_state = synch_count;
		}
		playing->channel_initialized = true;
	}
	else
	{
		if (playing->state == 2 || playing->state == 3 || playing->state == 1)
		{
			short channel = ((s_looping_voice_view *)g_4e6378)[playing->channel_index].channel_index;
			short channel_state = ((s_looping_driver_channel *)g_51ebe4)[channel].state;
			if (!TEST_FIELD_BIT(playing->transition_pending) &&
				(channel_state != 3 || (byte)looping_sound_track_has_next_sound(track, track_definition)))
			{
				dword *seed = &g_4e7408->seed;
				if (definition->flags & 0x30)
					seed = primary ? &track->random_seed : &playing->loop.random_seed;
				short permutation_index = playing->permutation_index;
				s_looping_playback_definition *current = (s_looping_playback_definition *)g_4e3b44[playing->definition_index & 0xffff].bytes;
				s_permutation_set *current_range = &g_51ebd4->sets[current->pitch_range_base + playing->pitch_range_index];
				s_looping_permutation *permutation = &((s_looping_permutation *)g_51ebd4->chances)[current_range->first_index + playing->permutation_index];
				short next_chunk = NONE;
				if (permutation && playing->current_chunk_index < permutation->chunk_count - 1)
					next_chunk = playing->current_chunk_index + 1;
				bool last_chunk = next_chunk == NONE;
				if (!(playing->loop.flags & 1) && primary && (byte)looping_sound_track_has_next_sound(track, track_definition))
				{
					if (last_chunk || (looping_sound_track_transition_requires_stop(track) &&
						TEST_FIELD_BIT(((s_looping_chunk_tables *)g_51ebd4)->chunks[permutation->first_chunk + playing->current_chunk_index].transition_boundary)))
					{
						if (sound_commit_looping_definition(sound, track, voice->sound_index, seed))
							return;
						current = (s_looping_playback_definition *)g_4e3b44[playing->definition_index & 0xffff].bytes;
						pitch_range = &g_51ebd4->sets[current->pitch_range_base + playing->pitch_range_index];
						playing->transition_pending = true;
						goto prepare_chunk;
					}
				}
				else if (last_chunk)
				{
					if (definition->flags & 2)
					{
						playing->state = 4;
						if (primary)
							sound->flags |= 8;
						goto stop_channel;
					}
					dword permutations = primary ? looping_sound_get_permutations(sound, definition, playback, playing->loop.track_index, playing->pitch_range_index, false) : playing->loop.permutations;
					permutation_index = function_219110((s_set_ref *)playback, playing->pitch_range_index, &permutations, playing->permutation_index, seed, NULL, false);
					if (primary)
						looping_sound_set_permutations(sound, definition, playback, playing->loop.track_index, playing->pitch_range_index, permutations, permutation_index);
					else
						playing->loop.permutations = permutations;
					next_chunk = 0;
				}
				if (permutation_index != NONE)
				{
					sound_playback_set_chunk(voice->sound_index, playing->definition_index, playing->pitch_range_index, (char)permutation_index, next_chunk);
					playing->transition_pending = true;
				}
				else
				{
stop_channel:
					playing->stop_requested = true;
					sound_voice_mark_channel(voice_index);
				}
			}
prepare_chunk:
			if (TEST_FIELD_BIT(playing->transition_pending))
			{
				s_looping_permutation *permutation = &((s_looping_permutation *)g_51ebd4->chances)[pitch_range->first_index + playing->permutation_index];
				if ((byte)function_125e60(track_sound_get(g_4e637c, voice->sound_index)) && channel_state != 3)
				{
					sound_voice_queue_chunk(voice_index, (s_sound_permutation const *)permutation, playing->current_chunk_index);
					playing->transition_pending = false;
					if (primary && !(byte)looping_sound_track_has_next_sound(track, track_definition) &&
						(short)function_219290(playing->current_chunk_index, (s_permutation_group *)permutation) == NONE)
					{
						if (playing->state == 1)
							playing->state = 2;
						else if (playing->state == 3)
							playing->state = 4;
					}
				}
			}
		}
		else
		{
			playing->stop_requested = true;
			sound_voice_mark_channel(voice_index);
		}
		s_looping_permutation *permutation = &((s_looping_permutation *)g_51ebd4->chances)[pitch_range->first_index + playing->permutation_index];
		*(real *)&properties.gain_bits += (real)permutation->gain;
		function_21fa80(((s_looping_voice_view *)g_4e6378)[voice_index].channel_index, &properties, &properties.effects.playback, false);
	}
	if (primary)
	{
		playing->loop.random_seed = track->random_seed;
		playing->loop.permutations = track->permutations;
	}
}

struct s_sound_location_source;
void sound_source_get_position(s_sound_location_source const *source, long listener_index, real_point3d *position);
real function_192d70(real minimum, real maximum, real distance);
real_point3d *function_142700(real_matrix4x3 const *matrix, real_point3d const *point, real_point3d *out);
real function_30bf0(real_vector3d *vector);
void function_192ce0(real_point3d const *direction, real *gains, bool linear);

// @retail 0x21a250
bool sound_refresh_looping(long definition_index, dword sound_flags, long identifier, long controller_definition_index,
	s_sound_location *source, long state, dword flags, real fade_duration)
{
	bool result = state == 2;
	if (definition_index == NONE)
		return true;
	bool alternate = (flags & 1) != 0;
	if (!looping_sound_system_available())
		return result;
	looping_sound_definition *definition = (looping_sound_definition *)g_4e3b44[definition_index & 0xffff].bytes;
	bool combine_sources = source->audible != 0 && (definition->flags & 0x80);
	if (combine_sources)
		identifier = definition_index | 0x6000;
	long index = looping_sound_find(definition_index, identifier);
	bool created = false;
	result = true;
	if (index != NONE)
	{
		if ((flags & 0x10) && state == 0)
			state = 1;
	}
	else
	{
		if (!((1 << state) & 3))
			return result;
		index = looping_sound_new(definition_index, (short)sound_flags, identifier, controller_definition_index);
		created = true;
		if (index == NONE)
			return false;
	}
	looping_sound_datum *sound = (looping_sound_datum *)LOOPING_SOUNDS->data + (index & 0xffff);
	sound->sound_class = (short)sound_flags;
	if (!TEST_FIELD_BIT(((s_looping_source_flags *)source)->flag1))
		sound->source = *source;
	render_debug_looping_sound(definition_index, &sound->source);
	if (combine_sources)
	{
		if (sound->update_phase != *((byte *)g_4e6380 + 0x7f))
			memset(sound->listener_gains, 0, sizeof(sound->listener_gains));
		s_looping_source_tail *tail = (s_looping_source_tail *)sound->source.unknown30;
		real maximum = TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag3) ? tail->value38 : definition->maximum_distance;
		real distance;
		long listener = function_127d00(&sound->source, maximum, &distance);
		if (listener != NONE)
		{
			real minimum = TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag2) ? tail->value34 : definition->minimum_distance;
			minimum = minimum > 0.001f ? minimum : 0.001f;
			real gain = function_192d70(minimum, maximum, distance);
			real_point3d position;
			sound_source_get_position((s_sound_location_source const *)&sound->source, listener, &position);
			if (sound->source.audible == 1)
			{
				real_matrix4x3 const *matrix = (real_matrix4x3 const *)((byte *)g_4e6380 + 0x90 + listener * 0x48);
				function_142700(matrix, &position, &position);
			}
			if (distance > minimum)
				function_30bf0((real_vector3d *)&position);
			real gains[4];
			function_192ce0(&position, gains, true);
			real scale = 1.f;
			real total = 0.f;
			for (long i = 0; i < 4; ++i)
			{
				real value = gains[i] * gain;
				total += value * value;
			}
			if (total > 1.f)
				scale = 1.f / total;
			for (long i = 0; i < 4; ++i)
				sound->listener_gains[i] += gains[i] * scale * gain;
		}
		sound->sound_class &= ~2;
	}
	sound->update_phase = *((byte *)g_4e6380 + 0x7f);
	if ((state == 2 || (sound->flags & 8)) && sound->playing_sound_count == 0)
	{
		looping_sound_delete(index);
		return result;
	}
	result = false;
	for (long i = 0; i < definition->track_count; ++i)
	{
		looping_sound_track_definition *track_definition = &definition->tracks[i];
		looping_sound_track_datum *track = (looping_sound_track_datum *)sound->track_storage + i;
		dword *seed = &g_4e7408->seed;
		if (definition->flags & 0x30)
			seed = &track->random_seed;
		bool track_alternate = alternate;
		if (track_definition->alternate_start_sound_index == NONE && track_definition->alternate_loop_sound_index == NONE &&
			track_definition->alternate_end_sound_index == NONE && track_definition->alternate_transition_sound_index == NONE &&
			!(track_definition->flags & 0x14))
			track_alternate = false;
		if (created)
		{
			track->gain = sound->source.scale;
			track->completed_count = 0;
		}
		if (created || track->sound_index == NONE)
		{
			track->sound_index = NONE;
			track->current_state = track_alternate ? 3 : 0;
			track->next_state = track->current_state;
		}
		if (state == 0 && track_definition->start_sound_index != NONE)
		{
			long old_end = looping_sound_find_sound(index, track_definition->end_sound_index, NONE, (short)i);
			if (!(track_definition->flags & 1))
				track->sound_index = looping_sound_new_sound(index, track_definition->start_sound_index, NONE, (short)i, 1, seed);
			else
			{
				looping_sound_new_sound(index, track_definition->start_sound_index, NONE, (short)i, 0, seed);
				if (old_end != NONE)
				{
					real duration = track_definition->fade_in_duration;
					s_looping_track_sound *playing = track_sound_get(g_4e637c, old_end);
					playing->fade_gain_bits = function_12a810(old_end);
					playing->fade_curve = 1;
					playing->fade_start_time = (long)(duration * 1000.f);
					playing->fade_end_time = NONE;
					playing->looping = true;
				}
				looping_sound_fade_in(track->sound_index, 1, track_definition->fade_in_duration);
			}
			track->current_state = 0;
			track->next_state = track->current_state;
		}
		if (state != 2 && !(sound->flags & 8))
		{
			long next_definition = NONE;
			long impulse_definition = NONE;
			dword impulse_states = track_definition->flags & 1;
			if (track_definition->flags & 2)
				impulse_states |= 4;
			else
				impulse_states &= ~4;
			if (track_definition->flags & 2)
				impulse_states |= 0x40;
			else
				impulse_states &= ~0x40;
			if (track_definition->flags & 4)
				impulse_states |= 8;
			else
				impulse_states &= ~8;
			if (track_definition->flags & 4)
				impulse_states |= 0x20;
			else
				impulse_states &= ~0x20;
			bool current_alternate = ((1 << track->current_state) & 0x58) != 0;
			if (track->current_state == track->next_state ||
				*(long *)((byte *)track_definition + g_44a188[track->next_state] + 4) == NONE || track_alternate != current_alternate)
			{
				long next = track_alternate ? g_44a16c[track->current_state] : g_44a150[track->current_state];
				long previous = track->current_state;
				while (next != previous)
				{
					long offset = g_44a188[next];
					byte *reference = offset == NONE ? NULL : (byte *)track_definition + offset;
					if (reference && *(long *)(reference + 4) != NONE)
					{
						if (!(impulse_states & (1 << next)))
						{
							next_definition = *(long *)(reference + 4);
							break;
						}
						impulse_definition = *(long *)(reference + 4);
					}
					previous = next;
					next = track_alternate ? g_44a16c[next] : g_44a150[next];
				}
				track->next_state = (char)next;
				if (impulse_definition != NONE)
					looping_sound_new_sound(index, impulse_definition, NONE, (short)i, 0, seed);
			}
			if (next_definition == NONE && track_alternate == (((1 << track->current_state) & 0x58) != 0))
				continue;
			bool start_with_fade = state == 0 && (track_definition->flags & 1);
			if (track->sound_index != NONE && !start_with_fade)
			{
				s_looping_track_sound *playing = track_sound_get(g_4e637c, track->sound_index);
				if (playing->definition_index == next_definition ||
					track_alternate == (((1 << track->current_state) & 0x58) != 0) || !(track_definition->flags & 4))
					continue;
				long replacement = NONE;
				if (next_definition != NONE)
				{
					replacement = looping_sound_find_or_create_sound(index, next_definition, NONE, (short)i, 2, seed);
					if (replacement == NONE)
						continue;
				}
				function_126df0(replacement, track->sound_index, 1, track_definition->crossfade_duration);
				track->sound_index = replacement;
				track->current_state = track_alternate ? 4 : 1;
				track->next_state = track->current_state;
			}
			else if (next_definition != NONE)
			{
				s_looping_playback_definition *playback = (s_looping_playback_definition *)g_4e3b44[next_definition & 0xffff].bytes;
				s_looping_sound_tables *tables = (s_looping_sound_tables *)g_51ebd4;
				s_looping_pitch_modifier *modifier = &tables->pitch_modifiers[playback->playback_index];
				real pitch = ((real)modifier->upper - (real)modifier->lower) * track->gain + (real)tables->classes[playback->sound_class].pitch + (real)modifier->lower;
				short pitch_range = function_218f50(playback, NONE, pitch);
				long replacement = looping_sound_find_sound(index, next_definition, pitch_range, (short)i);
				if (replacement == NONE)
					replacement = looping_sound_new_sound(index, next_definition, pitch_range, (short)i, 2, seed);
				if (replacement != NONE)
				{
					if (state != 0)
						function_126df0(replacement, NONE, 1, 0.5f);
					else if (track_definition->flags & 1)
						function_126df0(replacement, NONE, 1, track_definition->fade_in_duration);
					track->sound_index = replacement;
					track->current_state = track->next_state;
				}
			}
		}
		else if (sound->state != 2)
		{
			if (fade_duration != 0.f)
			{
				if (track->sound_index != NONE)
					looping_sound_fade_out(fade_duration, track->sound_index);
				continue;
			}
			bool current_alternate = ((1 << track->current_state) & 0x58) != 0;
			bool alternate_end = current_alternate && ((track_definition->flags & 0x10) || track_definition->alternate_end_sound_index != NONE);
			long end_definition = alternate_end ? track_definition->alternate_end_sound_index : track_definition->end_sound_index;
			bool impulse_end = (track_definition->flags & (alternate_end ? 0x10 : 2)) != 0;
			if (track->sound_index != NONE)
			{
				if (!impulse_end && end_definition == NONE && (definition->flags & 2))
					continue;
				if (impulse_end || end_definition == NONE)
				{
					real duration = current_alternate && (track_definition->flags & 0x10) ?
						track_definition->alternate_fade_out_duration : track_definition->fade_out_duration;
					looping_sound_fade_out(duration, track->sound_index);
				}
			}
			if (end_definition != NONE)
			{
				if (impulse_end)
					looping_sound_new_sound(index, end_definition, NONE, (short)i, 0, seed);
				else if (track->sound_index != NONE)
				{
					s_looping_track_sound *playing = track_sound_get(g_4e637c, track->sound_index);
					if (playing->channel_index != NONE)
					{
						track->next_state = alternate_end ? 6 : 2;
						playing->state = 3;
					}
				}
			}
		}
	}
	if (sound->playing_sound_count == 0)
	{
		real maximum = TEST_FIELD_BIT(((s_looping_source_flags *)&sound->source)->flag3) ?
			((s_looping_source_tail *)sound->source.unknown30)->value38 :
			((looping_sound_definition *)g_4e3b44[sound->definition_index & 0xffff].bytes)->maximum_distance;
		if (function_127d00(&sound->source, maximum, NULL) == NONE)
		{
			looping_sound_delete(index);
			return result;
		}
	}
	if (alternate)
		sound->flags |= 1;
	else
		sound->flags &= ~1;
	if (flags & 2)
		sound->flags |= 2;
	else
		sound->flags &= ~2;
	if (flags & 4)
		sound->flags |= 4;
	else
		sound->flags &= ~4;
	sound->state = (byte)state;
	return result;
}
