// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_261280.CPP: choosing a reference for an actor to follow, falling
   back to the one it already follows */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_0259a0.h"
#include "unknown_2626b0.h"
#include <float.h>
#include "unknown_25fc30.h"

bool function_2624d0(s_261d20_entry *entry, s_reference reference);
__declspec(noinline) bool function_260160(long actor_index, s_prop_search *search, s_261d20_entry *entry);

/* the request as function_261280 reads it */
struct s_prop_search_request_view
{
	byte unknown000[0x15];
	bool unknown015;
	byte unknown016[0x618 - 0x16];
	bool unknown618;
	byte unknown619[0x620 - 0x619];
	point3f point;
};

// @retail 0x261280
s_reference function_261280(s_prop_search *search, long actor_index, s_261d20_entry *entry, long *b, byte *buffer, bool *c)
{
	s_actor_view *actor = actor_get(actor_index);
	s_prop_search_request_view *request = (s_prop_search_request_view *)search;
	s_reference reference;

	if (!REFERENCE_EQUAL(actor->unknown418, g_470fa0) && actor->unknown3f0)
	{
		request->unknown015 = false;
	}
	else
	{
		request->unknown015 = true;
	}
	reference = function_2605d0(actor_index, (s_2605d0_request const *)search, (long)entry, (long)b, buffer, c);
	if (REFERENCE_EQUAL(reference, g_470fa0) && !REFERENCE_EQUAL(actor->unknown418, g_470fa0) && actor->unknown3f0)
	{
		reference = actor->unknown418;
		if (entry && function_262b40(reference) && function_2624d0(entry, reference))
		{
			if (request->unknown618)
			{
				vector3f vector;

				vector.i = entry->point.x - request->point.x;
				vector.j = entry->point.y - request->point.y;
				vector.k = entry->point.z - request->point.z;
				entry->distance_squared = length_sq3f(&vector);
			}
			else
			{
				entry->distance_squared = 0.0f;
			}
			if (!function_260160(actor_index, search, entry))
			{
				reference = g_470fa0;
			}
		}
		*b = NONE;
		*c = false;
	}
	return reference;
}


struct s_reference_candidate_view
{
	s_type_c3b527 *location;
	s_reference reference;
	short field08;
	byte unknown0a[2];
	point3f point;
	real distance18;
	vector3f vector1c;
	real distance28;
	real distance2c;
	real distance_squared;
	vector3f vector34;
	vector3f vector40;
	bool flag4c;
	bool flag4d;
	byte unknown4e[2];
	real value50;
	real value54;
	bool flag58;
	bool flag59;
	bool flag5a;
	bool flag5b;
	short field5c;
	byte unknown5e[0x78 - 0x5e];
};

// @retail 0x2624d0
bool function_2624d0(s_261d20_entry *entry, s_reference reference)
{
	bool result = false;
	s_type_c3b527 *location = (s_type_c3b527 *)function_262b40(reference);
	if (location)
	{
		s_reference_candidate_view *candidate = (s_reference_candidate_view *)entry;
		candidate->location = location;
		candidate->reference = reference;
		candidate->field08 = 0;
		candidate->distance18 = FLT_MAX;
		candidate->vector1c = *g_4687a4;
		candidate->distance28 = FLT_MAX;
		candidate->vector40 = *g_4687a4;
		candidate->distance2c = FLT_MAX;
		candidate->vector34 = *g_4687a4;
		candidate->distance_squared = 0.0f;
		candidate->value50 = 0.0f;
		candidate->value54 = 0.0f;
		candidate->flag4c = true;
		candidate->flag4d = false;
		candidate->flag5b = false;
		candidate->flag5a = false;
		candidate->flag58 = false;
		candidate->flag59 = false;
		candidate->field5c = 0;
		function_210850(location, &candidate->point);
		result = true;
	}
	return result;
}


struct s_reference_direction_request
{
	byte unknown000[0x54];
	bool has_direction;
	byte unknown055[0x620 - 0x55];
	point3f point;
};

real function_30bf0(vector3f *vector);

