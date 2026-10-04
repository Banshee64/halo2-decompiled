// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_0259d0.h"
#include "units.h"
#include "ai_actor.h"
#include "unknown_1e3920.h"
#include "unit_requests.h"

/* slot type 0x50: boarding a vehicle; handler g_47eeb8 */

struct s_slot_vehicle_board
{
	s_slot_header header;
	long prop_index;
	long vehicle_index;
	short seat_index;
	short vehicle_seat_index;
	bool boarding;
	bool requested;
	bool unknown1a;
	bool unknown1b;
	point3f unknown1c;
	byte unknown28[0x40 - 0x28];
};

/* the block of the actor's character tag function_1e4ad0 returns */
struct s_character_vehicle
{
	byte flags;
	byte unknown01[3];
	real unknown04;
	real unknown08;
	real unknown0c;
};

/* a unit's tag and its seats */
struct s_vehicle_board_unit_tag
{
	byte unknown000[0x1c8];
	long seat_count;
	s_unit_seat_definition *seats;
};

long function_1e4ad0(long index);
bool function_10f630(long object_index, long *first, long *second);
bool function_1bf7f0(long actor_index, short seat_index, long object_index);

short __stdcall function_1bf990(long actor_index);
short __stdcall function_1bfd00(long actor_index, s_slot *slot, bool active);
bool __stdcall function_1bfb10(long actor_index, s_slot *slot);
void __stdcall function_1bfc30(long actor_index, s_slot *slot);
void __stdcall function_1c1990(long actor_index, s_slot *slot, long index);
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index);
bool __stdcall function_1bff80(long actor_index, s_slot *slot);
void __stdcall function_1c0230(long actor_index, s_slot *slot);

inline s_vehicle_board_unit_tag *vehicle_board_unit_tag_get(s_slot_object_view *unit)
{
	return (s_vehicle_board_unit_tag *)g_4e3b44[unit->tag_index & 0xffff].bytes;
}

/* the view of a prop's tracking data, or NULL */
inline s_prop_view_fields *vehicle_board_prop_view_get(long tracking_index)
{
	s_prop_view_fields *view = NULL;

	if (tracking_index != NONE)
	{
		byte *tracking = g_502414->data + (tracking_index & 0xffff) * 0x124;

		if (tracking)
			view = (s_prop_view_fields *)(tracking + 0x70);
	}
	return view;
}

// @retail 0x1bf990
short __stdcall function_1bf990(long actor_index)
{
	short result = 0;
	s_actor_view *actor = actor_get(actor_index);

	if (!actor->unknown224 && actor->prop_index != NONE && actor->unknown26c == NONE && actor->unknown018 != NONE)
	{
		s_character_vehicle *character = (s_character_vehicle *)function_1e4ad0(actor->unknown054);

		if (character)
		{
			s_prop_node_view *node = prop_node_get(actor->prop_index);

			if (node->unknown24 >= 1 && node->unknown24 <= 2 &&
				(!function_1e2030(actor_index) || character->unknown04 > node->unknown28))
			{
				s_prop_state_view *state = prop_node_state(node);

				if (function_25d740((s_prop_node *)node) && state->unknown3c != NONE)
				{
					s_slot_object_view *object = object_get(state->unknown3c);

					if (length_sq3f(&object->velocity) < character->unknown0c * character->unknown0c)
					{
						s_slot_object_view *unit = object_get(node->object_index);

						if (unit->parent_index != NONE && function_1bf7f0(actor_index, unit->unknown1fc, unit->parent_index))
							result = 3;
					}
				}
			}
		}
	}
	return result;
}

// @retail 0x1bfc30
void __stdcall function_1bfc30(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_object_view *unit = object_get(actor->unknown018);

	if (unit->parent_index != NONE && unit->unknown1fc != NONE)
	{
		s_vehicle_board_unit_tag *tag = vehicle_board_unit_tag_get(object_get(unit->parent_index));

		if (TEST_FIELD_BIT(tag->seats[unit->unknown1fc].flags.bit11))
		{
			long first;
			long second = NONE;

			function_10f630(actor->unknown018, &first, &second);
			if (second != 0x50000c3 && !function_e68c0(0x1d, actor->unknown018))
				function_e68c0(0x1e, actor->unknown018);
		}
	}
}

// @retail 0x1bfd00
short __stdcall function_1bfd00(long actor_index, s_slot *slot, bool active)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_vehicle_board *state = (s_slot_vehicle_board *)slot;
	short result = g_46fbe4;

	if (actor->prop_index == state->prop_index && state->vehicle_index != NONE)
	{
		s_prop_node_view *node = prop_node_get(actor->prop_index);
		s_prop_view_fields *view = vehicle_board_prop_view_get(node->view_index);

		if (node->unknown24 >= 1 && node->unknown24 <= 2 || view && view->unknown00 >= 5)
		{
			s_slot_object_view *unit;
			s_slot_object_view *target;
			bool seated;

			result = g_46fbe8;
			unit = object_get(actor->unknown018);
			target = object_get(node->object_index);
			seated = target->parent_index == state->vehicle_index && target->unknown1fc == state->seat_index;
			if (actor->unknown26c == NONE)
			{
				state->boarding = false;
				if (seated)
				{
					s_character_vehicle *character = (s_character_vehicle *)function_1e4ad0(actor->unknown054);

					if (character && (!function_1e2030(actor_index) || !(node->unknown28 > character->unknown08)))
					{
						s_prop_state_view *s_type_5cfb45 = prop_node_state(node);

						if (function_25d740((s_prop_node *)node) && s_type_5cfb45->unknown3c != NONE)
						{
							s_slot_object_view *object = object_get(s_type_5cfb45->unknown3c);
							real speed = character->unknown0c * 1.2f;

							if (!(length_sq3f(&object->velocity) > speed * speed))
								return result;
						}
					}
				}
				function_1f86a0(actor_index);
				result = g_46fbe4;
			}
			else if (unit->parent_index == state->vehicle_index && unit->unknown1fc == state->vehicle_seat_index)
			{
				if (state->boarding)
				{
					if (!seated && state->requested)
						result = g_46fbe4;
				}
				else
				{
					long first;
					long second;

					if (function_10f630(actor->unknown018, &first, &second))
					{
						if (second != 0x5000049)
							state->boarding = true;
					}
					else
					{
						result = g_46fbe4;
					}
				}
			}
			else
			{
				result = g_46fbe4;
			}
		}
	}
	return result;
}

s_slot_handler_2 g_47eeb8 =
{
	{
		0x50, 2, 0, -2, 0,
		function_1bf990, function_1bfd00, function_1bfb10, function_1bfc30, NONE, {0},
		0, function_1c1990, function_1c1520, 0, 0, 0, 0
	},
	(t_slot_proc)function_1bff80, 0, function_1c0230
};
