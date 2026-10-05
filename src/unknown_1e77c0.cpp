// @flags /O2 /Gr
/* UNKNOWN_1E77C0.CPP: recording a unit request in its player's local state
   (the e6900 callee) */

#include "unknown_11c920.h"
#include "globals.h"

/* a player (g_4e8c24, 0x21c bytes): +0x28 is its local player slot */
struct s_player_request_view
{
	byte unknown00[0x28];
	short local_player_index;
	byte unknown2a[2];
	long unit_index;
	byte unknown30[0x21c - 0x30];
};

struct s_time_entry
{
	long time;
	short a;
	short b;
};

/* a local player's state in g_51e9c0 (src/unknown_1e6a40.cpp), 0x1b0 bytes:
   the four last unit requests at +0x150 */
struct s_local_player_state_view
{
	byte unknown000[0x150];
	s_time_entry requests[4];
	byte unknown170[0x194 - 0x170];
	byte field_194;
	byte field_195;
	byte field_196;
	byte field_197;
	byte unknown198[3];
	byte field_19b;
	byte unknown19c[0x1b0 - 0x19c];
};

struct s_unknown_1e6a40;
extern s_unknown_1e6a40 *g_51e9c0;

void function_1e6980(s_time_entry *entries, short a, byte b);

// @retail 0x1e8f60
void function_1e8f60(long player_index)
{
	s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long index = player->local_player_index;
	if (index != NONE)
		((s_local_player_state_view *)g_51e9c0)[index].field_195 = (byte)g_510c54->field_2_3;
}

// @retail 0x1e9070
void function_1e9070(long player_index)
{
	s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long index = player->local_player_index;
	if (index != NONE)
		((s_local_player_state_view *)g_51e9c0)[index].field_19b = (byte)g_510c54->field_2_3;
}

PRIVATE inline long next_request_player(long current)
{
	long result = NONE;
	long index = current;
	if (index == NONE)
		index = 0;
	else
		index++;
	for (; index < 4; index++)
	{
		if (g_4e8c20->entries[index] != NONE)
		{
			result = index;
			break;
		}
	}
	return result;
}

PRIVATE inline void set_request_flag(long local_index, byte flag)
{
	long player_index = local_index == NONE ? NONE : g_4e8c20->entries[local_index];
	if (player_index != NONE)
	{
		s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
		long index = player->local_player_index;
		if (index != NONE)
		{
			s_local_player_state_view *state = &((s_local_player_state_view *)g_51e9c0)[index];
			byte flags = state->field_194;
			state->field_194 = flags | flag;
		}
	}
}

// @retail 0x1e7800
void function_1e7800(void)
{
	for (long index = next_request_player(NONE); index != NONE; index = next_request_player(index))
		set_request_flag(index, 1);
}

// @retail 0x1e78b0
void function_1e78b0(void)
{
	for (long index = next_request_player(NONE); index != NONE; index = next_request_player(index))
		set_request_flag(index, 2);
}

// @retail 0x1e7960
void function_1e7960(void)
{
	for (long index = next_request_player(NONE); index != NONE; index = next_request_player(index))
		set_request_flag(index, 4);
}

struct s_entry_420
{
	long value;
	word field_04;
	word limit;
	word field_08;
	short duration;
	byte field_0c[8];
};

struct s_table_420
{
	byte field_00[0x420];
	long count;
	s_entry_420 *entries;
};

inline s_entry_420 *entry_420_get(long index)
{
	s_entry_420 *result = NULL;
	s_table_420 *table = (s_table_420 *)g_510c94;
	if (table && index >= 0 && index < table->count)
		result = &table->entries[index];
	return result;
}

// @retail 0x1e8cb0
long function_1e8cb0(long index)
{
	long result = 0;
	s_entry_420 *entry = entry_420_get(index);
	if (entry)
		result = entry->value;
	return result;
}

// @retail 0x1e8d50
s_entry_420 *function_1e8d50(long index)
{
	return entry_420_get(index);
}

// @retail 0x1e77c0
void function_1e77c0(long player_index, long type, byte result)
{
	s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);

	if (player->local_player_index != NONE)
		function_1e6980(((s_local_player_state_view *)g_51e9c0)[player->local_player_index].requests, (short)type, result);
}

// @retail 0x1e69d0
bool function_1e69d0(s_time_entry *entries, long type, long age_limit)
{
	long now = g_510c54->game_time;
	bool result = false;
	long i = 0;
	do
	{
		if ((word)entries[i].a == type && now - entries[i].time < age_limit)
		{
			result = true;
			break;
		}
		i++;
	} while (i < 4);
	return result;
}

