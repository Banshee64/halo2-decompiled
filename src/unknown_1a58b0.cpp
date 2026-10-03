// @flags /O2 /arch:SSE /Gr
#include <math.h>
#include "cseries.h"
#include "globals.h"
#include "unknown_1a58b0.h"

/* ---- views of the data used below ---- */

struct s_object_view
{
	long definition_index;
	dword flags4;
	byte unknown08[0x2b0 - 8];
	real value2b0;
};

struct s_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	s_object_view *object;
};

struct s_player_view
{
	byte unknown00[0x2c];
	long unit_index;
	byte unknown30[0x21c - 0x30];
};

struct s_unit_view
{
	byte unknown00[0x14];
	long unknown14;
	byte unknown18[0x1fc - 0x18];
	short unknown1fc;
};

struct s_definition_view
{
	byte unknown00[0x86];
	byte unknown86;
	byte unknown87;
	long tag_index88;
	byte unknown8c[0xd4 - 0x8c];
};

struct s_sort_element
{
	long identifier;
	byte unknown04[0x24];
	byte unknown28;
	byte unknown29[0x1b];
	real value44;
	real value48;
	real value4c;
	real value50;
	real value54;
	byte flags58;
};

#define OBJECT_HEADER(index) (&((s_object_header *)g_4e0300->data)[(index) & 0xffff])


/* ---- callees ---- */

void function_1c2330(long bit_count, dword *a, dword *b, dword *result);

/* ---- globals ---- */

s_candidate_table g_4ee4f0[3];
byte g_4f09c8[0x84];
dword g_4f0a4c[5];
s_sort_globals *g_51e99c;
s_flag_bits g_557c74;

/* ---- helpers ---- */

#define ACTION_ID_VALID(id) ((id) >= 0 && (id) < 0x83)

/* whether the handler is enabled in this build */
#define ACTION_HANDLER_ENABLED(id) \
	(g_46eeb8[id]->unknown8 != g_46f348 && (g_46eeb8[id]->mask & g_4ee4ec) == g_4ee4ec && (g_557c40[(id) >> 5] & (1 << ((id) & 0x1f))))

/* link a node in after an element */
#define ACTION_NODE_INSERT(element, node) \
	do \
	{ \
		(node)->next = (element)->next; \
		(element)->next = (node); \
		(node)->order = 0; \
	} \
	while (0)

/* link a node into the list of an element, after the nodes of a lower order */
#define ACTION_NODE_INSERT_SORTED(element, node, order_value) \
	do \
	{ \
		s_action_node *previous = (element); \
		\
		if (!previous->next) \
		{ \
			(node)->next = 0; \
			previous->next = (node); \
		} \
		else \
		{ \
			do \
			{ \
				if (!previous->next || previous->next->order >= (order_value)) \
				{ \
					(node)->next = previous->next; \
					previous->next = (node); \
					break; \
				} \
				previous = previous->next; \
			} \
			while (previous); \
		} \
		(node)->order = (order_value); \
	} \
	while (0)
/* ---- functions ---- */

// @retail 0x1a58b0
bool function_1a58b0(long object_index)
{
	s_object_header *header = (s_object_header *)(g_4e0300->data + (object_index & 0xffff) * sizeof(s_object_header));
	s_object_view *object = header->object;

	if (((1 << header->type) & 0x83) && !(header->flags & 0x10) && !(object->flags4 & 1))
	{
		if (!((1 << header->type) & 3) || 1.0f > object->value2b0)
			return true;
	}
	return false;
}
// @retail 0x1a6340
void function_1a6340(real_vector3d const *a, real_vector3d const *b, real_point3d *out, real_point3d const *c, real_point3d const *p, real radius)
{
	real ni = a->k * b->j - b->k * a->j;
	real nj = b->k * a->i - b->i * a->k;
	real nk = b->i * a->j - a->i * b->j;
	real m = nj * nj + nk * nk + ni * ni;

	if (m > 0.0f)
	{
		real dz = c->z - p->z;
		real dy = c->y - p->y;
		real dx = c->x - p->x;
		real ci = a->k * dy - dz * a->j;
		real cj = a->i * dz - a->k * dx;
		real ck = dx * a->j - dy * a->i;
		real t = (cj * nj + ck * nk + ci * ni) / m;

		if (t < 0.0f)
			t = 0.0f;
		else if (t > 1.0f)
			t = 1.0f;
		out->x = b->i * t + p->x;
		out->y = t * b->j + p->y;
		out->z = b->k * t + p->z;
	}
	else
	{
		*out = *p;
	}

	real vx = out->x - c->x;
	real vz = out->z - c->z;
	real vy = out->y - c->y;
	real dot = a->k * vz + a->i * vx + vy * a->j;
	real s = 0.0f - dot;
	real_vector3d w;

	w.i = a->i * s + vx;
	w.j = s * a->j + vy;
	w.k = a->k * s + vz;
	real wm = w.k * w.k + w.j * w.j + w.i * w.i;

	if (wm > radius * radius)
	{
		real scale = radius / (real)sqrt(wm);

		w.i *= scale;
		w.j *= scale;
		w.k *= scale;
	}
	out->x = out->x - w.i;
	out->y = out->y - w.j;
	out->z = out->z - w.k;
}

