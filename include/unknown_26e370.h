/* UNKNOWN_26E370.H: the joint behavior callbacks (unknown_26e370.cpp) that
   the actor slot handlers of slot_handler.h call through their tables */

#ifndef JOINT_BEHAVIOR_H
#define JOINT_BEHAVIOR_H

#include "unknown_11c920.h"
#include "slot_owner.h"

/* kind 2x handlers: start, stop and the three updates */
bool __stdcall joint_initiate(long actor_index, s_slot *slot);
void __stdcall joint_leave(long actor_index, s_slot *slot);
bool __stdcall joint_update(long actor_index, s_slot *slot);
void __stdcall joint_activate(long actor_index, s_slot *slot);
void __stdcall joint_deactivate(long actor_index, s_slot *slot);

/* kind 1x handlers: start (stop is joint_leave) and choose */
bool __stdcall joint_initiate_b(long actor_index, s_slot *slot);
short __stdcall function_26e8a0(long actor_index, short slot_index, bool active);

/* joints and invitations, called by the handlers' callbacks */
struct s_joint_behavior_state;

long function_26e940(long actor_index);
bool joint_decline(long actor_index, short invitation_index);
bool function_26eae0(long joint_index, long actor_index, short priority, real score);
bool function_26ecc0(long actor_index, short invitation_index, s_joint_behavior_state *behavior);

#endif
