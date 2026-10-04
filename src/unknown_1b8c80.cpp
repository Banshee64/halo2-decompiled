// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "joint_behavior.h"
#include "unknown_11cc90.h"
#include "units.h"
#include "unknown_0d0690.h"

/* the slot tests 0x5f, 0x60, 0x5e, 0x4d, 0x4e and 0x4f, and slot type 0x4c */

struct s_slot_4c
{
	s_slot_header header;
	byte unknown0c[4];
	long element_index;
	byte unknown14[0x1c - 0x14];
	long unknown1c;
	short seat_index;
	byte flags;
	byte unknown23;
	short unknown24;
	byte unknown26[2];
	real unknown28;
	real unknown2c;
	byte unknown30[0x34 - 0x30];
	real_point3d point;
};

short __stdcall function_1b94c0(long actor_index, s_slot *slot);
short __stdcall function_1b9500(long actor_index, s_slot *slot);
short __stdcall function_1b9540(long actor_index, s_slot *slot);
short __stdcall function_1b9640(long actor_index, s_slot *slot);
short __stdcall function_1b9890(long actor_index, s_slot *slot);
short __stdcall function_1b99d0(long actor_index, s_slot *slot);
short __stdcall function_1b9fc0(long actor_index);
short __stdcall function_1ba4e0(long actor_index, s_slot *slot, bool active);
void __stdcall function_1ba090(long actor_index, s_slot *slot);
void __stdcall function_1ba3f0(long actor_index, s_slot *slot, s_slot_target_list *list);
void __stdcall function_1ba5c0(long actor_index, s_slot *slot, long index);
short __stdcall function_1bb3a0(long actor_index, long joint_index, long a, long b);
short __stdcall function_1b2ff0(long actor_index);
void *function_1e5240(long actor_index);

/* how the actor's character uses its weapon (unknown_1e1f20.h) */
struct s_character_weapon_1b9
{
	byte unknown00[0x10];
	real unknown10;
};
short __stdcall function_1bcc90(long actor_index);

/* a seat of a vehicle's tag (0xb0 bytes) */
struct s_vehicle_seat_definition
{
	s_seat_definition_flags flags;
	byte unknown04[0x88 - 0x4];
	real unknown88;
	real unknown8c;
	byte unknown90[0xb0 - 0x90];
};

struct s_vehicle_tag_view
{
	byte unknown000[0xbc];
	byte flagsbc;
	byte unknownbd[0x1c8 - 0xbd];
	long seat_count;
	s_vehicle_seat_definition *seats;
	byte unknown1d0[0x1f0 - 0x1d0];
	short unknown1f0;
};


struct s_player_view
{
	byte unknown000[0xc0];
	char team;
	byte unknown0c1[0x21c - 0xc1];
};

real_point3d *function_b9dd0(long object_index, real_point3d *result);
real function_30bf0(real_vector3d *v);
real function_11cc90(real_vector2d const *a, real_vector2d const *b);

/* an element of g_502424 as slot type 0x4c sees it */
struct s_4c_element
{
	byte unknown00[0x7c];
	short unknown7c;
	byte unknown7e[2];
	long object_index;
	bool unknown84;
	byte unknown85;
	bool unknown86;
};

inline void object_seat_unreserve(s_slot_object_view *object, long seat_index)
{
	if (seat_index >= 0 && seat_index < 32)
		object->unknown3b4 &= ~(1 << seat_index);
}

/* the outermost vehicle carrying the object */
// @retail 0x1b8c80
long function_1b8c80(long object_index)
{
	long result = NONE;

	while (object_index != NONE)
	{
		s_slot_object_view *object = object_get(object_index);

		if (object->type != 1)
			break;
		result = object_index;
		object_index = object->parent_index;
	}
	return result;
}

// @retail 0x1b8cc0
short function_1b8cc0(long object_index, long *seat_object_index)
{
	s_object_seat seats[0x40];
	short result = NONE;
	long seat_object = NONE;
	short count = 0;

	function_c8a40(function_1b8c80(object_index), seats, &count, 0x40);
	for (short i = 0; i < count; i++)
	{
		if (TEST_FIELD_BIT(seats[i].definition->flags.bit2))
		{
			result = i;
			seat_object = seats[i].object_index;
			break;
		}
	}
	if (seat_object_index)
		*seat_object_index = seat_object;
	return result;
}

