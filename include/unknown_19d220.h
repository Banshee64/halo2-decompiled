#pragma once
/* the game variant as the variant menus and the session pass it, and its
   checks (unknown_19d220.cpp, lane H) */

#include "cseries.h"
#include <stddef.h>

/* a game variant (0x130 bytes; the UI lane's s_game_variant) */
struct s_menu_game_variant
{
	long unknown00;
	wchar_t name[0x20];
	long unknown44;
	byte unknown48[0x130 - 0x48];
};

void __stdcall function_19d220(s_menu_game_variant *variant, long type);
bool function_19d620(s_menu_game_variant *variant);
