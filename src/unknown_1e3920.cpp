// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1E3920.CPP: the radius within which an actor counts as arrived */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_1e3920.h"

long function_1e4a50(long index);

struct s_actor_reset_view_x
{
	byte field_000[0x290];
	vector3f a;
	vector3f b;
	vector3f c;
	byte field_2b4[0x7fc - 0x2b4];
	long field_7fc;
	long field_800;
	byte field_804[0xc];
	dword flags;
	vector3f field_814;
	real field_820;
	real field_824;
	vector3f field_828;
	vector3f field_834;
	vector3f field_840;
};

// @retail 0x1e3860
void function_1e3860(long actor_index)
{
	s_actor_reset_view_x *actor = (s_actor_reset_view_x *)actor_get(actor_index);
	actor->field_828 = actor->a;
	actor->field_834 = actor->b;
	actor->field_840 = actor->c;
	actor->flags = 0;
	actor->field_820 = 0.0f;
	actor->field_824 = 0.0f;
	actor->field_814 = *g_4687a4;
	actor->field_800 = NONE;
	actor->field_7fc = 0x6000086;
}

/* the actor's movement block of its character tag (function_1e4a50) */
struct s_character_movement_view
{
	byte unknown0[8];
	real arrival_radius;
};

// @retail 0x1e3920
real function_1e3920(long actor_index)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	real radius = actor->unknown4cc;

	if (radius == 0.0f)
	{
		s_moving_object *unit = moving_object_get(actor->unit_index);

		if (actor->unknown26c != NONE)
		{
			s_tag_element *element = function_1e5450(actor_index, moving_object_get(actor->unknown26c)->tag_index);

			if (element)
				radius = *(real *)((byte *)element + 0x18);
			if (radius <= 0.0f)
				radius = 0.5f;
		}
		else
		{
			s_tag_element *element;

			if (actor->unknown229 && (element = function_1e5450(actor_index, unit->tag_index)) != 0)
			{
				radius = *(real *)((byte *)element + 0x18);
			}
			else
			{
				s_character_movement_view *movement = (s_character_movement_view *)function_1e4a50(actor->tag_index);

				if (movement)
					radius = movement->arrival_radius;
			}
			if (radius <= 0.0f)
				radius = 0.3f;
		}
	}

	if (radius > 0.2f)
		return radius;
	return 0.2f;
}

struct s_actor_point_request
{
	byte field_0[0x14];
	s_record_pool *pool;
	long iterator_index;
	long datum_index;
	byte field_20[0xc];
	short type;
	short mode;
	real radius;
	real radius_squared;
	long index;
	point3f point;
	long result_index;
};

__forceinline void actor_point_iterator_initialize(s_record_pool_iterator *iterator)
{
	if (g_4f55d0->active)
	{
		iterator->data = g_502420;
		iterator->index = NONE;
		iterator->datum_index = NONE;
	}
}

// @retail 0x1e4770
void function_1e4770(s_actor_point_request *request, short mode, const point3f *point, short type, real radius)
{
	const short *type_reference = &type;
	request->mode = mode;
	request->type = *type_reference;
	request->radius = radius;
	request->radius_squared = radius * radius;
	request->point = *point;
	request->index = NONE;
	request->result_index = NONE;
	actor_point_iterator_initialize((s_record_pool_iterator *)&request->pool);
	request->index = NONE;
}

long function_1e4a10(long index);
real function_30bf0(vector3f *vector);

struct s_actor_impulse_view
{
	byte field_0[0x18];
	long unit_index;
	byte field_1c[0x54 - 0x1c];
	long tag_index;
	byte field_58[0x3b8 - 0x58];
	bool active;
	byte field_3b9[3];
	real magnitude;
	vector3f direction;
};

struct s_character_impulse_view
{
	byte field_0[0x50];
	real threshold;
};

// @retail 0x1e28b0
void function_1e28b0(long actor_index, const vector3f *direction, real magnitude)
{
	s_actor_impulse_view *actor = (s_actor_impulse_view *)actor_moving_get(actor_index);
	s_character_impulse_view *definition = (s_character_impulse_view *)function_1e4a10(actor->tag_index);
	if (definition && magnitude > definition->threshold)
	{
		actor->active = true;
		actor->magnitude = magnitude;
		actor->direction = *direction;
		if (function_30bf0(&actor->direction) == 0.0f)
			actor->direction = *g_4687a8;
	}
}

__forceinline void actor_direction_between_points(const point3f *position, const s_type_c3b527 *target, vector3f *direction)
{
	function_210c90(target, position, direction);
}

