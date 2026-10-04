// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1EB550.CPP: the lifecycle callbacks of entry 25 */

#include "cseries.h"
#include "game_state.h"
#include "globals.h"
#include "unknown_1eb550.h"

s_unknown_1eb550 *g_51e9c4;

// @retail 0x1eb550
void function_1eb550(void)
{
	s_unknown_1eb550 *data = (s_unknown_1eb550 *)function_123d40("unknown", "unknown", sizeof(s_unknown_1eb550));

	data->unknown0 = 4.1712594f;
	data->unknown4 = 1.0f;
	data->unknown8 = 0.0011f;
	data->unknown18 = 0;
	g_51e9c4 = data;
	data->vector = *g_4687a4;
}

// @retail 0x1eb5e0
void function_1eb5e0(void)
{
	g_51e9c4 = 0;
}

// @retail 0x1eb5f0
void function_1eb5f0(void)
{
	s_unknown_1eb550 *data = g_51e9c4;

	data->unknown0 = 4.1712594f;
	data->unknown4 = 1.0f;
	data->unknown8 = 0.0011f;
	data->unknown18 = 0;
	data->vector = *g_4687a4;
}
