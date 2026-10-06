// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_265CB0.CPP: the props an actor knows (0x265cb0..) */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "props.h"
#include <math.h>
#include "unknown_26b230.h"

real __stdcall function_265d30(long actor_index, long prop_index);

void *function_1e51a0(long actor_index);

struct s_distance_rates_view
{
	byte unknown00[8];
	real near_distance;
	real far_distance;
	real maximum_distance;
	real far_rate;
};

// @retail 0x263d70
void function_263d70(long actor_index, real distance, real rate, real scale, real *secondary, real *primary)
{
	s_distance_rates_view *settings = (s_distance_rates_view *)function_1e51a0(actor_index);
	real *const *primary_reference = &primary;
	real *const *secondary_reference = &secondary;
	real a = 0.0f;
	real b = 0.0f;
	if (settings)
	{
		if (distance > settings->maximum_distance)
		{
			a = 0.0f;
			b = 0.0f;
		}
		else
		{
			real near_rate = rate * scale;
			real far_rate = settings->far_rate * scale;
			real near_secondary = near_rate * 0.7f;
			real far_secondary = far_rate * 0.7f;
			if (far_secondary > 3.5f)
				far_secondary = 3.5f;
			if (distance > settings->far_distance)
			{
				a = far_rate;
				b = far_secondary;
			}
			else
			{
				real near_distance = settings->near_distance;
				real secondary_distance = near_distance * 0.8f;
				if (distance < near_distance)
					a = near_rate;
				else
				{
					real range = settings->far_distance - near_distance;
					if (range <= 0.0f)
						a = far_rate;
					else
					{
						real fraction = (distance - near_distance) / range;
						a = (1.0f - fraction) * near_rate + fraction * far_rate;
					}
				}
				if (distance < secondary_distance)
					b = near_secondary;
				else
				{
					real range = settings->far_distance - secondary_distance;
					if (range <= 0.0f)
						b = far_secondary;
					else
					{
						real fraction = (distance - secondary_distance) / range;
						b = (1.0f - fraction) * near_secondary + fraction * far_secondary;
					}
				}
			}
		}
	}
	**primary_reference = a;
	**secondary_reference = b;
}

PRIVATE inline real basis_dot3f(vector3f const *a, vector3f const *b)
{
	return a->i * b->i + a->j * b->j + a->k * b->k;
}

// @retail 0x263740
void function_263740(long actor_index, vector3f const *direction, vector3f const *axis_a,
	vector3f const *axis_b, vector3f const *axis_c, real rate, real scale, real *secondary, real *primary)
{
	vector3f components;
	components.i = basis_dot3f(axis_a, direction);
	components.j = basis_dot3f(axis_b, direction);
	components.k = basis_dot3f(axis_c, direction);
	real vertical = (real)atan2(components.k, sqrt(components.i * components.i + components.j * components.j));
	if (vertical > 0.5235987901687622f || vertical < -0.7853981852531433f)
	{
		*secondary = 0.0f;
		*primary = 0.0f;
	}
	else
		function_263d70(actor_index, (real)fabs(atan2(components.j, components.i)), rate, scale, secondary, primary);
}

short const g_44ae3c[13] = { 1, 1, 2, 3, 4, 6, 7, 8, 8, 9, 9, 9, 9 };
short g_470fa4[4] = { 0, 0, 2, 3 };
short g_470fe0[3] = { 1, 2, 3 };

void *function_272a00(long actor_index);
bool function_1a8220(long index, short a, short b, long unknown, short c, short d, short e);

