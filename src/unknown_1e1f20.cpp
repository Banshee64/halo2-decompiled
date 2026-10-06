// @flags /O2 /Gr
/* UNKNOWN_1E1F20.CPP: the actor's weapon (lane M; called by the behaviors of
   0x1a8000..0x1affff) */

#include "unknown_11c920.h"
#include "ai_actor.h"
#include "unknown_2551c0.h"
#include "object_markers.h"
#include "object_queries.h"
#include "unknown_19ec40.h"
#include "unknown_1e46c0.h"
#include <string.h>

struct s_ai_weapon_definition
{
	byte unknown000[0x12f];
	byte flags12f_0 : 1;
};

static inline long unit_get_current_weapon(long unit_index)
{
	s_ai_object *unit = ai_object_get(unit_index);
	short slot = unit->current_weapon;
	long result = NONE;

	if (slot != NONE)
		result = unit->weapons[slot];
	return result;
}

// @retail 0x1e1f20
long function_1e1f20(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	long result = NONE;

	if (actor->unknown268 && actor->unknown274 != NONE)
		result = unit_get_current_weapon(actor->unknown274);
	if (result == NONE && actor->unknown018 != NONE)
		result = unit_get_current_weapon(actor->unknown018);
	return result;
}

PRIVATE inline long secondary_weapon_get(long unit_index)
{
	s_ai_object *unit = ai_object_get(unit_index);
	short slot = *((signed char *)unit + 0x213);
	long result = NONE;
	if (slot != NONE)
		result = unit->weapons[slot];
	return result;
}

// @retail 0x1e1fd0
long function_1e1fd0(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	long unit_index = actor->unknown018;
	long result = NONE;
	if (unit_index != NONE)
		result = secondary_weapon_get(unit_index);
	return result;
}

// @retail 0x1e2030
bool function_1e2030(long actor_index)
{
	s_actor_view *actor = actor_get(actor_index);
	long weapon_index = function_1e1f20(actor_index);
	bool result = false;

	if (weapon_index != NONE)
	{
		s_ai_weapon_definition *definition = (s_ai_weapon_definition *)g_4e3b44[ai_object_get(weapon_index)->definition_index & 0xffff].bytes;
		result = !TEST_FIELD_BIT(definition->flags12f_0);
		if (result && actor->unknown018 != NONE && (ai_object_get(actor->unknown018)->flags19 & 2))
			result = false;
	}
	return result;
}

void __stdcall function_cb500(long unit_index, short mode, const point3f *origin,
	const vector3f *forward, const real *offsets, point3f *point);

struct s_object_position_view
{
	byte field_0[0x30];
	point3f position;
	byte field_3c[0xaa - 0x3c];
	byte type;
};

// @retail 0x1e3b00
void function_1e3b00(long object_index, long mode, point3f const *reference,
	void const *unknown0, void const *unknown1, point3f *position)
{
	s_object_position_view *object = (s_object_position_view *)ai_object_get(object_index);
	const long *mode_reference = &mode;
	const point3f *const *origin_reference = &reference;
	const void *const *local_41f0cc = &unknown0;
	const void *const *offset_reference = &unknown1;
	if ((1 << object->type) & 3)
		function_cb500(object_index, (short)*mode_reference, *origin_reference,
			(const vector3f *)*local_41f0cc, (const real *)*offset_reference, position);
	else
		*position = object->position;
}

struct s_unit_state_c6ef0;
void function_c6ef0(s_unit_state_c6ef0 *state);
void function_c6de0(long object_index, void *control);
void function_118e80(long object_index, vector3f *forward);
void __stdcall function_cbf60(long unit_index, bool active);

struct s_actor_unit_state
{
	long name;
	short mode;
	byte field_6[0x28 - 6];
	vector3f forward;
	vector3f first;
	vector3f second;
	byte field_4c[0x7c - 0x4c];
};

