// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"
#include <math.h>

/* slot type 0x5d */

struct s_slot_5d
{
	s_slot_header header;
	bool active;
	byte unknown0d[3];
	long element_index;
	byte unknown14[0x1c - 0x14];
	short flags;
	byte unknown1e[2];
	long unknown20;
	short unknown24;
	byte unknown26[0x38 - 0x26];
	long unknown38;
	byte unknown3c[0x40 - 0x3c];
};

short __stdcall function_1bd850(long actor_index);
short __stdcall function_1bda60(long actor_index, s_slot *slot, bool active);
long __stdcall function_1bd890(long actor_index, s_slot *slot);
void __stdcall function_1bdad0(long actor_index, s_slot *slot, long index);
short __stdcall function_1bde80(long actor_index, long joint_index, long a, long b);
point3f *function_b9dd0(long object_index, point3f *result);

/* an element of g_502424 as slot type 0x5d sees it */
struct s_5d_element
{
	short unknown00;
	short unknown02;
	byte unknown04[0x7c - 0x4];
	short unknown7c;
	byte unknown7e[2];
	long object_index;
	short unknown84;
	short unknown86;
	byte unknown88[0x94 - 0x88];
	short unknown94;
};

/* the tag of the object a slot 0x5d joint is about */
struct s_5d_object_tag
{
	byte unknown000[0x244];
	short unknown244;
};

bool function_f5dc0(long object_index);

/* the object's velocity is under 0.2 */
// @retail 0x1bd3b0
bool function_1bd3b0(long object_index)
{
	bool result = false;

	if (magnitude3d(&object_get(object_index)->velocity) < 0.2f)
		result = true;
	return result;
}

// @retail 0x1bd850
short __stdcall function_1bd850(long actor_index)
{
	s_slot_entry_iterator iterator;
	short result = 0;

	iterator.actor_index = actor_index;
	iterator.reference.unknown2 = 0x5d;
	iterator.reference.unknown0 = NONE;
	if (function_26f0c0(&iterator))
		result = 3;
	return result;
}

// @retail 0x1bd890
long __stdcall function_1bd890(long actor_index, s_slot *slot)
{
	s_slot_5d *state = (s_slot_5d *)slot;
	s_actor_view *actor = actor_get(actor_index);
	long result = NONE;
	long object_index = NONE;

	if (state->flags & 1)
		object_index = state->unknown20;
	if (actor->prop_index != NONE && prop_node_view(prop_node_get(actor->prop_index)))
	{
		s_slot_entry_iterator iterator;
		s_slot_memory_entry *entry;

		iterator.actor_index = actor_index;
		iterator.reference.unknown2 = 0x5d;
		iterator.reference.unknown0 = NONE;
		for (entry = function_26f0c0(&iterator); entry; entry = function_26f0c0(&iterator))
		{
			if ((object_index == NONE || ((s_5d_element *)element_502424_get(entry->unknown4))->object_index == object_index) &&
				function_26ecc0(actor_index, iterator.reference.unknown0, (s_joint_behavior_state *)slot))
			{
				result = entry->unknown4;
				break;
			}
		}
		if (result == NONE)
		{
			if (object_index == NONE)
				return result;
			result = function_26e940(actor_index);
			if (result != NONE)
			{
				state->active = true;
				state->element_index = result;
			}
		}
		if (result != NONE)
		{
			s_5d_element *element = (s_5d_element *)element_502424_get(result);

			if (state->active)
			{
				element->object_index = object_index;
				element->unknown84 = 0;
				element->unknown94 = 0;
				element->unknown86 = (((s_5d_object_tag *)g_4e3b44[object_get(object_index)->tag_index & 0xffff].bytes)->unknown244 > 0) ? 3 : 1;
			}
			state->flags = (state->flags & ~0x1e) | 1;
			state->unknown20 = element->object_index;
			state->unknown24 = NONE;
		}
	}
	return result;
}
// @retail 0x1bda60
short __stdcall function_1bda60(long actor_index, s_slot *slot, bool active)
{
	s_slot_5d *state = (s_slot_5d *)slot;
	s_5d_element *element = (s_5d_element *)element_502424_get(state->element_index);
	short result = g_46fbe8;

	if (element->object_index != NONE && state->unknown20 != NONE && element->unknown7c >= element->unknown86 &&
		function_f5dc0(element->object_index))
	{
		return result;
	}
	element->unknown02 = 2;
	return g_46fbe4;
}

// @retail 0x1bdd70
void __stdcall function_1bdd70(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_5d *state = (s_slot_5d *)slot;
	s_502424_target *target = &element_502424_get(state->element_index)->target;

	if (list->count == 0)
	{
		actor->unknown41c = 2;
		actor->unknown420 = 2;
		if (actor->unknown086 >= 7)
		{
			actor->unknown488 = true;
			actor->unknown41c = 3;
			actor->unknown420 = 2;
		}
	}
	else if ((state->flags & 2) && (state->flags & 0x10))
	{
		actor->unknown41c = 3;
		actor->unknown420 = 4;
		actor->unknown424.point = target->point;
		actor->unknown44d = true;
		actor->unknown4cc = 0.2f;
	}
	else
	{
		actor->unknown430 = 2;
		actor->unknown434 = 6;
		actor->unknown438.object_index = state->unknown20;
		actor->unknown444 = NONE;
	}
}

/* the joint behavior (g_502424) as function_1bde80 reads it */
struct s_1bde80_joint
{
	byte unknown00[0x80];
	long object_index;
	short unknown84;
	short participant_count;
};

/* invites the clump members within 10 of the joint's object, the nearest
   first */
// @retail 0x1bde80
short __stdcall function_1bde80(long actor_index, long joint_index, long a, long b)
{
	s_actor_view *actor = actor_get(actor_index);
	s_1bde80_joint *joint = (s_1bde80_joint *)element_502424_get(joint_index);
	point3f position;
	long count = 0;

	function_b9dd0(joint->object_index, &position);
	if (joint->participant_count > 1)
	{
		long index = element_502420_get(actor->unknown07c)->first_actor_index;

		while (index != NONE)
		{
			s_actor_view *other = actor_get(index);
			long other_index = index;

			index = other->next_index;
			if (other != actor)
			{
				real i = position.x - other->position.x;
				real j = position.y - other->position.y;
				real k = position.z - other->position.z;
				real distance = (real)sqrt(j * j + (i * i + k * k));

				if (10.0f > distance && function_26eae0(joint_index, other_index, 3, 1.0f / (distance + 0.1f)))
					count++;
			}
		}
	}
	return (short)count;
}
// @retail 0x1bdfd0
void __stdcall function_1bdfd0(long actor_index, s_slot *slot, long index)
{
	s_slot_5d *state = (s_slot_5d *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if ((state->flags & 1) && state->unknown20 == index || element->target.unknown0 == index)
	{
		state->unknown20 = NONE;
		state->flags &= ~0x11;
		element->target.unknown0 = NONE;
	}
}

// @retail 0x1be020
void __stdcall function_1be020(long actor_index, s_slot *slot)
{
	s_slot_5d *state = (s_slot_5d *)slot;

	state->flags &= ~0x10;
	state->unknown38 = 0;
}

s_slot_handler_2x g_47ec48 =
{
	{
		{
			0x5d, 2, 0, -2, 0,
			function_1bd850, function_1bda60, joint_initiate, joint_leave, 1, {0},
			0, 0, function_1bdfd0, function_1be020, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	(t_slot_proc)function_1bd890, 0, function_1bdad0, slot_release_nothing, function_1bdd70, (t_slot_proc4)function_1bde80,
	1, 3, 0.2f, 0
};
