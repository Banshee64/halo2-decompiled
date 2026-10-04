// @flags /O2 /Ob1 /Gr
/* UNKNOWN_125360.CPP: the camera of the first local player.
   Decompiled by lane F: 0x18c3b0 calls it. */

#include "unknown_11c920.h"
#include "globals.h"
#include "local_cameras.h"

static inline long local_player_first_index(void)
{
	long result = NONE;

	for (long i = 0; i < 4; i++)
	{
		if (g_4e8c20->entries[i] != NONE)
		{
			result = i;
			break;
		}
	}
	return result;
}

// @retail 0x125360
s_local_camera *function_125360(void)
{
	s_local_camera *result = NULL;

	if (local_player_first_index() != NONE)
	{
		result = local_camera_get(local_player_first_index());
		if (result && !result->active)
		{
			result = NULL;
		}
	}
	return result;
}