// @retail 0x1b8d40
long function_1b8d40(long object_index)
{
	long result = NONE;
	long seat_object_index = NONE;
	long vehicle_index = function_1b8c80(object_index);
	short seat_index = function_1b8cc0(vehicle_index, &seat_object_index);

	if (seat_object_index != NONE && seat_index != NONE)
		result = unit_seat_get_occupant(seat_object_index, seat_index);
	return result;
}

// @retail 0x1b8d80
bool function_1b8d80(long actor_index, long object_index, short seat_index, bool ignore_reserved)
{
	s_actor_view *actor = actor_get(actor_index);
	s_object_header_view *header = object_header_get(object_index);

	if (header->type == 1)
	{
		s_slot_object_view *object = (s_slot_object_view *)header->object;

		if (seat_index >= 0 && seat_index < 32)
		{
			dword mask = 1 << seat_index;

			if (object->unknown3b0 & mask)
				return true;
			if (!ignore_reserved && (object->unknown3b4 & mask))
				return true;
		}
	}
	if (actor->unknown024 != NONE && !team_is_enemy(actor->unknown024, 1))
	{
		for (short i = 0; i < MAXIMUM_AI_PLAYERS; i++)
		{
			s_ai_player *entry = &g_4f55cc[i];

			if (entry->player_index != NONE && entry->unit_index == object_index && entry->unknown08 == seat_index &&
				entry->unknown0a > 0)
			{
				return true;
			}
		}
	}
	if (actor->unknown2f2 > 0 && actor->unknown2e8 == object_index && actor->unknown2ec == seat_index)
		return true;
	return false;
}

// @retail 0x1b8eb0
bool function_1b8eb0(long actor_index, long object_index, short seat_index, bool ignore_reserved)
{
	bool result = false;

	if (unit_seat_get_occupant(object_index, seat_index) == NONE &&
		!function_1b8d80(actor_index, object_index, seat_index, ignore_reserved))
	{
		s_actor_view *actor = actor_get(actor_index);

		if (actor->unknown018 != NONE && function_c8200(object_index, seat_index, actor->unknown018))
			result = true;
	}
	return result;
}

// @retail 0x1b8f30
bool function_1b8f30(long actor_index, long object_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short count = 0;
	bool result = false;
	long last_object_index = NONE;
	s_object_seat seats[0x40];

	function_c8a40(object_index, seats, &count, 0x40);
	for (short i = 0; i < count; i++)
	{
		s_object_seat *seat = &seats[i];
		long seat_object_index = seat->object_index;
		s_seat_definition_flags *definition = &seat->definition->flags;

		if (seat_object_index != last_object_index)
		{
			s_slot_object_view *object = object_get(seat_object_index);

			if (object->type == 1 && object->unknown3b4)
				return false;
			last_object_index = seat_object_index;
		}
		if (!TEST_FIELD_BIT(definition->bit11))
		{
			long occupant = unit_seat_get_occupant(seat_object_index, seat->seat_index);

			if (occupant != NONE)
			{
				short team = object_get(occupant)->team;

				if (team != actor->unknown024 && game_team_is_enemy(team, actor->unknown024))
					return false;
			}
			else if (!function_1b8d80(actor_index, seat_object_index, seat->seat_index, false) &&
				function_c8200(seat->object_index, seat->seat_index, actor->unknown018))
			{
				result = true;
			}
		}
	}
	return result;
}

/* false when an enemy of the actor rides the object */
// @retail 0x1b90b0
bool function_1b90b0(long actor_index, long object_index)
{
	s_actor_view *actor = actor_get(actor_index);
	bool result = true;

	for (long child_index = object_get(object_index)->first_child_index; child_index != NONE; )
	{
		s_slot_object_view *child = object_get(child_index);

		if (child->type == 0 && child->unknown1fc != NONE)
		{
			short team = 0;

			if (child->actor_index != NONE)
				team = actor_get(child->actor_index)->unknown024;
			else if (child->player_index != NONE)
				team = ((s_player_view *)g_4e8c24->data)[child->player_index & 0xffff].team;
			result = !game_team_is_enemy(team, actor->unknown024);
			if (!result)
				break;
		}
		child_index = child->next_object_index;
	}
	return result;
}

/* the player riding the object */
// @retail 0x1b9190
long function_1b9190(long object_index, long *rider_index)
{
	s_slot_object_view *object = object_get(object_index);
	long result = NONE;

	if (rider_index)
		*rider_index = NONE;
	for (long child_index = object->first_child_index; child_index != NONE; )
	{
		s_slot_object_view *child = object_get(child_index);

		if (child->type == 0 && child->player_index != NONE)
		{
			if (rider_index)
				*rider_index = child_index;
			return child->player_index;
		}
		child_index = child->next_object_index;
	}
	return result;
}

