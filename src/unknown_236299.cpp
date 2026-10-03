// @flags /O1 /arch:SSE /Gr
/* UNKNOWN_236299.CPP: playing the user interface sounds */

#include "cseries.h"
#include "globals.h"

struct s_interface_sound_reference
{
	long index;
	byte unknown04[4];
};

struct s_interface_sound_globals
{
	byte unknown00[0x94];
	s_interface_sound_reference sounds[13];
};

struct s_sound_tag_view
{
	byte flags;
	byte unknown01[7];
	short first_set;
	char set_count;
};

struct s_sound_set_view
{
	short name;
	byte unknown02[0xa];
};

struct s_sound_globals_view
{
	byte unknown00[0x14];
	long *names;
	byte unknown18[0xc];
	s_sound_set_view *sets;
};

struct s_sound_play
{
	long object_index;
	long tag_index;
	real scale;
	byte *set;
};

struct s_user_interface_globals;
s_user_interface_globals *function_148350(void);
long function_11cae0(void);
byte __stdcall function_219070(long set_index);
/* unknown_189010.cpp */
struct s_sound_label_play;
long function_189760(s_sound_label_play const *play);
long function_1896c0(real scale, long tag_index);

// @retail 0x236299
void function_236299(long sound)
{
	s_interface_sound_globals *globals = (s_interface_sound_globals *)function_148350();
	if (globals)
	{
		long tag_index = sound < 13 ? globals->sounds[sound].index : NONE;
		if (tag_index != NONE)
		{
			s_sound_tag_view *tag = (s_sound_tag_view *)g_4e3b44[tag_index & 0xffff].bytes;
			if (tag->flags & 0x80)
			{
				long name;
				switch (function_11cae0())
				{
				case 0:
					name = 0x7000770;
					break;
				case 1:
					name = 0x8000771;
					break;
				case 2:
					name = 0x6000772;
					break;
				case 3:
					name = 0x6000773;
					break;
				case 4:
					name = 0x7000774;
					break;
				case 5:
					name = 0x7000775;
					break;
				case 6:
					name = 0x6000776;
					break;
				case 7:
					name = 0x7000777;
					break;
				case 8:
					name = 0xa000778;
					break;
				default:
					__assume(0);
				}

				for (long i = 0; i < tag->set_count; i++)
				{
					s_sound_globals_view *sound_globals = (s_sound_globals_view *)g_51ebd4;
					if (sound_globals->names[sound_globals->sets[tag->first_set + i].name] == name)
					{
						byte set[2];
						s_sound_play play;
						set[0] = (byte)i;
						set[1] = function_219070(i);
						play.object_index = NONE;
						play.tag_index = tag_index;
						play.scale = 1.f;
						play.set = set;
						function_189760((s_sound_label_play const *)&play);
						break;
					}
				}
			}
			else
			{
				function_1896c0(1.f, tag_index);
			}
		}
	}
}
