/* SOUND_MANAGER.H: the playing sounds of the sound manager (src/sound_manager.cpp)
   and the request a sound is started from (0x21d110 builds it) */
#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "cseries.h"
#include "sound_sources.h"

/* what a sound is started from (0xa0 bytes); the flags say which of the
   optional fields are set */
enum
{
	_sound_play_state_has_flags_bit = 0x2,
	_sound_play_state_has_object_bit = 0x8,
	_sound_play_state_has_effect_bit = 0x10,
	_sound_play_state_has_source_bit = 0x20,
	_sound_play_state_has_value8e_bit = 0x40,
	_sound_play_state_has_gain_bit = 0x80,
	_sound_play_state_has_platform_playback_bit = 0x100,
	_sound_play_state_has_value90_bit = 0x200,
	_sound_play_state_has_variants_bit = 0x400,
};

struct s_sound_play_state
{
	dword flags;
	short priority;
	byte unknown06[2];
	long playback_flags;
	s_sound_location location;
	long object_index;
	long effect_index;
	s_sound_source_callbacks const *source;
	union
	{
		s_sound_marker marker;
		byte source_data[0x30];
	};
	short source_data_size;
	short value8e;
	char value90;
	byte unknown91[3];
	real gain;
	long platform_playback;
	word variant0;
	word variant1;
};

/* the flags of a playing sound */
struct s_sound_playback_flags
{
	word flag0 : 1;
	word holds_reference : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word source_updated : 1;
	word unknown04 : 10;
};

/* a sound being played, in the 0x4e637c array (0xbc bytes) */
struct s_sound_playback
{
	word identifier;
	char state;
	char unknown03;
	union
	{
		word flags;
		struct
		{
			word flag0 : 1;
			word holds_reference : 1;
			word flag2 : 1;
			word flag3 : 1;
			word flag4 : 1;
			word source_updated : 1;
			word unknown04 : 10;
		};
	};
	short priority;
	long platform_playback;
	long definition_index;
	long object_index;
	s_sound_source_callbacks const *source;
	s_sound_location location;
	union
	{
		s_sound_marker marker;
		byte source_data[0x30];
	};
	long start_time;
	real pitch;
	real gain;
	dword seed;
	char pitch_range_index;
	char permutation_index;
	short chunk_index;
	char value_a0;
	char listener_index;
	short value_a2;
	char value_a4;
	byte unknown_a5[3];
	long effect_index;
	short value_ac;
	byte unknown_ae[6];
	long value_b4;
	long value_b8;
};

#define SOUND_PLAYBACK_GET(index) (&((s_sound_playback *)g_4e637c->data)[(index) & 0xffff])

#endif
