#include "unknown_11c920.h"
#include "globals.h"
#include "squads.h"
#include "unknown_1a58b0.h"
#include <math.h>
#include "object_iterator.h"

// @flags /O2 /Gr /arch:SSE

struct s_scenario_identifier_ab;
void *__stdcall function_b7a40(s_scenario_identifier_ab const *identifier, long *index_out);

// @retail 0x201330
long function_201330(short placement_index)
{
	long result = NONE;
	struct
	{
		s_object *object;
		s_type_f1af8e iterator;
	} state;
	state.iterator.signature = 0x86868686;
	state.iterator.type_mask = 2;
	state.iterator.flags = 0;
	state.iterator.index = 0;
	state.iterator.object_index = NONE;
	while ((state.object = function_baeb0(&state.iterator)) != NULL)
	{
		long index;
		if (function_b7a40((s_scenario_identifier_ab const *)((byte *)state.object + 0xa4), &index) && index == placement_index)
		{
			result = state.iterator.object_index;
			break;
		}
	}
	return result;
}

extern s_flag_bits g_557c74;

struct s_squad_activity_flags
{
	byte unknown00[2];
	word unused : 7;
	word active : 1;
	word remaining : 8;
};

// @retail 0x200930
void function_200930(void)
{
	for (long i = 0; i < 5; i++)
		g_557c74.d[i] = NONE;
	g_51e9d8 = data_new_inlined("squad", 335, sizeof(s_squad_datum), 0, g_510c2c);
	g_51e9dc = data_new_inlined("squad group", 100, sizeof(s_squad_group_datum), 0, g_510c2c);
}

// @retail 0x202420
void __stdcall function_202420(long group_index)
{
	(void)&group_index;
	long const *group_reference = &group_index;
	s_squad_group_datum *group = squad_group_get(*group_reference);
	bool active = false;
	for (long child = group->first_child_index; child != NONE; )
	{
		function_202420(child);
		s_squad_group_datum *child_group = squad_group_get(child);
		active |= *(bool *)((byte *)child_group + 0x22);
		child = child_group->next_sibling_index;
	}
	for (long index = group->first_squad_index; index != NONE; )
	{
		s_squad_datum *squad = squad_get(index);
		active |= TEST_FIELD_BIT(((s_squad_activity_flags *)squad)->active);
		index = squad->next_squad_index;
	}
	if (active && !*(bool *)((byte *)group + 0x22))
		*(bool *)((byte *)squad_group_get(*group_reference) + 0x22) = true;
}

struct s_group_record_iterator
{
	s_squad_group_datum *group;
	s_record_pool_iterator records;
	long unknown10;
};

struct s_squad_group_totals
{
	byte unknown00[4];
	long first_child_index;
	long first_squad_index;
	long next_sibling_index;
	byte unknown10[0x24 - 0x10];
	short count;
	short actor_count;
	byte active;
	byte unknown29;
	short initial_count;
	short remaining_count;
	byte unknown2e[2];
	real average;
	short maximum;
	byte unknown36[2];
};

struct s_squad_count_flags
{
	byte unused : 4;
	byte active : 1;
	byte remaining : 3;
};

// @retail 0x202570
void __stdcall function_202570(long group_index)
{
	s_squad_group_totals *group = (s_squad_group_totals *)squad_group_get(group_index);
	byte active = 0;
	short remaining_count = 0;
	short count = 0;
	group->maximum = 0;
	group->actor_count = 0;
	real weighted_sum = 0.0f;
	for (long child_index = group->first_child_index; child_index != NONE; )
	{
		function_202570(child_index);
		s_squad_group_totals *child = (s_squad_group_totals *)squad_group_get(child_index);
		count += child->count;
		remaining_count += child->remaining_count;
		active |= child->active;
		if (child->maximum > group->maximum)
			group->maximum = child->maximum;
		group->actor_count += child->actor_count;
		if (child->actor_count > 0)
			weighted_sum += child->actor_count * child->average;
		child_index = child->next_sibling_index;
	}
	for (long squad_index = group->first_squad_index; squad_index != NONE; )
	{
		s_squad_datum *squad = squad_get(squad_index);
		count += squad->count_a;
		remaining_count += squad->value16;
		active |= ((s_squad_count_flags *)((byte *)squad + 2))->active;
		group->actor_count += squad->actor_count;
		if (squad->actor_count > 0)
			weighted_sum += squad->actor_count * squad->value10;
		short maximum = *(signed char *)((byte *)squad + 0x26);
		if (maximum > group->maximum)
			group->maximum = maximum;
		squad_index = squad->next_squad_index;
	}
	group->average = group->actor_count > 0 ? weighted_sum / group->actor_count : 0.0f;
	if ((group->remaining_count == NONE || group->remaining_count == 0) && remaining_count > 0)
	{
		if (group->count == NONE)
			group->initial_count = count;
		else
			group->initial_count = group->count;
	}
	group->count = count;
	group->remaining_count = remaining_count;
	group->active = active;
}

