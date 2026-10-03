// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"

/* slot type 0x25 */

struct s_slot_25
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x40 - 0x14];
};

short __stdcall function_1b6450(long actor_index, s_slot *slot, bool active);
void __stdcall function_1b6120(long actor_index, s_slot *slot);
short __stdcall function_1b6740(long actor_index, long leader_index, long a, long b);
short function_1a6fe0(long owner_index, short type);

/* the state of slot type 0x22 another actor has */
struct s_slot_22
{
	s_slot_header header;
	byte unknown0c[0x18 - 0xc];
	long prop_index;
	byte unknown1c[0x40 - 0x1c];
};

/* invites the clump members whose slot 0x22 is at the actor's prop */
// @retail 0x1b6740
short __stdcall function_1b6740(long actor_index, long leader_index, long a, long b)
{
	s_actor_view *actor = actor_get(actor_index);
	long count = 0;

	if (actor->unknown07c != NONE && actor->prop_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		long index = element_502420_get(actor->unknown07c)->first_actor_index;

		while (index != NONE)
		{
			s_actor_view *other = actor_get(index);
			long other_index = index;

			index = other->next_index;
			if (other != actor && other->unknown221)
			{
				short slot_index = function_1a6fe0(other_index, 0x22);

				if (slot_index != NONE)
				{
					s_slot_22 *state = (s_slot_22 *)&other->slots[slot_index];

					if (state->prop_index != NONE)
					{
						s_prop_node_view *other_node = (s_prop_node_view *)datum_get(g_502418, state->prop_index);

						if (other_node && node->object_index == other_node->object_index &&
							invite_actor(leader_index, other_index, 3, 1.0f))
						{
							count++;
						}
					}
				}
			}
		}
	}
	return (short)count;
}
// @retail 0x1b6880
void __stdcall function_1b6880(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	actor_get(actor_index)->unknown4a1 = true;
}

// @retail 0x1b68b0
void __stdcall function_1b68b0(long actor_index, s_slot *slot, long index)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown4 == index)
		element->target.unknown4 = NONE;
}

// @retail 0x1b68f0
void __stdcall function_1b68f0(long actor_index, s_slot *slot)
{
	s_slot_25 *state = (s_slot_25 *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (element->target.unknown14 == 1)
		element->target.unknown4 = NONE;
}

s_slot_handler_2x g_47e648 =
{
	{
		{
			0x25, 2, 0, -2, 0,
			function_1adcd0, function_1b6450, joint_initiate, joint_leave, NONE, {0},
			0, 0, function_1b68b0, function_1b68f0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_1b6120, 0, 0, 0, function_1b6880, (t_slot_proc4)function_1b6740,
	2, 10, 150.0f, 0
};
