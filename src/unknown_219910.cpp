// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_219910.CPP: the per-source sound records in g_51ebd8: one
   reference-counted record (with its own random seed) per sound source key,
   found by key, added and released by the sounds that play from that source */

#include "cseries.h"
#include "data_array.h"
#include "sound_records.h"
#include <string.h>

/* the sound system's state, as these functions read it */
struct s_sound_system_record_view
{
	byte unknown00[0x78];
	bool initialized;
	bool hardware_available;
	bool enabled;
	byte unknown7b[4];
	byte flag7f_0 : 1;
	byte unknown7f : 7;
};

struct s_4e6380;
extern s_4e6380 *g_4e6380;
extern void *g_51ebd8;
extern void *g_51ebdc;

dword random_seed_generate(void);

static inline s_sound_system_record_view *sound_system_get(void)
{
	return (s_sound_system_record_view *)g_4e6380;
}

static inline bool sound_system_available(void)
{
	s_sound_system_record_view *sound_system = sound_system_get();
	return sound_system->initialized && sound_system->hardware_available && sound_system->enabled;
}

static inline s_data_array *sound_records(void)
{
	return (s_data_array *)g_51ebd8;
}

static inline s_sound_record *sound_record_get(long record_index)
{
	s_sound_record *record = (s_sound_record *)g_51ebdc;

	if (record_index != NONE)
	{
		record = &((s_sound_record *)sound_records()->data)[record_index & 0xffff];
	}
	return record;
}

/* data_next_index (unknown_16b570.cpp, /Ob1), which retail inlines here */
static inline long data_next_index_inlined(s_data_array *data, long datum_index)
{
	long start;
	long index;
	long result;

	if (datum_index == NONE)
	{
		start = 0;
	}
	else
	{
		start = (datum_index & 0xffff) + 1;
	}

	index = data_next_absolute_index(data, start);
	result = NONE;
	if (index != NONE)
	{
		result = (*(short *)(data->data + data->size * index) << 16) | index;
	}
	return result;
}

// @retail 0x219960
long sound_record_find(long key)
{
	long result = NONE;

	if (sound_system_available() && key != NONE)
	{
		s_data_array *records = sound_records();
		long record_index = data_next_index_inlined(records, NONE);

		while (record_index != NONE)
		{
			s_sound_record *record = sound_record_get(record_index);
			if (record->key == key)
			{
				break;
			}
			record_index = data_next_index_inlined(records, record_index);
		}
		result = record_index;
	}
	return result;
}

// @retail 0x219a30
long sound_record_add_reference(long key)
{
	long result = NONE;

	if (sound_system_available() && key != NONE)
	{
		long record_index = sound_record_find(key);
		if (record_index != NONE)
		{
			s_sound_record *record = &((s_sound_record *)sound_records()->data)[record_index & 0xffff];
			record->reference_count++;
		}
		result = record_index;
	}
	return result;
}

// @retail 0x219e90
void sound_record_initialize(s_sound_record *record, long key, dword seed, bool flag)
{
	record->key = key;
	record->seed = seed;
	record->unused = !flag;
	record->unknown03 = 0x80;
	memset(&record->values, 0, sizeof(record->values));
}

// @retail 0x219a90
long sound_record_new(long key)
{
	long result = NONE;

	if (sound_system_available() && key != NONE)
	{
		long record_index = sound_record_find(key);
		if (record_index == NONE)
		{
			record_index = datum_new(sound_records());
		}

		if (record_index != NONE)
		{
			s_sound_record *record = sound_record_get(record_index);
			record->reference_count++;
			if (record->reference_count == 1)
			{
				sound_record_initialize(record, key, random_seed_generate(), sound_system_get()->flag7f_0);
			}
		}
		result = record_index;
	}
	return result;
}

// @retail 0x219c80
void sound_record_release(long record_index)
{
	if (record_index != NONE)
	{
		s_sound_record *record = &((s_sound_record *)sound_records()->data)[record_index & 0xffff];

		record->reference_count--;
		if (record->reference_count == 0)
		{
			datum_delete(sound_records(), record_index);
		}
	}
}
