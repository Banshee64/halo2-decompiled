// @flags /O2 /Gr
/* UNKNOWN_0E6FE0.CPP: whether a unit is performing an action (unit action
   system; an outside function the unit board-vehicle event 0x9efa0 needs) */

#include "unknown_11c920.h"
#include "globals.h"

/* a unit's actions: a block reference at +0x344 (its size, then its offset
   from the object) to a bit per action type after a header dword */
struct s_unit_actions
{
	long unknown00;
	dword active[2];
};

/* an object header (12 bytes) */
struct s_unit_action_object_header
{
	byte unknown00[8];
	void *object;
};

struct s_unit_actions_view
{
	byte unknown000[0x344];
	short actions_size;
	short actions_offset;
};

// @retail 0xe6fe0
bool unit_action_active(long unit_index, long action_type)
{
	s_unit_actions_view *unit = (s_unit_actions_view *)((s_unit_action_object_header *)g_4e0300->data)[unit_index & 0xffff].object;
	s_unit_actions *actions = (s_unit_actions *)((byte *)unit + unit->actions_offset);

	return (actions->active[action_type >> 5] & (1 << (action_type & 31))) != 0;
}
