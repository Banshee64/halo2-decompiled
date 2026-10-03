// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "slot_handler.h"
#include "unknown_11cc90.h"

/* slot type 0x7e: the actors of a group take the positions of a formation
   around their leader */

struct s_slot_7e
{
	s_slot_header header;
	bool unknown0c;
	byte unknown0d[3];
	long element_index;
	byte unknown14[0x1c - 0x14];
	short position_index;
	short unknown1e;
	long unknown20;
	bool unknown24;
	byte unknown25[3];
	long unknown28;
	long unknown2c;
	byte unknown30[0x40 - 0x30];
};

/* the groups the actor iterates (function_272d90 and function_272e20) */
struct s_actor_group_iterator
{
	byte unknown00[8];
	long actor_index;
	byte unknown0c[0x20 - 0xc];
};

/* the actors of an encounter squad (function_204d30) */
struct s_squad_actor_iterator
{
	byte unknown00[8];
	long actor_index;
};

struct s_4f55d0_view
{
	bool unknown0;
	bool active;
};

s_4f55d0_view *g_4f55d0;

bool function_26ecc0(long actor_index, s_slot *slot, s_reference reference);
long function_26e940(long actor_index);
void __stdcall function_1f4280(long actor_index);
bool function_25ab50(long point_reference);
real normalize2d(real_point2d *v);
bool invite_actor(long actor_index, long other_index, long a, short type, real weight);
void function_272d90(long group_index, s_actor_group_iterator *iterator);
s_actor_view *function_272e20(s_actor_group_iterator *iterator);
void function_204d30(long squad_index, s_squad_actor_iterator *iterator);

/* the points function_25ab50 validates: g_4e0350 + 0x1dc */
struct s_point_block
{
	byte unknown00[0x24];
	byte *points;
	byte unknown28[0x30 - 0x28];
};

struct s_point_globals
{
	long count;
	s_point_block *blocks;
};

struct s_4e0350_point_view
{
	byte unknown000[0x1dc];
	s_point_globals *points;
};

inline real_point2d *point_get(long reference)
{
	s_point_block *block = &((s_4e0350_point_view *)g_4e0350)->points->blocks[(reference >> 16) & 0xffff];

	return (real_point2d *)(block->points + (reference & 0xffff) * 0x3c + 0x20);
}

real_vector2d *g_468778;

inline real magnitude2d(real_vector2d const *v)
{
	return (real)sqrt(v->i * v->i + v->j * v->j);
}

inline real normalize2d_fast(real_vector2d *v)
{
	real magnitude = magnitude2d(v);

	if (fabs(magnitude) < 0.0001f)
		return 0.0f;

	real scale = 1.0f / magnitude;

	v->i *= scale;
	v->j *= scale;
	return magnitude;
}

inline real distance2d(real_point2d const *a, real_point2d const *b)
{
	real_vector2d d;

	d.i = a->x - b->x;
	d.j = a->y - b->y;
	return magnitude2d(&d);
}

short __stdcall function_1b0cd0(long actor_index);
bool __stdcall function_26e4b0(long actor_index, s_slot *slot);
void __stdcall function_26e600(long actor_index, s_slot *slot);
void __stdcall function_26e650(long actor_index, s_slot *slot);
void __stdcall function_26e6d0(long actor_index, s_slot *slot);
void __stdcall function_26e710(long actor_index, s_slot *slot);
void __stdcall function_1b13b0(long actor_index, s_slot *slot, long index);

// @retail 0x1b0cd0
short __stdcall function_1b0cd0(long actor_index)
{
	s_slot_entry_iterator iterator;
	short result = 0;

	iterator.actor_index = actor_index;
	iterator.reference.unknown2 = 0x7e;
	iterator.reference.unknown0 = NONE;
	if (function_26f0c0(&iterator))
		result = 3;
	return result;
}

// @retail 0x1b0d10
short __stdcall function_1b0d10(long actor_index, s_slot *slot, bool active)
{
	s_slot_7e *state = (s_slot_7e *)slot;
	short result = g_46fbe8;

	if (element_502424_get(state->element_index)->unknown04 == NONE)
		result = g_46fbe4;
	return result;
}

// @retail 0x1b0d50
void __stdcall function_1b0d50(long actor_index, s_slot *slot)
{
	s_slot_7e *state = (s_slot_7e *)slot;
	s_slot_entry_iterator iterator;

	iterator.actor_index = actor_index;
	iterator.reference.unknown2 = 0x7e;
	iterator.reference.unknown0 = NONE;
	for (s_slot_memory_entry *entry = function_26f0c0(&iterator); entry; entry = function_26f0c0(&iterator))
	{
		if (function_26ecc0(actor_index, slot, iterator.reference))
		{
			state->position_index = NONE;
			if (entry->unknown4 != NONE)
				return;
			break;
		}
	}

	long element_index = function_26e940(actor_index);

	if (element_index != NONE)
	{
		s_slot_target_list *list = (s_slot_target_list *)element_502424_get(element_index);
		bool unknown24 = state->unknown24;

		state->unknown0c = true;
		state->element_index = element_index;
		state->position_index = 0;
		if (unknown24)
		{
			list->mode = state->unknown1e;
			list->unknown84 = state->unknown28;
			list->unknown88 = state->unknown2c;
			list->unknown80 = false;
		}
		else
		{
			state->unknown20 = NONE;
			list->mode = 5;
			list->unknown84 = NONE;
			list->unknown80 = false;
		}
	}
}