struct s_actor_unit_vectors
{
	byte field_0[0x168];
	vector3f first;
	byte field_174[0x18c - 0x174];
	vector3f second;
};

// @retail 0x1e31b0
void function_1e31b0(long unit_index)
{
	s_actor_unit_state state;
	function_c6ef0((s_unit_state_c6ef0 *)&state);
	state.name = 0x6000085;
	state.mode = 1;
	function_118e80(unit_index, &state.forward);
	s_actor_unit_vectors *unit = (s_actor_unit_vectors *)ai_object_get(unit_index);
	state.first = unit->first;
	state.second = unit->second;
	function_c6de0(unit_index, &state);
	function_cbf60(unit_index, false);
}

struct s_actor_object_query
{
	long definition_index;
	dword flags;
	byte field_8[0x14 - 8];
	long parent_index;
	byte field_18[0x28 - 0x18];
	dword location[2];
	byte field_30[0x70 - 0x30];
	vector3f direction;
	byte field_7c[0xaa - 0x7c];
	byte type;
	byte field_ab[0x134 - 0xab];
	dword flags134;
	byte field_138[2];
	short link_offset;
};

PRIVATE inline s_actor_object_query *actor_query_object(long index)
{
	return (s_actor_object_query *)ai_object_get(index);
}

PRIVATE inline long actor_query_root(long index)
{
	long result = NONE;
	while (index != NONE)
	{
		result = index;
		index = actor_query_object(index)->parent_index;
	}
	return result;
}

struct s_actor_object_sample
{
	point3f point;
	point3f center;
	vector3f direction;
	dword location[2];
	vector3f velocity;
};

// @retail 0x1e3a00
void function_1e3a00(long object_index, s_actor_object_sample *sample)
{
	s_actor_object_sample *const *sample_reference = &sample;
	s_actor_object_query *object = actor_query_object(object_index);
	function_b9dd0(object_index, &(*sample_reference)->center);
	(*sample_reference)->direction = object->direction;
	if ((1 << object->type) & 3)
	{
		s_object_marker marker;
		function_b8d30(object_index, 0x4000095, &marker, 1, false);
		memcpy(&(*sample_reference)->point, &marker.matrix.position, sizeof(point3f));
	}
	else
		memcpy(&(*sample_reference)->point, &(*sample_reference)->center, sizeof(point3f));
	function_ba1d0(object_index, &(*sample_reference)->velocity, NULL);
	s_actor_object_query *root = actor_query_object(actor_query_root(object_index));
	memcpy((*sample_reference)->location, root->location, sizeof(root->location));
}

struct s_actor_variant_entry
{
	long value;
	short variant;
	byte field_6[6];
};

struct s_actor_variant_definition
{
	byte field_0[0x2c];
	long count;
	s_actor_variant_entry *entries;
};

// @retail 0x1e06b0
long function_1e06b0(long tag_index)
{
	s_actor_variant_definition *definition = (s_actor_variant_definition *)g_4e3b44[tag_index & 0xffff].bytes;
	long result = 0;
	if (definition->count == 1)
		return definition->entries[0].value;
	if (definition->count > 1)
	{
		signed char counts[64];
		signed char choices[64];
		memset(counts, 0, sizeof(counts));
		s_actor_iterator iterator;
		function_x66da2b(&iterator, false);
		s_actor_view *actor;
		while ((actor = (s_actor_view *)function_1e46c0(&iterator)) != NULL)
		{
			if (*(long *)((byte *)actor + 0x54) == tag_index && actor->unknown018 != NONE)
			{
				signed char variant = *((signed char *)ai_object_get(actor->unknown018) + 0xb1);
				if (variant >= 0 && variant < 64)
					counts[variant]++;
			}
		}
		short choice_count = 0;
		short minimum = 32767;
		for (short i = 0; i < definition->count; i++)
		{
			s_actor_variant_entry *entry = &definition->entries[i];
			if (entry->value && entry->variant >= 0 && entry->variant < 64)
			{
				short count = counts[entry->variant];
				if (count < minimum)
				{
					choices[0] = (signed char)i;
					choice_count = 1;
					minimum = count;
				}
				else if (count == minimum)
					choices[choice_count++] = (signed char)i;
			}
		}
		if (choice_count > 0)
		{
			dword seed = g_4e7408->unknown0 * 0x19660d + 0x3c6ef35f;
			g_4e7408->unknown0 = seed;
			short choice = (short)(((seed >> 16) * choice_count) >> 16);
			result = definition->entries[choices[choice]].value;
		}
	}
	return result;
}