PRIVATE __forceinline s_squad_group_datum *next_group_record(s_group_record_iterator *iterator)
{
	s_squad_group_datum *result = NULL;
	if (g_4f55d0->active)
	{
		result = (s_squad_group_datum *)data_iterator_next_inlined(&iterator->records);
		iterator->group = result;
	}
	return result;
}

// @retail 0x202370
void function_202370(void)
{
	s_group_record_iterator iterator;
	if (g_4f55d0->active)
	{
		iterator.records.data = g_51e9dc;
		iterator.records.index = NONE;
	}
	s_squad_group_datum *group;
	while ((group = next_group_record(&iterator)) != NULL)
	{
		if (group->parent_index == NONE)
			function_202420(iterator.records.datum_index);
	}
}

// @retail 0x2024c0
void function_2024c0(void)
{
	s_group_record_iterator iterator;
	if (g_4f55d0->active)
	{
		iterator.records.data = g_51e9dc;
		iterator.records.index = NONE;
	}
	s_squad_group_datum *group;
	while ((group = next_group_record(&iterator)) != NULL)
	{
		if (group->parent_index == NONE)
			function_202570(iterator.records.datum_index);
	}
}

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
long function_baf80(long object_index);

struct s_actor_response_flags
{
	dword unused : 1;
	dword disabled : 1;
	dword remaining : 30;
};

// @retail 0x200240
void function_200240(long object_index, long target_index)
{
	(void)&target_index;
	byte *object = *(byte **)(g_4e0300->data + (object_index & 0xffff) * 12 + 8);
	byte *definition = g_4e3b44[*(long *)object & 0xffff].bytes;
	if (!TEST_FIELD_BIT(((s_actor_response_flags *)(definition + 0xbc))->disabled))
	{
		byte *owner = (byte *)function_badc0(*(long *)(object + 0x140), NONE);
		if (owner && !*(byte *)(owner + 0xaa))
		{
			long actor_index = *(long *)(owner + 0x12c);
			if (actor_index != NONE)
			{
				byte *actor = g_4f55f0->data + (actor_index & 0xffff) * 0x888;
				if (!*(bool *)(actor + 0x714) && *(long *)(actor + 0x718) > 0 && *(short *)(actor + 0x722) == 1)
				{
					long reference = *(long *)(actor + 0x724);
					if (reference != NONE && g_510c54->game_time - *(long *)(actor + 0x718) >= *(short *)(object + 0x1aa))
					{
						long root;
						if (!(*(real *)(object + 0x168) + 0.5f > *(real *)(actor + 0x71c)) &&
							(!function_badc0(target_index, NONE) ||
							(root = function_baf80(*(long *)(g_502418->data + (reference & 0xffff) * 0x3c + 0x20)), function_baf80(target_index) != root)))
						{
							++*(short *)(actor + 0x720);
						}
						else
						{
							*(short *)(actor + 0x720) -= 2;
							if (*(short *)(actor + 0x720) < 0)
								*(short *)(actor + 0x720) = 0;
						}
					}
				}
			}
		}
	}
}

struct s_actor_limit_view
{
	byte unknown000[0x706];
	short value706;
	byte unknown708[0x720 - 0x708];
	short value720;
	byte unknown722[0x888 - 0x722];
};

