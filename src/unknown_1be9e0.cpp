// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* slot type 0x36 */

short __stdcall function_1beb70(long actor_index, s_slot *slot, bool active);
void __stdcall function_1bead0(long actor_index, s_slot *slot);
void __stdcall function_1bee40(long actor_index, s_slot *slot, long a, long b);

// @retail 0x1bed40
void __stdcall function_1bed40(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_actor_view *actor = actor_get(actor_index);
	s_prop_view_fields *view = prop_node_view(prop_node_get(actor->prop_index));

	if (view->unknown8c <= 0 && !view->unknown88)
	{
		actor->unknown41c = 3;
		actor->unknown420 = 2;
		actor->unknown4a1 = true;
		return;
	}

	long target = NONE;
	short i;

	for (i = 0; i < 10; i++)
	{
		long other_index = list->entries[i].actor_index;

		if (other_index != NONE && other_index != actor_index)
			target = actor_get(other_index)->unknown018;
	}
	if (target != NONE)
	{
		actor->unknown430 = 2;
		actor->unknown434 = 6;
		actor->unknown438.object_index = target;
		actor->unknown444 = NONE;
	}
	actor->unknown4a1 = true;
}

s_slot_handler_2x g_47eda8 =
{
	{
		{
			0x36, 2, 0, -2, 0,
			function_1adcd0, function_1beb70, joint_initiate, joint_leave, NONE, {0},
			0, 0, 0, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_1bead0, 0, (t_slot_release)slot_release_true, slot_release_nothing, function_1bed40, function_1bee40,
	2, 3, 4.0f, 0
};