void function_290bf0(long perception_index, short team);
void function_290040(long perception_index);

// @retail 0x1e1150
void function_1e1150(long actor_index, short team)
{
 s_actor_view *actor = actor_get(actor_index);
 actor->unknown024 = team;
 if (actor->unknown007)
  function_290bf0(*(long *)actor->unknown01c, team);
 else if (actor->unknown018 != NONE)
  *(short *)((byte *)ai_object_get(actor->unknown018) + 0x138) = team;
}

// @retail 0x1e3240
void function_1e3240(void)
{
 s_actor_iterator iterator;
 function_x66da2b(&iterator, true);
 while (function_1e46c0(&iterator))
 {
  s_actor_view *actor = actor_get(iterator.actor_index);
  if (actor->unknown007 && *(long *)actor->unknown01c != NONE)
   function_290040(*(long *)actor->unknown01c);
  else
   function_1e31b0(actor->unknown018);
  actor->unknown008 = 1;
 }
}

void function_1f86a0(long actor_index);

// @retail 0x1e22d0
void function_1e22d0(long actor_index, bool conditional)
{
 s_actor_view *actor = actor_get(actor_index);
 function_2628f0(actor_index, g_470fa0);
 if (actor->unknown4ac == 4 || actor->unknown4ac == 5 || actor->unknown4ac == 6)
 {
  if (!conditional || (*(word *)((byte *)actor + 0x4ba) & 0x8000))
   function_1f86a0(actor_index);
 }
 byte *entry = (byte *)actor + 0x402;
 long count = 4;
 do
 {
  if (!conditional || (*(word *)(entry + 2) & 0x8000))
   *(s_reference *)entry = g_470fa0;
  entry += 6;
 } while (--count);
 for (short slot_index = 0; slot_index <= actor->current; ++slot_index)
 {
  short type = actor->slots[slot_index].type;
  s_slot_handler *handler = g_46eeb8[type];
  t_slot_notify callback = handler->notify34;
  if (callback)
   callback(actor_index, slot_index < 4 ? &actor->slots[slot_index] : NULL, conditional);
 }
}

void function_28fd90(long perception_index, long index);

// @retail 0x1e2150
void function_1e2150(long actor_index, long object_index)
{
 s_actor_view *actor = actor_get(actor_index);
 if (*(long *)((byte *)actor + 0x338) == object_index)
  *(long *)((byte *)actor + 0x338) = NONE;
 if (*(long *)((byte *)actor + 0x32c) == object_index)
  *(long *)((byte *)actor + 0x32c) = NONE;
 if (actor->unknown344 == object_index)
  actor->unknown344 = NONE;
 if (actor->unknown368 == object_index)
 {
  actor->unknown368 = NONE;
  actor->unknown358 = 0;
 }
 if (*(short *)((byte *)actor + 0x722) == 1 && *(long *)((byte *)actor + 0x724) == object_index)
 {
  *(long *)((byte *)actor + 0x724) = NONE;
  *(short *)((byte *)actor + 0x722) = 0;
 }
 if (*(long *)((byte *)actor + 0x7e0) == object_index)
  *(long *)((byte *)actor + 0x7e0) = NONE;
 if (actor->unknown3b4 == object_index)
  actor->unknown3b4 = NONE;
 if (actor->unknown4ac == 7 && *(long *)((byte *)actor + 0x4b8) == object_index)
 {
  actor->unknown4ac = 0;
  actor->unknown4e4 = NONE;
 }
 if (*(short *)((byte *)actor + 0x688) == 1 && *(long *)((byte *)actor + 0x68c) == object_index)
  *(long *)((byte *)actor + 0x68c) = NONE;
 if (*(short *)((byte *)actor + 0x6a0) == 1 && *(long *)((byte *)actor + 0x6a4) == object_index)
  *(long *)((byte *)actor + 0x6a4) = NONE;
 if (*(short *)((byte *)actor + 0x6b0) == 1 && *(long *)((byte *)actor + 0x6b4) == object_index)
  *(long *)((byte *)actor + 0x6b4) = NONE;
 if (actor->unknown007 && *(long *)actor->unknown01c != NONE)
  function_28fd90(*(long *)actor->unknown01c, object_index);
 for (short i = 0; i <= actor->current; ++i)
 {
  s_slot *slot = &actor->slots[i];
  t_slot_release callback = g_46eeb8[slot->type]->release28;
  if (callback) callback(actor_index, i < 4 ? slot : NULL, object_index);
 }
}

