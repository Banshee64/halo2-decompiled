/* MAIN_GLOBALS.H: the main loop's requests (main.cpp: src/unknown_12be90.cpp
   defines them): loading a core save, restarting or reverting the level,
   saving, quitting and switching structure bsps. They are one structure in
   retail (0x547f28..0x547f7a): the core name's address goes to strncpy, so
   the compiler keeps stores to the flags ahead of loads through pointers. */
#ifndef MAIN_GLOBALS_H
#define MAIN_GLOBALS_H

#include "unknown_11c920.h"

struct s_main_globals
{
	byte unknown28;
	bool unknown29;
	byte unknown2a[3];
	byte field_5;
	char field_6_2[0x40];	/* the last byte stays the terminator */
	byte reset_map;
	byte unknown6f;
	byte save_map;
	byte quit_game;
	byte unknown72;
	byte switch_structure_bsp;
	byte unknown74;
	byte unknown75;
	byte unknown76;
	byte unknown77;
	short field_0_3;
};

extern s_main_globals main_globals;

#endif
