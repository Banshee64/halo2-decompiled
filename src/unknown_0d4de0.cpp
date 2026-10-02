// @flags /O2 /Gr
/* UNKNOWN_0D4DE0.CPP: the object placement lifecycle callbacks (entry 57) */

#include "cseries.h"
#include "game_state.h"

struct s_object_placement_globals
{
	short unknown0;
	short unknown2;
};

s_object_placement_globals *g_4e0324;

// @retail 0xd4de0
void object_placement_initialize(void)
{
	g_4e0324 = (s_object_placement_globals *)game_state_malloc("object placement", "object placement", sizeof(s_object_placement_globals));
}

// @retail 0xd4e20
void object_placement_initialize_for_new_map(void)
{
	g_4e0324->unknown0 = NONE;
}
