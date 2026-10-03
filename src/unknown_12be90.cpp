// @flags /O2 /Gr
/* UNKNOWN_12BE90.CPP: the main loop's checks of whether something is busy
   (loading, saving, a menu or a movie up) and its reset of the game time
   when the game loses focus */

#include "cseries.h"
#include "network_session_manager.h"
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