// @retail 0x1a65b0
void function_1a65b0(real_vector3d const *a, real_point3d *out, real_point3d const *p, real_point3d const *c, real radius)
{
	*out = *p;

	real vx = out->x - c->x;
	real vy = out->y - c->y;
	real vz = out->z - c->z;
	real dot = a->j * vy + a->i * vx + a->k * vz;
	real s = 0.0f - dot;
	real_vector3d w;

	w.i = a->i * s + vx;
	w.j = a->j * s + vy;
	w.k = a->k * s + vz;
	real wm = w.k * w.k + w.j * w.j + w.i * w.i;

	if (wm > radius * radius)
	{
		real scale = radius / (real)sqrt(wm);

		w.i *= scale;
		w.j *= scale;
		w.k *= scale;
	}
	out->x = out->x - w.i;
	out->y = out->y - w.j;
	out->z = out->z - w.k;
}
// @retail 0x1a66f0
int __cdecl function_1a66f0(s_sort_element const *a, s_sort_element const *b)
{
	real score_a = a->value4c;
	real score_b = b->value4c;

	if (g_51e99c)
	{
		dword flags = g_51e99c->flags;

		if (flags & 0x20)
		{
			if (!(a->flags58 & 1))
				score_a -= 0.5f;
			if (!(b->flags58 & 1))
				score_b -= 0.5f;
		}
		if (flags & 0x10)
		{
			if (!(a->flags58 & 2))
				score_a -= 0.5f;
			if (!(b->flags58 & 2))
				score_b -= 0.5f;
		}
	}

	if (score_a > score_b)
		return -1;
	if (score_b > score_a)
		return 1;
	if (a->value50 > b->value50)
		return -1;
	if (b->value50 > a->value50)
		return 1;
	if (a->value54 > b->value54)
		return -1;
	if (b->value54 > a->value54)
		return 1;
	if (b->value44 > a->value44)
		return -1;
	if (a->value44 > b->value44)
		return 1;
	if (b->value48 > a->value48)
		return -1;
	if (a->value48 > b->value48)
		return 1;
	if (!a->unknown28 && b->unknown28)
		return -1;
	if (a->unknown28 && !b->unknown28)
		return 1;
	return (a->identifier & 0xffff) - (b->identifier & 0xffff);
}

// @retail 0x1a6d30
bool __fastcall pin_aiming_for_player(long player_index)
{
	long unit_index = ((s_player_view *)g_4e8c24->data)[player_index & 0xffff].unit_index;
	bool result = true;

	if (unit_index != NONE)
	{
		s_unit_view *unit = (s_unit_view *)OBJECT_HEADER(unit_index)->object;

		if (unit->unknown14 == NONE || unit->unknown1fc == NONE)
			result = false;
	}
	return result;
}
// @retail 0x1a6ea0
short function_1a6ea0(s_action_node **out, s_candidate_list *list, s_action_node *nodes, short node_count)
{
	short current = g_46fbe4;
	short i = 0;
	short out_count = 0;
	short j = 0;
	short count = 0;

	if (list)
		count = list->count;

	for (;;)
	{
		s_candidate_entry *entry = 0;

		if (i < count)
		{
			entry = &list->entries[i];
			switch (entry->kind)
			{
			case 0:
				break;
			case 1:
				entry = 0;
				break;
			case 2:
				if (nodes[j].key == entry->key)
					j++;
				else
					entry = 0;
				break;
			case 3:
				if (current != entry->key)
					entry = 0;
				break;
			case 4:
				if (j < node_count)
					entry = 0;
				break;
			default:
				entry = 0;
				break;
			}
		}

		if (entry)
		{
			out[out_count] = &entry->node;
			i++;
		}
		else
		{
			if (j >= node_count)
				return out_count;
			out[out_count] = &nodes[j];
			current = nodes[j].key;
			j++;
		}
		out_count++;
		if (out_count >= 0x32)
			return out_count;
	}
}