// @retail 0x2662f0
void function_2662f0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	short desired;
	long task_index = actor->unknown858;
	if (task_index != NONE)
	{
		byte *task = g_502408->data + (task_index & 0xffff) * 0xd4;
		short level = *(short *)(task + 0x84);
		if (level > 0)
		{
			desired = level - 1;
			goto reset_time;
		}
	}
	desired = g_44ae3c[actor->unknown328];
	{
		short group_level = 1;
		short event_level = 0;
		if (actor->unknown07c != NONE)
		{
			s_clump *group = (s_clump *)(g_502420->data + (actor->unknown07c & 0xffff) * 0x50);
			group_level = g_470fa4[group->state];
		}
		if (actor->unknown26c == NONE && desired == 1 && actor->unknown086 == 0)
			desired = 0;
		if (actor->unknown358 > 0)
			event_level = 2;
		short maximum = group_level > event_level ? group_level : event_level;
		desired = desired > maximum ? desired : (group_level > event_level ? group_level : event_level);
	}
	if (desired >= 3)
		goto reset_time;
	if (actor->unknown086 > 3)
	{
		actor->unknown086 = 3;
		*(long *)actor->unknown088 = 0;
	}
	{
		byte *definition = NULL;
		if (actor->unknown030 != NONE)
		{
			byte *squad = g_51e9d8->data + (actor->unknown030 & 0xffff) * 0x98;
			short definition_index = *(short *)(squad + 0x2a);
			if (definition_index != NONE)
				definition = *(byte **)((byte *)g_4e0350 + 0x244) + definition_index * 0x7c;
		}
		if (definition && desired < *(short *)(definition + 0x28) - 1)
			desired = *(short *)(definition + 0x28) - 1;
		else
		{
			byte *variant = (byte *)function_272a00(actor_index);
			if (variant)
			{
				short level = g_470fe0[*(short *)(variant + 0x20)];
				if (actor->unknown086 >= level && desired < level)
					desired = level;
			}
		}
	}
	if (desired >= actor->unknown086)
		goto reset_time;
	desired = actor->unknown086;
	switch (desired)
	{
	case 3:
		if (*(short *)actor->unknown088 * g_510c54->rate < 0.0f)
			break;
		desired = 2;
	case 2:
		if (*(short *)actor->unknown088 * g_510c54->rate >= 15.0f)
			desired = 1;
		break;
	}
	goto update;
reset_time:
	*(long *)actor->unknown088 = 0;
update:
	if (actor->unknown086 != desired)
	{
		if (desired >= 3 && actor->unknown086 < 2)
			function_1a8220(actor_index, 7, 2, 3, 1, 42, 3);
		*(long *)actor->unknown088 = 0;
		actor->unknown086 = desired;
	}
	else
		++*(long *)actor->unknown088;
	if (actor->unknown086 >= 7)
		*(bool *)((byte *)actor + 0x8c) = true;
}

// @retail 0x265cb0
void function_265cb0(long actor_index)
{
	long prop_index = actor_get(actor_index)->first_prop_index;

	for (;;)
	{
		s_prop_node_view *node;
		long current_index;
		s_prop_view_fields *view;

		if (prop_index == NONE)
		{
			break;
		}
		node = prop_node_get(prop_index);
		current_index = prop_index;
		prop_index = node->next_index;
		view = prop_node_view(node);
		if (view)
		{
			view->unknown3c = function_265d30(actor_index, current_index);
		}
	}
}

// @retail 0x263d30
long function_263d30(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	long result;
	if (actor->unknown086 >= 3)
		result = 2;
	else
		result = actor->unknown084 >= 4;
	return result;
}

PRIVATE __forceinline s_type_f95cd3 *actor_tracked_prop_view(long prop_index)
{
	s_prop_node_view *node = prop_node_get(prop_index);
	s_type_f95cd3 *result = NULL;
	if (node->view_index != NONE)
	{
		byte *base = g_502414->data + (node->view_index & 0xffff) * sizeof(s_type_e5ff81);
		if (base)
			result = (s_type_f95cd3 *)(base + 0x70);
	}
	return result;
}

// @retail 0x267180
short function_267180(s_prop_datum *node)
{
	s_type_f95cd3 *view = NULL;
	if (node->tracking_index != NONE)
	{
		s_type_e5ff81 *tracking = tracking_get(node->tracking_index);
		if (tracking)
			view = &tracking->view;
	}
	long result;
	short state = node->state;
	if (!state)
		result = 0;
	else if (node->type != 1 && node->type != 6)
		result = 0;
	else if (function_25d690(node)->unknown5e)
		result = 1;
	else if (view->unknown2a)
		result = 12;
	else if (state >= 1 && state <= 2)
	{
		result = 6;
		if (node->unknown27 >= 1)
		{
			if (node->unknown28 < 1.0f)
				result = 11;
			else
			{
				if (node->unknown28 < 12.0f)
					result = 7;
				char level = view->unknown39;
				if (level <= 2)
				{
					result = 8;
					if (function_25d690(node)->unknown63)
					{
						result = 9;
						if (level <= 1)
							result = 10;
					}
				}
			}
		}
	}
	else if (state >= 3)
	{
		if (view->unknown69)
			result = 2;
		else if (view->unknown88)
			result = 3;
		else if ((g_510c54->game_time - view->unknown10) * g_510c54->rate < 1.0f && *(short *)view >= 5)
			result = 5;
		else
			result = 4;
	}
	if (view)
		*(short *)view = (short)result;
	return (short)result;
}

