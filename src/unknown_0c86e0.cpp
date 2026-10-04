// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0C86E0.CPP: a unit leaving its weapon's zoom. Lane S
   wrote it for the weapon functions 0x103b10..0x103ce0, which call it. */

#include "cseries.h"
#include "globals.h"

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
bool function_100f70(long weapon_index);
long function_1896c0(real scale, long tag_index);

/* the unit (a view of the object data) */
struct s_zoom_unit
{
	byte unknown000[0x13c];
	long player_index;
	byte unknown140[0x212 - 0x140];
	char current_weapon_slot;
	byte unknown213[0x218 - 0x213];
	long weapon_indices[4];
	byte unknown228[0x240 - 0x228];
	char field_240;
	char desired_zoom_level;
	byte unknown242[0x26c - 0x242];
	real zoom_transition;
};

struct s_zoom_object_header
{
	byte unknown00[8];
	s_zoom_unit *unit;
};

/* the weapon's definition: its zoom-out sound */
struct s_zoom_weapon_definition
{
	byte unknown000[0x278];
	long zoom_out_sound_index;
};

/* the player (0x21c bytes): its local user */
struct s_zoom_player
{
	byte unknown00[0x28];
	short user_index;
};

/* the globals' sound tags */
struct s_zoom_sounds
{
	byte unknown00[0xd0];
	long zoom_out_sound_index;
};

struct s_zoom_globals
{
	byte unknown000[0x134];
	s_zoom_sounds *sounds;
};

#define ZOOM_UNIT_GET(index) (((s_zoom_object_header *)g_4e0300->data)[(index) & 0xffff].unit)

static inline long unit_get_player_index(long unit_index)
{
	s_zoom_unit *unit = (s_zoom_unit *)function_badc0(unit_index, 3);
	long result = NONE;

	if (unit)
		result = unit->player_index;
	return result;
}

static inline long unit_get_current_weapon(s_zoom_unit *unit)
{
	short slot = unit->current_weapon_slot;
	long result = NONE;

	if (slot != NONE)
		result = unit->weapon_indices[slot];
	return result;
}

static __forceinline long weapon_get_zoom_out_sound(long weapon_index)
{
	long result = NONE;

	if (weapon_index != NONE)
	{
		s_zoom_unit *weapon = ZOOM_UNIT_GET(weapon_index);
		result = ((s_zoom_weapon_definition *)g_4e3b44[*(long *)weapon & 0xffff].bytes)->zoom_out_sound_index;
	}
	return result;
}

// @retail 0xc86e0
void function_c86e0(long unit_index, bool keep_weapon_zoom)
{
	s_zoom_unit *unit = ZOOM_UNIT_GET(unit_index);
	long user_index = unit_get_player_index(unit_index) != NONE ?
		((s_zoom_player *)(g_4e8c24->data + (unit_get_player_index(unit_index) & 0xffff) * 0x21c))->user_index : NONE;
	long weapon_index = unit_get_current_weapon(ZOOM_UNIT_GET(unit_index));
	bool weapon_zoomed = weapon_index != NONE ? function_100f70(weapon_index) : false;
	bool was_zoomed = false;

	if (!keep_weapon_zoom || !weapon_zoomed)
	{
		was_zoomed = unit->field_240 != NONE;
		unit->field_240 = NONE;
		unit->desired_zoom_level = NONE;
		unit->zoom_transition = 0.0f;
		if (user_index != NONE)
		{
			s_unknown_185ab0_entry *entry = &g_4ed284->entries[user_index];

			entry->index = NONE;
			entry->flag = false;
		}
	}
	if (user_index != NONE && was_zoomed)
	{
		long sound_index;

		if (weapon_zoomed && !keep_weapon_zoom)
			sound_index = weapon_get_zoom_out_sound(weapon_index);
		else
			sound_index = ((s_zoom_globals *)g_4e034c)->sounds->zoom_out_sound_index;
		if (sound_index != NONE)
			function_1896c0(1.0f, sound_index);
	}
}