void function_28fdf0(long perception_index);

// @retail 0x1e23c0
void function_1e23c0(long actor_index)
{
 const long *index_reference = &actor_index;
 s_actor_view *actor = actor_get(*index_reference);
 *(long *)((byte *)actor + 0x250) = NONE;
 *(short *)((byte *)actor + 0x254) = NONE;
 *(short *)((byte *)actor + 0x256) = g_4686c4;
 *(bool *)((byte *)actor + 0x278) = false;
 *(long *)((byte *)actor + 0x28c) = NONE;
 s_actor_view *state = actor_get(*index_reference);
 *(bool *)((byte *)state + 0x50c) = false;
 *(long *)((byte *)state + 0x5ac) = NONE;
 *(short *)((byte *)state + 0x5b0) = NONE;
 *(short *)((byte *)state + 0x5b4) = 0;
 *(short *)((byte *)state + 0x5b6) = 0;
 state->unknown4ac = 0;
 *(short *)((byte *)state + 0x504) = 0;
 if (actor->unknown4ac == 2)
  *(long *)((byte *)actor + 0x4c8) = NONE;
 *(long *)((byte *)actor + 0x4fc) = NONE;
 if (actor->unknown007 && *(long *)actor->unknown01c != NONE)
  function_28fdf0(*(long *)actor->unknown01c);
 for (short i = 0; i <= actor->current; ++i)
 {
  s_slot *slot = &actor->slots[i];
  t_slot_proc callback = g_46eeb8[slot->type]->proc30;
  if (callback) callback(*index_reference, i < 4 ? slot : NULL);
 }
 long prop_index = actor_get(*index_reference)->first_prop_index;
 while (prop_index != NONE)
 {
  s_prop_node_view *prop = (s_prop_node_view *)(g_502418->data + (prop_index & 0xffff) * 0x3c);
  prop_index = prop->next_index;
  byte *prop_block = (byte *)function_25d690((s_prop_datum *)prop);
  byte *view = NULL;
  if (prop->view_index != NONE)
  {
   byte *entry = g_502414->data + (prop->view_index & 0xffff) * 0x124;
   if (entry) view = entry + 0x70;
  }
  *(long *)(prop_block + 0x28) = NONE;
  *(short *)(prop_block + 0x2c) = NONE;
  *(short *)(prop_block + 0x2e) = g_4686c4;
  *(long *)(prop_block + 0x44) = NONE;
  if (view && *(short *)(view + 0x70) == 1)
  {
   view[0x68] = false;
   view[0x69] = false;
   view[0x4c] = false;
   *(short *)(view + 0x70) = 0;
   view[0x6c] = true;
   view[0x6d] = true;
   view[0x88] = false;
   *(short *)(view + 0x90) = 0;
   *(short *)(view + 0x8a) = 0;
  }
 }
}