struct s_actor_prop_iterator
{
	long index;
	long next;
};

PRIVATE __forceinline s_prop_node_view *actor_next_prop(s_actor_prop_iterator *iterator)
{
	s_prop_node_view *node = NULL;
	long index = iterator->next;
	if (index != NONE)
	{
		node = prop_node_get(index);
		iterator->index = index;
		iterator->next = node->next_index;
	}
	return node;
}

// @retail 0x265bb0
void function_265bb0(long actor_index)
{
	s_actor_prop_iterator iterator;
	iterator.next = actor_get(actor_index)->first_prop_index;
	s_prop_node_view *node;
	while ((node = actor_next_prop(&iterator)) != NULL)
	{
		s_type_f95cd3 *view = actor_tracked_prop_view(iterator.index);
		if (view)
		{
			view->unknown2a = false;
			*(short *)((byte *)view + 0x28) = NONE;
		}
	}
}

struct s_prop_thresholds
{
	byte unknown000[0x158];
	real actor_threshold;
	real object_threshold;
};

struct s_prop_threshold_table
{
	byte unknown000[0xc8];
	long count;
	s_prop_thresholds *entries;
};

long function_1e4990(long index);

long function_1e1f20(long actor_index);
long function_cbd50(long unit_index, short weapon_index);

struct s_object_activity_flags
{
	byte unknown00[0x10a];
	byte flag0 : 1;
	byte flag1 : 1;
	byte blocked : 1;
	byte unused : 5;
};

// @retail 0x267370
real function_267370(long object_index)
{
	long const *object_reference = &object_index;
	real result = 0.0f;
	if (*object_reference != NONE)
	{
		s_slot_object_view *object = object_get(*object_reference);
		s_actor_view *actor = NULL;
		byte *settings = NULL;
		if (((s_prop_threshold_table *)g_4e034c)->count > 0)
			settings = (byte *)((s_prop_threshold_table *)g_4e034c)->entries;
		if (!((bool)((s_object_activity_flags *)object)->blocked) && ((1 << object->type) & 3))
		{
			long actor_index = object->actor_index;
			if (actor_index != NONE)
				actor = actor_get(actor_index);
			long first;
			if (actor)
			{
				result = *(real *)((byte *)function_1e4990(actor->unknown054) + 8);
				first = function_1e1f20(actor_index);
			}
			else
			{
				first = function_cbd50(*object_reference, *(char *)((byte *)object + 0x212));
				if (object->player_index != NONE && settings)
					result = *(real *)(settings + 0x160);
			}
			long second = function_cbd50(*object_reference, *(char *)((byte *)object + 0x213));
			if (object->parent_index != NONE)
			{
				s_slot_object_view *parent = object_get(object->parent_index);
				if (parent->type == 1 && object->unknown1fc != NONE)
				{
					byte *tag = g_4e3b44[parent->tag_index & 0xffff].bytes;
					byte *seats = *(byte **)(tag + 0x1cc);
					result = *(real *)(seats + object->unknown1fc * 0xb0 + 0x38) + result;
				}
			}
			if (first != NONE)
				result = *(real *)(g_4e3b44[object_get(first)->tag_index & 0xffff].bytes + 0x238) + result;
			if (second != NONE)
				result = *(real *)(g_4e3b44[object_get(second)->tag_index & 0xffff].bytes + 0x238) + result;
			if (actor && actor->unknown225 && settings)
				result = *(real *)(settings + 0x164) + result;
		}
	}
	return result;
}

// @retail 0x267700
void function_267700(long actor_index, s_prop_datum *node, s_type_76cf92 *prop, s_type_f95cd3 *view)
{
	s_prop_datum *const *node_reference = &node;
	s_actor_view *actor = actor_get(actor_index);
	real value;
	if (prop->actor_index != NONE)
		value = *(real *)((byte *)actor_get(prop->actor_index) + 0x2cc);
	else
		value = function_267370((*node_reference)->object_index);
	value -= *(real *)((byte *)actor + 0x2cc);
	if (value < 0.0f)
		view->unknown60 = 0.0f;
	else
		view->unknown60 = value;
}

