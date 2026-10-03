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

/* the joint's flag at +0x88 (s_502424_element keeps a long there) */
struct s_joint_7f_view
{
	byte unknown00[0x88];
	bool started;
};

/* the slot the evaluation hands over to (type 0x3a) */
struct s_slot_3a_request
{
	s_slot_header header;
	short timer;
	byte unknown0e[0x1c - 0xe];
	long target_index;
	bool unknown20;
};

/* the unit request of type 0x19 (s_unit_request of slot_handler.h) */
struct s_unit_request_19
{
	long type;
	byte unknown4;
	byte unknown5[3];
	long unknown8;
	bool unknownc;
	byte unknownd[3];
	real_point2d direction;
	byte unknown18[8];
};

short g_470c58 = -1;
short g_470c5c = -2;

real function_30bf0(real_vector3d *v);
real normalize2d(real_point2d *v);
bool actor_has_joint_invitation(long actor_index, short type);
void __stdcall function_1f4280(long actor_index);

short __stdcall function_257630(long actor_index);
short __stdcall function_2576c0(long actor_index, s_slot *slot, s_slot *next);
void __stdcall function_257cb0(long actor_index, s_slot *slot, long index);
long __stdcall function_257870(long actor_index, s_slot *slot);
void __stdcall function_257a40(long actor_index, s_slot *slot, long index);
bool __stdcall function_257a90(long actor_index, s_slot *slot, s_502424_element *joint);
void __stdcall function_257b60(long actor_index, s_slot *slot, s_slot_target_list *list);
short __stdcall function_257c60(long actor_index, long joint_index, s_slot *slot, long unknown);

s_slot_handler_2x g_47faa8 =
{
	{
		{
			0x7f, 2, 0, -2, 0,
			function_257630, (t_slot_evaluate)function_2576c0, joint_initiate, joint_leave, NONE, {0},
			function_257cb0, 0, 0, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	(t_slot_proc)function_257870, function_257a40, (t_slot_release)function_257a90, 0, function_257b60, (t_slot_proc4)function_257c60,
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

// @retail 0x2576c0
short __stdcall function_2576c0(long actor_index, s_slot *slot, s_slot *next)
{
	short result = g_470c5c;
	s_actor_view *actor = actor_get(actor_index);
	s_slot_7f_state *state = (s_slot_7f_state *)slot;
	s_502424_element *joint = element_502424_get(state->joint_index);

	if (joint->target.unknown0 == NONE || joint->target.unknown4 == NONE)
	{
		return g_470c58;
	}

	if (state->following)
	{
		if (((s_joint_7f_view *)joint)->started)
		{
			s_slot_3a_request *request = (s_slot_3a_request *)next;

			request->unknown20 = true;
			request->target_index = joint->target.unknown4;
			request->timer = g_510c54->ticks_per_second * 3;
			return 0x3a;
		}
	}
	else
	{
		real_point3d *leader_position = &actor_get(joint->target.unknown0)->position;
		real_point2d direction;

		direction.x = leader_position->x - actor->position.x;
		direction.y = leader_position->y - actor->position.y;
		normalize2d(&direction);
		if (direction.y * actor->unknown290.j + direction.x * actor->unknown290.i > 0.8 && actor->unknown26c == NONE)
		{
			s_unit_request_19 request = {0};

			request.type = 0x19;
			request.unknown4 = 2;
			request.unknown8 = 0x700002c;
			request.unknownc = true;
			request.direction = direction;
			if (function_e6900(actor->unknown018, (s_unit_request *)&request))
			{
				((s_joint_7f_view *)joint)->started = true;
				return g_470c58;
			}
		}
	}
	return result;
}

// @retail 0x257870
long __stdcall function_257870(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	long result = NONE;

	if (actor->unknown004 == 2)
	{
		if (actor->unknown07c != NONE)
		{
			s_502420_element *clump = element_502420_get(actor->unknown07c);
			real best_distance = 3.4028235e38f;
			long best_index = NONE;
			long index = clump->first_actor_index;

			while (index != NONE)
			{
				s_actor_view *other = actor_get(index);
				long other_index = index;

				index = other->next_index;
				if (other != actor && other->unknown004 == 0)
				{
					real_vector3d delta;
					real distance;

					vector3d_from_points3d(&actor->position, &other->position, &delta);
					distance = (real)sqrt(magnitude_squared3d(&delta));
					if (distance < 1.f && distance < best_distance)
					{
						best_distance = distance;
						best_index = other_index;
					}
				}
			}

			if (best_index != NONE)
			{
				long joint_index = joint_new(actor_index);

				if (joint_index != NONE)
				{
					s_slot_7f_state *state = (s_slot_7f_state *)slot;
					s_502424_element *joint;

					state->following = true;
					state->joint_index = joint_index;
					joint = element_502424_get(joint_index);
					joint->target.unknown4 = best_index;
					joint->target.unknown0 = actor_index;
					((s_clump_7f_view *)clump)->time = g_510c54->game_time;
				}
				return joint_index;
			}
			return NONE;
		}
	}
	else if (actor->unknown004 == 0)
	{
		s_slot_entry_iterator iterator;

		iterator.actor_index = actor_index;
		iterator.reference.unknown2 = 0x7f;
		iterator.reference.unknown0 = NONE;
		for (s_slot_memory_entry *entry = function_26f0c0(&iterator); entry; entry = function_26f0c0(&iterator))
		{
			if (joint_accept(actor_index, iterator.reference.unknown0, (s_joint_behavior_state *)slot))
			{
				return entry->unknown4;
			}
		}
		return NONE;
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

// @retail 0x257a90
bool __stdcall function_257a90(long actor_index, s_slot *slot, s_502424_element *joint)
{
	bool result = true;

	if (joint->target.unknown0 == NONE || joint->target.unknown4 == NONE)
	{
		return false;
	}

	if (((s_slot_7f_state *)slot)->following)
	{
		if (actor_get(actor_index)->unknown040)
		{
			s_actor_view *leader = (s_actor_view *)datum_get(g_4f55f0, joint->target.unknown4);

			if (!leader)
			{
				return false;
			}
			function_26c180(joint->target.unknown4);
			return function_1f4460(actor_index, &leader->unknown27c.point, leader->unknown27c.unknown10, NONE, false);
		}
	}
	else
	{
		s_actor_view *leader = (s_actor_view *)datum_get(g_4f55f0, joint->target.unknown0);

		function_1f4280(actor_index);
		if (!leader)
		{
			return false;
		}
	}
	return result;
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
