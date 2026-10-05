// @flags /O2 /Ob1 /Gr
/* UNKNOWN_161E60.CPP: game engine queries outside lane Q's region that its
   game engine code calls (the team test and the statistics getter) */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_157450.h"

// @retail 0x161e60
bool game_engine_team_is_active(long team)
{
	bool result = false;

	if (function_x340af0() && team >= 0 && team < 8)
	{
		result = (function_xaee93d()->team_mask & (1 << team)) != 0;
	}
	return result;
}

// @retail 0x162030
s_statborg *game_engine_get_statborg()
{
	return game_engine_statborg_inline();
}