// @retail 0x267a80
short function_267a80(real *distance, point3f const *point, vector3f const *direction, point3f const *position, long unknown)
{
	long result = 0;
	vector3f *offset = (vector3f *)unknown;
	if (distance)
		*distance = 0.0f;
	if (offset)
		*offset = *g_4687a4;
	point2f planar = *(point2f const *)direction;
	real magnitude = (real)sqrt(planar.x * planar.x + planar.y * planar.y);
	if (fabs(magnitude) < 0.0001f)
		magnitude = 0.0f;
	else
	{
		real scale = 1.0f / magnitude;
		planar.x *= scale;
		planar.y = ((point2f volatile *)&planar)->y * scale;
	}
	if (magnitude > 0.0f)
	{
		vector3f delta;
		delta.i = position->x - point->x;
		delta.j = position->y - point->y;
		delta.k = position->z - point->z;
		real length = (real)sqrt(delta.i * delta.i + delta.j * delta.j);
		if (distance)
			*distance = length;
		real projection = delta.i * planar.x + delta.j * planar.y;
		if (projection > length * 0.8660253882408142f)
		{
			real scale = 0.0f - projection;
			delta.i = direction->i * scale + delta.i;
			delta.j = direction->j * scale + delta.j;
			delta.k = direction->k * scale + delta.k;
			if (offset)
			{
				offset->i = 0.0f - delta.i;
				offset->j = 0.0f - delta.j;
				offset->k = 0.0f - delta.k;
			}
			if (delta.k > -0.5f && delta.k < 0.9f)
				result = 2;
			else if (delta.k > -0.8f && delta.k < 1.2f)
				result = 1;
			else
				goto done;
			real squared = delta.i * delta.i + delta.j * delta.j;
			if (squared < 0.36000001430511475f)
				goto done;
			if (squared < 1.2100000381469727f)
			{
				result = 1;
				goto done;
			}
		}
	}
	result = 0;
done:
	return (short)result;
}

s_ai_player *ai_player_get(long player_index);
void function_cb7e0(long unit_index, vector3f *vector);
bool function_1e1e50(long actor_index, vector3f *direction);
void function_26be00(long actor_index, s_iterator *iterator);
real function_30bf0(vector3f *vector);

// @retail 0x267840
bool function_267840(long actor_index, long prop_index, vector3f *direction)
{
	s_actor_view *actor = actor_get(actor_index);
	s_prop_datum *node = prop_ref_get(prop_index);
	s_type_76cf92 *prop = prop_get(node->prop_index);
	s_type_5cfb45 *state = function_25d690(node);
	volatile bool result = false;
	if (!prop->unknown22)
	{
		if (prop->unknown25)
		{
			long player_index = object_get(*(long *)((byte *)prop + 8))->player_index;
			s_ai_player *player;
			if (player_index != NONE && (player = ai_player_get(player_index)) != NULL)
			{
				if (state->unknown63)
					result = true;
				else
				{
					real ticks = (real)g_510c54->field_2_3 * 2.5f;
					long rounded;
					__asm { fld ticks }
					__asm { fistp rounded }
					result = g_510c54->game_time - player->unknown0c < rounded;
				}
				function_cb7e0(node->object_index, direction);
				if (!result && *(char *)((byte *)actor + 0x324) > 0)
				{
					s_iterator iterator;
					function_26be00(actor_index, &iterator);
					long index = iterator.next;
					while (index != NONE)
					{
						s_prop_datum *other = prop_ref_get(index);
						index = other->next_index;
						if (other->state >= 1 && prop_get(other->prop_index)->unknown23)
						{
							s_type_5cfb45 *other_state = function_25d690(other);
							vector3f delta;
							delta.i = other_state->position.x - state->position.x;
							delta.j = other_state->position.y - state->position.y;
							delta.k = other_state->position.z - state->position.z;
							if (function_30bf0(&delta) > 0.0f &&
								direction->i * delta.i + direction->j * delta.j + direction->k * delta.k > 0.5f)
							{
								result = true;
								break;
							}
						}
					}
				}
			}
		}
		else if (prop->actor_index != NONE)
			result = function_1e1e50(prop->actor_index, direction);
	}
	return result;
}

