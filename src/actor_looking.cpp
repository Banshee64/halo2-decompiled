// @flags /O2 /Ob1 /arch:SSE /Gr
/* ACTOR_LOOKING.CPP: actor direction selection and looking state. */

#include "cseries.h"
#include "data_array.h"
#include "real_math.h"
#include "globals.h"
#include "props.h"

/* A local view of the shared actor datum; other modules own the array. */
struct s_actor_looking_view
{
	byte unknown000[0x18];
	long unit_index;
	byte unknown01c[0x54 - 0x1c];
	long character_definition_index;
	byte unknown058[0x84 - 0x58];
	short movement_mode;
	short alert_state;
	byte unknown088[0x266 - 0x88];
	bool using_object;
	bool seated;
	byte unknown268[4];
	long object_index;
	byte unknown270[0x290 - 0x270];
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

real normalize2d(real_point2d *vector);

static inline real actor_looking_normalize2d(real_point2d *vector)
{
	real magnitude = (real)sqrt(vector->x * vector->x + vector->y * vector->y);
	if (!(fabs(magnitude) < 0.0001f))
	{
		real scale = 1.f / magnitude;
		vector->x = vector->x * scale;
		vector->y = scale * vector->y;
		return magnitude;
	}
	return 0.f;
}

/* Retail adds a vertical-component switch to the older map's signature. */
// @retail 0x296d60
PRIVATE bool actor_look_valid_aim_vector(real cosine, real_vector3d const *aim, real_vector3d const *reference, bool three_dimensional)
{
	bool result = false;
	if (three_dimensional)
		return reference->k * aim->k + reference->j * aim->j + reference->i * aim->i > cosine;
	real_point2d aim_horizontal = { aim->i, aim->j };
	real_point2d reference_horizontal = { reference->i, reference->j };
	if (actor_looking_normalize2d(&aim_horizontal) > 0.f &&
		normalize2d(&reference_horizontal) > 0.f &&
		reference_horizontal.y * aim_horizontal.y + aim_horizontal.x * reference_horizontal.x > cosine)
		result = true;
	return result;
}

// @retail 0x296600
PRIVATE real actor_look_compute_prop_interest(long actor_index, long prop_ref_index)
{
	s_actor_looking_view *actor = actor_looking_get(actor_index);
	s_prop_datum *reference = prop_ref_get(prop_ref_index);
	prop_state *state = prop_state_get(reference);
	prop_view *view = NULL;
	if (reference->tracking_index != NONE)
	{
		tracking_datum *tracking = tracking_get(reference->tracking_index);
		if (tracking)
			view = &tracking->view;
	}
	real interest = 0.f;
	if (reference->state >= 1 && reference->state <= 2)
	{
		if (state->unknown5e)
		{
			short ticks = *(short *)((byte *)state + 0x5c);
			interest = 7.f > (real)ticks * g_510c54->rate ? 1.8f : 0.4f;
		}
		else
			interest = prop_get(reference->prop_index)->unknown23 ? 2.f : 1.f;
	}
	else if (prop_get(reference->prop_index)->unknown23 && reference->state >= 3)
		interest = 1.5f;
	if (reference->object_index == actor->object_index || state->unknown3c == actor->object_index)
		interest = 0.f;
	real scale = state->unknown3c == NONE ? 1.f : 1.5f;
	if (view)
	{
		switch (*(char *)((byte *)view + 0x3a))
		{
		case 1: interest += scale * 0.5f; break;
		case 2: interest += scale; break;
		case 3: interest += scale * 2.f; break;
		}
	}
	if (state->unknown63)
		interest += scale * 2.f;
	if (view)
	{
		switch (*(char *)((byte *)view + 0x38))
		{
		case 1: return interest * 0.6f;
		case 3: return interest * 0.4f;
		case 4: break;
		default: return interest;
		}
	}
	return interest * 0.2f;
}

struct s_actor_looking_properties
{
	byte unknown00[0x10];
	real aiming_cosine;
	byte unknown14[4];
	real looking_cosine;
	byte unknown1c[4];
	real idle_aiming_angle;
	real idle_looking_angle;
	real moving_aiming_angle;
	real moving_looking_angle;
};

/* The original character-property lookup has not been recovered yet. */
s_actor_looking_properties *function_1e5160(long character_definition_index);

struct s_actor_looking_object
{
	long definition_index;
	byte unknown04[0x14 - 4];
	long parent_index;
	byte unknown18[0x1fc - 0x18];
	short seat_index;
};

struct s_actor_looking_object_header
{
	byte unknown00[8];
	s_actor_looking_object *object;
};

static inline s_actor_looking_object *actor_looking_object_get(long index)
{
	return ((s_actor_looking_object_header *)g_4e0300->data)[index & 0xffff].object;
}

struct s_actor_looking_seat
{
	byte unknown00[0x88];
	real minimum_yaw;
	real maximum_yaw;
	byte unknown90[0xb0 - 0x90];
};

struct s_actor_looking_unit_definition
{
	byte unknown00[0x1cc];
	s_actor_looking_seat *seats;
};

// @retail 0x2973f0
PRIVATE bool actor_get_looking_bounds(long actor_index, real *looking_cosine, real *aiming_cosine, real *idle_aiming_cosine, real *idle_looking_cosine)
{
	s_actor_looking_view *actor = actor_looking_get(actor_index);
	s_actor_looking_properties *properties = function_1e5160(actor->character_definition_index);
	bool result = false;
	if (properties)
	{
		*aiming_cosine = properties->aiming_cosine;
		result = true;
		if (actor->using_object)
		{
			s_actor_looking_object *object = actor_looking_object_get(actor->object_index);
			s_tag_element *element = function_1e5450(actor_index, object->definition_index);
			if (element)
				*aiming_cosine = (real)cos(*(real *)((byte *)element + 0x70));
		}
		else if (actor->seated)
		{
			s_actor_looking_object *unit = actor_looking_object_get(actor->unit_index);
			if (unit->seat_index != NONE)
			{
				s_actor_looking_object *parent = actor_looking_object_get(unit->parent_index);
				s_actor_looking_unit_definition *definition = (s_actor_looking_unit_definition *)g_4e3b44[parent->definition_index & 0xffff].bytes;
				s_actor_looking_seat *seat = &definition->seats[unit->seat_index];
				real yaw = 0.f - seat->minimum_yaw;
				yaw = yaw > seat->maximum_yaw ? yaw : seat->maximum_yaw;
				*aiming_cosine = (real)cos(yaw);
			}
		}
		*looking_cosine = properties->looking_cosine;
		if (actor->movement_mode == 4)
		{
			*idle_aiming_cosine = (real)cos(properties->moving_aiming_angle);
			*idle_looking_cosine = (real)cos(properties->moving_looking_angle);
		}
		else
		{
			*idle_aiming_cosine = (real)cos(properties->idle_aiming_angle);
			*idle_looking_cosine = (real)cos(properties->idle_looking_angle);
		}
	}
	return result;
}
