// @flags /O2 /Gr
/* UNKNOWN_12BE90.CPP: the main loop's checks of whether something is busy
   (loading, saving, a menu or a movie up) and its reset of the game time
   when the game loses focus */

#include "cseries.h"
#include "network_session_manager.h"
#include "async.h"
#include "globals.h"
#include <xtl.h>

/* hs_library_external.cpp and unknown_230612.cpp */
extern byte g_547f6e;
extern byte g_547f6f;
extern byte g_547f70;
extern byte g_547f71;

/* unknown_12de70.cpp: the geometry cache's pending loads */
extern long g_4e64a0;

byte g_547f72;
byte g_547f73;
byte g_547f76;
bool g_547f29;
bool g_4ed39d;
bool g_4ed39e;
dword g_4ed3a0;
long g_4e6470;

long g_55bd04;
long g_55bd08;
bool g_55c14c;

void function_593e0(void);

// @retail 0x12be90
bool function_12be90(void)
{
	bool result = false;

	if (g_4ed39d)
	{
		result = true;
	}
	if (g_547f73 || g_547f6e || g_547f6f || g_547f72 || g_547f70 || g_547f71 || g_547f76 || g_4e6470 > 0 || g_4e64a0 > 0)
	{
		result = true;
	}
	return result;
}

/* the game's state flags (unknown_13bf00.cpp), as these functions read them */
struct s_unknown_13bf00;
extern s_unknown_13bf00 *g_510c50;

struct s_510c50_view
{
	byte unknown00[6];
	bool revert_requested;
	byte unknown07[0x22 - 7];
	bool revert_checked;
};

extern byte g_547f74;
extern byte g_547f75;

bool function_163b60(void);
void function_18e700(void);

// @retail 0x12ba90
void function_12ba90(void)
{
	if (g_4e6948 && g_4e6948->flag1120 && (!g_510c54->active || !g_510c54->unknown01))
	{
		function_18e700();
		g_547f6e = false;
	}
}

// @retail 0x12bad0
void function_12bad0(void)
{
	s_510c50_view *state = (s_510c50_view *)g_510c50;
	bool revert = state->revert_requested;

	if (revert && state->revert_checked)
	{
		revert = !function_163b60();
	}
	g_547f72 = false;
	if (revert)
	{
		g_547f75 = false;
		g_547f6f = true;
		g_547f74 = true;
	}
}

// @retail 0x12bf00
void function_12bf00(void)
{
	g_547f29 = true;
	g_4ed39e = true;
	g_4ed39d = true;
	g_4ed3a0 = GetTickCount();
	if (g_527330.initialized && (g_527330.state == 3 || g_527330.state == 8))
	{
		function_593e0();
	}
}

// @retail 0x12bf40
void function_12bf40(void)
{
	if (g_55bd04 && g_55bd04 < 0x11)
	{
		g_55bd08 = 2;
		g_55bd04 = 0x11;
	}
	async_yield_until_done(&g_55c14c, true);
}