// @retail 0x260530
void function_260530(s_reference_candidate_view *entry, s_reference_direction_request const *request)
{
	vector3f delta;
	vector3d_from_points3d(&request->point, &entry->point, &delta);
	if (length_sq3f(&delta) < 400.0f)
	{
		entry->distance28 = function_30bf0(&delta);
		if (request->has_direction)
			entry->vector34 = delta;
	}
}


long function_262a30(s_reference reference);

struct s_reference_filter_view
{
	byte unknown00[0x56];
	bool allow_a;
	bool require_flag;
	bool allow_b;
	bool allow_any;
};

// @retail 0x2623a0
bool function_2623a0(s_reference_filter_view const *filter, long actor_index, s_actor_view *actor, s_reference reference)
{
	if (filter)
	{
		s_262b40_result *entry = function_262b40(reference);
		if (!entry || (!filter->allow_a && !filter->allow_b &&
			(*(long *)((byte *)entry + 0x14) == NONE || !(entry->flags & 0x60))))
			return false;
		bool result;
		bool flag = (bool)(((dword)entry->flags >> 5) & 1);
		if (filter->require_flag)
			result = flag;
		else if (filter->allow_any)
			result = true;
		else
			result = !flag;
		if (result)
		{
			long other_index = function_262a30(reference);
			if (other_index != actor_index && other_index != NONE)
			{
				s_actor_view *other = actor_get(other_index);
				if (other->unknown024 != actor->unknown024)
					return false;
				point3f point;
				function_210850((s_type_c3b527 *)entry, &point);
				real distance = distance3d((point3f *)((byte *)other + 0x238), &point);
				if (distance < 1.0f || distance3d((point3f *)((byte *)actor + 0x238), &point) * 2.0f > distance)
					return false;
			}
		}
		return result;
	}
	return true;
}

struct s_candidate_range
{
    byte unknown00[0x20];
    dword flags;
    byte unknown24[0x38 - 0x24];
    short first;
    short count;
    byte unknown3c[0x88 - 0x3c];
};

struct s_candidate_block
{
    byte unknown00[0x24];
    short structure_index;
    byte unknown26[0xe];
    s_candidate_range *ranges;
};

struct s_29db90;
void function_29db90(short range_index, short block_index, s_29db90 *list);

// @retail 0x262180
short __stdcall function_262180(long actor_index, long block_index, long range_index,
    s_261d20_entry *entries, short *count, long maximum_count, s_prop_search *search, short type)
{
    long const *actor_reference = &actor_index;
    long const *block_reference = &block_index;
    long const *range_reference = &range_index;
    s_261d20_entry *const *entries_reference = &entries;
    short *const *count_reference = &count;
    long const *maximum_reference = &maximum_count;
    s_prop_search *const *search_reference = &search;
    short const *type_reference = &type;
    s_actor_view *actor = actor_get(*actor_reference);
    bool special = actor->unknown26c != NONE;
    if (*block_reference != NONE && *range_reference != NONE)
    {
        s_type_967e20 *context = (s_type_967e20 *)*search_reference;
        if (!context || !context->unknown68c ||
            (*block_reference == context->unknown68e && *range_reference == context->unknown690))
        {
            s_candidate_block *block = &(*(s_candidate_block **)((byte *)g_4e0350 + 0x16c))[*block_reference & 0xffff];
            if (block->structure_index == g_4686c4)
            {
                s_candidate_range *range = &block->ranges[*range_reference];
                if (special == (bool)(range->flags & 1))
                {
                    bool allowed = (bool)((range->flags >> 1) & 1);
                    if (context)
                    {
                        if (!special && (context->unknown57 || context->unknown59))
                        {
                            if (context->unknown57 && !allowed)
                                goto done;
                        }
                        else if (allowed)
                            goto done;
                    }
                    for (short index = range->first; *count < *maximum_reference && index < range->first + range->count; ++index)
                    {
                        s_reference reference;
                        reference.unknown0 = index;
                        reference.unknown2 = (short)*block_reference;
                        if (function_2623a0((s_reference_filter_view const *)context, *actor_reference, actor, reference))
                        {
                            s_reference_candidate_view *entry = (s_reference_candidate_view *)&(*entries_reference)[*count];
                            if (function_2624d0((s_261d20_entry *)entry, reference))
                            {
                                if (*type_reference == 1)
                                    ((s_reference_candidate_view *)&(*entries_reference)[*count])->flag5b = true;
                                else if (*type_reference == 2)
                                    ((s_reference_candidate_view *)&(*entries_reference)[*count])->flag5a = true;
                                ((s_reference_candidate_view *)&(*entries_reference)[*count])->flag58 =
                                    (bool)(((dword)*(char *)((byte *)((s_reference_candidate_view *)&(*entries_reference)[*count])->location + 0xe) >> 5) & 1);
                                ((s_reference_candidate_view *)&(*entries_reference)[*count])->flag59 =
                                    !(bool)(((dword)*((byte *)((s_reference_candidate_view *)&(*entries_reference)[*count])->location + 0xe) >> 6) & 1);
                                ++*count;
                            }
                        }
                    }
                    if (context && context->unknown56)
                        function_29db90((short)*range_reference, (short)*block_reference, (s_29db90 *)((byte *)context + 0x6a0));
                }
            }
        }
    }
done:
    return **count_reference;
}

