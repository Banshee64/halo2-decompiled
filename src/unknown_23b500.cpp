// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_23B500.CPP: the influences that weigh the game engine's spawn
   points (0x23b500..0x23bc40) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

/* the influences' weights (g_502258, cleared by unknown_157450.cpp): two
   shared values, then three per influence type */
struct s_spawn_influence_type
{
	real value0;
	real value4;
	real value8;
};

struct s_spawn_influence_globals
{
	real value0;
	real value4;
	s_spawn_influence_type types[12];
};

extern dword g_502258[0x27];

/* an influence on the spawn points (0x20 bytes) */
struct s_spawn_influence
{
	point3f point;
	real value0c;
	real value10;
	real value14;
	real value18;
	real value1c;
};

/* the influences gathered for one spawn (at most 0x80) */
struct s_spawn_influence_list
{
	long count;
	s_spawn_influence influences[0x80];
};

__forceinline void spawn_influence_add(s_spawn_influence_list *list, point3f const *point,
	real value0c, real value10, real value14, real value18, real value1c)
{
	if (list->count < 0x80)
	{
		s_spawn_influence *influence = &list->influences[list->count++];

		influence->point = *point;
		influence->value0c = value0c;
		influence->value10 = value10;
		influence->value14 = value14;
		influence->value18 = value18;
		influence->value1c = value1c;
	}
}

/* adds an influence of this type at the point */
// @retail 0x23ba10
void function_23ba10(long type, s_spawn_influence_list *list, point3f const *point)
{
	s_spawn_influence_globals *globals = (s_spawn_influence_globals *)g_502258;
	s_spawn_influence_type *definition = &globals->types[type];

	spawn_influence_add(list, point, definition->value0, definition->value4, globals->value0,
		globals->value4, definition->value8);
}
