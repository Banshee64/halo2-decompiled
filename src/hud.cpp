// @flags /O1 /Oi /Gr
/* HUD.CPP: the hud's game state and its per-map setup (lane H) */

#include "unknown_11c920.h"
#include "globals.h"
#include <string.h>

void *function_123d40(char const *name, char const *type, long size);
void game_state_initialize_24c819(void);
void hud_nav_points_initialize();
void motion_sensor_initialize(void);
void function_139130(void);

/* the hud's three flags in the game state (unknown_24c7c1.cpp) */
struct s_view_globals;
extern s_view_globals *g_510c98;

bool g_4ee4c0;
bool g_4ee4c1;

/* the three flags as bytes */
struct s_hud_flags
{
	bool value0;
	bool value1;
	bool value2;
};

/* the hud's state (unknown_24c7c1.cpp), nav points (unknown_24d6c7.cpp) and
   motion sensor (unknown_1a1c3e.cpp) */
struct s_hud_state;
struct s_nav_points;
struct s_motion_sensor_globals;
extern s_hud_state *g_5023f4;
extern s_nav_points *g_5023f8;
extern s_motion_sensor_globals *g_51e994;

long function_13e615(short index);
void new_hud_initialize_for_new_map(void);

// @retail 0x19170f
void function_19170f(void)
{
	g_510c98 = (s_view_globals *)function_123d40("hud", NULL, 3);
	g_4ee4c1 = false;
	g_4ee4c0 = false;
	game_state_initialize_24c819();
	hud_nav_points_initialize();
	motion_sensor_initialize();
	function_139130();
}

/* resets the hud's state for a new map and finds the hud globals tag */
// @retail 0x19173e
void function_19173e(void)
{
	s_hud_flags *flags = (s_hud_flags *)g_510c98;

	memset(flags, 0, sizeof(s_hud_flags));
	flags->value0 = !g_4ee4c0;
	flags->value1 = false;
	flags->value2 = true;
	g_510c94 = (s_hud_globals_definition *)g_4e3b44[function_13e615(6) & 0xffff].bytes;
	memset(g_5023f4, 0, 0x13a8);
	memset(g_5023f8, NONE, 0xc0);
	memset(g_51e994, 0, 0xbc8);
	new_hud_initialize_for_new_map();
}

void function_1a22b4(void);
void function_1391ed(void);
void __stdcall function_24d8d3(long player_index);
void function_24cdd8(long player_index);

/* the hud state's byte at +0x1385 */
struct s_hud_state_view
{
	byte unknown0000[0x1385];
	bool field_1385;
};

/* resets the hud for each local player */
// @retail 0x1917b2
void function_1917b2(void)
{
	long player_index;

	function_1a22b4();
	function_1391ed();
	((s_hud_state_view *)g_5023f4)->field_1385 = false;
	for (player_index = 0; player_index < 4; player_index++)
	{
		function_24d8d3(player_index);
		if (((s_hud_flags *)g_510c98)->value2)
		{
			function_24cdd8(player_index);
		}
	}
}