// @retail 0x1a6f70
void function_1a6f70(long owner_index)
{
	s_actor_view *owner = actor_get(owner_index);
	long stamp = ++g_46f34c;
	short i = 0;

	if (owner->current >= 0)
	{
		do
		{
			short type = owner->slots[i].type;

			if (ACTION_ID_VALID(type))
				g_46eeb8[type]->unknownc = stamp;
			i++;
		}
		while (i <= owner->current);
	}
}

/* a slot and the one after it, seen from the owner base (the slots start at +0x90) */
struct s_slot_pair_view
{
	byte unknown00[0x94];
	short unknown94;
	byte unknown96[0x3a];
	s_slot next;
};

// @retail 0x1a7030
short function_1a7030(long owner_index, short slot_index, long argument, short *out)
{
	s_actor_view *owner = actor_get(owner_index);
	s_slot_pair_view *slot = (s_slot_pair_view *)((byte *)owner + slot_index * sizeof(s_slot));
	s_slot *next = &slot->next;
	short result = g_46fbe4;
	short selected = NONE;
	short type = next->type;

	if (next->state == 0 && ACTION_ID_VALID(type) && slot->unknown94 != NONE && ACTION_HANDLER_ENABLED(type))
	{
		s_slot_handler *handler = g_46eeb8[type];
		short code;

		if (handler->evaluate_argument)
			code = handler->evaluate_argument(owner_index, next, argument);
		else
			code = g_46fbe8;

		if (code == g_46fbe8)
		{
			selected = next->type;
			result = code;
		}
		else
		{
			next->state = 1;
			if (ACTION_ID_VALID(code))
			{
				if (ACTION_HANDLER_ENABLED(code))
				{
					selected = code;
					result = code;
				}
				else
				{
					slot->unknown94 = NONE;
					result = g_46fbe4;
				}
			}
			else
			{
				slot->unknown94 = NONE;
				if (code == g_46fbec)
					result = code;
				else
					result = g_46fbe4;
			}
		}
	}
	else
	{
		slot->unknown94 = NONE;
	}

	*out = selected;
	return result;
}
// @retail 0x1a71f0
bool function_1a71f0(short count, short id, s_action_node **list)
{
	if (!(g_4f0a4c[id >> 5] & (1 << (id & 0x1f))))
		return false;

	short i;

	for (i = 0; i < count; i++)
		list[i]->next = 0;

	long row;

	for (row = 0; row < g_4f09c8[id]; row++)
	{
		s_candidate_entry *entry = &g_4ee4f0[row].entries[id];
		s_action_node *node = &entry->node;

		if (entry->kind == 0)
		{
			ACTION_NODE_INSERT(list[0], node);
		}
		else if (entry->kind == 4)
		{
			ACTION_NODE_INSERT_SORTED(list[count - 1], node, 2);
		}
		else
		{
			short j = 0;

			while (j < count)
			{
				if (list[j]->key == entry->key)
					break;
				j++;
			}
			if (j < count)
			{
				s_action_node *element = list[j];

				if (entry->kind == 1)
					ACTION_NODE_INSERT(element, node);
				else if (entry->kind == 2)
					ACTION_NODE_INSERT_SORTED(element, node, 1);
				else if (entry->kind == 3)
					ACTION_NODE_INSERT_SORTED(element, node, 2);
			}
		}
	}
	return true;
}

/* where each handler's nodes start in g_4f0a60 and how many there are, by
   handler and actor type (filled by 0x1a6d80) */
struct s_action_table_entry
{
	short index;
	char count;
	byte unknown03;
};

s_action_table_entry g_4f2cc0[k_slot_type_count][0x14];
s_action_node *g_4f0a60[0x800];