// @retail 0x1e3370
bool function_1e3370(long actor_index, void *unknown)
{
	vector3f *direction = (vector3f *)unknown;
	s_actor_moving *actor = actor_moving_get(actor_index);
	bool result = false;
	if (!actor->unknown007)
	{
		if (actor->unknown5d0)
			*direction = actor->unknown5ec;
		else if (actor->unknown50c)
		{
			const s_type_c3b527 *target = &actor->unknown4ec;
			const point3f *position = &actor->position;
			actor_direction_between_points(position, target, direction);
		}
		else
			goto done;
		result = true;
		if (function_30bf0(direction) == 0.0f)
			result = false;
	}
done:
	return result;
}

short function_1a6fe0(long owner_index, short type);

// @retail 0x1e1e50
bool function_1e1e50(long actor_index, vector3f *direction)
{
	const long *index_reference = &actor_index;
	vector3f *const *direction_reference = &direction;
	s_actor_moving *actor = actor_moving_get(*index_reference);
	bool result = false;
	if (actor->prop_index != NONE)
	{
		s_type_f95cd3 *view = function_25d700(actor->prop_index);
		if (view && *(short *)view > 6 && function_1a6fe0(*index_reference, 14) != NONE && actor->unknown722 > 0)
		{
			**direction_reference = *(vector3f *)((byte *)actor + 0x758);
			vector3f *value = *direction_reference;
			result = value->i * value->i + value->j * value->j + value->k * value->k > 0.0f;
		}
	}
	return result;
}

struct s_actor_periodic_view
{
	byte field_0[0x40];
	bool update_first;
	byte field_41;
	short first_ticks;
	bool update_second;
	byte field_45;
	short second_ticks;
};

struct s_ai_periodic_view
{
	byte field_0[6];
	short first_threshold;
	short first_maximum;
	bool first_used;
	byte field_b;
	short second_threshold;
	short second_maximum;
	bool second_used;
};

bool function_26c120(long actor_index);

// @retail 0x1e3790
void function_1e3790(long actor_index)
{
	s_actor_periodic_view *actor = (s_actor_periodic_view *)actor_moving_get(actor_index);
	actor->first_ticks++;
	actor->second_ticks++;
	bool first = false;
	bool eligible = function_26c120(actor_index);
	s_game_time_globals *time = g_510c54;
	if (eligible)
	{
		if (!((s_ai_periodic_view *)g_4f55d0)->first_used && actor->first_ticks > ((s_ai_periodic_view *)g_4f55d0)->first_threshold &&
			actor->first_ticks * time->rate > 0.5f)
		{
			actor->first_ticks = 0;
			((s_ai_periodic_view *)g_4f55d0)->first_used = true;
			first = true;
		}
		else if (actor->first_ticks > ((s_ai_periodic_view *)g_4f55d0)->first_maximum)
			((s_ai_periodic_view *)g_4f55d0)->first_maximum = actor->first_ticks;
	}
	actor->update_first = first;
	bool second = false;
	if (!((s_ai_periodic_view *)g_4f55d0)->second_used && actor->second_ticks > ((s_ai_periodic_view *)g_4f55d0)->second_threshold &&
		actor->second_ticks * time->rate > 0.5f)
	{
		actor->second_ticks = 0;
		((s_ai_periodic_view *)g_4f55d0)->second_used = true;
		second = true;
	}
	else if (actor->second_ticks > ((s_ai_periodic_view *)g_4f55d0)->second_maximum)
		((s_ai_periodic_view *)g_4f55d0)->second_maximum = actor->second_ticks;
	actor->update_second = second;
}

struct s_actor_height_definition
{
	byte field_0[0x1a];
	short height_index;
};

struct s_height_table
{
	byte field_0[0x98];
	real heights[6];
};

struct s_height_globals
{
	byte field_0[0xc8];
	long count;
	s_height_table *table;
};

// @retail 0x1e20b0
real function_1e20b0(long actor_index)
{
	s_actor_moving *actor = actor_moving_get(actor_index);
	s_actor_height_definition *definition = (s_actor_height_definition *)function_1e4a50(actor->tag_index);
	short height_index = definition->height_index;
	real result;
	if (height_index == 7)
		result = 100000.0f;
	else
	{
		short index = (height_index < 6 ? height_index : 6) - 1;
		real height = 0.0f;
		s_height_globals *globals = (s_height_globals *)g_4e034c;
		if (index >= 0 && index < 6 && globals && globals->count > 0)
			height = globals->table->heights[index];
		result = (real)sqrt((height * 2.0f) * 6.417322635650635f);
	}
	return result;
}
