// @flags /O2 /Gr
/* UNKNOWN_12B070.CPP: the main loop's timing: the time it started, and the
   lengths of the last 15 vertical blanks reported by the rasterizer */

#include "cseries.h"
#include "globals.h"
#include "network_connection.h"
#include <xtl.h>
#include <string.h>

long __fastcall rasterizer_vblank_callback(void const *data);

extern s_connection_counter g_4e6398;
extern byte g_4e6388;
extern byte g_4e6389;

dword g_4e6390;
long g_4e6394;
/* the lengths of the last vertical blanks, a ring of 15 */
struct s_vblank_history
{
	short next;
	short counts[15];
};

s_vblank_history g_4e6400;
short g_485aca;

// @retail 0x12b2a0
void __cdecl main_vblank_callback(D3DVBLANKDATA *data)
{
	long count = rasterizer_vblank_callback(data);

	if (count > 0)
	{
		g_4e6400.counts[g_4e6400.next] = (short)count;
		g_4e6400.next = (g_4e6400.next + 1) % 15;
	}
}

// @retail 0x12b070
void main_time_initialize(void)
{
	g_4e6390 = GetTickCount();
	g_4e6394 = 0;
	g_4e6398.low = 0;
	g_4e6398.high = 0;
	D3DDevice_SetVerticalBlankCallback(main_vblank_callback);
	memset(g_4e6400.counts, 0, sizeof(g_4e6400.counts));
	g_4e6400.next = 0;
}

// @retail 0x12b3c0
bool function_12b3c0(void)
{
	bool result = true;

	if (!g_485aca)
	{
		return false;
	}
	if (!g_4e6948 || !g_4e6948->flag1120)
	{
		return false;
	}
	if (g_4e6388)
	{
		result = *(bool *)&g_4e6389;
	}
	return result;
}
