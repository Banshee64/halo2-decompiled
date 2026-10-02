// @flags /O2 /arch:SSE /Gr
/* JOINT_BEHAVIOR.CPP: AI joint behaviors

Actors join a joint behavior through invitations: the joint's leader invites
participants, each invited actor keeps the invitation in one of its four
invitation slots until it accepts, declines or the invitation expires.

The functions follow joint_behavior.obj in Bungie's May 2003 debug builds
(halo-symbol-atlas); names the debug map does not give are function_<va>. */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "slot_owner.h"

enum
{
	k_maximum_joint_participants = 10,
	k_maximum_joint_invitations = 4,
	k_lowest_participant_priority = 8,

	_participant_invited = 0,
	_participant_withdrawn,
	_participant_declined,
	_participant_accepted
};

struct joint_participant
{
	long actor_index;
	short status;
	short priority;
	real score;
};

struct joint_state
{
	short salt;
	short state;
	joint_participant participants[k_maximum_joint_participants];
	short participant_count;
	byte unknown7e[0xbc - 0x7e];
};

/* the joint behavior's part of an actor's behavior state */
struct s_joint_behavior_state
{
	short type;
	byte unknown02[2];
	long joint_index;
	long expiration_time;
	bool waiting;
	byte unknown0d[3];
	long state_joint_index;
	short state;
	short timer;
	short participant_index;
};

s_data_array *g_502424;

#define JOINT_STATE(index) ((joint_state *)(g_502424->data + ((index) & 0xffff) * sizeof(joint_state)))
#define ACTOR_ENTRY(index) ((s_slot_owner_entry *)(g_4f55f0->data + ((index) & 0xffff) * sizeof(s_slot_owner_entry)))

// @retail 0x26e940
long joint_new(long actor_index)
{
	long joint_index = NONE;

	if (ACTOR_ENTRY(actor_index)->joint_index != NONE)
	{
		joint_index = datum_new(g_502424);
		if (joint_index != NONE)
		{
			joint_state *joint = JOINT_STATE(joint_index);

			for (short i = 1; i < k_maximum_joint_participants; i++)
			{
				joint->participants[i].actor_index = NONE;
				joint->participants[i].status = _participant_invited;
			}
			joint->participants[0].actor_index = actor_index;
			joint->participants[0].status = _participant_accepted;
			joint->participant_count = 1;
		}
	}
	return joint_index;
}

// @retail 0x26e9c0
short find_invitation_index(long actor_index, long joint_index)
{
	s_slot_owner_entry *actor = ACTOR_ENTRY(actor_index);

	for (short i = 0; i < k_maximum_joint_invitations; i++)
	{
		if (actor->joint_invitations[i].type != NONE && actor->joint_invitations[i].joint_index == joint_index)
			return i;
	}
	return NONE;
}

// @retail 0x26ed40
bool joint_decline(long actor_index, short invitation_index)
{
	s_joint_invitation *invitation = &ACTOR_ENTRY(actor_index)->joint_invitations[invitation_index];
	joint_state *joint = JOINT_STATE(invitation->joint_index);

	joint->participants[invitation->participant_index].actor_index = NONE;
	joint->participants[invitation->participant_index].status = _participant_withdrawn;
	invitation->type = NONE;
	return true;
}

// @retail 0x26ea10
short find_empty_invitation_index(long actor_index, short priority)
{
	s_slot_owner_entry *actor = ACTOR_ENTRY(actor_index);
	short result = NONE;
	short lowest_priority = k_lowest_participant_priority;
	short lowest_index = NONE;

	for (short i = 0; i < k_maximum_joint_invitations; i++)
	{
		s_joint_invitation *invitation = &actor->joint_invitations[i];

		if (invitation->type == NONE)
		{
			result = i;
			break;
		}
		if (invitation->expiration_time < g_510c54->game_time)
		{
			joint_decline(actor_index, i);
			result = i;
			break;
		}

		short participant_priority = JOINT_STATE(invitation->joint_index)->participants[invitation->participant_index].priority;
		if (participant_priority < lowest_priority)
		{
			lowest_priority = participant_priority;
			lowest_index = i;
		}
	}

	if (result == NONE && lowest_priority < priority && lowest_index != NONE)
	{
		joint_decline(actor_index, lowest_index);
		result = lowest_index;
	}
	return result;
}

// @retail 0x26eae0
bool invite_actor(long joint_index, long actor_index, short priority, real score)
{
	bool result = false;

	if (find_empty_invitation_index(actor_index, priority) != NONE)
	{
		joint_state *joint = JOINT_STATE(joint_index);
		short i = 0;

		while (joint->participants[i].actor_index != NONE && i < k_maximum_joint_participants)
			i++;
		if (i < k_maximum_joint_participants)
		{
			joint->participants[i].actor_index = actor_index;
			joint->participants[i].score = score;
			joint->participants[i].priority = priority;
			result = true;
		}
	}
	return result;
}