struct s_actor_channel_view
{
	byte unknown000[0x18];
	long object_index;
	byte unknown01c[0x6ce - 0x1c];
	short channel;
	byte unknown6d0[0x888 - 0x6d0];
};

void function_10e9f0(long object_index, short channel, real value, real time);

// @retail 0x20d870
void function_20d870(long actor_index, short channel, short value)
{
	(void)&value;
	s_actor_channel_view *actor = &((s_actor_channel_view *)g_4f55f0->data)[actor_index & 0xffff];
	if (channel >= 0 && channel < 13)
	{
		function_10e9f0(actor->object_index, channel, 1.0f, 0.5f);
		actor->channel = value;
	}
}

struct s_squad_definition_view
{
	byte unknown00[0x2a];
	short definition_index;
};

struct s_squad_definition_entry
{
	byte data[0x7c];
};

struct s_squad_definition_table_view
{
	byte unknown000[0x244];
	s_squad_definition_entry *entries;
};

struct s_squad_object_state
{
	byte unknown000[0x10a];
	word unknown_bits : 2;
	word blocked : 1;
	word unused_bits : 13;
	byte unknown10c[0x346 - 0x10c];
	short state_offset;
};

struct s_squad_object_header
{
	byte unknown00[8];
	s_squad_object_state *object;
};

struct s_squad_vehicle_view
{
	byte unknown000[0x3a0];
	long squad_index;
	long next;
};

struct s_squad_vehicle_header
{
	byte unknown00[8];
	s_squad_vehicle_view *object;
};

// @retail 0x201a50
void function_201a50(long squad_index, long object_index)
{
	long *link = &squad_get(squad_index)->first_vehicle_index;
	while (*link != NONE)
	{
		long current = *link;
		s_squad_vehicle_view *object = ((s_squad_vehicle_header *)g_4e0300->data)[current & 0xffff].object;
		if (current == object_index)
		{
			*link = object->next;
			((s_squad_vehicle_header *)g_4e0300->data)[object_index & 0xffff].object->squad_index = NONE;
			break;
		}
		link = &object->next;
	}
}

struct s_squad_object_extra
{
	byte unknown00[8];
	dword unknown_bits : 18;
	dword blocked : 1;
	dword unused_bits : 13;
};

PRIVATE inline void squad_actor_begin_inline(s_squad_actor_iterator *iterator, long squad_index)
{
	if (g_4f55d0->active)
	{
		iterator->squad_index = squad_index;
		iterator->actor_index = NONE;
		iterator->next_actor_index = squad_index == NONE ? g_4f55d0->unknown14 : squad_get(squad_index)->first_actor_index;
	}
}

PRIVATE inline s_actor_datum *squad_actor_next_inline(s_squad_actor_iterator *iterator)
{
	s_actor_datum *actor = NULL;
	if (g_4f55d0->active && iterator->next_actor_index != NONE)
	{
		actor = actor_datum_get(iterator->next_actor_index);
		iterator->actor_index = iterator->next_actor_index;
		iterator->next_actor_index = actor->next_actor_index;
	}
	return actor;
}

bool function_1e13f0(long actor_index);

PRIVATE inline s_actor_datum *next_actor_204390(s_squad_actor_iterator *iterator)
{
	s_actor_datum *actor = NULL;
	if (*(volatile bool *)&g_4f55d0->active && iterator->next_actor_index != NONE)
	{
		actor = actor_datum_get(iterator->next_actor_index);
		iterator->actor_index = iterator->next_actor_index;
		iterator->next_actor_index = actor->next_actor_index;
	}
	return actor;
}