// @retail 0x1b9200
bool function_1b9200(long object_index, long prop_index)
{
	s_slot_object_view *object = object_get(object_index);
	s_vehicle_tag_view *tag = (s_vehicle_tag_view *)g_4e3b44[object->tag_index & 0xffff].bytes;
	bool result = false;

	if (prop_index != NONE && tag->unknown1f0 == 6 && object->parent_index == NONE)
	{
		result = true;
		if (!(tag->flagsbc & 1))
		{
			s_vehicle_seat_definition *seat = NULL;

			for (short i = 0; i < tag->seat_count; i++)
			{
				if (TEST_FIELD_BIT(tag->seats[i].flags.bit3))
					seat = &tag->seats[i];
			}
			if (seat)
			{
				s_prop_state_view *state = prop_node_state(prop_node_get(prop_index));
				real_point3d position;
				real_vector3d forward;

				function_b9dd0(object_index, &position);
				object_get_forward(object_index, &forward);
				forward.k = 0.0f;
				if (function_30bf0(&forward) > 0.0f)
				{
					real_vector3d direction;

					direction.i = state->position.x - position.x;
					direction.j = state->position.y - position.y;
					direction.k = 0.0f;
					if (function_30bf0(&direction) > g_45dbd8)
					{
						real angle = function_11cc90((real_vector2d *)&direction, (real_vector2d *)&forward);

						if (angle <= seat->unknown88 || angle > seat->unknown8c)
							result = false;
					}
				}
			}
		}
	}
	return result;
}

// @retail 0x1b9420
bool function_1b9420(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	bool result = false;

	if (actor->unknown086 >= 3)
		return true;

	s_object_child_iterator iterator;

	function_d0620(actor->unknown26c, &iterator);
	while (function_d0690(&iterator))
	{
		if (object_get(iterator.child_value)->player_index != NONE)
			return true;
	}
	return result;
}

// @retail 0x1b94c0
short __stdcall function_1b94c0(long actor_index, s_slot *slot)
{
	short result = g_46fbe4;

	if (function_1b9420(actor_index) && function_1b2ff0(actor_index) > 0)
		result = 0x6d;
	return result;
}

// @retail 0x1b9500
short __stdcall function_1b9500(long actor_index, s_slot *slot)
{
	short result = g_46fbe4;

	if (function_1b9420(actor_index) && function_1bcc90(actor_index) > 0)
		result = 0x6b;
	return result;
}

/* the vehicle's entry in the actor's character tag says when the actor
   wants to be in it */
// @retail 0x1b9540
short __stdcall function_1b9540(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_object_view *vehicle = object_get(actor->unknown26c);
	s_tag_element *element = function_1e5450(actor_index, vehicle->tag_index);
	bool wanted = actor->unknown5d4;

	if (element && actor->prop_index != NONE)
	{
		if (wanted)
		{
			real elapsed = (real)(g_510c54->game_time - actor->unknown2f8) * g_510c54->rate;

			if (element->unknowna8 > elapsed)
			{
				wanted = true;
			}
			else if (elapsed > element->unknownac)
			{
				wanted = false;
			}
			else
			{
				s_character_weapon_1b9 *weapon = (s_character_weapon_1b9 *)function_1e5240(actor_index);

				if (weapon && prop_node_get(actor->prop_index)->unknown28 > weapon->unknown10)
					wanted = false;
			}
		}
		else if (vehicle->unknown100 > element->unknowna4)
		{
			wanted = true;
			actor->unknown2f8 = g_510c54->game_time;
		}
	}
	else
	{
		wanted = false;
	}
	actor->unknown44a = wanted;
	actor->unknown449 = wanted;
	return g_46fbe4;
}

// @retail 0x1b9fc0
short __stdcall function_1b9fc0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->unknown858 == NONE)
	{
		s_slot_entry_iterator iterator;

		iterator.actor_index = actor_index;
		iterator.reference.unknown2 = 0x4c;
		iterator.reference.unknown0 = NONE;
		while (function_26f0c0(&iterator))
		{
			s_4c_element *element = (s_4c_element *)element_502424_get(actor->memory[iterator.reference.unknown0].unknown4);

			if (!element->unknown86)
			{
				if (actor->unknown26c != NONE)
				{
					if (actor->unknown266 && actor->unknown26c == element->object_index)
						result = 3;
				}
				else if (actor->unknown328 < 10)
				{
					result = 3;
				}
			}
		}
	}
	return result;
}

