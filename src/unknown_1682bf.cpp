// @flags /O1 /Oi /arch:SSE /Ob1 /Gr
/* UNKNOWN_1682BF.CPP: sets the unit a local user's first person weapons view
   from (part of first_person_weapons.cpp in the original; it has its own
   /Ob1 file because retail calls it out of line from the player code). */

#include "cseries.h"

#define MAXIMUM_FIRST_PERSON_WEAPONS 2
#define FLAG(bit) (1 << (bit))
#define SET_FLAG(flags, bit, value) ((value) ? ((flags) |= FLAG(bit)) : ((flags) &= ~FLAG(bit)))

enum
{
	_first_person_user_active_bit = 0
};

/* the head of a local user's first person state (0x20cc bytes,
   first_person_weapons.cpp) */
struct s_first_person_user_head
{
	dword flags;
	long unit_index;
	long character_index;
	byte unknown00c[0x20cc - 0xc];
};

struct s_16658d_group;
extern s_16658d_group *g_4e9bc8;
#define first_person_users ((s_first_person_user_head *)g_4e9bc8)

void first_person_weapon_set_active(long user_index, long weapon_slot, bool active);
void __stdcall function_167e86(long user_index, long weapon_slot);

// @retail 0x1682bf
void function_1682bf(long unit_index, long user_index, long character_index)
{
	s_first_person_user_head *user = &first_person_users[user_index];

	if (user->unit_index != unit_index)
	{
		long weapon_slot;

		for (weapon_slot = 0; weapon_slot < MAXIMUM_FIRST_PERSON_WEAPONS; weapon_slot++)
		{
			first_person_weapon_set_active(user_index, weapon_slot, false);
		}
		SET_FLAG(user->flags, _first_person_user_active_bit, false);
		user->unit_index = unit_index;
		user->character_index = character_index;
		for (weapon_slot = 0; weapon_slot < MAXIMUM_FIRST_PERSON_WEAPONS; weapon_slot++)
		{
			function_167e86(user_index, weapon_slot);
		}
	}
}
