/* SOUND_RECORDS.H: what the sound effects in g_51ebe0 (unknown_21d110.cpp)
   use: the looping sound controllers in g_51ebd8 (looping_sound_manager.cpp)
   and the state a sound is started from (s_sound_play_state, with the effect
   marker, in sound_manager.h) */
#ifndef SOUND_RECORDS_H
#define SOUND_RECORDS_H

#include "cseries.h"
#include "sound_sources.h"
#include "sound_manager.h"

/* looping_sound_manager.cpp */
long looping_sound_controller_find_existing(long definition_index);
long looping_sound_controller_find_and_reference(long definition_index);
long looping_sound_controller_find_or_create(long definition_index);
void looping_sound_controller_release(long index);

long function_21d110(s_sound_play_state *state, long tag_index);

#endif
