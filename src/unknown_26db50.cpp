// @flags /O2 /Ob1 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_20fe20.h"
#include "unknown_11cc90.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_2626b0.h"
#include "object_markers.h"

/* Location records share one owner and type, with a bitmap of active entries. */
struct s_location_record_view
{
	short salt;
	short type;
	long owner;
	short count;
	short users;
	s_type_c3b527 point;
	vector3f up;
	vector3f forward;
	long location_index;
	bool valid;
	byte unknown39[3];
	long time;
	dword active[1];
	s_262b40_result entries[32];
	byte unknown444[0x40];
};

struct s_location_record_actor_view
{
	byte unknown000[0x3f4];
	long record_index;
	byte unknown3f8[0x888 - 0x3f8];
};

void function_26bfa0(long object_index, long *location_index, s_location_view *location);
void function_b9fc0(long object_index, vector3f *forward, vector3f *up);

// @retail 0x26d3f0
long function_26d3f0(long object_index, short type)
{
	long const volatile *object_reference = &object_index;
	long location_index = NONE;
	s_type_c3b527 location;
	vector3f up, forward;
	function_26bfa0(object_index, &location_index, (s_location_view *)&location);
	if (location_index != NONE)
	{
		long result = record_pool_allocate(g_51eca4);
		if (result != NONE)
		{
			s_location_record_view *record = (s_location_record_view *)(g_51eca4->data + (result & 0xffff) * sizeof(s_location_record_view));
			record->owner = object_index;
			record->count = 0;
			record->users = 0;
			record->type = type;
			record->active[0] = 0;
			function_b9fc0(*object_reference, &forward, &up);
			record->point = location;
			record->location_index = location_index;
			record->up = up;
			record->forward = forward;
			record->valid = true;
			record->time = g_510c54->game_time;
		}
		return result;
	}
	return NONE;
}

// @retail 0x26d500
long function_26d500(long object_index)
{
	byte *tag = g_4e3b44[object_get(object_index)->tag_index & 0xffff].bytes;
	long result = NONE;
	if (*(long *)(tag + 0x5c) > 0 && (**(byte **)(tag + 0x60) & 4))
	{
		result = function_26d3f0(object_index, 1);
		if (result != NONE)
		{
			s_location_record_view *record = (s_location_record_view *)(g_51eca4->data + (result & 0xffff) * sizeof(s_location_record_view));
			record->count = 32;
		}
	}
	return result;
}

PRIVATE inline real record_dot3f(vector3f const *a, vector3f const *b)
{
	return a->i * b->i + a->j * b->j + a->k * b->k;
}

// @retail 0x26dc90
bool function_26dc90(long record_index)
{
	s_location_record_view *record = (s_location_record_view *)(g_51eca4->data + (record_index & 0xffff) * sizeof(s_location_record_view));
	if (g_510c54->game_time > record->time)
	{
		long owner = record->owner;
		record->valid = false;
		if (owner != NONE)
		{
			vector3f up, forward;
			function_b9fc0(owner, &forward, &up);
			if (record->type != 1 || (!(record_dot3f(&record->up, &up) < 0.9f) && !(record_dot3f(&record->forward, &forward) < 0.99f)))
			{
				real threshold = record->type == 1 ? 0.002500000176951289f : 0.04000000283122063f;
				long location_index;
				s_type_c3b527 location;
				function_26bfa0(owner, &location_index, (s_location_view *)&location);
				if (location_index != NONE && function_210a30(&location, &record->point) <= threshold)
				{
					record->valid = true;
					record->time = g_510c54->game_time;
					return true;
				}
			}
		}
		record->time = g_510c54->game_time;
		return false;
	}
	return record->valid;
}

struct s_record_motion_view
{
	byte unknown00[0x48];
	vector3f direction;
	vector3f side;
	point3f position;
};

real function_30bf0(vector3f *vector);
bool function_26d290(point3f const *origin, point3f const *target, long sector_index, long *output_sector);

// @retail 0x26e180
bool function_26e180(long record_index, s_record_motion_view const *motion, short mode, point3f *world_point,
	s_type_c3b527 *output, long *output_sector)
{
	s_location_record_view *record = (s_location_record_view *)(g_51eca4->data + (record_index & 0xffff) * sizeof(s_location_record_view));
	bool result = false;
	if (record->type == 1)
	{
		vector3f direction = motion->direction;
		direction.k = 0.0f;
		if (function_30bf0(&direction) > 0.0f)
		{
			point3f point;
			switch (mode)
			{
			case 1:
				point.x = direction.i * 0.25f + motion->position.x;
				point.y = direction.j * 0.25f + motion->position.y;
				point.z = direction.k * 0.25f + motion->position.z;
				break;
			case 2:
				point.x = motion->position.x - direction.i * 0.25f;
				point.y = motion->position.y - direction.j * 0.25f;
				point.z = motion->position.z - direction.k * 0.25f;
				break;
			case 3:
				point = motion->position;
				break;
			default:
				mode = NONE;
				break;
			}
			if (mode != NONE)
			{
				vector3f side = motion->side;
				point.x += side.i * 0.15f;
				point.y += side.j * 0.15f;
				short index = record->point.output_index;
				if (function_210690(index, &point, &output->point))
				{
					output->output_index = index;
					if (function_26d290(&record->point.point, &output->point, record->location_index, output_sector))
					{
						if (world_point)
							*world_point = point;
						result = true;
					}
				}
			}
		}
	}
	return result;
}

