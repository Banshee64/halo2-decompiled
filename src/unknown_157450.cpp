// @flags /O2 /Gr
/* UNKNOWN_157450.CPP: the lifecycle callbacks of entry 22 */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include "engine_peer.h"
#include <string.h>

dword g_502258[0x27];

// @retail 0x157450
void function_157450(void)
{
	s_mp_globals *data = (s_mp_globals *)game_state_malloc("unknown", "unknown", sizeof(s_mp_globals));

	memset(data, 0, sizeof(*data));
	memset(g_502258, 0, sizeof(g_502258));
	g_4e9ae8 = data;
	data->engine_index = NONE;
}

// @retail 0x157bb0
void function_157bb0(void)
{
	c_engine_peer *object = g_55e4d0[g_4e9ae8->engine_index];

	if (object)
	{
		object->p2();
		g_4e9ae8->engine_index = NONE;
	}
}