/* the position of a formation slot: in a line behind the leader, or (mode 5)
   in two staggered lines */
// @retail 0x1b0e40
bool function_1b0e40(real_point2d *point, real_point2d const *origin, real_vector2d const *forward, short index,
	short mode, real_vector2d const *left)
{
	switch (mode)
	{
	case 0:
	{
		real distance = index * 0.45f;

		point->x = forward->i * distance + origin->x;
		point->y = forward->j * distance + origin->y;
		break;
	}
	case 5:
	{
		real row = (real)(short)((index + 1) / 2);
		real distance = row * -0.225f;

		point->x = forward->i * distance + origin->x;
		point->y = forward->j * distance + origin->y;

		double side = (index % 2 - 0.5) * (row * 0.9f);

		point->x += side * left->i;
		point->y += side * left->j;
		break;
	}
	}
	return true;
}

// @retail 0x1b0f20
void function_1b0f20(s_slot_target_list *list, real_vector2d *direction)
{
	if (function_25ab50(list->unknown84) && function_25ab50(list->unknown88))
	{
		real_point2d *end = point_get(list->unknown88);
		real_point2d *start = point_get(list->unknown84);

		direction->i = end->x - start->x;
		direction->j = end->y - start->y;
		if (normalize2d((real_point2d *)direction) != 0.0f)
			return;
	}
	else
	{
		s_actor_view *leader = actor_get(list->entries[0].actor_index);

		*direction = *(real_vector2d *)&leader->unknown290;
		if (normalize2d_fast(direction) != 0.0f)
			return;
	}
	*direction = *g_468778;
}

// @retail 0x1b1080
s_slot *function_1b1080(short index, s_slot_target_list *list)
{
	s_slot *result = NULL;

	if (index >= 0 && index < list->entry_count)
	{
		s_slot_target_entry *entry = &list->entries[index];

		if (entry->actor_index != NONE && entry->type == 3)
		{
			s_actor_view *actor = actor_get(entry->actor_index);

			if (actor->current >= 0 && actor->current < 4)
				result = &actor->slots[actor->current];
		}
	}
	return result;
}

// @retail 0x1b10e0
short function_1b10e0(short index, s_slot_target_list *list)
{
	short result = NONE;
	s_slot_7e *state = (s_slot_7e *)function_1b1080(index, list);

	if (state)
		result = state->position_index;
	return result;
}

/* chooses the free formation position nearest the actor */
// @retail 0x1b1100
short function_1b1100(long actor_index, real_vector2d const *forward, s_slot_7e *state, s_slot_target_list *list,
	real_point2d const *origin, real_vector2d const *left, real_point2d *position)
{
	s_actor_view *actor = actor_get(actor_index);

	if (state->unknown0c)
		return 0;

	real distances[10];
	real_point2d points[10];
	short best_index = NONE;
	real best_distance = 3.4028235e38f;
	short count = list->entry_count;

	for (short i = 1; i < count; i++)
	{
		if (function_1b0e40(&points[i], origin, forward, i, list->mode, left))
		{
			real distance = distance2d((real_point2d *)&actor->position, &points[i]);

			distances[i] = distance;
			if (best_distance > distance)
			{
				best_index = i;
				best_distance = distance;
			}
		}
		else
		{
			distances[i] = 3.4028235e38f;
		}
	}
	while (best_index != NONE)
	{
		short other_index = NONE;

		for (short j = 0; j < 10; j++)
		{
			s_slot_7e *other = (s_slot_7e *)function_1b1080(j, list);

			if (other && other->position_index == best_index)
			{
				other_index = j;
				break;
			}
		}
		if (other_index == NONE)
			break;

		s_actor_view *other_actor = actor_get(list->entries[other_index].actor_index);

		if (distance2d((real_point2d *)&other_actor->position, &points[best_index]) + 0.1f > distances[best_index])
		{
			((s_slot_7e *)&other_actor->slots[other_actor->current])->position_index = NONE;
			break;
		}
		distances[best_index] = 3.4028235e38f;
		best_index = NONE;
		best_distance = 3.4028235e38f;
		for (short k = 1; k < count; k++)
		{
			if (best_distance > distances[k])
			{
				best_distance = distances[k];
				best_index = k;
			}
		}
	}
	if (best_index >= 0 && best_index < 10 && position)
		*position = points[best_index];
	state->position_index = best_index;
	return best_index;
}

