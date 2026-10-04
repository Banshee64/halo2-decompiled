// @flags /O2 /arch:SSE /Gr
/* UNIT_ANIMATION.CPP: queries of a unit's animation state (the state at the
   unit's offset +0x12a, unknown_1cafc0.cpp's s_animation_state). The rest of
   this file's functions are in unknown_10db60.cpp and unknown_10dc70.cpp. */

#include "cseries.h"
#include "globals.h"
#include "animation_graph.h"
#include "unknown_1cafc0.h"

/* the unit (a view of the object data) */
struct s_unit_animation_view
{
	long definition_index;
	byte unknown004[0x12a - 4];
	short animation_state_offset;
};

struct s_unit_animation_header
{
	byte unknown00[8];
	s_unit_animation_view *unit;
};

#define GRAPH_GET(index) ((s_graph_tag *)g_4e3b44[(index) & 0xffff].bytes)

// @retail 0x10f340
bool function_10f340(long unit_index, long mode, long set)
{
	s_unit_animation_view *unit = ((s_unit_animation_header *)g_4e0300->data)[unit_index & 0xffff].unit;
	s_animation_state *state = (s_animation_state *)((byte *)unit + unit->animation_state_offset);
	c_animation_id animation_id;
	s_animation_names found;
	s_animation_names names;

	state->animation_lookup(&found, &names, mode, 0x7000101, 0x7000101, set, 0, &animation_id);
	return animation_id.index != NONE;
}

// @retail 0x10f3b0
bool function_10f3b0(long unit_index, long weapon_class, long mode)
{
	s_unit_animation_view *unit = ((s_unit_animation_header *)g_4e0300->data)[unit_index & 0xffff].unit;
	s_animation_state *state = (s_animation_state *)((byte *)unit + unit->animation_state_offset);

	if (weapon_class == 0x7000101)
	{
		weapon_class = state->unknown70;
		if (weapon_class == NONE)
			weapon_class = 0x6000086;
	}
	else if (weapon_class == 0x7000001)
	{
		weapon_class = 0x6000086;
	}
	c_animation_id animation_id = GRAPH_GET(state->graph_tag_index)->overlay_get(mode, weapon_class, state->unknown74,
		state->unknown78, NULL, NULL, NULL);
	return animation_id.index != NONE;
}

// @retail 0x10fcd0
bool function_10fcd0(long unit_index, long mode, long weapon_class, long weapon_type)
{
	s_unit_animation_view *unit = ((s_unit_animation_header *)g_4e0300->data)[unit_index & 0xffff].unit;
	s_animation_state *state = (s_animation_state *)((byte *)unit + unit->animation_state_offset);
	c_animation_id animation_id;
	s_animation_names found;
	s_animation_names names;

	state->animation_lookup(&found, &names, mode, weapon_class, weapon_type, 0x400000c, 2, &animation_id);
	return animation_id.index != NONE;
}