void function_260060(long actor_index, long mode, s_type_967e20 *context, s_type_b36ac5 *position);
bool function_2600a0(long actor_index, s_type_967e20 *context, s_type_b36ac5 *position);
void __stdcall function_25f7b0(long actor_index, s_type_967e20 *context, s_type_b36ac5 *position);

// @retail 0x260160
bool function_260160(long actor_index, s_prop_search *search, s_261d20_entry *entry)
{
    s_type_b36ac5 *position = (s_type_b36ac5 *)entry;
    s_type_967e20 *context = (s_type_967e20 *)search;
    long count = *(long *)((byte *)g_4e0348 + 0xc4);
    long *data = NULL;
    if (count > 0)
        data = *(long **)((byte *)g_4e0348 + 0xc8);
    position->score = 0.0f;
    position->unknown50 = 0.0f;
    position->unknown4c = true;
    position->unknown4d = false;
    if (!context->unknown56 && (!data || position->definition->unknown14 < 0 || position->definition->unknown14 >= *data))
    {
        position->unknown4c = false;
        position->unknown4d = true;
    }
    if (position->unknown4c)
    {
        function_260060(actor_index, 1, context, position);
        if (position->unknown4c)
        {
            if (context->unknown618)
                function_25f7b0(actor_index, context, position);
            position->unknown50 = position->score;
            position->unknown4c = function_2600a0(actor_index, context, position);
        }
    }
    return position->unknown4c;
}

byte *ai_scratch_buffer_get(void);
void ai_scratch_buffer_release(byte *buffer);
long function_1e4a50(long index);
bool function_29e050(byte *point, long target_index, s_type_d4fbfa *definition, s_reference reference, long *list);

struct s_squad_iterator
{
    short squad_index;
    short current;
    short next;
    short palette_index;
    word flags;
    bool flag_a;
    bool flag_b;
    bool flag_c;
    byte unknown0d[3];
    void *definition;
};

void function_204ec0(s_squad_iterator *iterator, short squad_index, short flags, short mode);
short function_205010(s_squad_iterator *iterator);

struct s_candidate_position
{
    s_type_c3b527 point;
    byte unknown0e[6];
    long sector;
    byte unknown18[8];
};

struct s_candidate_positions_block
{
    byte unknown00[0x28];
    long count;
    s_candidate_position *positions;
    long range_count;
    s_candidate_range *ranges;
};

