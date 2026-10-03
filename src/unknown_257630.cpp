// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_257630.CPP: joint slot handler 0x7f (handler at 0x47faa8) */

#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"
#include "unknown_2551c0.h"

/* the slot state of handler 0x7f */
struct s_slot_7f_state
{
	s_slot_header header;
	bool following;
	byte unknown0d[3];
	long joint_index;
	byte unknown14[0x40 - 0x14];
};

/* the actor's point at +0x22c */
struct s_actor_7f_view
{
	byte unknown000[0x22c];
	real_point3d unknown22c;
};

/* the clumps of g_502420 (0x50 bytes), with the time at +0x4c */
struct s_clump_7f_view
{
	byte unknown00[0x4c];
	long time;
};

real function_30bf0(real_vector3d *v);
bool actor_has_joint_invitation(long actor_index, short type);

short __stdcall function_257630(long actor_index);
short __stdcall function_2576c0(long actor_index, s_slot *slot, bool active);
void __stdcall function_257cb0(long actor_index, s_slot *slot, long index);
void __stdcall function_257870(long actor_index, s_slot *slot);
void __stdcall function_257a40(long actor_index, s_slot *slot, long index);
void __stdcall function_257a90(long actor_index, s_slot *slot, long index);
void __stdcall function_257b60(long actor_index, s_slot *slot, s_slot_target_list *list);
short __stdcall function_257c60(long actor_index, long joint_index, s_slot *slot, long unknown);

s_slot_handler_2x g_47faa8 =
{
	{
		{
			0x7f, 2, 0, -2, 0,
			function_257630, function_2576c0, joint_initiate, joint_leave, NONE, {0},
			function_257cb0, 0, 0, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_257870, function_257a40, function_257a90, 0, function_257b60, (t_slot_proc4)function_257c60,
	2, 2, 1.0f, 0
};

// @retail 0x257630
short __stdcall function_257630(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->unknown07c != NONE)
	{
		s_clump_7f_view *clump = (s_clump_7f_view *)element_502420_get(actor->unknown07c);

		if (actor->unknown004 == 2)
		{
			if (clump->time == NONE || (g_510c54->game_time - clump->time) * g_510c54->rate > 30.f)
			{
				result = 3;
			}
		}
		else if (actor_has_joint_invitation(actor_index, 0x7f))
		{
			result = 3;
		}
	}
	return result;
}

// @retail 0x257a40
void __stdcall function_257a40(long actor_index, s_slot *slot, long index)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown07c != NONE)
	{
		((s_clump_7f_view *)element_502420_get(actor->unknown07c))->time = g_510c54->game_time;
	}
}

// @retail 0x257b60
void __stdcall function_257b60(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_slot_7f_state *state = (s_slot_7f_state *)slot;
	s_actor_view *actor = actor_get(actor_index);
	s_502424_element *joint = (s_502424_element *)list;
	long target_index = state->following ? joint->target.unknown4 : joint->target.unknown0;
	s_actor_7f_view *target = (s_actor_7f_view *)datum_get_inlined(g_4f55f0, target_index);

	if (target)
	{
		real_vector3d direction;

		direction.i = target->unknown22c.x - ((s_actor_7f_view *)actor)->unknown22c.x;
		direction.j = target->unknown22c.y - ((s_actor_7f_view *)actor)->unknown22c.y;
		direction.k = target->unknown22c.z - ((s_actor_7f_view *)actor)->unknown22c.z;
		function_30bf0(&direction);
		actor->unknown41c = 3;
		actor->unknown420 = 4;
		actor->unknown424.vector = direction;
	}
}

// @retail 0x257c60
short __stdcall function_257c60(long actor_index, long joint_index, s_slot *slot, long unknown)
{
	short result = 0;

	if (invite_actor(joint_index, element_502424_get(joint_index)->target.unknown4, 3, 1.0f))
	{
		result = 1;
	}
	return result;
}

// @retail 0x257cb0
void __stdcall function_257cb0(long actor_index, s_slot *slot, long index)
{
	s_slot_7f_state *state = (s_slot_7f_state *)slot;
	s_502424_element *joint = element_502424_get(state->joint_index);

	if (joint->target.unknown0 == index || joint->target.unknown4 == index)
	{
		joint->target.unknown0 = NONE;
		joint->target.unknown4 = NONE;
	}
}
