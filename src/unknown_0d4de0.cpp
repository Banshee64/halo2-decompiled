// @flags /O2 /Gr
/* UNKNOWN_0D4DE0.CPP: the object placement lifecycle callbacks (entry 57) */

#include "unknown_11c920.h"
#include "unknown_123b30.h"

struct s_object_placement_globals
{
	short unknown0;
	short unknown2;
};

s_object_placement_globals *g_4e0324;

// @retail 0xd4de0
void function_d4de0(void)
{
	g_4e0324 = (s_object_placement_globals *)function_123d40("object placement", "object placement", sizeof(s_object_placement_globals));
}

// @retail 0xd4e20
void function_d4e20(void)
{
	g_4e0324->unknown0 = NONE;
}
