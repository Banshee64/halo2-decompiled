// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* slot type 0x36 */

short __stdcall function_1beb70(long actor_index, s_slot *slot, bool active);
long __stdcall function_1bead0(long actor_index, s_slot *slot);
short __stdcall function_1bee40(long actor_index, long leader_index, long a, long b);
short function_1b6e50(long actor_index);

struct s_slot_36
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long element_index;
	byte unknown14[0x40 - 0x14];
};


// @retail 0x1be9e0
bool function_1be9e0(long actor_index, long other_index, real *distance, short *ticks)
{
	s_actor_view *actor = actor_get(actor_index);
	s_actor_view *other = actor_get(other_index);
	real_vector3d delta;
	bool result = false;
	short slot_ticks = 0x7fff;

	delta.i = actor->position.x - other->position.x;
	delta.j = actor->position.y - other->position.y;
	delta.k = actor->position.z - other->position.z;

	real magnitude = magnitude3d(&delta);

	if (magnitude < 4.0f)
	{
		slot_ticks = function_1b6e50(actor_index);
		if (slot_ticks * g_510c54->rate < 4.0f)
			result = true;
	}
	if (distance)
		*distance = magnitude;
	if (ticks)
		*ticks = slot_ticks;
	return result;
}

// @retail 0x1bead0
long __stdcall function_1bead0(long actor_index, s_slot *slot)
{
	s_slot_36 *state = (s_slot_36 *)slot;
	long result = NONE;
	s_slot_entry_iterator iterator;

	iterator.actor_index = actor_index;
	iterator.reference.unknown2 = 0x36;
	iterator.reference.unknown0 = NONE;
	for (s_slot_memory_entry *entry = function_26f0c0(&iterator); entry; entry = function_26f0c0(&iterator))
	{
		if (result == NONE)
		{
			if (joint_accept(actor_index, iterator.reference.unknown0, (s_joint_behavior_state *)slot))
				result = entry->unknown4;
		}
		else
		{
			joint_decline(actor_index, iterator.reference.unknown0);
		}
	}
	if (result != NONE)
		return result;

	long element_index = joint_new(actor_index);

	if (element_index != NONE)
	{
		state->unknown0c = true;
		state->element_index = element_index;
	}
	return element_index;
}

/* invites the actor's clump members near enough, the nearest first */
// @retail 0x1bee40
short __stdcall function_1bee40(long actor_index, long leader_index, long a, long b)
{
	s_actor_view *actor = actor_get(actor_index);
	long count = 0;

	if (actor->unknown07c == NONE)
		return 0;

	long index = element_502420_get(actor->unknown07c)->first_actor_index;

	while (index != NONE)
	{
		s_actor_view *other = actor_get(index);
		long other_index = index;
		real distance;
		short ticks;

		index = other->next_index;
		if (other != actor && function_1be9e0(actor_index, other_index, &distance, &ticks) &&
			invite_actor(leader_index, other_index, 3, 1.0f / (distance * 10.0f + (real)ticks)))
		{
			count++;
		}
	}
	return (short)count;
}
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
	(t_slot_proc)function_1bead0, 0, (t_slot_release)slot_release_true, slot_release_nothing, function_1bed40, (t_slot_proc4)function_1bee40,
	2, 3, 4.0f, 0
};
