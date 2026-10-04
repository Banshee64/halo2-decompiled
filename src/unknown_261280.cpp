// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_261280.CPP: choosing a reference for an actor to follow, falling
   back to the one it already follows */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"
#include "lane_c_callees.h"

/* not decompiled yet (src/stubs/lane_c.cpp) */
bool function_2624d0(s_261d20_entry *entry, s_reference reference);
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
