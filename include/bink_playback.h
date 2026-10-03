/* BINK_PLAYBACK.H: the sound settings Bink plays through (bink_playback.cpp
   defines g_51ebe4; the voice effects of unknown_191270.cpp and the sound
   streams of unknown_2ae170.cpp use its DirectSound object) */

#ifndef BINK_PLAYBACK_H
#define BINK_PLAYBACK_H

#include "cseries.h"

struct s_bink_sound_settings
{
	byte unknown0000;
	bool surround;
	byte unknown0002[0x2ab0 - 0x2];
	void *direct_sound;
};

extern s_bink_sound_settings *g_51ebe4;

#endif
