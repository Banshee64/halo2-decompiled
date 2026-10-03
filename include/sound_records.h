/* SOUND_RECORDS.H: the per-source sound records in g_51ebd8
   (unknown_219910.cpp), the sound effects in g_51ebe0 (unknown_21d110.cpp)
   that hold them, and the state a sound is started from */
#ifndef SOUND_RECORDS_H
#define SOUND_RECORDS_H

#include "cseries.h"
#include "sound_sources.h"

/* a sound source's record (0x1c bytes) */
struct s_sound_record
{
	short salt;
	byte reference_count : 7;
	byte unused : 1;
	byte unknown03;
	long key;
	dword seed;
	long values[4];
};

long sound_record_find(long key);
long sound_record_add_reference(long key);
long sound_record_new(long key);
void sound_record_release(long record_index);

/* what a sound playing through a sound effect adds to its marker: the effect
   and the source the sound had before (s_sound_play_state::marker) */
struct s_sound_effect_link
{
	long effect_index;
	s_sound_source_callbacks const *source;
};

struct s_sound_effect_marker
{
	s_sound_marker marker;
	s_sound_effect_link link;
};

/* what 0x21d110 starts a sound from (0xa0 bytes) */
struct s_sound_play_state
{
	dword flags;
	long priority;
	byte unknown08[4];
	s_sound_location location;
	long object_index;
	long record_key;
	s_sound_source_callbacks const *source;
	s_sound_effect_marker marker;
	short marker_size;
	byte unknown8e[0x98 - 0x8e];
	long platform_playback;
	short variant0;
	short variant1;
};

long function_21d110(s_sound_play_state *state, long tag_index);

#endif
