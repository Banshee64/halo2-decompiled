#ifndef KILL_VOLUMES_H
#define KILL_VOLUMES_H

#include "cseries.h"

/* the enabled kill volumes: a bit vector of 256 bits, allocated from the game
   state (src/unknown_1eb8a0.cpp); the scripts set bits (2a0d70) */
struct s_kill_volume_globals
{
	dword bits[8];
	bool enabled;
};

extern s_kill_volume_globals *g_51e9c8;

#endif