// @retail 0x26eb70
void joint_submit_invitation(long actor_index, s_joint_behavior_state const *behavior)
{
	s_slot_owner_entry *actor = ACTOR_ENTRY(actor_index);
	joint_state *joint = JOINT_STATE(behavior->joint_index);

	for (short i = 0; i < k_maximum_joint_participants; i++)
	{
		if (joint->participants[i].actor_index == actor_index)
		{
			short invitation_index = find_empty_invitation_index(actor_index, joint->participants[i].priority);

			if (invitation_index != NONE)
			{
				actor->joint_invitations[invitation_index].joint_index = behavior->joint_index;
				actor->joint_invitations[invitation_index].type = behavior->type;
				actor->joint_invitations[invitation_index].expiration_time = behavior->expiration_time;
				actor->joint_invitations[invitation_index].participant_index = i;
				return;
			}
		}
	}
}

// @retail 0x26ec20
void joint_submit_invitation_all(s_joint_behavior_state const *behavior, long leader_index)
{
	joint_state *joint = JOINT_STATE(behavior->joint_index);

	for (short i = 0; i < k_maximum_joint_participants; i++)
	{
		long actor_index = joint->participants[i].actor_index;

		if (actor_index != NONE && actor_index != leader_index)
		{
			s_slot_owner_entry *actor = ACTOR_ENTRY(actor_index);
			short invitation_index = find_empty_invitation_index(actor_index, joint->participants[i].priority);

			if (invitation_index != NONE)
			{
				actor->joint_invitations[invitation_index].joint_index = behavior->joint_index;
				actor->joint_invitations[invitation_index].type = behavior->type;
				actor->joint_invitations[invitation_index].expiration_time = behavior->expiration_time;
				actor->joint_invitations[invitation_index].participant_index = i;
			}
		}
	}
}

// @retail 0x26ecc0
bool joint_accept(long actor_index, short invitation_index, s_joint_behavior_state *behavior)
{
	s_joint_invitation *invitation = &ACTOR_ENTRY(actor_index)->joint_invitations[invitation_index];
	short participant_index = invitation->participant_index;
	joint_state *joint = JOINT_STATE(invitation->joint_index);

	joint->participants[participant_index].status = _participant_accepted;
	behavior->participant_index = participant_index;
	behavior->waiting = false;
	behavior->state_joint_index = invitation->joint_index;
	joint->participant_count++;
	invitation->type = NONE;
	return true;
}

// @retail 0x26edb0
void function_26edb0(long joint_index, short participant_index)
{
	joint_participant *participant = &JOINT_STATE(joint_index)->participants[participant_index];

	if (participant->status == _participant_invited)
	{
		s_slot_owner_entry *actor = ACTOR_ENTRY(participant->actor_index);

		for (short i = 0; i < k_maximum_joint_invitations; i++)
		{
			s_joint_invitation *invitation = &actor->joint_invitations[i];

			if (invitation->type != NONE && invitation->joint_index == joint_index)
			{
				joint_decline(participant->actor_index, i);
				break;
			}
		}
	}
	participant->status = _participant_declined;
}

// @retail 0x26ee40
void joint_withdraw(s_joint_behavior_state *behavior)
{
	joint_state *joint = JOINT_STATE(behavior->state_joint_index);

	joint->participants[behavior->participant_index].actor_index = NONE;
	joint->participants[behavior->participant_index].status = _participant_withdrawn;
	if (--joint->participant_count == 0)
	{
		for (short i = 0; i < k_maximum_joint_participants; i++)
		{
			joint_participant *participant = &joint->participants[i];

			if (participant->actor_index != NONE && participant->status == _participant_invited)
			{
				s_slot_owner_entry *actor = (s_slot_owner_entry *)datum_get_inlined(g_4f55f0, participant->actor_index);

				if (actor)
				{
					short invitation_index = find_invitation_index(participant->actor_index, behavior->state_joint_index);

					if (invitation_index != NONE)
					{
						actor->joint_invitations[invitation_index].type = NONE;
						actor->joint_invitations[invitation_index].joint_index = NONE;
						actor->joint_invitations[invitation_index].participant_index = NONE;
					}
				}
			}
		}
		datum_delete(g_502424, behavior->state_joint_index);
	}
}

// @retail 0x26ef40
long joint_count_invited_participants(joint_state const *joint)
{
	long count = 0;

	for (short i = 0; i < k_maximum_joint_participants; i++)
	{
		if (joint->participants[i].actor_index != NONE && joint->participants[i].status == _participant_invited)
			count++;
	}
	return count;
}
