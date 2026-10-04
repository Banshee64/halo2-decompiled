// @flags /O2 /arch:SSE /Gr
/* HAVOK_COLLISION_DAMAGE.CPP: the damage of havok collisions (0x1d96d0..) */

#include "cseries.h"

/* an entry of the 16 collisions function_1d96d0 gathers on its stack (0x94
   bytes); it constructs them with the vector constructor iterator */
struct s_collision_damage_entry
{
	byte unknown00[0x7c];
	short unknown7c;
	byte unknown7e[0x94 - 0x7e];

	s_collision_damage_entry();
};

// @retail 0x1da540
s_collision_damage_entry::s_collision_damage_entry()
{
	unknown7c = NONE;
}
