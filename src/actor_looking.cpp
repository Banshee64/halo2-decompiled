// @flags /O2 /Ob1 /arch:SSE /Gr
/* ACTOR_LOOKING.CPP: actor direction selection and looking state. */

#include "cseries.h"
#include "data_array.h"
#include "real_math.h"
#include "globals.h"

/* A local view of the shared actor datum; other modules own the array. */
struct s_actor_looking_view
{
	byte unknown000[0x86];
	short alert_state;
	byte unknown088[0x290 - 0x88];
	real_vector3d forward;
	byte unknown29c[0x338 - 0x29c];
	long target_prop_index;
	byte unknown33c[0x41c - 0x33c];
	short aiming_mode;
	byte unknown41e[2];
	short aiming_direction_type;
	byte unknown422[2];
	long aiming_prop_index;
	byte unknown428[0x430 - 0x428];
	short looking_mode;
	byte unknown432[2];
	short looking_direction_type;
	byte unknown436[2];
	long looking_prop_index;
	byte unknown43c[0x698 - 0x43c];
	long idle_aiming_timer;
	long idle_looking_timer;
	short idle_aiming_direction_type;
	byte unknown6a2[2];
	real_vector3d idle_aiming_direction;
	short idle_looking_direction_type;
	byte unknown6b2[2];
	real_vector3d idle_looking_direction;
	byte unknown6c0[0x6f8 - 0x6c0];
	bool aiming;
	byte unknown6f9[0x888 - 0x6f9];
};

static inline s_actor_looking_view *actor_looking_get(long actor_index)
{
	return (s_actor_looking_view *)g_4f55f0->data + (actor_index & 0xffff);
}

// @retail 0x298b60
bool aiming_at_target(long actor_index)
{
	s_actor_looking_view *actor = actor_looking_get(actor_index);
	bool result = false;
	if (actor->aiming && actor->aiming_mode >= 2)
	{
		if (actor->aiming_direction_type == 2 ||
			(actor->aiming_direction_type == 1 && actor->aiming_prop_index == actor->target_prop_index))
			result = true;
	}
	return result;
}

// @retail 0x298bc0
bool looking_at_target(long actor_index)
{
	s_actor_looking_view *actor = actor_looking_get(actor_index);
	bool result = false;
	if (actor->looking_mode == 0)
	{
		if (actor->alert_state >= 3)
			return aiming_at_target(actor_index);
	}
	else if (actor->looking_mode == 2)
	{
		if (actor->looking_direction_type == 2 ||
			(actor->looking_direction_type == 1 && actor->looking_prop_index == actor->target_prop_index))
			result = true;
	}
	return result;
}

/* Name inferred from the timer and direction initialization. */
// @retail 0x297560
PRIVATE void reset_idle_timers(long actor_index)
{
	s_actor_looking_view *actor = actor_looking_get(actor_index);
	real ticks = (real)g_510c54->ticks_per_second;
	long rounded_ticks;
	/* Retail rounds using the current x87 rounding mode. */
	__asm
	{
		fld ticks
		fistp rounded_ticks
	}
	actor->idle_aiming_timer = (short)rounded_ticks;
	actor->idle_looking_timer = (short)rounded_ticks;
	actor->idle_aiming_direction_type = 4;
	actor->idle_looking_direction_type = 4;
	actor->idle_aiming_direction = actor->forward;
	actor->idle_looking_direction = actor->forward;
}

// @retail 0x297600
PRIVATE void advance_idle_timers(long actor_index)
{
	s_actor_looking_view *actor = actor_looking_get(actor_index);
	/* Retail clamps after converting each decremented integer timer to real. */
	real aiming = (real)(actor->idle_aiming_timer - 1);
	actor->idle_aiming_timer = (long)(aiming > 0.f ? aiming : 0.f);
	real looking = (real)(actor->idle_looking_timer - 1);
	actor->idle_looking_timer = (long)(looking > 0.f ? looking : 0.f);
}
