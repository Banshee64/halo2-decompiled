// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1DF3E0.CPP: one-time hud messages that a script shows by index */

#include "cseries.h"
#include "globals.h"

/* whether each message has been shown (hs_library_external.cpp) */
extern bool g_4f55dc[16];

/* the message string of each index */
long const g_445514[15] =
{
	0x100006f6, 0x100006f7, 0x100006f8, 0x100006f9, 0x100006fa, 0x100006fb, 0x100006fc, 0x100006fd,
	0x100006fe, 0x110006ff, 0x11000700, 0x11000701, 0x11000702, 0x11000703, 0
};

struct s_1df3e0_sounds
{
	byte unknown000[0x118];
	long sound_index;
};

struct s_1df3e0_globals
{
	byte unknown000[0x134];
	s_1df3e0_sounds *sounds;
};

void __fastcall scripted_hud_messages_clear(void);
long players_first_active_local_player(void);
void function_24cbbf(long player_index, long string_id);
void function_154220(short seconds, real x, real y, real z);
long function_1896c0(real scale, long tag_index);

/* shows the message once: clears the scripted messages, shows the index's
   string to the first local player for 20 seconds, and plays the sound */
// @retail 0x1df3e0
void function_1df3e0(long index)
{
	if (index >= 0 && index < 15 && !g_4f55dc[index])
	{
		long sound_index;

		g_4f55dc[index] = true;
		scripted_hud_messages_clear();
		function_24cbbf(players_first_active_local_player(), g_445514[index]);
		function_154220(20, 1.0f, 1.0f, 1.0f);
		sound_index = ((s_1df3e0_globals *)g_4e034c)->sounds->sound_index;
		if (sound_index != NONE)
		{
			function_1896c0(1.0f, sound_index);
		}
	}
}
