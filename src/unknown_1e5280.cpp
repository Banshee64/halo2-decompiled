// @flags /O2 /Gr
/* UNKNOWN_1E5280.CPP: how an actor's character (or the characters it
   inherits from) uses a weapon (an outside function lane I's firing position
   evaluators call) */

#include "cseries.h"
#include "globals.h"
#include "unknown_1e1f20.h"

/* the actor's character tag (+0x54) */
struct s_actor_character_view
{
	byte unknown000[0x54];
	long character_tag_index;
	byte unknown058[0x888 - 0x58];
};

/* a character tag: the tag it inherits from, and its weapons */
struct s_character_tag_view
{
	byte unknown00[8];
	long parent_tag_index;
	byte unknown0c[0xcc - 0xc];
	long weapon_count;
	s_character_weapon *weapons;
};

// @retail 0x1e5280
s_character_weapon *function_1e5280(long actor_index, long weapon_tag_index)
{
	long tag_index = ((s_actor_character_view *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_actor_character_view)))->character_tag_index;

	while (tag_index != NONE)
	{
		s_character_tag_view *character = (s_character_tag_view *)g_4e3b44[tag_index & 0xffff].bytes;

		for (short i = 0; i < character->weapon_count; i++)
		{
			s_character_weapon *weapon = &character->weapons[i];

			if (weapon->weapon_tag_index == weapon_tag_index)
			{
				return weapon;
			}
		}
		tag_index = character->parent_tag_index;
	}
	return NULL;
}
