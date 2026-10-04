// @flags /O1 /arch:SSE /Gr
/* ATTRACT_MODE.CPP: the attract, intro and credits movies (retail's
   attract_mode.cpp) */

#include "cseries.h"
#include "globals.h"
#include "real_math.h"
#include "files.h"

file_reference *function_136710(file_reference *file, bool replace, const char *name);
bool function_1368f0(file_reference *file);
char *csprintf_256(char *buffer, const char *format, ...);
void bink_playback_start(char const *name, dword flags);
bool bink_playback_active(void);

extern dword g_51ebec;
extern dword g_54d5b8;
dword g_54d5bc;

bool function_138800();
bool function_163870(void);
bool network_session_manager_session_unready(void);
long function_147f4f();
dword function_235ca9(void);

/* whether the main menu may count down to the attract movie, and how long
   it waits (in milliseconds) */
bool g_4701b8 = true;
dword g_4701bc = 75000;

/* whether the attract movie should start: the main menu has been idle for
   long enough */
// @retail 0x223976
bool attract_mode_should_start(void)
{
	volatile bool result = false;

	if (function_138800() && g_4e6948->state == 3 && function_163870() && !network_session_manager_session_unready() &&
		!bink_playback_active())
	{
		dword idle_time = g_54d5b8 - (g_51ebec > g_54d5bc ? g_51ebec : g_54d5bc);
		bool main_menu = function_147f4f() == 9 || function_147f4f() == 6;
		dword delay = function_235ca9();

		g_4701b8 = !(idle_time >= g_4701bc - delay && main_menu);
		if (idle_time >= g_4701bc && main_menu)
		{
			result = true;
		}
	}
	return result;
}

/* the path of a movie, which is looked up on the disc */
// @retail 0x223aa8
bool attract_mode_movie_path(char *path, char const *name)
{
	file_reference file;

	csprintf_256(path, "d:\\bink\\%s_%d.bik", name, 60);
	function_1368f0(function_136710(&file, false, path));
	return true;
}

/* the path of the attract (0), intro (1) or credits movie */
// @retail 0x223a7d
bool attract_mode_movie_path_by_type(long type, char *path)
{
	switch (type)
	{
	case 0:
		return attract_mode_movie_path(path, "attract");
	case 1:
		return attract_mode_movie_path(path, "intro");
	default:
		return attract_mode_movie_path(path, "credits");
	}
}

/* plays the attract movie */
// @retail 0x223a21
void attract_mode_start(void)
{
	char path[256];
	long type = random_index(&g_4e7408->seed, 1);

	path[0] = 0;
	if (attract_mode_movie_path_by_type(type, path))
	{
		bink_playback_start(path, 14);
	}
	if (!bink_playback_active())
	{
		g_51ebec = g_54d5b8;
	}
}
