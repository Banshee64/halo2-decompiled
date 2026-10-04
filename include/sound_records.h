/* SOUND_RECORDS.H: what the sound effects in g_51ebe0 (unknown_21d110.cpp)
   use: the looping sound controllers in g_51ebd8 (unknown_12a1b0.cpp)
   and the state a sound is started from (s_sound_play_state, with the effect
   marker, in unknown_124f90.h) */
#ifndef SOUND_RECORDS_H
#define SOUND_RECORDS_H

#include "unknown_11c920.h"
#include "sound_sources.h"
#include "unknown_124f90.h"

/* unknown_12a1b0.cpp */
long function_219960(long definition_index);
long looping_sound_controller_find_and_reference(long definition_index);
long function_219a90(long definition_index);
void looping_sound_controller_release(long index);

long function_21d110(s_sound_play_state *state, long tag_index);

#endif
