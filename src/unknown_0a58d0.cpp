// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0A58D0.CPP: the simulation entity database: an entity's object */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0a58d0.h"
#include "unknown_xa19f52.h"

/* the entity an index stands for, or none when its salt is stale; retail
   has no copy of its own (LTCG inlines it everywhere), and inlining it from
   here keeps the null test after the identifier comparison that retail has */
s_simulation_entity *simulation_entity_try_get(s_simulation_entity_table *table, long entity_index)
{
	s_simulation_entity *result = 0;
	long absolute_index = entity_index & 0x3ff;
	if (table->entities[absolute_index].identifier == entity_index)
		result = table->entities + absolute_index;
	return result;
}

/* 0xa58d0, kept out of the build: retail takes the index in ecx (the
   __fastcall the stub in src/stubs/unknown_09a9f0.cpp has), our LTCG passes it
   in eax or edx (it depends on the body), which breaks the matched caller
   0xa3a20. With simulation_entity_try_get as it is now, the body is otherwise
   the same as retail's (the null test after the identifier comparison
   included). */
#if 0
long function_a58d0(long entity_index)
{
	long object_index = NONE;
	if (entity_index != NONE)
	{
		s_simulation_world_view *world = (s_simulation_world_view *)g_4cf77c;
		s_simulation_entity *entity = simulation_entity_try_get(&world->database->table, entity_index);
		if (entity)
		{
			s_entity_definitions *definitions = (s_entity_definitions *)g_4cf784;
			if (definitions->definitions[entity->type]->v7((s_entity *)entity) && entity->object_index != NONE)
				object_index = entity->object_index;
		}
	}
	return object_index;
}
#endif

struct s_z_entity_flags
{
	byte flags;
	byte unknown01[7];
};

struct s_z_entity_flags_table
{
	byte unknown00[0x44];
	s_z_entity_flags entries[1];
};

struct s_z_entity_flags_database
{
	byte unknown0000[0x20a4];
	s_z_entity_flags_table *flags;
};

struct s_z_entity_flags_world
{
	byte unknown00[4];
	s_z_entity_flags_database *database;
};