// @retail 0x2601f0
bool function_2601f0(long actor_index, s_type_c3b527 const *point, long sector, real radius, real path_distance)
{
    s_type_c3b527 const *const *point_reference = &point;
    long const *sector_reference = &sector;
    real const *radius_reference = &radius;
    real const *path_distance_reference = &path_distance;
    s_actor_view *actor = actor_get(actor_index);
    long *pathfinding = NULL;
    if (*(long *)((byte *)g_4e0348 + 0xc4) > 0)
        pathfinding = *(long **)((byte *)g_4e0348 + 0xc8);
    bool result = false;
    if (actor->unknown030 != NONE &&
        ((*sector_reference >= 0 && *sector_reference < *pathfinding) || *((bool *)actor + 0x229)))
    {
        byte *buffer = ai_scratch_buffer_get();
        if (!*((bool *)actor + 0x229) && *path_distance_reference > 0.0f)
        {
            s_path_source source;
            memset(&source, 0, sizeof(source));
            source.radius = *(real *)((byte *)function_1e4a50(actor->unknown054) + 4);
            source.object_index = NONE;
            source.unknown0c = NONE;
            memcpy(&source.point, *point_reference, sizeof(source.point));
            source.unknown04 = true;
            source.has_point = true;
            source.unknown24 = *sector_reference;
            source.unknown45 = true;
            source.unknown48 = *path_distance_reference;
            source.unknown4c = 0.0f;
            s_path_settings settings;
            function_1f9240(actor_index, &settings);
            function_271300((s_type_f17a25 *)buffer, NULL, &settings, &source, 0);
            function_2715a0(buffer);
        }
        s_squad_iterator iterator;
        function_204ec0(&iterator, (short)actor->unknown030, 15, actor->unknown26c != NONE);
        while (function_205010(&iterator) != NONE)
        {
            s_candidate_positions_block *block = &(*(s_candidate_positions_block **)((byte *)g_4e0350 + 0x16c))[(word)iterator.palette_index];
            s_candidate_range *range = &block->ranges[iterator.current];
            for (short index = range->first; index < range->first + range->count; ++index)
            {
                if (index >= 0 && index < block->count)
                {
                    s_candidate_position *candidate = &block->positions[index];
                    vector3f delta;
                    if ((*point_reference)->output_index == candidate->point.output_index)
                        vector3d_from_points3d(&(*point_reference)->point, &candidate->point.point, &delta);
                    else
                    {
                        point3f start, end;
                        function_210850(*point_reference, &start);
                        function_210850(&candidate->point, &end);
                        vector3d_from_points3d(&start, &end, &delta);
                    }
                    if (*radius_reference * *radius_reference > delta.k * delta.k + delta.j * delta.j + delta.i * delta.i)
                    {
                        if (*((bool *)actor + 0x229))
                        {
                            point3f raised;
                            function_210850(*point_reference, &raised);
                            raised.z += 0.10000000149011612f;
                            if (function_29e050((byte *)&raised, actor->unknown018, (s_type_d4fbfa *)candidate, g_470fa0, NULL))
                            {
                                result = true;
                                goto done;
                            }
                        }
                        else if (*path_distance_reference <= 0.0f)
                        {
                            result = true;
                            goto done;
                        }
                        else
                        {
                            real distance;
                            function_270750(buffer, candidate->sector, (s_actor_point_target *)candidate, &distance, 0, 0);
                            if (*path_distance_reference > distance)
                            {
                                result = true;
                                goto done;
                            }
                        }
                    }
                }
            }
        }
done:
        ai_scratch_buffer_release(buffer);
    }
    return result;
}


struct s_candidate_range_reference
{
    word type;
    word unknown02;
    short block_index;
    short range_index;
};

struct s_candidate_range_list
{
    long count;
    s_candidate_range_reference *entries;
};