// @retail 0x267550
bool function_267550(long actor_index)
{
	byte *definition = (byte *)function_1e4990(actor_get(actor_index)->unknown054);
	bool result = false;
	if (definition && ((s_prop_threshold_table *)g_4e034c)->count > 0)
		result = *(real *)(definition + 8) >= ((s_prop_threshold_table *)g_4e034c)->entries->actor_threshold;
	return result;
}

struct s_equipped_object_view
{
	long tag_index;
	byte unknown004[0x212 - 4];
	char selected;
	byte unknown213[5];
	long objects[4];
};

/* The held-object lookup rereads the pool's data field. */
// @retail 0x2675f0
bool function_2675f0(long object_index)
{
	s_equipped_object_view *object = (s_equipped_object_view *)((s_object_header_view *)((s_record_pool volatile *)g_4e0300)->data)[object_index & 0xffff].object;
	long held_index = NONE;
	short selected = object->selected;
	if (selected != NONE)
		held_index = object->objects[selected];
	bool result = false;
	if (held_index != NONE && ((s_prop_threshold_table *)g_4e034c)->count > 0)
	{
		s_prop_thresholds *thresholds = ((s_prop_threshold_table *)g_4e034c)->entries;
		s_equipped_object_view *held = (s_equipped_object_view *)((s_object_header_view *)((s_record_pool volatile *)g_4e0300)->data)[held_index & 0xffff].object;
		byte *definition = g_4e3b44[held->tag_index & 0xffff].bytes;
		result = thresholds->object_threshold <= *(real *)(definition + 0x238);
	}
	return result;
}

// @retail 0x2684f0
short function_2684f0(s_actor_view *actor)
{
	if (actor->prop_index != NONE)
	{
		s_prop_view_fields *view = prop_view_fields_get(actor->prop_index);
		if (view)
			return view->unknown00;
	}
	return 0;
}

// @retail 0x2675b0
bool function_2675b0(long node_index)
{
	s_prop_node_view *node = prop_node_get(node_index);
	s_type_76cf92 *prop = prop_get(node->unknown08);
	bool result = false;
	if (prop->actor_index != NONE)
		result = function_267550(prop->actor_index);
	return result;
}

/* These wrappers read the handle after resolving the complete record address. */
// @retail 0x267680
bool function_267680(long node_index)
{
	s_prop_node_view volatile *node = prop_node_get(node_index);
	long object_index = node->object_index;
	long type = ((s_object_header_view *)g_4e0300->data)[object_index & 0xffff].type;
	bool result = false;
	if ((1 << type) & 3)
		result = function_2675f0(object_index);
	return result;
}

// @retail 0x2676d0
bool function_2676d0(long actor_index)
{
	s_actor_view volatile *actor = actor_get(actor_index);
	long object_index = actor->unknown018;
	bool result = false;
	if (object_index != NONE)
		result = function_2675f0(object_index);
	return result;
}

// @retail 0x2672e0
void function_2672e0(s_type_f95cd3 *view, s_prop_node_view const *node)
{
	s_prop_node_view const *const *node_reference = &node;
	if (view->unknown54 > 1.0f)
		view->unknown54 = 1.0f;
	real value = 1.0f;
	if (!view->unknown2a)
	{
		if ((*node_reference)->unknown27 < 2 || view->unknown39 == 4)
			value = 0.0f;
		else if (view->unknown39 == 3)
			value = 0.3f;
		else if (view->unknown39 == 2)
			value = 0.6f;
		else
			value = 0.8f;
	}
	if (!(value >= view->unknown54))
		value = value * (1.0f - 0.995f) + view->unknown54 * 0.995f;
	view->unknown54 = value;
}


PRIVATE inline s_type_f95cd3 *countdown_prop_view(s_prop_datum *node)
{
	s_type_f95cd3 *result = NULL;
	if (node->tracking_index != NONE)
	{
		s_type_e5ff81 *tracking = tracking_get(node->tracking_index);
		if (tracking)
			result = &tracking->view;
	}
	return result;
}

// @retail 0x263670
void function_263670(s_prop_datum *node)
{
	short *countdown = (short *)node->unknown1e;
	if (*countdown > 0)
	{
		--*countdown;
		if (*countdown == 0)
			node->unknown1c = NONE;
	}
	s_type_5cfb45 *state = function_25d690(node);
	s_type_f95cd3 *view = countdown_prop_view(node);
	if (state->unknown5e)
		++*(short *)((byte *)state + 0x5c);
	else
		*(short *)((byte *)state + 0x5c) = 0;
	if (view)
	{
		short *age = (short *)((byte *)view + 0x28);
		if (*age != NONE)
		{
			++*age;
			if ((real)*age * g_510c54->rate >= 1.5f)
			{
				view->unknown2a = false;
				*age = NONE;
			}
		}
		short *remaining = (short *)view->unknowna0;
		if (*remaining > 0)
			--*remaining;
		short *duration = (short *)view->unknown08;
		if (node->unknown27 >= 1)
		{
			if (*duration < 0x7fff)
				++*duration;
		}
		else
			*duration = 0;
	}
}


