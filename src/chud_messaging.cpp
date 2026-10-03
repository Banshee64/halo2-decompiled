#include "cseries.h"

// @flags /O1 /arch:SSE /Gr

/* CHUD_MESSAGING.CPP: the messages the HUD shows about the player's weapons */

struct weapon_interface_state
{
	byte unknown00[4];
	real age;
	byte unknown08[0x24 - 0x08];
	short total_rounds;
	byte unknown26[2];
	short loaded_rounds;
	short magazines;
	short charge;
};

// @retail 0x1916dc
bool WeaponStateIsDepleted(weapon_interface_state const *state)
{
	bool depleted = false;

	if (state->total_rounds > 0 && state->magazines != 0 && state->loaded_rounds == 0 && state->charge == 0)
		depleted = true;

	if (state->age == 1.0f)
		depleted = true;

	return depleted;
}