struct s_flag_pair
{
	dword low;
	dword high;
};

// @retail 0x1e6a00
void function_1e6a00(long value, s_flag_pair *flags, long index)
{
	bool high = (bool)(((dword)value >> 1) & 1);
	if (value & 1)
		flags->low |= 1 << index;
	else
		flags->low &= ~(1 << index);
	if (high)
		flags->high |= 1 << index;
	else
		flags->high &= ~(1 << index);
}

// @retail 0x1e7a10
bool function_1e7a10(long player, long index)
{
	bool result = false;
	if (player != NONE)
	{
		s_flag_pair *flags = (s_flag_pair *)((byte *)g_51e9c0 + player * 0x1b0 + 0x170);
		long value = ((flags->high & (1 << index)) ? 2 : 0) + ((flags->low & (1 << index)) ? 1 : 0);
		result = value == 3;
	}
	return result;
}

// @retail 0x1e7a60
long function_1e7a60(long player, long index)
{
	long result = 0;
	if (player != NONE)
	{
		s_flag_pair *flags = (s_flag_pair *)((byte *)g_51e9c0 + player * 0x1b0 + 0x170);
		result = ((flags->high & (1 << index)) ? 2 : 0) + ((flags->low & (1 << index)) ? 1 : 0);
	}
	return result;
}

struct s_request_source
{
	byte field_00[0x1c];
	dword flags;
	byte field_20;
	byte field_21[7];
	short type;
	byte value;
	byte field_2b;
	long id;
	byte field_30[0x2c];
};

struct s_request_state
{
	word type;
	word field_02;
	long id;
	short count;
	short field_0a;
	short value;
};

// @retail 0x1e8ce0
void function_1e8ce0(s_request_state *state, s_request_source *source)
{
	short type = source->type;
	long stored_type = state->type;
	if (type != stored_type || source->id != state->id)
	{
		state->type = type;
		state->id = source->id;
		state->value = source->value;
		state->count = 0;
		state->field_0a = 0;
	}
	if (state->type == 1 || state->type == 3 || state->type == 4 || state->type == 5 || state->type == 6)
	{
		if (source->flags & 8)
			state->count++;
	}
}

struct s_request_object
{
	byte field_000[0x1fc];
	short field_1fc;
	byte field_1fe[0x216 - 0x1fe];
	byte field_216;
};

struct s_request_object_header
{
	byte field_00[8];
	s_request_object *object;
};

// @retail 0x1e6b00
void function_1e6b00(long player_index, s_request_source *source)
{
	s_player_request_view *player = (s_player_request_view *)(g_4e8c24->data + (player_index & 0xffff) * 0x21c);
	long index = player->local_player_index;
	if (index != NONE)
	{
		s_local_player_state_view *state = &((s_local_player_state_view *)g_51e9c0)[index];
		s_request_source *current = (s_request_source *)((byte *)state + 0x88);
		*(s_request_source *)((byte *)state + 0xe4) = *current;
		*current = *source;
		if (player->unit_index != NONE)
		{
			s_request_object *unit = ((s_request_object_header *)g_4e0300->data)[player->unit_index & 0xffff].object;
			if (unit->field_1fc == NONE && source->field_20 != 0xff && source->field_20 != unit->field_216)
			{
				state->field_197++;
				state->field_196 = (byte)(g_510c54->field_2_3 * 2);
			}
		}
		function_1e8ce0((s_request_state *)((byte *)state + 0x140), source);
	}
}

struct s_request_transition
{
	short state;
	short ticks;
};

// @retail 0x1e8d80
bool __stdcall function_1e8d80(long index, s_local_player_state_view *state, s_entry_420 *entry, s_request_transition *output)
{
	s_flag_pair *flags = (s_flag_pair *)((byte *)state + 0x170);
	long value = ((flags->high & (1 << index)) ? 2 : 0) + ((flags->low & (1 << index)) ? 1 : 0);
	if (index == 21)
		function_1e8d80(22, state, entry_420_get(22), (s_request_transition *)((byte *)state + 0x58));
	if (!entry)
	{
		output->state = 4;
		output->ticks = g_510c54->field_2_3 * 5;
		return false;
	}
	if (entry->limit != 0 && entry->limit <= 3)
	{
		if (value + 1 >= entry->limit)
		{
			output->state = 5;
			output->ticks = 0;
			function_1e6a00(3, flags, index);
			return true;
		}
		output->state = 4;
		output->ticks = g_510c54->field_2_3 * entry->duration;
		function_1e6a00(value + 1, flags, index);
		return false;
	}
	output->state = 4;
	output->ticks = g_510c54->field_2_3 * entry->duration;
	return false;
}
