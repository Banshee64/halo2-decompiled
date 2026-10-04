// @flags /O2 /Gr
/* UNKNOWN_19D220.CPP: game variant defaults and checks (lane H) */

#include "cseries.h"
#include "unknown_19d220.h"

bool function_19d650(s_menu_game_variant *variant);

/* whether the variant is valid as it is: checks a copy */
// @retail 0x19d620
bool function_19d620(s_menu_game_variant *variant)
{
	s_menu_game_variant copy = *variant;

	return function_19d650(&copy);
}