/* whether the actor stands on the right side of its formation position */
// @retail 0x1b1970
bool function_1b1970(long actor_index, short mode, short index, real_vector2d const *forward,
	real_vector2d const *left)
{
	bool result = false;

	switch (mode)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		result = true;
		break;
	case 5:
		if (index == 0)
		{
			result = true;
		}
		else
		{
			s_actor_view *actor = actor_get(actor_index);
			real_point3d *target = &prop_node_state(prop_node_get(actor->prop_index))->position;
			real_point2d direction;

			direction.x = target->x - actor->position.x;
			direction.y = target->y - actor->position.y;
			if (normalize2d(&direction) != 0.0f)
			{
				real forward_dot = forward->i * direction.x + forward->j * direction.y;
				real left_dot = left->i * direction.x + left->j * direction.y;

				if ((index % 2 == 0 ? 0.0f >= left_dot : left_dot >= 0.0f) || forward_dot >= 0.9f)
					result = true;
				else
					result = false;
			}
		}
		break;
	}
	return result;
}

// @retail 0x1b1a90
void __stdcall function_1b1a90(long actor_index, s_slot *slot, s_slot_target_list *list)
{
	s_actor_view *actor = actor_get(actor_index);

	if (list->entries[0].actor_index == NONE)
		return;

	s_slot_7e *state = (s_slot_7e *)slot;
	s_actor_view *leader = actor_get(list->entries[0].actor_index);
	real_vector2d forward;
	real_vector2d left;

	forward.i = leader->unknown290.i;
	forward.j = leader->unknown290.j;
	normalize2d_fast(&forward);
	left.i = 0.0f - forward.j;
	left.j = forward.i;
	if (list->unknown80)
	{
		if (!state->unknown0c)
		{
			if (actor->unknown50c)
				function_1f4280(actor_index);
			if (leader->unknown5d0)
			{
				real_point2d point;

				if (function_1b0e40(&point, (real_point2d *)&leader->position, &forward, state->position_index, list->mode, &left))
				{
					real_point2d direction;

					direction.x = point.x - actor->position.x;
					direction.y = point.y - actor->position.y;
					normalize2d(&direction);
					if (leader->unknown290.i * direction.x + leader->unknown290.j * direction.y < 0.5f)
					{
						function_1f4280(actor_index);
					}
					else
					{
						actor->unknown456 = true;
						actor->unknown458.i = direction.x;
						actor->unknown458.j = direction.y;
						actor->unknown458.k = 0.0f;
					}
				}
			}
		}
		if (actor->unknown086 >= 7)
		{
			if (state->position_index != NONE)
				actor->unknown488 = function_1b1970(actor_index, list->mode, state->position_index, &forward, &left);
			else
				actor->unknown488 = false;
			actor->unknown41c = 3;
			actor->unknown420 = 2;
		}
		actor->unknown44a = true;
		actor->unknown449 = true;
	}
	else
	{
		actor->unknown4cc = 0.2f;
		if (actor->unknown504 == 2)
		{
			real_vector2d direction;

			function_1b0f20(list, &direction);
			actor->unknown420 = 4;
			actor->unknown424.vector.i = direction.i;
			actor->unknown424.vector.j = direction.j;
			actor->unknown44d = true;
			actor->unknown41c = 3;
			actor->unknown424.vector.k = 0.0f;
		}
		else if (actor->unknown086 >= 7)
		{
			actor->unknown488 = false;
			actor->unknown420 = 2;
			actor->unknown41c = 3;
		}
	}
}

/* invites the actors of the actor's group (or squad) of the same team */
// @retail 0x1b1d90
short __stdcall function_1b1d90(long actor_index, long leader_index, s_slot *slot, long unknown)
{
	s_actor_view *actor = actor_get(actor_index);
	s_slot_7e *state = (s_slot_7e *)slot;
	long count = 0;

	if (state->unknown20 != NONE)
	{
		s_actor_group_iterator iterator;

		function_272d90(state->unknown20, &iterator);
		for (s_actor_view *other = function_272e20(&iterator); other; other = function_272e20(&iterator))
		{
			if (other != actor && actor->unknown004 == other->unknown004 &&
				invite_actor(leader_index, iterator.actor_index, 3, 0, 1.0f))
			{
				count++;
			}
		}
		return (short)count;
	}
	if (actor->unknown030 != NONE)
	{
		s_squad_actor_iterator iterator;

		function_204d30(actor->unknown030, &iterator);
		long other_index = iterator.actor_index;

		while (g_4f55d0->active && other_index != NONE)
		{
			s_actor_view *other = actor_get(other_index);
			long index = other_index;

			other_index = other->unknown020;
			if (other != actor && actor->unknown004 == other->unknown004 &&
				invite_actor(leader_index, index, 3, 0, 1.0f))
			{
				count++;
			}
		}
		return (short)count;
	}
	return 0;
}

s_slot_handler_2x g_47df60 =
{
	{
		{
			0x7e, 2, 0, -2, 0,
			function_1b0cd0, function_1b0d10, function_26e4b0, function_26e600, NONE, {0},
			0, 0, 0, 0, 0, 0, 1
		},
		function_26e650, function_26e6d0, function_26e710
	},
	function_1b0d50, 0, function_1b13b0, 0, function_1b1a90, (t_slot_proc4)function_1b1d90,
	3, 10, 1.0f, 0
};
