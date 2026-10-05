// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_293070.CPP: the flocks of the scenario: creating one, activating one and
   finding one by its scenario name (outside functions lane A's script
   functions need) */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_0259d0.h"

/* a flock (g_51ecb4, ai.cpp): the index of its scenario definition at +2 */
struct s_flock
{
	short salt;
	short definition_index;
	long unknown04;
	short unknown08;
	short unknown0a;
	bool unknown0c;
	byte unknown0d;
	bool unknown0e;
	byte unknown0f[0x12 - 0xf];
	word nearby_player_mask;
	point3f position14;
	real value20;
	short unknown24;
	byte unknown26[0x28 - 0x26];
};

/* the scenario's flocks (0x84 bytes each), named at +0x80 */
struct s_scenario_flock
{
	short field_0_3;
	byte unknown02[2];
	short trigger_volume_index;
	byte flags;
	byte unknown07;
	real boundary_distance;
	byte unknown0c[0x2c - 0xc];
	long unknown2c;
	byte unknown30[0x6c - 0x30];
	real proximity_radius;
	byte unknown70[0x80 - 0x70];
	long name;
};

struct s_scenario_flocks_view
{
	byte unknown000[0x350];
	long flock_count;
	s_scenario_flock *flocks;
};

extern s_record_pool *g_51ecb4;

/* a new flock of a scenario flock definition */
// @retail 0x293070
long flock_new(short definition_index)
{
	long flock_index = record_pool_allocate(g_51ecb4);
	if (flock_index != NONE)
	{
		s_flock *flock = &((s_flock *)g_51ecb4->data)[flock_index & 0xffff];
		flock->unknown04 = NONE;
		flock->unknown0a = NONE;
		flock->unknown08 = 0;
		flock->definition_index = definition_index;
		flock->unknown24 = 0;
		flock->unknown0c = false;
		flock->unknown0e = true;
	}
	return flock_index;
}

/* an object's flock membership: its flock and the next member */
struct s_flock_member
{
	long flock_index;
	byte unknown04[4];
	long next_object_index;
};

/* the object fields a flock member has (+0x134 is 1 for a creature) */
struct s_flock_object
{
	byte unknown000[0x134];
	long type134;
	byte unknown138[2];
	short flock_member_offset;
};

struct s_flock_object_header
{
	byte unknown00[8];
	s_flock_object *object;
};

inline s_flock_member *flock_object_get_member(s_flock_object *object)
{
	s_flock_member *member;
	if (object->type134 == 1)
		member = (s_flock_member *)((byte *)object + object->flock_member_offset);
	else
		member = NULL;
	return member;
}

