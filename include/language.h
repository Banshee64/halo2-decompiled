#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "cseries.h"
#include <xtl.h>

/* the language the game's text is in, NONE until first asked (g_47ff38,
   globals.cpp); unknown_11c9c0.cpp converts XGetLanguage's value */
extern long g_47ff38;

long function_11ca80(long value);

inline long get_current_language(void)
{
	long language = g_47ff38;
	if (language == NONE)
	{
		language = function_11ca80(XGetLanguage());
		g_47ff38 = language;
	}
	return language;
}

#endif