// @retail 0x261ec0
bool __stdcall function_261ec0(long actor_index, long squad_index, s_261d20_entry *entries,
    short *count, long maximum_count, s_prop_search *search)
{
    long const *actor_reference = &actor_index;
    long const *squad_reference = &squad_index;
    s_261d20_entry *const *entries_reference = &entries;
    short *const *count_reference = &count;
    long const *maximum_reference = &maximum_count;
    s_prop_search *const *search_reference = &search;
    byte *squad = g_51e9d8->data + (*squad_reference & 0xffff) * 0x98;
    bool result = false;
    if (*(long *)(squad + 0x80) != NONE)
    {
        if (*(char *)(squad + 0x30) >= 0)
        {
            result = true;
            for (short index = 0; index < *(char *)(squad + 0x30); ++index)
                function_262180(*actor_reference, ((short *)(squad + 0x32))[index * 2],
                    ((short *)(squad + 0x34))[index * 2], *entries_reference, *count_reference,
                    *maximum_reference, *search_reference, 0);
        }
    }
    else if (*(short *)(squad + 0x2a) != NONE)
    {
        byte *definition = *(byte **)((byte *)g_4e0350 + 0x244) + *(short *)(squad + 0x2a) * 0x7c;
        if ((*(byte *)(definition + 0x24) & 0x20) && *(short *)(definition + 0x4e) != NONE)
        {
            result = function_261ec0(*actor_reference, *(short *)(definition + 0x4e),
                *entries_reference, *count_reference, *maximum_reference, *search_reference);
            return result;
        }
        s_actor_view *actor = actor_get(*actor_reference);
        s_candidate_range_list *list = *(volatile byte *)(squad + 0x60) ? (s_candidate_range_list *)(definition + 0x5c) : (s_candidate_range_list *)(definition + 0x54);
        short flags = *((bool *)actor + 0x3f2) ? 6 : 4;
        if (!*search_reference || !*((bool *)*search_reference + 0x69c)) flags |= 1;
        if ((flags & 4) && !(flags & 1))
        {
            short index;
            for (index = 0; index < list->count; ++index)
            {
                s_candidate_range_reference *reference = &(*(volatile byte *)(squad + 0x60) ? (s_candidate_range_list *)(definition + 0x5c) :
                    (s_candidate_range_list *)(definition + 0x54))->entries[index];
                if (reference->type == 2) break;
            }
            if (index >= list->count) flags |= 1;
        }
        result = true;
        if (*search_reference) *(short *)((byte *)*search_reference + 0x69e) = flags;
        for (short index = 0; index < list->count; ++index)
        {
            s_candidate_range_reference *reference = &(*(volatile byte *)(squad + 0x60) ? (s_candidate_range_list *)(definition + 0x5c) :
                (s_candidate_range_list *)(definition + 0x54))->entries[index];
            word type = reference->type;
            if (flags & (1 << type))
                function_262180(*actor_reference, reference->block_index, reference->range_index,
                    *entries_reference, *count_reference, *maximum_reference, *search_reference, (short)type);
        }
    }
    else
    {
        short block_index = *(short *)(*(byte **)((byte *)g_4e0350 + 0x164) +
            (*squad_reference & 0xffff) * 0x74 + 0x38);
        if (block_index != NONE)
        {
            s_candidate_block *block = &(*(s_candidate_block **)((byte *)g_4e0350 + 0x16c))[(word)block_index];
            for (short index = 0; index < *(long *)((byte *)block + 0x30); ++index)
                function_262180(*actor_reference, block_index, index, *entries_reference,
                    *count_reference, *maximum_reference, *search_reference, 0);
        }
    }
    return result;
}

#include "unknown_2605d0.h"

// @retail 0x261d20
short __stdcall function_261d20(long actor_index, s_261d20_entry *entries, long maximum_count,
    s_2605d0_request const *request)
{
    s_actor_view *actor = actor_get(actor_index);
    long count = 0;
    if (actor->unknown030 != NONE)
        function_261ec0(actor_index, actor->unknown030, entries, (short *)&count, maximum_count, (s_prop_search *)request);
    long list_index = *(long *)((byte *)actor + 0x3f4);
    if (list_index != NONE)
    {
        if (request && *(short const *)request == 4 && actor->unknown030 != NONE && (short)count > 0)
        {
            byte *squad = g_51e9d8->data + (actor->unknown030 & 0xffff) * 0x98;
            short definition_index = *(short *)(squad + 0x2a);
            if (definition_index != NONE && !*((bool *)actor + 0x3c))
            {
                byte *definition = *(byte **)((byte *)g_4e0350 + 0x244) + definition_index * 0x7c;
                dword flags = *(dword *)(definition + 0x24);
                if (((flags & 0x10) || ((flags & 0x20) && *(short *)(definition + 0x4e) != NONE)) &&
                    *(long *)(squad + 0x80) != NONE)
                    return count;
            }
        }
        byte *list = g_51eca4->data + (list_index & 0xffff) * 0x484;
        for (short index = 0; index < *(short *)(list + 8) && (short)count < maximum_count; ++index)
        {
            s_reference reference = { index, (word)(list_index | 0x8000) };
            if (function_2623a0((s_reference_filter_view const *)request, actor_index, actor, reference) &&
                function_2624d0(&entries[(short)count], reference))
            {
                if (*(short *)(list + 2) == 1)
                    *(short *)((byte *)&entries[(short)count] + 0x5c) = ((short *)(list + 0x444))[index];
                ++count;
            }
        }
    }
    return count;
}
