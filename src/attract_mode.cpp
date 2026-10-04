// @flags /O1 /arch:SSE /Gr
/* ATTRACT_MODE.CPP: the attract, intro and credits movies (retail's
   attract_mode.cpp; 0x223976, which decides when the attract movie starts,
   is not decompiled yet) */

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