// @retail 0x204390
bool function_204390(long squad_index)
{
	bool result = false;
	s_squad_actor_iterator iterator;
	squad_actor_begin_inline(&iterator, squad_index);
	while (next_actor_204390(&iterator))
	{
		if (function_1e13f0(iterator.actor_index))
		{
			result = true;
			goto done;
		}
	}
	for (long index = g_4f55d0->unknown14; index != NONE; )
	{
		s_actor_datum *actor = &((s_actor_datum *)(*(byte *volatile *)&g_4f55f0->data))[index & 0xffff];
		long source_squad = *(volatile long *)((byte *)actor + 0x34);
		if (source_squad == squad_index && function_1e13f0(index))
		{
			result = true;
			goto done;
		}
		index = actor->next_actor_index;
	}
done:
	return result;
}

// @retail 0x2003a0
long function_2003a0(long actor_index)
{
	s_actor_limit_view *actor = &((s_actor_limit_view *)g_4f55f0->data)[actor_index & 0xffff];
	return actor->value706 > g_510c54->field_2_3 || actor->value720 > 6;
}

// @retail 0x2013c0
bool function_2013c0(long object_index)
{
	bool result = true;
	s_squad_object_state *object = ((s_squad_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	if (TEST_FIELD_BIT(object->blocked) || TEST_FIELD_BIT(((s_squad_object_extra *)((byte *)object + object->state_offset))->blocked))
		result = false;
	return result;
}

struct s_squad_placement_entry
{
	byte unknown00[0x3c];
	short placement_index;
	byte unknown3e[2];
	char command_script_name[32];
	short command_script_index;
	byte unknown62[2];
};

struct s_squad_placement_definition
{
	byte unknown00[0x48];
	long count;
	s_squad_placement_entry *entries;
	char command_script_name[32];
	short command_script_index;
	byte unknown72[2];
};

struct s_squad_placement_table
{
	byte unknown000[0x164];
	s_squad_placement_definition *squads;
	byte unknown168[0x1b8 - 0x168];
	long script_count;
	byte *scripts;
};

long function_257ed0(long thread_index, long actor_index, short script_index);

PRIVATE __forceinline short squad_valid_command_script(s_squad_placement_table *scenario, short index, char const *name)
{
	if (index >= 0 && index < scenario->script_count)
	{
		unsigned long length = 0;
		while (length < 32 && *name++)
			length++;
		if (length > 0 && *(short *)(scenario->scripts + index * 0x28 + 0x20) == 5)
			return index;
	}
	return NONE;
}

// @retail 0x201b70
void function_201b70(long squad_index, short entry_index, long actor_index)
{
	(void)&actor_index;
	s_squad_placement_table *scenario = (s_squad_placement_table *)g_4e0350;
	s_squad_placement_definition *squad = &scenario->squads[squad_index & 0xffff];
	s_squad_placement_entry *entry = &squad->entries[entry_index];
	short script_index = squad_valid_command_script(scenario, entry->command_script_index, entry->command_script_name);
	if (script_index == NONE)
		script_index = squad_valid_command_script(scenario, squad->command_script_index, squad->command_script_name);
	if (script_index != NONE)
		function_257ed0(NONE, actor_index, script_index);
}

PRIVATE __forceinline s_squad_placement_entry *squad_placement_entry(s_squad_placement_definition *squad, long index)
{
	return &squad->entries[index];
}

// @retail 0x201400
bool function_201400(long squad_index, short entry_index)
{
	s_squad_placement_definition *squad = &((s_squad_placement_table *)g_4e0350)->squads[squad_index & 0xffff];
	bool result = true;
	if (entry_index >= 0 && entry_index < squad->count)
	{
		word placement_index = squad_placement_entry(squad, entry_index)->placement_index;
		if (placement_index != (word)NONE)
		{
			long object_index = function_201330((short)placement_index);
			if (object_index != NONE)
				result = function_2013c0(object_index);
			else
				result = false;
		}
	}
	return result;
}

// @retail 0x201460
void function_201460(long squad_index, dword *unavailable)
{
	(void)&unavailable;
	s_squad_placement_definition *squad = &((s_squad_placement_table *)g_4e0350)->squads[squad_index & 0xffff];
	for (short i = 0; i < squad->count; i++)
	{
		long placement_index = squad_placement_entry(squad, i)->placement_index;
		if ((short)placement_index != NONE)
		{
			long object_index = function_201330((short)placement_index);
			if (object_index == NONE || !function_2013c0(object_index))
				unavailable[i >> 5] |= 1 << (i & 0x1f);
		}
	}
}

// @retail 0x203240
bool function_203240(long squad_index)
{
	bool result = true;
	s_squad_actor_iterator iterator;
	squad_actor_begin_inline(&iterator, squad_index);
	s_actor_datum *actor;
	while (result && (actor = squad_actor_next_inline(&iterator)) != NULL)
		result &= *(bool *)&actor->unknown224[3];
	return result;
}

// @retail 0x2032b0
bool function_2032b0(long squad_index)
{
	bool result = true;
	s_squad_actor_iterator iterator;
	squad_actor_begin_inline(&iterator, squad_index);
	s_actor_datum *actor;
	while (result && (actor = squad_actor_next_inline(&iterator)) != NULL)
		result &= actor->unknown26c != NONE;
	return result;
}

// @retail 0x203330
s_squad_definition_entry *function_203330(s_squad_definition_view const *squad)
{
	if (squad->definition_index != NONE)
		return &((s_squad_definition_table_view *)g_4e0350)->entries[squad->definition_index];
	return NULL;
}

// @retail 0x203ed0
void function_203ed0(long actor_index, bool keep_count)
{
	if (g_4f55d0->active)
	{
		s_actor_datum *actor = actor_datum_get(actor_index);
		if (actor->squad_index != NONE)
		{
			s_squad_datum *squad = squad_get(actor->squad_index);
			long *link = &squad->first_actor_index;
			long current = *link;
			while (current != actor_index)
			{
				link = &actor_datum_get(current)->next_actor_index;
				current = *link;
			}
			*link = actor->next_actor_index;
			if (!keep_count)
				squad->actor_count--;
			actor->next_actor_index = NONE;
			actor->squad_index = NONE;
			((byte *)squad)[3] |= 1;
		}
	}
}

// @retail 0x203fb0
void function_203fb0(long actor_index)
{
	if (g_4f55d0->active)
	{
		s_actor_datum *actor = actor_datum_get(actor_index);
		long *link = &g_4f55d0->unknown14;
		long current = *link;
		while (current != actor_index)
		{
			link = &actor_datum_get(current)->next_actor_index;
			current = *link;
		}
		*link = actor->next_actor_index;
		actor->flag00a = false;
		actor->next_actor_index = NONE;
		actor->flag00b = false;
	}
}

void function_205280(long group_index, long squad_index);

struct s_squad_tree_group_definition
{
	byte unknown00[0x20];
	short parent;
	byte unknown22[2];
};

struct s_squad_tree_squad_definition
{
	byte unknown00[0x26];
	short parent;
	byte unknown28[0x74 - 0x28];
};

struct s_squad_tree_definition
{
	byte unknown000[0x158];
	long group_count;
	s_squad_tree_group_definition *groups;
	long squad_count;
	s_squad_tree_squad_definition *squads;
};

// @retail 0x2009d0
void function_2009d0(void)
{
	s_squad_tree_definition *definition = (s_squad_tree_definition *)g_4e0350;
	s_record_pool *groups = g_51e9dc;
	s_record_pool *squads = g_51e9d8;
	short i;
	for (i = 0; i < definition->group_count; i++)
	{
		s_squad_group_datum *group = &((s_squad_group_datum *)groups->data)[(word)i];
		group->first_child_index = NONE;
		group->first_squad_index = NONE;
		group->next_sibling_index = NONE;
		group->parent_index = definition->groups[(word)i].parent;
	}
	for (i = 0; i < definition->squad_count; i++)
	{
		s_squad_datum *squad = (&((s_squad_datum *)squads->data)[(word)i]);
		squad->next_squad_index = NONE;
		*(long *)squad->unknown6c = definition->squads[(word)i].parent;
	}
	for (i = 0; i < definition->group_count; i++)
	{
		s_squad_group_datum *group = &((s_squad_group_datum *)groups->data)[(word)i];
		if (group->parent_index != NONE)
		{
			s_squad_group_datum *parent = &((s_squad_group_datum *)groups->data)[group->parent_index & 0xffff];
			long child = parent->first_child_index;
			if (child == NONE)
				parent->first_child_index = i;
			else
			{
				s_squad_group_datum *last;
				do
				{
					last = &((s_squad_group_datum *)groups->data)[child & 0xffff];
					child = last->next_sibling_index;
				} while (child != NONE);
				last->next_sibling_index = i;
			}
		}
	}
	for (i = 0; i < definition->squad_count; i++)
	{
		long parent = *(long *)(&((s_squad_datum *)squads->data)[(word)i])->unknown6c;
		if (parent != NONE)
			function_205280(parent, i);
	}
}

// @retail 0x205280
void function_205280(long group_index, long squad_index)
{
	s_squad_group_datum *group = squad_group_get(group_index);
	long index = group->first_squad_index;
	if (index == NONE)
	{
		group->first_squad_index = squad_index;
	}
	else
	{
		s_squad_datum *squad;
		do
		{
			squad = squad_get(index);
			index = squad->next_squad_index;
		} while (index != NONE);
		squad->next_squad_index = (short)squad_index;
	}
}

// @retail 0x2052d0
bool function_2052d0(long squad_index, long group_index)
{
	bool result = false;
	long index = *(long *)squad_get(squad_index)->unknown6c;
	while (index != NONE)
	{
		if (index == group_index)
		{
			result = true;
			break;
		}
		index = squad_group_get(index)->parent_index;
	}
	return result;
}

struct s_squad_record_iterator
{
	s_squad_datum *squad;
	s_record_pool_iterator records;
	long squad_index;
};

PRIVATE __forceinline s_squad_datum *next_squad_record(s_squad_record_iterator *iterator)
{
	s_squad_datum *result = NULL;
	if (g_4f55d0->active)
	{
		s_record_pool *records = iterator->records.data;
		long index = data_next_absolute_index_inlined(records, iterator->records.index + 1);
		if (index != NONE)
		{
			result = (s_squad_datum *)(records->data + records->size * index);
			iterator->records.index = index;
		}
		iterator->squad = result;
	}
	return result;
}

// @retail 0x205320
void __stdcall function_205320(long group_index)
{
	long const *group_reference = &group_index;
	s_squad_record_iterator iterator;
	if (g_4f55d0->active)
	{
		iterator.records.data = g_51e9d8;
		iterator.records.index = NONE;
	}
	s_squad_datum *squad;
	while ((squad = next_squad_record(&iterator)) != NULL)
	{
		if (*(long *)((byte *)squad + 0x80) == *group_reference)
			*(long *)((byte *)squad + 0x80) = NONE;
	}
}

// @retail 0x201ad0
void function_201ad0(long squad_index, long object_index)
{
	long const *object_reference = &object_index;
	s_squad_vehicle_view *object = ((s_squad_vehicle_header *)g_4e0300->data)[*object_reference & 0xffff].object;
	if (object->squad_index != NONE)
		function_201a50(object->squad_index, *object_reference);
	if (squad_index != NONE)
	{
		s_squad_datum *squad = (s_squad_datum *)(g_51e9d8->data + (squad_index & 0xffff) * sizeof(s_squad_datum));
		((s_squad_vehicle_header *)g_4e0300->data)[*object_reference & 0xffff].object->next = squad->first_vehicle_index;
		squad->first_vehicle_index = *object_reference;
		if (!(squad_index & 0xffff0000))
			object->squad_index = (*(short *)squad << 16) | squad_index;
		else
			object->squad_index = squad_index;
	}
}

struct s_actor_position_state
{
	s_actor_position_state();
	volatile long flags;
	point3f position;
	short field10;
	byte unknown12[2];
	real field14;
	real field18;
	long field1c;
	short field20;
	short field22;
	short field24;
	byte unknown26[2];
	short field28;
	short field2a;
	short field2c;
	short field2e;
	long field30;
	long field34;
	real field38;
	short field3c;
	short field3e;
	byte field40;
	byte unknown41[0x60 - 0x41];
	short field60;
};

PRIVATE __forceinline void reset_actor_position(s_actor_position_state *state)
{
	state->flags = 0;
	state->position = *g_468788;
}

// @retail 0x200e60
s_actor_position_state::s_actor_position_state() : flags(0)
{
	s_actor_position_state *state = this;
	reset_actor_position(state);
	state->field10 = NONE;
	state->field14 = 0.0f;
	state->field18 = 0.0f;
	state->field1c = 0;
	state->field20 = NONE;
	state->field22 = NONE;
	state->field24 = NONE;
	state->field28 = NONE;
	state->field2a = NONE;
	state->field2c = NONE;
	state->field2e = 0;
	state->field30 = 0;
	state->field34 = 0;
	state->field38 = 0.0f;
	state->field3e = 0;
	state->field3c = NONE;
	state->field60 = NONE;
	state->field40 = 0;
}

// @retail 0x203900
bool function_203900(short squad_index, real probability)
{
	(void)&probability;
	s_squad_datum *squad = squad_get((word)squad_index);
	real *global_balance = (real *)((byte *)g_4f55d0 + 0x1c);
	real delta = *global_balance * (-1.0f / 3.0f);
	real correction = -*(real *)squad->unknown04;
	real adjustment = fabs(delta) > fabs(correction) ? delta : correction;
	bool result = function_x82e52f(&g_4e7408->unknown0, NULL, 0) < adjustment + probability;
	real change = (real)result - probability;
	*(real *)squad->unknown04 += change;
	*global_balance += change;
	return result;
}

PRIVATE __forceinline s_actor_datum *next_active_squad_actor(s_squad_actor_iterator *iterator)
{
	s_actor_datum *result = NULL;
	while (g_4f55d0->active && iterator->next_actor_index != NONE)
	{
		long current = iterator->next_actor_index;
		s_actor_datum *actor = actor_datum_get(current);
		iterator->actor_index = current;
		iterator->next_actor_index = actor->next_actor_index;
		if (*(bool *)((byte *)actor + 9))
		{
			result = actor_datum_get(current);
			break;
		}
	}
	return result;
}

// @retail 0x203cc0
void function_203cc0(long squad_index)
{
	s_squad_activity_flags *squad = (s_squad_activity_flags *)squad_get(squad_index);
	squad->active = false;
	s_squad_actor_iterator iterator;
	squad_actor_begin_inline(&iterator, squad_index);
	s_actor_datum *actor;
	while ((actor = next_active_squad_actor(&iterator)) != NULL)
	{
		if (*(bool *)((byte *)actor + 9))
		{
			*(bool *)((byte *)actor + 9) = false;
			*(long *)((byte *)actor + 0x10) = g_510c54->game_time;
			(*(short *)((byte *)g_4f55d0 + 0x36a))--;
		}
	}
}

struct s_squad_difficulty_values
{
	long unknown00;
	real values_a[4];
	real values_b[4];
	real values_c[4];
};

long function_1e49d0(long index);
bool g_4f55df;

// @retail 0x2036c0
void function_2036c0(long definition_index, short mode, bool *enabled, bool *forced, real *value)
{
	(void)&mode;
	(void)&forced;
	s_squad_difficulty_values *data = (s_squad_difficulty_values *)function_1e49d0(definition_index);
	if (data)
	{
		short difficulty = g_4e6948->state == 1 ? g_4e6948->difficulty : 1;
		switch (mode)
		{
		case 4:
			if (data->values_c[difficulty] > 0.0f)
			{
				*enabled = false;
				*forced = true;
			}
			else
			{
				*enabled = true;
				*value = 0.0f;
			}
			break;
		case 1:
			*enabled = true;
			*value = data->values_a[difficulty];
			break;
		case 2:
			*enabled = true;
			*value = data->values_c[difficulty];
			break;
		case 3:
			*enabled = false;
			*forced = false;
			break;
		default:
			*enabled = true;
			*value = data->values_b[difficulty];
			break;
		}
	}
	else
	{
		*enabled = false;
		*forced = false;
	}
	if (g_4e6948->state == 1 && g_4f55df)
	{
		*enabled = false;
		*forced = true;
	}
}