// @retail 0xa5930
long function_a5930(long index)
{
	long result = NONE;
	if (index != NONE)
	{
		long identifier = ((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d4;
		if (identifier != NONE)
		{
			s_z_entity_flags_world *world = (s_z_entity_flags_world *)g_4cf77c;
			if (world->database->flags->entries[identifier & 0x3ff].flags & 4)
				result = identifier;
		}
	}
	return result;
}

// @retail 0xa5980
long function_a5980(long index)
{
	long result = NONE;
	if (index != NONE)
	{
		long identifier = ((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d4;
		if (identifier != NONE)
		{
			s_z_entity_flags_world *world = (s_z_entity_flags_world *)g_4cf77c;
			if (!(world->database->flags->entries[identifier & 0x3ff].flags & 4))
				result = identifier;
		}
	}
	return result;
}

// @retail 0xa7670
bool function_a7670(long index)
{
	bool result = false;
	if (g_4e6948->mode == 4)
		result = ((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d4 != NONE;
	return result;
}

void function_b58c0(long index, dword mask);

// @retail 0xa7a30
void function_a7a30(long index, dword mask)
{
	long identifier = ((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d4;
	if (identifier != NONE)
		function_b58c0(identifier, mask);
}


struct s_z_entity_record
{
	long identifier;
	short type;
	bool active;
	byte unknown07;
	long object_index;
	byte unknown0c[0xc];
	long size;
	s_entity_data *data;
};

struct s_z_state_database
{
	byte unknown0000[0x20ac];
	s_z_entity_record entities[0x400];
};

struct s_z_state_world
{
	byte unknown00[4];
	s_z_state_database *database;
	long state;
};

// @retail 0xa9820
bool function_a9820(long *block, long index)
{
	bool result = false;
	s_z_state_world *world = (s_z_state_world *)g_4cf77c;
	if (world->state == 4 || world->state == 5)
	{
		long identifier = ((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d4;
		if (identifier != NONE)
		{
			s_z_entity_record *entity = &world->database->entities[identifier & 0x3ff];
			if (entity->active)
			{
				long type = entity->type;
				s_entity_definitions *definitions = (s_entity_definitions *)g_4cf784;
				if (definitions->definitions[type]->v34(entity->size, entity->data, block))
					result = true;
			}
		}
	}
	return result;
}

// @retail 0xa99f0
void function_a99f0(long index, long *block)
{
	s_z_state_world *world = (s_z_state_world *)g_4cf77c;
	if (world->state == 4 || world->state == 5)
	{
		long identifier = ((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d4;
		if (identifier != NONE)
		{
			s_z_entity_record *entity = &world->database->entities[identifier & 0x3ff];
			if (entity->active)
			{
				long type = entity->type;
				s_entity_definitions *definitions = (s_entity_definitions *)g_4cf784;
				definitions->definitions[type]->v35(entity->size, entity->data, block);
			}
		}
	}
}


#include <string.h>

struct s_z_copy_state
{
	dword flags;
	vector3f position;
	vector3f forward;
	vector3f up;
};

PRIVATE inline void z_copy_state_clear(s_z_copy_state *state)
{
	state->flags = 0;
	memset(&state->position, 0, sizeof(*state) - sizeof(state->flags));
}

// @retail 0xa9640
void function_a9640(long index)
{
	s_object_view *object = ((s_object_header *)g_4e0300->data)[index & 0xffff].object;
	if (object->field_d8 & 1)
	{
		s_z_copy_state state;
		z_copy_state_clear(&state);
		function_a9820((long *)&state, index);
		object->field_d8 &= ~1;
		state.flags = 0;
		function_a99f0(index, (long *)&state);
	}
}

// @retail 0xa96d0
void function_a96d0(long index, vector3f const *position)
{
	s_z_copy_state state;
	z_copy_state_clear(&state);
	if (function_a9820((long *)&state, index))
	{
		byte *flags = &((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d8;
		*flags |= 1;
		state.flags |= 1;
		state.position = *position;
		function_a99f0(index, (long *)&state);
	}
}

// @retail 0xa9770
void function_a9770(long index, vector3f const *up, vector3f const *forward)
{
	s_z_copy_state state;
	z_copy_state_clear(&state);
	if (function_a9820((long *)&state, index))
	{
		byte *flags = &((s_object_header *)g_4e0300->data)[index & 0xffff].object->field_d8;
		*flags |= 1;
		state.flags |= 2;
		state.up = *up;
		state.forward = *forward;
		function_a99f0(index, (long *)&state);
	}
}

// @retail 0xa98b0
bool function_a98b0(long index, vector3f *position)
{
	s_object_view *object = ((s_object_header *)g_4e0300->data)[index & 0xffff].object;
	bool result = false;
	if (object->field14 == NONE && (object->field_d8 & 1))
	{
		s_z_copy_state state;
		z_copy_state_clear(&state);
		if (function_a9820((long *)&state, index) && (state.flags & 1))
		{
			*position = state.position;
			result = true;
		}
	}
	return result;
}

// @retail 0xa9940
bool function_a9940(long index, vector3f *up, vector3f *forward)
{
	s_object_view *object = ((s_object_header *)g_4e0300->data)[index & 0xffff].object;
	bool result = false;
	if (object->field14 == NONE && (object->field_d8 & 1))
	{
		s_z_copy_state state;
		z_copy_state_clear(&state);
		if (function_a9820((long *)&state, index) && (state.flags & 2))
		{
			*up = state.up;
			*forward = state.forward;
			result = true;
		}
	}
	return result;
}


struct s_z_unit_mapping_view
{
	byte unknown000[0xd4];
	long identifier;
	byte unknown0d8[0x130 - 0xd8];
	long mapping_index;
	byte unknown134[8];
	long player_index;
};

struct s_z_unit_mapping_entry
{
	long first;
	long second;
	long flags;
	byte unknown0c[0x90 - 0xc];
};

struct s_z_unit_mapping_world
{
	byte unknown000[0x8fc];
	s_z_unit_mapping_entry entries[1];
};

void __stdcall function_cbf60(long unit_index, bool active);
void function_14cad0(long player_index, long unit_index);
void function_152340(void);
long function_101f50(long object_index);

// @retail 0xa7bc0
void function_a7bc0(long index)
{
	s_z_unit_mapping_view *object = (s_z_unit_mapping_view *)((s_object_header *)g_4e0300->data)[index & 0xffff].object;
	if (object->mapping_index != NONE)
	{
		s_z_unit_mapping_world *world = (s_z_unit_mapping_world *)g_4cf77c;
		s_z_unit_mapping_entry *entry = &world->entries[object->mapping_index];
		entry->first = NONE;
		entry->second = NONE;
		entry->flags = 0;
		object->mapping_index = NONE;
		function_cbf60(index, false);
		object = (s_z_unit_mapping_view *)((s_object_header *)g_4e0300->data)[index & 0xffff].object;
		if (object->identifier != NONE)
			function_b58c0(object->identifier, 0x400);
	}
}

// @retail 0xa94b0
void function_a94b0(long index)
{
	s_z_unit_mapping_view *object = (s_z_unit_mapping_view *)((s_object_header *)g_4e0300->data)[index & 0xffff].object;
	long player_index = object->player_index;
	byte *player = g_4e8c24->data + (player_index & 0xffff) * 0x21c;
	*(long *)(player + 0x30) = *(long *)(player + 0x2c);
	function_14cad0(player_index, NONE);
	function_152340();
}

// @retail 0xa7cd0
void function_a7cd0(long index)
{
	long mode = g_4e6948->mode;
	if (mode >= 4 && mode <= 5)
	{
		switch (mode)
		{
		case 2:
		case 4:
			break;
		default:
		{
		byte *object = (byte *)((s_object_header *)g_4e0300->data)[index & 0xffff].object;
		long parent = NONE;
		if (object[0x12c] & 1)
			parent = *(long *)(object + 0x154);
		long slot = function_101f50(index);
		if (parent != NONE && slot >= 0 && slot < 4)
			function_a7a30(parent, 1 << (slot + 18));
			break;
		}
		}
	}
}


// @retail 0xa9a70
void function_a9a70(long index, s_z_copy_state *state)
{
	byte *object = (byte *)((s_object_header *)g_4e0300->data)[index & 0xffff].object;
	vector3f const *position = (vector3f const *)(object + 0x64);
	vector3f *saved = &state->position;
	vector3f difference;
	vector3d_from_points3d((point3f const *)saved, (point3f const *)position, &difference);
	if (sqrt(difference.i * difference.i + (difference.j * difference.j + difference.k * difference.k)) > 0.05000000074505806f)
	{
		saved->i = saved->i * 0.550000011920929f + position->i * 0.44999998807907104f;
		saved->j = saved->j * 0.550000011920929f + position->j * 0.44999998807907104f;
		saved->k = saved->k * 0.550000011920929f + position->k * 0.44999998807907104f;
	}
	else
	{
		*saved = *position;
		state->flags &= ~1;
	}
}


// @retail 0xa7700
bool function_a7700(long index, long which, long *player_out)
{
	long player = NONE;
	long mode = g_4e6948->mode;
	if (mode >= 4 && mode <= 5)
	{
		byte *object = (byte *)((s_object_header *)g_4e0300->data)[index & 0xffff].object;
		if ((1 << object[0xaa]) & 3)
		{
			player = *(long *)(object + 0x13c);
			switch (which)
			{
			case 0:
				if (player == NONE && *(long *)(object + 0x248) != NONE)
				{
					byte *parent = (byte *)((s_object_header *)g_4e0300->data)[*(long *)(object + 0x248) & 0xffff].object;
					player = *(long *)(parent + 0x13c);
				}
				break;
			case 1:
				if (player == NONE && *(long *)(object + 0x24c) != NONE)
				{
					byte *parent = (byte *)((s_object_header *)g_4e0300->data)[*(long *)(object + 0x24c) & 0xffff].object;
					player = *(long *)(parent + 0x13c);
				}
				break;
			default:
				break;
			}
		}
	}
	if (player_out)
		*player_out = player;
	return player != NONE;
}


// @retail 0xa76b0
bool function_a76b0(long index, long which)
{
	bool result = false;
	if (g_4e6948->mode == 4 && function_a7700(index, which, &index))
	{
		byte *player = g_4e8c24->data + (index & 0xffff) * 0x21c;
		result = *(short *)(player + 0x28) != NONE;
	}
	return result;
}


bool function_e4050(long object_index);
bool function_cc410(long unit_index);

// @retail 0xaa970
bool function_aa970(long index)
{
 s_object_header *header = &((s_object_header *)g_4e0300->data)[index & 0xffff];
 bool result = false;
 if (!header->unknown03[0])
 {
  s_object_view *object = header->object;
  if ((TEST_FIELD_BIT(object->flag2) && !function_e4050(index)) || function_cc410(index) ||
   (bool)((*(dword *)((byte *)object + 0x134) >> 27) & 1))
   result = true;
 }
 return result;
}

#include "unknown_0d0690.h"

// @retail 0xaa9e0
void function_aa9e0(long index, point3f const *previous_position, vector3f const *velocity)
{
 s_record_pool *objects = g_4e0300;
 s_object_header *header = &((s_object_header *)objects->data)[index & 0xffff];
 if (header->unknown03[0] == 1)
 {
  byte *object = (byte *)header->object;
  real elapsed = g_510c54->rate;
  if (previous_position)
  {
   real x = *(real *)(object + 0x64) - previous_position->x;
   real y = *(real *)(object + 0x68) - previous_position->y;
   real z = *(real *)(object + 0x6c) - previous_position->z;
   *(real *)(object + 0x270) += x;
   *(real *)(object + 0x274) += y;
   *(real *)(object + 0x278) += z;
  }
  vector3f difference;
  if (velocity)
  {
   difference.i = (velocity->i - *(real *)(object + 0x88)) * elapsed;
   difference.j = (velocity->j - *(real *)(object + 0x8c)) * elapsed;
   difference.k = (velocity->k - *(real *)(object + 0x90)) * elapsed;
  }
  s_object_child_iterator iterator;
  function_d0620(index, &iterator);
  while (function_d0690(&iterator))
  {
   byte *child = (byte *)((s_object_header *)objects->data)[iterator.child_index & 0xffff].object;
   if (velocity)
   {
    *(real *)(child + 0x270) += difference.i;
    *(real *)(child + 0x274) += difference.j;
    *(real *)(child + 0x278) += difference.k;
   }
   *(dword *)(child + 0x134) |= 0x10000000;
  }
 }
}
