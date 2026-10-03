// @flags /O1 /Gr
/* HUD.CPP: the hud's game state and its per-map setup (lane H) */

#include "cseries.h"

void *game_state_malloc(char const *name, char const *type, long size);
void game_state_initialize_24c819(void);
void hud_nav_points_initialize();
void motion_sensor_initialize(void);
void function_139130(void);

/* the hud's three flags in the game state (unknown_24c7c1.cpp) */
struct s_view_globals;
extern s_view_globals *g_510c98;

bool g_4ee4c0;
bool g_4ee4c1;

// @retail 0x19170f
void hud_initialize(void)
{
	g_510c98 = (s_view_globals *)game_state_malloc("hud", NULL, 3);
	g_4ee4c1 = false;
	g_4ee4c0 = false;
	game_state_initialize_24c819();
	hud_nav_points_initialize();
	motion_sensor_initialize();
	function_139130();
}
