// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_261280.CPP: choosing a reference for an actor to follow, falling
   back to the one it already follows */

#include "unknown_11c920.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_0259a0.h"
#include "unknown_2626b0.h"
#include <float.h>

bool function_2624d0(s_261d20_entry *entry, s_reference reference);
/* not decompiled yet (src/stubs/lane_c.cpp) */
bool function_260160(long actor_index, s_261d20_entry *entry, s_prop_search *search);

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
			if (!function_260160(actor_index, entry, search))
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
