#ifndef RECORDED_ANIMATIONS_H
#define RECORDED_ANIMATIONS_H

#include "cseries.h"

/* one recorded animation playing on an object (0xa0 bytes;
   src/unknown_1fb350.cpp) */
struct s_recorded_animation
{
	short salt;
	byte unknown02[2];
	long object_index;
	word ticks;
	word flags;
	long unknown0c;
	long unknown10;
	byte unknown14[0x9c - 0x14];
	short unknown9c;
	byte unknown9e[2];
};

s_recorded_animation *recorded_animation_find(long object_index, long *datum_index);
long recorded_animation_get_frames(long object_index);

#endif