// @retail 0x26db50
long function_26db50(long owner, short type)
{
	long result = NONE;
	long const *owner_reference = &owner;
	s_record_pool_iterator iterator;
	iterator.data = g_51eca4;
	iterator.index = NONE;
	s_location_record_view *record;
	while ((record = (s_location_record_view *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		if (record->owner == *owner_reference && record->type == type)
		{
			result = iterator.datum_index;
			break;
		}
	}
	return result;
}

// @retail 0x26dbd0
void function_26dbd0(long record_index, long actor_index)
{
	s_location_record_view *record = (s_location_record_view *)(g_51eca4->data + (record_index & 0xffff) * sizeof(s_location_record_view));
	s_location_record_actor_view *actor = (s_location_record_actor_view *)(g_4f55f0->data + (actor_index & 0xffff) * sizeof(s_location_record_actor_view));
	if (actor->record_index != record_index)
	{
		actor->record_index = record_index;
		record->users++;
	}
}

// @retail 0x26e030
s_262b40_result *__stdcall function_26e030(s_reference reference)
{
	s_262b40_result *result = NULL;
	long index = reference.unknown0;
	s_location_record_view *record = &((s_location_record_view *)g_51eca4->data)[reference.unknown2 & 0x7fff];
	if (index >= 0 && index < record->count && (record->active[index >> 5] & (1 << (index & 0x1f))))
		result = &record->entries[index];
	return result;
}

struct s_record_object_motion_view
{
	byte unknown00[0x30];
	point3f position;
	byte unknown3c[0x88 - 0x3c];
	vector3f velocity;
};

long function_baf80(long object_index);

// @retail 0x26df40
bool function_26df40(long actor_index, long object_index, bool ignore_speed)
{
	s_actor_view *actor = actor_get(actor_index);
	bool result = false;
	if (object_index != NONE)
	{
		long root_index = function_baf80(object_index);
		s_record_object_motion_view *object = (s_record_object_motion_view *)((s_object_header_view *)g_4e0300->data)[root_index & 0xffff].object;
		if (ignore_speed || sqrt(length_sq3f(&object->velocity)) < 0.1f)
		{
			point3f position = object->position;
			vector3f delta;
			vector3d_from_points3d(&position, &actor->position, &delta);
			if (sqrt(delta.j * delta.j + (delta.i * delta.i + delta.k * delta.k)) < 15.0f)
				result = true;
		}
	}
	return result;
}


struct s_location_entry_view
{
	union
	{
		s_type_c3b527 location;
		struct
		{
			byte unknown00[0xe];
			word flags;
		};
	};
	short field10;
	word sector;
	long owner;
	vector2f direction;
};

struct s_record_sector_map
{
	byte unknown00[0x30];
	struct s_sector_entry
	{
		word sector;
		byte unknown02[6];
	} *entries;
};

struct s_bsp3d;
struct s_slot_entry_list;
extern s_slot_entry_list *g_4e0340;
long function_14a280(s_bsp3d *bsp, point3f *point, long index);
vector2f *function_11df30(vector2f *angles, vector3f const *vector);

PRIVATE __forceinline bool location_entry_active(dword const *bits, long index)
{
	return (bits[index >> 5] & (1UL << (index & 31))) != 0;
}

PRIVATE __forceinline void location_entry_activate(dword *bits, long index)
{
	bits[index >> 5] |= 1UL << (index & 31);
}

// @retail 0x26d9c0
bool function_26d9c0(long record_index, s_type_c3b527 const *point, short entry_index, short type, long owner, vector3f const *direction)
{
	s_location_record_view *record = (s_location_record_view *)(g_51eca4->data + (record_index & 0xffff) * sizeof(s_location_record_view));
	bool result = false;
	if (entry_index >= 0 && entry_index < 32)
	{
		if (!location_entry_active(record->active, entry_index))
		{
			s_location_entry_view *entry = (s_location_entry_view *)&record->entries[entry_index];
			point3f position;
			function_210850(point, &position);
			position.x = g_4687b0->i * 0.05f + position.x;
			position.y = g_4687b0->j * 0.05f + position.y;
			position.z = g_4687b0->k * 0.05f + position.z;
			long sector_index = function_14a280((s_bsp3d *)g_4e0340, &position, 0);
			if (sector_index == NONE)
				goto done;
			entry->sector = ((s_record_sector_map *)g_4e0348)->entries[sector_index].sector;
			if (entry->sector == (word)NONE)
				goto done;
			location_entry_activate(record->active, entry_index);
			entry->owner = owner;
			entry->location = *point;
			entry->flags = 0;
			entry->field10 = NONE;
			if (direction)
				function_11df30(&entry->direction, direction);
			else
			{
				entry->direction.i = 0.0f;
				entry->direction.j = 0.0f;
			}
			entry->flags |= 0x40;
			long type_value = type;
			dword flags = entry->flags;
			if (type_value > 0 && type_value <= 3)
				entry->flags = (word)(flags | 0x90);
			((short *)record->unknown444)[entry_index] = type;
		}
		result = true;
	}
done:
	return result;
}

// @retail 0x26e090
short __stdcall function_26e090(long object_index, s_object_marker *markers, short *types, short capacity)
{
	short count = function_b8d30(object_index, 0x100006c0, markers, capacity, false);
	if (types)
	{
		for (short i = 0; i < count; ++i)
			types[i] = 2;
	}
	if (count < capacity)
	{
		short added = function_b8d30(object_index, 0x110006c1, markers + count, capacity - count, false);
		if (types)
		{
			for (short i = count; i < count + added; ++i)
				types[i] = 1;
		}
		count += added;
		if (count < capacity)
		{
			added = function_b8d30(object_index, 0x0b0006c2, markers + count, capacity - count, false);
			if (types)
			{
				for (short i = count; i < count + added; ++i)
					types[i] = 3;
			}
			count += added;
		}
	}
	return count;
}
