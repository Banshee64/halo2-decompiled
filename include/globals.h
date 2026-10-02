/* GLOBALS.H: globals shared by more than one source file (defined in
src/globals.cpp) */

#ifndef GLOBALS_H
#define GLOBALS_H

/* the game time globals; unknown02 is a scale read by firing position code */
struct s_game_time_globals
{
	byte unknown00[2];
	short unknown02;
	real rate;
	long game_time;
};

extern s_game_time_globals *g_510c54;

#endif
