#include "cseries.h"

struct s_sound_position;

// @stub 0x12a9d0
real function_12a9d0(long listener_index, s_sound_position const *position) { return 0; }

struct s_sound_location;
struct s_looping_track_sound;
struct s_looping_playback_definition;

// @stub 0x127d00
long __stdcall function_127d00(s_sound_location const *source, real maximum_distance, real *distance) { return NONE; }

// @stub 0x218f50
short function_218f50(s_looping_playback_definition *definition, short previous, real pitch) { return NONE; }

struct s_looping_voice_counts;

struct s_looping_detail_request;

// @stub 0x125f70
long function_125f70(long definition_index, s_looping_detail_request *request, long *reason) { return NONE; }

// @stub 0x126df0
void function_126df0(long new_sound, long old_sound, short curve, real duration) { }

struct s_looping_channel_properties;
struct s_looping_channel_spatialization;
struct s_looping_effect_playback;
struct s_looping_impulse_parameters;

// @stub 0x12a1b0
void function_12a1b0(short voice_index, s_looping_track_sound *sound, s_looping_channel_spatialization const *spatialization, s_looping_channel_properties *properties) { }

// @stub 0x21f8a0
void function_21f8a0(short channel_index, s_looping_channel_properties const *properties, s_looping_effect_playback const *effects) { }

// @stub 0x21fa80
void function_21fa80(short channel_index, s_looping_channel_properties const *properties, s_looping_effect_playback const *effects, bool force) { }
