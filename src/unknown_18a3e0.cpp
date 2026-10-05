// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_18A3E0.CPP: requests the first chunks of every sound a looping
   sound's tracks play (an outside function lane A's script functions need) */

#include "unknown_11c920.h"
#include "globals.h"

/* a track of a looping sound tag (0x58 bytes) */
struct s_looping_sound_track_view
{
	byte unknown00[0x20];
	long sound_index;
	byte unknown24[0x58 - 0x24];
};

struct s_looping_sound_definition_view
{
	byte unknown00[0x1c];
	long track_count;
	s_looping_sound_track_view *tracks;
};

void sound_definition_request_first_chunk(long definition_index); /* unknown_124f90.cpp */

// @retail 0x18a3e0
void looping_sound_definition_request_first_chunks(long definition_index)
{
	if (definition_index != NONE)
	{
		s_looping_sound_definition_view *definition = (s_looping_sound_definition_view *)g_4e3b44[definition_index & 0xffff].bytes;
		for (short i = 0; i < definition->track_count; i++)
			sound_definition_request_first_chunk(definition->tracks[i].sound_index);
	}
}
