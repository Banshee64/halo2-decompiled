// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"

/* slot type 6 */

/* g_51e9d8: 0x98 byte elements, each naming an entry (0x7c bytes) of the
   table at +0x244 of g_4e0350 */
struct s_51e9d8_element
{
	byte unknown00[0x2a];
	short entry_index;
	byte unknown2c[0x80 - 0x2c];
	long unknown80;
	byte unknown84[0x98 - 0x84];
};

struct s_4e0350_entry
{
	byte unknown00[0x24];
	dword flags;
	byte unknown28[0x4e - 0x28];
	short unknown4e;
	byte unknown50[0x7c - 0x50];
};

struct s_4e0350_view
{
	byte unknown000[0x244];
	s_4e0350_entry *entries;
};

s_data_array *g_51e9d8;

void __stdcall function_1b0ab0(long actor_index, s_slot *slot);

struct s_262b40_result;
struct s_261d20_entry
{
	byte unknown00[0xc];
	real_point3d point;
	byte unknown18[0x78 - 0x18];
};

real_point3d *function_b9dd0(long object_index, real_point3d *result);
s_262b40_result *__stdcall function_262b40(s_reference reference);
real function_210b60(s_262b40_result *path);
short __stdcall function_261d20(long actor_index, s_261d20_entry *entries, long maximum_count, long unknown, real_point3d const *point);

inline real distance_squared3d(real_point3d const *a, real_point3d const *b)
{
	real i = a->x - b->x;
	real j = a->y - b->y;
	real k = a->z - b->z;

	return i * i + j * j + k * k;
}

/* the squared distances within which the actor follows the object */
// @retail 0x1b0710
void function_1b0710(long object_index, long actor_index, real *distance_squared, real *maximum_distance_squared)
{
	real result = 100.0f;

	if (object_get(object_index)->unknown1fc == NONE && actor_get(actor_index)->unknown26c == NONE)
		result = 16.0f;
	if (distance_squared)
		*distance_squared = result;
	if (maximum_distance_squared)
		*maximum_distance_squared = result * 2.25f;
}

// @retail 0x1b0780
short __stdcall function_1b0780(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short result = 0;

	if (actor->unknown030 != NONE)
	{
		s_51e9d8_element *element = (s_51e9d8_element *)(g_51e9d8->data + (actor->unknown030 & 0xffff) * sizeof(s_51e9d8_element));

		if (element->entry_index != NONE)
		{
			s_4e0350_entry *entry = &((s_4e0350_view *)g_4e0350)->entries[element->entry_index];

			if (entry && ((entry->flags & 0x10) || (entry->flags & 0x20) && entry->unknown4e != NONE) &&
				element->unknown80 != NONE &&
				!game_team_is_enemy(actor->unknown024, object_get(element->unknown80)->team))
			{
				real maximum_distance_squared;
				real_point3d position;

				function_1b0710(element->unknown80, actor_index, NULL, &maximum_distance_squared);
				function_b9dd0(element->unknown80, &position);
				if (distance_squared3d(&actor->position, &position) > maximum_distance_squared &&
					(*(long *)&actor->unknown418 == *(long *)&g_470fa0 || !actor->unknown5d0 ||
					function_210b60(function_262b40(actor->unknown418)) > maximum_distance_squared))
				{
					s_261d20_entry entries[0x200];
					short count = function_261d20(actor_index, entries, 0x200, 0, &actor->position);

					for (short i = 0; i < count; i++)
					{
						if (maximum_distance_squared > distance_squared3d(&entries[i].point, &position))
						{
							result = 3;
							break;
						}
					}
				}
			}
		}
	}
	return result;
}

// @retail 0x1b09b0
bool __stdcall function_1b09b0(long actor_index, s_slot *slot)
{
	actor_reset_state(actor_index);
	return true;
}

// @retail 0x1b0a10
short __stdcall function_1b0a10(long actor_index, s_slot *slot, bool active)
{
	short result = g_46fbe4;
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown504 != 2 && actor->unknown030 != NONE)
	{
		s_51e9d8_element *element = (s_51e9d8_element *)(g_51e9d8->data + (actor->unknown030 & 0xffff) * sizeof(s_51e9d8_element));

		if (element->entry_index != NONE)
		{
			s_4e0350_entry *entry = &((s_4e0350_view *)g_4e0350)->entries[element->entry_index];

			if (entry && ((entry->flags & 0x10) || (entry->flags & 0x20) && entry->unknown4e != NONE) &&
				element->unknown80 != NONE)
			{
				result = g_46fbe8;
			}
		}
	}
	return result;
}

// @retail 0x1b0c70
void __stdcall function_1b0c70(long actor_index, s_slot *slot)
{
	s_actor_view *actor = actor_get(actor_index);

	if (actor->unknown270 != 4)
	{
		if (actor->unknown086 >= 7)
		{
			actor->unknown488 = true;
			actor->unknown41c = 3;
			actor->unknown420 = 2;
		}
		else
		{
			actor->unknown41c = 2;
			actor->unknown420 = 2;
		}
	}
}

s_slot_handler_2 g_47df10 =
{
	{
		6, 2, 0, -2, 0,
		function_1b0780, function_1b0a10, function_1b09b0, 0, NONE, {0},
		0, 0, 0, 0, 0, 0, 0
	},
	function_1b0ab0, slot_proc_nothing, function_1b0c70
};