// @retail 0x1a73c0
s_action_node **function_1a73c0(long owner_index, short id, bool *valid, short *count)
{
	s_actor_view *owner = actor_get(owner_index);
	s_action_table_entry *entry = &g_4f2cc0[id][owner->unknown004];
	s_action_node **nodes = &g_4f0a60[entry->index];
	short node_count = entry->count;

	*valid = function_1a71f0(node_count, g_46eeb8[id]->index, nodes);
	*count = node_count;
	return nodes;
}
struct s_action_request
{
	short id;
	byte unknown02[2];
	short timer;
	byte unknown06[2];
	real seconds;
};

struct s_action_context
{
	byte unknown00[8];
	long time;
};

// @retail 0x1a7430
void function_1a7430(long owner_index, s_action_request *request, short *out_id, short *out_state, s_action_context *context, long argument)
{
	short id = request->id;

	*out_id = NONE;
	*out_state = 0;
	if (!ACTION_ID_VALID(id))
		return;

	s_slot_handler *handler = g_46eeb8[id];

	if (g_46eeb8[id]->unknown8 == g_46f348 || (g_4ee4ec & handler->mask) != g_4ee4ec || !(g_557c40[id >> 5] & (1 << (id & 0x1f))))
		return;

	s_actor_view *owner = actor_get(owner_index);
	short timer = request->timer;
	s_game_time_globals *time = g_510c54;

	if (timer != NONE)
	{
		if (timer == -2 && context->time == time->game_time)
		{
		}
		else if (timer >= 0 && timer < 14)
		{
			long last = owner->times[timer];

			if (last != NONE && !((real)(time->game_time - last) * g_510c54->rate > request->seconds))
				return;
		}
		else
		{
			return;
		}
	}

	if (handler->kind == 0)
	{
		short result = ((s_slot_handler_0 *)handler)->query(owner_index, argument);

		if (ACTION_ID_VALID(result) && ACTION_HANDLER_ENABLED(result))
		{
			*out_id = result;
			*out_state = 3;
		}
	}
	else
	{
		*out_id = id;
		if (handler->priority)
			*out_state = handler->priority(owner_index);
		else
			*out_state = 0;
	}

	timer = request->timer;
	if (timer >= 0 && timer < 14)
		owner->times[timer] = g_510c54->game_time;
}

// @retail 0x1a7ad0
dword function_1a7ad0(long owner_index)
{
	s_actor_view *owner = actor_get(owner_index);
	long shift = 0;

	if (owner->unknown267)
		shift = 2;
	else if (owner->unknown266)
		shift = 1;
	return (1 << owner->unknown086_byte) | ((1 << (byte)shift) << 10);
}

// @retail 0x1a7b30
void function_1a7b30(s_flag_bits *result, long owner_index)
{
	s_actor_view *owner = actor_get(owner_index);
	s_tag_instance *tags = g_4e3b44;
	s_flag_bits *source;
	long index = owner->unknown858;

	if (index != NONE && ((s_definition_view *)(g_502408->data + (index & 0xffff) * sizeof(s_definition_view)))->unknown86)
	{
		s_definition_view *definition = (s_definition_view *)(g_502408->data + (index & 0xffff) * sizeof(s_definition_view));

		source = (s_flag_bits *)(tags[definition->tag_index88 & 0xffff].bytes + 0x38);
		*result = *source;
	}
	else if (owner->unknown030 != NONE)
	{
		source = (s_flag_bits *)(g_51e9d8->data + (owner->unknown030 & 0xffff) * 0x98 + 0x84);
		*result = *source;
	}
	else
	{
		*result = g_557c74;
	}

	if (owner->unknown26c != NONE && !owner->unknown269)
	{
		s_object_view *object = OBJECT_HEADER(owner->unknown26c)->object;
		s_tag_element *element = function_1e5450(owner_index, object->definition_index);

		if (element && element->tag_index != NONE)
		{
			function_1c2330(0x83, result->d, (dword *)(tags[element->tag_index & 0xffff].bytes + 0x38), result->d);
			return;
		}
	}

	long other = *(long *)(tags[owner->unknown054 & 0xffff].bytes + 0x20);

	if (other != NONE)
	{
		s_flag_bits *mask = (s_flag_bits *)(tags[other & 0xffff].bytes + 0x38);
		long k;

		for (k = 0; k < 5; k++)
			result->d[k] &= mask->d[k];
	}
}