// @retail 0x1ba3f0
void __stdcall function_1ba3f0(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_slot_4c *state = (s_slot_4c *)slot;

	if (state->unknown1c != NONE && state->seat_index != NONE)
		object_seat_unreserve(object_get(state->unknown1c), state->seat_index);

	s_4c_element *element = (s_4c_element *)list;

	if (element->object_index != NONE && element->unknown7c == 1)
	{
		short count = 0;
		s_object_seat seats[0x40];

		function_c8a40(element->object_index, seats, &count, 0x40);
		for (short i = 0; i < count; i++)
		{
			s_object_seat *seat = &seats[i];
			s_object_header_view *header = object_header_get(seat->object_index);

			if (header->type == 1)
				object_seat_unreserve((s_slot_object_view *)header->object, seat->seat_index);
		}
	}
}

// @retail 0x1ba8c0
void __stdcall function_1ba8c0(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_slot_4c *state = (s_slot_4c *)slot;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown26c == NONE)
	{
		if (state->flags & 4)
		{
			actor->unknown420 = 4;
			actor->unknown41c = 3;
			actor->unknown424.point = state->point;
			actor->unknown44d = true;
		}
		else if (actor->unknown086 >= 7)
		{
			actor->unknown41c = 3;
			actor->unknown420 = 2;
			actor->unknown488 = true;
		}
		else if (actor->unknown50c)
		{
			actor->unknown41c = 2;
			actor->unknown420 = 0;
		}
	}
	actor->unknown450 = 0x6000086;
}

/* the unit fields function_1ba990 reads */
struct s_unit_1ba990
{
	byte unknown000[0x84];
	real unknown084;
	real_vector3d velocity;
	byte unknown094[0xec - 0x94];
	real unknownec;
	byte unknownf0[0x10a - 0xf0];
	word unknown10a_0 : 2;
	word flag10a_2 : 1;
	word unknown10a_3 : 13;
};

/* real_math's distance_squared3d (0x24550), inlined */
static inline real distance_squared3d_1ba990(real_point3d const *a, real_point3d const *b)
{
	real_vector3d v;
	v.i = b->x - a->x;
	v.j = b->y - a->y;
	v.k = b->z - a->z;
	real sum = v.k * v.k;
	sum += v.i * v.i;
	sum += v.j * v.j;
	return sum;
}

/* whether the unit is close enough to the actor (and slow enough) for it */
// @retail 0x1ba990
bool function_1ba990(long actor_index, long unit_index, bool force, real near_radius, real far_radius, bool use_near_radius)
{
	s_actor_view *actor = actor_get(actor_index);
	s_unit_1ba990 *unit = (s_unit_1ba990 *)object_get(unit_index);
	bool result = false;

	if (TEST_FIELD_BIT(unit->flag10a_2))
	{
		result = false;
	}
	else if (force)
	{
		result = true;
	}
	else if (0.1f > unit->unknownec)
	{
		result = false;
	}
	else
	{
		real radius = use_near_radius ? near_radius : far_radius;
		real_point3d position;

		function_b9dd0(unit_index, &position);
		if (distance_squared3d_1ba990(&actor->position, &position) < radius * radius)
		{
			result = true;
			if (!use_near_radius && magnitude_squared3d(&unit->velocity) > 0.25f)
				result = false;
		}
	}
	if (0.5f > unit->unknown084)
		return false;
	return result;
}

bool function_f5dc0(long object_index);
long function_25d810(long object_index, long actor_index, bool create);
void __stdcall function_25c230(long actor_index, long prop_ref_index, short unknown);

/* slot test 0x4e: the actor goes for the vehicle it was told to use */
// @retail 0x1b9890
short __stdcall function_1b9890(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = g_46fbe4;

	if (!actor->unknown224 && !actor->unknown225 && actor->unknown858 == NONE)
	{
		long vehicle_index = actor->unknown2e8;

		if (vehicle_index != NONE && actor->unknown2f0 && !function_f5dc0(vehicle_index) &&
			object_get(vehicle_index)->unknown34f > 0 &&
			function_1ba990(actor_index, vehicle_index, false, 20.0f, 24.0f, false))
		{
			s_slot_4c *state = (s_slot_4c *)slot;
			long prop_index = function_25d810(actor->unknown2e8, actor_index, true);

			if (prop_index != NONE && prop_node_get(prop_index)->unknown24 < 1)
				function_25c230(actor_index, prop_index, 3);
			state->unknown1c = actor->unknown2e8;
			state->unknown28 = 20.0f;
			state->unknown2c = 24.0f;
			state->seat_index = NONE;
			state->flags |= 0x29;
			result = 0x4c;
		}
	}
	return result;
}