struct s_prop_object_links_view
{
	byte unknown00[0x10a];
	word unknown10a_0 : 2;
	word has_links : 1;
	word unknown10a_3 : 13;
	byte unknown10c[0x120 - 0x10c];
	short size;
	short offset;
};

struct s_prop_object_link
{
	byte unknown00[4];
	word packed_index;
	byte unknown06[2];
};

/* The optional output pointer stays on the stack until the scan is complete. */
// @retail 0x2651e0
bool function_2651e0(long object_index, short *volatile output_index)
{
	s_prop_object_links_view *object = (s_prop_object_links_view *)((s_object_header_view *)g_4e0300->data)[object_index & 0xffff].object;
	bool result = false;
	short index = NONE;
	if (TEST_FIELD_BIT(object->has_links))
	{
		long count = (dword)(long)object->size >> 3;
		s_prop_object_link *links = (s_prop_object_link *)((byte *)object + object->offset);
		index = 0x7fff;
		for (short i = 0; i < count; i++)
		{
			dword packed = links[i].packed_index;
			if ((packed & 0xfff8) > 0)
			{
				word candidate = links[i].packed_index;
				if ((word)(candidate >> 3) < (word)index)
				{
					index = candidate >> 3;
					result = true;
				}
				break;
			}
		}
	}
	short *output = output_index;
	if (output)
	{
		if (result)
			*output = index;
		else
			*output = NONE;
	}
	return result;
}


// @retail 0x264260
long function_264260(vector3f const *facing, vector3f const *direction, real distance)
{
	real cosine = 0.0f - (direction->k * facing->k + direction->j * facing->j + direction->i * facing->i);
	real lateral;
	if (cosine <= 0.0f)
		lateral = 3.402823466e+38F;
	else if (cosine >= 1.0f)
		lateral = 0.0f;
	else
		lateral = (real)(sqrt(1.0f - cosine * cosine) * distance);

	if (cosine > 0.9925f || lateral < 0.5f)
		return 0;
	if (cosine > 0.9063f || lateral < 1.5f)
		return 1;
	if (cosine > 0.5f)
		return 2;
	if (cosine > 0.0f)
		return 3;
	return 4;
}


void *function_1e51a0(long actor_index);

short const g_44ae58[4][4] =
{
	{0, 0, 1, 3},
	{0, 1, 2, 3},
	{0, 2, 3, 4},
	{0, 3, 4, 4}
};

struct s_prop_rate_definition
{
	byte unknown00[0x24];
	real durations[3];
};

// @retail 0x2683f0
real function_2683f0(long actor_index, long node_index, short type)
{
	real result = 0.0f;
	s_prop_rate_definition *definition = (s_prop_rate_definition *)function_1e51a0(actor_index);
	if (definition)
	{
		s_prop_datum *node = prop_ref_get(node_index);
		switch (node->type)
		{
		case 2:
			result = 1.0f;
			break;
		case 3:
			result = 1.0f;
			break;
		case 4:
			result = 1.0f;
			break;
		case 8:
			result = 1.0f;
			break;
		case 1:
			if (function_25d690(node)->unknown3c != NONE)
			{
				result = 1.0f;
				break;
			}
		case 5:
		case 6:
		case 7:
			switch (g_44ae58[(short)function_263d30(actor_index)][type])
			{
			case 0:
				break;
			case 1:
				result = definition->durations[0] > 0.0f ? g_510c54->rate / definition->durations[0] : 1.0f;
				break;
			case 2:
				result = definition->durations[1] > 0.0f ? g_510c54->rate / definition->durations[1] : 1.0f;
				break;
			case 3:
				result = definition->durations[2] > 0.0f ? g_510c54->rate / definition->durations[2] : 1.0f;
				break;
			case 4:
				result = 1.0f;
				break;
			default:
				__assume(0);
			}
			break;
		}
	}
	return result;
}
