/* UNKNOWN_222930.H: the impulse parameters of a sound effect (src/unknown_222930.cpp),
   which the driver's impulse buffers play (function_21f720, src/sound_dsound_xbox.cpp) */

#ifndef UNKNOWN_222930_H
#define UNKNOWN_222930_H

#include "cseries.h"

struct s_tag_data;

/* the impulse a sound effect plays (16 bytes) */
struct s_looping_impulse_parameters
{
	short index;
	short mixbin;
	long data_size;
	s_tag_data const *data;
	long identifier;
};

void function_222930(long tag_index, long handle, s_looping_impulse_parameters *parameters);
void __stdcall function_21f720(s_looping_impulse_parameters const *parameters);

#endif