/* invites the clump members to the joint, the nearest first, when the
   object has more than one seat to take */
// @retail 0x1bb3a0
short __stdcall function_1bb3a0(long actor_index, long joint_index, long a, long b)
{
	s_actor_view *actor = actor_get(actor_index);
	s_4c_element *element = (s_4c_element *)element_502424_get(joint_index);
	long count = 0;
	short seat_count = 0;
	short free_count = 0;
	real_point3d position;
	s_object_seat seats[0x40];

	function_c8a40(element->object_index, seats, &seat_count, 0x40);
	for (short i = 0; i < seat_count; i++)
	{
		if (!TEST_FIELD_BIT(seats[i].definition->flags.bit11))
			free_count++;
	}
	function_b9dd0(element->object_index, &position);
	if (free_count > 1 && actor->unknown07c != NONE)
	{
		long index = element_502420_get(actor->unknown07c)->first_actor_index;

		while (index != NONE)
		{
			s_actor_view *other = actor_get(index);
			long other_index = index;

			index = other->next_index;
			if (actor != other &&
				(other->unknown26c == NONE || other->unknown26c == element->object_index && other->unknown266))
			{
				real_vector3d delta;

				vector3d_from_points3d(&position, &other->position, &delta);
				if (invite_actor(joint_index, other_index, 3, (real)(1.0 / (magnitude_squared3d(&delta) + 0.1f))))
					count++;
			}
		}
	}
	return (short)count;
}

// @retail 0x1bb530
void __stdcall function_1bb530(long actor_index, s_slot *slot, long index)
{
	s_slot_4c *state = (s_slot_4c *)slot;
	s_502424_element *element = element_502424_get(state->element_index);

	if (state->unknown1c == index || element->target.unknown0 == index)
	{
		state->unknown1c = NONE;
		element->target.unknown0 = NONE;
	}
}

s_slot_handler_0 g_47e954 =
{
	0x5f, 0, 0xbff, -2, 0, function_1b94c0
};

s_slot_handler_0 g_47e968 =
{
	0x60, 0, 0xbff, -2, 0, function_1b9500
};

s_slot_handler_0 g_47e97c =
{
	0x5e, 0, 0, -2, 0, function_1b9540
};

s_slot_handler_0 g_47e990 =
{
	0x4d, 0, 0, -2, 0, function_1b9640
};

s_slot_handler_0 g_47e9a4 =
{
	0x4e, 0, 0, -2, 0, function_1b9890
};

s_slot_handler_0 g_47e9b8 =
{
	0x4f, 0, 0, -2, 0, function_1b99d0
};

s_slot_handler_2x g_47e9d0 =
{
	{
		{
			0x4c, 2, 0, -2, 0,
			function_1b9fc0, function_1ba4e0, joint_initiate, joint_leave, 1, {0},
			0, 0, function_1bb530, 0, 0, 0, 1
		},
		(t_slot_proc)joint_update, joint_activate, joint_deactivate
	},
	function_1ba090, (t_slot_release)function_1ba3f0, function_1ba5c0, slot_release_nothing, function_1ba8c0, (t_slot_proc4)function_1bb3a0,
	1, 10, 1.5f, 0x5b
};

// @retail 0x1ba4e0
short __stdcall function_1ba4e0(long actor_index, s_slot *slot, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_4c *state = (s_slot_4c *)slot;
	s_4c_element *element = (s_4c_element *)element_502424_get(state->element_index);
	short result = g_46fbe8;

	state->unknown24++;
	if (element->object_index != NONE && state->unknown1c != NONE)
	{
		real seconds = g_510c54->ticks_per_second * 10.0f;
		long ticks;

		__asm
		{
			fld seconds
			fistp ticks
		}

		if (state->unknown24 <= ticks)
		{
			if (actor->unknown26c != NONE)
			{
				result = g_46fbe4;
				if (element->unknown7c > 1)
				{
					if (actor->unknown266)
						result = g_46fbe8;
					if (!element->unknown84)
						function_1fb7e0(actor_index, 0x39, NULL, NONE, NONE);
				}
			}
			return result;
		}
	}

	return g_46fbe4;
}