/* adds an object at the head of a flock's member list */
// @retail 0x2936b0
bool function_2936b0(long flock_index, long object_index)
{
	bool result = false;
	s_flock *flock = &((s_flock *)g_51ecb4->data)[flock_index & 0xffff];
	s_flock_object *object = ((s_flock_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_flock_member *member = flock_object_get_member(object);
	if (member)
	{
		member->next_object_index = flock->unknown04;
		member->flock_index = flock_index;
		flock->unknown08++;
		flock->unknown04 = object_index;
		result = true;
	}
	return result;
}

/* removes one object from a flock's member list */
// @retail 0x293710
bool function_293710(long flock_index, long object_index)
{
	bool result = false;
	s_flock *flock = &((s_flock *)g_51ecb4->data)[flock_index & 0xffff];
	long *link = &flock->unknown04;
	while (*link != NONE)
	{
		long current_index = *link;
		s_flock_object *object = ((s_flock_object_header *)g_4e0300->data)[current_index & 0xffff].object;
		s_flock_member *member = flock_object_get_member(object);
		if (current_index == object_index)
		{
			*link = member->next_object_index;
			member->flock_index = NONE;
			flock->unknown08--;
			result = true;
			break;
		}
		link = &member->next_object_index;
	}
	return result;
}

point3f *function_b9dd0(long object_index, point3f *result);

// @retail 0x295210
void function_295210(long object_index)
{
	s_flock_object *object = ((s_flock_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_flock_member *member = flock_object_get_member(object);
	if (member && member->flock_index != NONE)
	{
		long flock_index = member->flock_index;
		point3f position;
		function_b9dd0(object_index, &position);
		if (function_293710(flock_index, object_index))
		{
			s_flock *flock = &((s_flock *)g_51ecb4->data)[flock_index & 0xffff];
			if (flock->unknown24 <= 0)
			{
				real ticks_real = (real)g_510c54->field_2_3 * 3.0f;
				long ticks;
				__asm
				{
					fld ticks_real
					fistp ticks
				}
				flock->unknown24 = (short)ticks;
				flock->position14 = position;
				flock->value20 = 1.0f;
			}
		}
	}
}

// @retail 0x2952e0
void function_2952e0(long object_index)
{
	s_flock_object *object = ((s_flock_object_header *)g_4e0300->data)[object_index & 0xffff].object;
	s_flock_member *member = flock_object_get_member(object);
	if (member && member->flock_index != NONE)
		function_293710(member->flock_index, object_index);
}

struct s_flock_member_iterator
{
	long next_object_index;
	long object_index;
};

/* advances an iterator while retaining the current object's index */
// @retail 0x2955b0
s_flock_object *function_2955b0(s_flock_member_iterator *iterator)
{
	long object_index = iterator->next_object_index;
	s_flock_object *result = NULL;
	iterator->object_index = object_index;
	if (object_index != NONE)
	{
		result = ((s_flock_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		s_flock_member *member = flock_object_get_member(result);
		if (member)
			iterator->next_object_index = member->next_object_index;
		else
			iterator->next_object_index = NONE;
	}
	return result;
}

void __stdcall function_b8540(long object_index);

/* deletes a flock and every object of it */
// @retail 0x2937a0
void function_2937a0(long flock_index)
{
	long next_object_index = ((s_flock *)g_51ecb4->data)[flock_index & 0xffff].unknown04;
	while (next_object_index != NONE)
	{
		long object_index = next_object_index;
		s_flock_object *object = ((s_flock_object_header *)g_4e0300->data)[object_index & 0xffff].object;
		s_flock_member *member = flock_object_get_member(object);
		next_object_index = member ? member->next_object_index : NONE;

		member = flock_object_get_member(object);
		if (member)
			member->flock_index = NONE;
		function_b8540(object_index);
	}
	record_pool_release(g_51ecb4, flock_index);
}

/* creates the flock of a scenario flock definition */
// @retail 0x2930c0
bool function_2930c0(long definition_index)
{
	bool result = false;
	s_scenario_flocks_view *scenario = (s_scenario_flocks_view *)g_4e0350;

	if (definition_index >= 0 && definition_index < scenario->flock_count)
	{
		s_scenario_flock *definition = &scenario->flocks[definition_index];
		if (definition->unknown2c != NONE)
		{
			long flock_index = flock_new((short)definition_index);
			if (flock_index != NONE)
			{
				if (definition->flags & 2)
					((s_flock *)g_51ecb4->data)[flock_index & 0xffff].unknown0e = false;
				result = true;
			}
		}
	}
	return result;
}

/* creates the scenario flocks belonging to the current section */
// @retail 0x292f10
void function_292f10(void)
{
	s_scenario_flocks_view *scenario = (s_scenario_flocks_view *)g_4e0350;
	for (short i = 0; i < scenario->flock_count; i++)
	{
		if (scenario->flocks[i].field_0_3 == g_4686c4)
			function_2930c0(i);
	}
}

/* the scenario's trigger volumes (0x44 bytes each), their extents at +0x30 */
struct s_flock_trigger_volume
{
	byte unknown00[0x30];
	real extents[3];
	byte unknown3c[0x44 - 0x3c];
};

struct s_scenario_flock_volumes_view
{
	byte unknown000[0x108];
	long trigger_volume_count;
	s_flock_trigger_volume *trigger_volumes;
	byte unknown110[0x350 - 0x110];
	long flock_count;
	s_scenario_flock *flocks;
};

bool function_11c380(long trigger_volume_index, transform4x3f *matrix); /* unknown_11c380.cpp */

/* whether a point (within a radius) is inside the trigger volume of a flock's
   scenario definition */
// @retail 0x295490
bool function_295490(long flock_index, point3f const *point, real radius)
{
	s_scenario_flock_volumes_view *scenario = (s_scenario_flock_volumes_view *)g_4e0350;
	s_scenario_flock *definition = &scenario->flocks[((s_flock *)g_51ecb4->data)[flock_index & 0xffff].definition_index];
	bool result = false;

	if (definition->trigger_volume_index >= 0 && definition->trigger_volume_index < scenario->trigger_volume_count)
	{
		transform4x3f matrix;

		if (function_11c380(definition->trigger_volume_index, &matrix))
		{
			s_flock_trigger_volume *volume = &scenario->trigger_volumes[definition->trigger_volume_index];
			vector3f vector;

			vector3d_from_points3d(&matrix.position, point, &vector);
			result = true;
			for (short i = 0; i < 3; i++)
			{
				real distance = dot3f(&vector, &(&matrix.forward)[i]);
				if (-radius > distance || distance > volume->extents[i] + radius)
				{
					result = false;
					break;
				}
			}
		}
	}
	return result;
}

inline real flock_boundary_clip(real distance, real maximum)
{
	return distance < 0.0f ? 0.0f : distance > maximum ? maximum : distance;
}

inline void flock_add_direction(vector3f *direction, vector3f const *axis, real scale)
{
	direction->i += axis->i * scale;
	direction->j += axis->j * scale;
	direction->k += axis->k * scale;
}

inline void flock_move_point(point3f *point, vector3f const *axis, real distance)
{
	point->x += axis->i * distance;
	point->y += axis->j * distance;
	point->z += axis->k * distance;
}

real function_30bf0(vector3f *vector);

/* steers a flock member away from its volume's faces, optionally moving
   an outside point back into the volume */
// @retail 0x293d30
void function_293d30(s_scenario_flock const *definition, point3f *point, real *strength, vector3f *direction, bool horizontal_only, bool *moved)
{
	/* Taking the address preserves retail's stack-passed horizontal flag. */
	bool const *horizontal_reference = &horizontal_only;
	real boundary_distance = 1.0f;
	s_scenario_flock_volumes_view *scenario = (s_scenario_flock_volumes_view *)g_4e0350;
	bool clamp_position = (definition->flags & 1) != 0;
	*strength = 0.0f;
	*direction = *g_4687a4;
	*moved = false;
	if (definition->boundary_distance > 0.0f)
		boundary_distance = definition->boundary_distance;
	if (definition->trigger_volume_index >= 0 && definition->trigger_volume_index < scenario->trigger_volume_count)
	{
		transform4x3f matrix;
		if (function_11c380(definition->trigger_volume_index, &matrix))
		{
			s_flock_trigger_volume *volume = &((s_scenario_flock_volumes_view *)g_4e0350)->trigger_volumes[definition->trigger_volume_index];
			real const *extents = volume->extents;
			real maximum_strength = 0.0f;
			vector3f relative;
			vector3d_from_points3d(&matrix.position, point, &relative);
			long axis_count = *horizontal_reference ? 2 : 3;
			for (short i = 0; i < axis_count; i++)
			{
				vector3f const *axis = &(&matrix.forward)[i];
				real distance = axis->k * relative.k + axis->j * relative.j + axis->i * relative.i;
				if (boundary_distance >= distance)
				{
					real value = flock_boundary_clip(boundary_distance - distance, boundary_distance) / boundary_distance * 3.0f;
					flock_add_direction(direction, axis, value);
					if (value > maximum_strength)
						maximum_strength = value;
					if (clamp_position && distance < 0.0f)
					{
						flock_move_point(point, axis, 0.0f - distance);
						*moved = true;
					}
				}
				if (distance >= extents[i] - boundary_distance)
				{
					real value = (1.0f - flock_boundary_clip(extents[i] - distance, boundary_distance) / boundary_distance) * 3.0f;
					flock_add_direction(direction, axis, 0.0f - value);
					if (value > maximum_strength)
						maximum_strength = value;
					if (clamp_position && distance > extents[i])
					{
						flock_move_point(point, axis, extents[i] - distance);
						*moved = true;
					}
				}
			}
			function_30bf0(direction);
			*strength = maximum_strength;
		}
	}
}

/* the scenario flock definition with the name, or NONE */
// @retail 0x295860
short flock_definition_find(long name)
{
	s_scenario_flocks_view *scenario = (s_scenario_flocks_view *)g_4e0350;
	short result = NONE;

	for (short i = 0; i < scenario->flock_count; i++)
	{
		s_scenario_flock *flock = &scenario->flocks[i];
		if (flock->name == name)
		{
			result = i;
			break;
		}
	}
	return result;
}

struct s_active_flock_iterator
{
	s_flock *flock;
	s_record_pool_iterator iterator;
	long flock_index;
};

inline void flock_iterator_begin(s_active_flock_iterator *iterator, bool active)
{
	if (active)
	{
		iterator->iterator.data = g_51ecb4;
		iterator->iterator.index = NONE;
	}
}

inline s_flock *flock_iterator_next(s_active_flock_iterator *iterator, bool active)
{
	iterator->flock = NULL;
	if (active)
	{
		iterator->flock = (s_flock *)data_iterator_next_inlined(&iterator->iterator);
		iterator->flock_index = iterator->iterator.datum_index;
	}
	return iterator->flock;
}

struct s_flock_player_view
{
	byte unknown00[0x2c];
	long object_index;
};

// @retail 0x293a20
void function_293a20(long flock_index)
{
	s_flock *flock = &((s_flock *)g_51ecb4->data)[flock_index & 0xffff];
	s_scenario_flocks_view *scenario = (s_scenario_flocks_view *)g_4e0350;
	if (flock->definition_index >= 0 && flock->definition_index < scenario->flock_count)
	{
		s_scenario_flock *definition = &scenario->flocks[flock->definition_index];
		long player_number = 0;
		real radius = 3.0f;
		if (definition->proximity_radius > 0.0f)
			radius = definition->proximity_radius + 0.2f;
		flock->nearby_player_mask = 0;
		s_record_pool_iterator iterator;
		iterator.data = g_4e8c24;
		iterator.index = NONE;
		s_flock_player_view *player;
		while ((player = (s_flock_player_view *)data_iterator_next_calling(&iterator)) != NULL)
		{
			if (player->object_index != NONE)
			{
				point3f position;
				function_b9dd0(player->object_index, &position);
				if (function_295490(flock_index, &position, radius))
					flock->nearby_player_mask |= 1 << (byte)player_number;
			}
			player_number++;
		}
		if (flock->unknown24 > 0)
			flock->unknown24--;
	}
}

// @retail 0x295320
void function_295320(point3f const *point)
{
	s_active_flock_iterator iterator;
	flock_iterator_begin(&iterator, g_4f55d0->active);
	while (flock_iterator_next(&iterator, g_4f55d0->active))
	{
		if (iterator.flock->unknown0c && iterator.flock->definition_index >= 0 &&
			iterator.flock->definition_index < ((s_scenario_flocks_view *)g_4e0350)->flock_count)
		{
			s_scenario_flock *definition = &((s_scenario_flocks_view *)g_4e0350)->flocks[iterator.flock->definition_index];
			real radius = 3.0f;
			if (definition->proximity_radius > 0.0f)
				radius = definition->proximity_radius;
			if (function_295490(iterator.flock_index, point, radius + 1.0f))
			{
				real ticks_real = (real)g_510c54->field_2_3 * 3.0f;
				long ticks;
				__asm
				{
					fld ticks_real
					fistp ticks
				}
				iterator.flock->unknown24 = (short)ticks;
				iterator.flock->position14 = *point;
				iterator.flock->value20 = 2.0f;
			}
		}
	}
}

/* the flock whose scenario definition has the name, or NONE */
// @retail 0x2958a0
long function_2958a0(long name)
{
	long result = NONE;
	s_active_flock_iterator iterator;
	bool active = g_4f55d0->active;
	flock_iterator_begin(&iterator, active);
	while (flock_iterator_next(&iterator, active))
	{
		if (((s_scenario_flocks_view *)g_4e0350)->flocks[iterator.flock->definition_index].name == name)
		{
			result = iterator.flock_index;
			break;
		}
	}
	return result;
}
