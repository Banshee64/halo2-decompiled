// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_2626b0.h"
#include "unknown_1e3920.h"
#include "unknown_2605d0.h"

/* slot type 0x3a (its first callbacks, from 0x1afde0, precede the region) */

struct s_slot_3a
{
	s_slot_header header;
	byte unknown0c[4];
	s_reference reference;
	byte unknown14;
	bool unknown15;
	bool unknown16;
	byte unknown17[0x40 - 0x17];
};

short __stdcall function_1afde0(long actor_index);
bool __stdcall function_1afe50(long actor_index, s_slot *slot);
void __stdcall function_1c0b60(long actor_index, s_slot *slot, long index);
long __stdcall function_1aff10(long actor_index, s_slot *slot);
void __stdcall function_1b0020(long actor_index, s_slot *slot);
void __stdcall function_1b0110(long actor_index, s_slot *slot);

// @retail 0x1b0270
short __stdcall function_1b0270(long actor_index, s_slot *slot, bool active)
{
	s_slot_3a *state = (s_slot_3a *)slot;
	short result = g_46fbe8;

	if (state->unknown15 || state->unknown16)
	{
		g_46eeb8[0x3a]->unknown8 = g_46f348;
		result = g_46fbe4;
	}
	return result;
}

// @retail 0x1b06f0
void __stdcall function_1b06f0(long actor_index, s_slot *slot, bool active)
{
	s_slot_3a *state = (s_slot_3a *)slot;

	if (!active || (state->reference.unknown2 & 0x8000))
		state->reference = g_470fa0;
}

s_slot_handler_2 g_47dec0 =
{
	{
		0x3a, 2, 0, -2, 0,
		function_1afde0, function_1b0270, function_1afe50, slot_proc_nothing, 0x38, {0},
		function_1c0b60, 0, 0, 0, function_1b06f0, 0, 0
	},
	(t_slot_proc)function_1aff10, function_1b0020, function_1b0110
};

/* Whether the actor has arrived at its reference and is free of a waiting prop. */
// @retail 0x1b02a0
bool function_1b02a0(long actor_index)
{
	s_reference invalid_reference = g_470fa0;
	s_record_pool *actors = g_4f55f0;
	s_actor_view *actor = (s_actor_view *)(actors->data + (actor_index & 0xffff) * sizeof(s_actor_view));
	bool result = false;

	if (!REFERENCE_EQUAL(actor->unknown418, invalid_reference))
	{
		s_type_c3b527 *target = (s_type_c3b527 *)function_262b40(actor->unknown418);

		if (target)
		{
			if (((s_actor_view *)actors->data)[actor_index & 0xffff].unknown504 == 2 &&
				(actor->unknown4ac == 4 || actor->unknown4ac == 6 || actor->unknown4ac == 5) &&
				REFERENCE_EQUAL(actor->unknown4b8, actor->unknown418))
			{
				return true;
			}

			volatile real radius = function_1e3920(actor_index);
			point3f point;
			vector3f offset;

			function_210850(target, &point);
			vector3d_from_points3d(&actor->position, &point, &offset);
			real distance_squared = offset.j * offset.j + offset.i * offset.i + offset.k * offset.k;
			real distance = radius;
			if (distance * distance > distance_squared)
			{
				if (actor->unknown344 == NONE)
					return true;
				bool waiting = false;
				s_prop_view_fields *view = prop_view_fields_get(actor->unknown344);

				if (view && (view->unknown06 == 0 || view->unknown06 == 1))
					waiting = true;
				return !waiting;
			}
		}
	}
	return result;
}

struct s_follow_search_state
{
	short count;
	bool flag2;
	bool flag3;
	s_reference reference;
	bool flag8;
	byte unknown09[0x10 - 9];
	long actor_index;
};

struct s_follow_search_fields
{
	byte unknown00[0x20];
	bool use_point;
	byte unknown21[3];
	point3f point;
	s_type_c3b527 location;
	long location_index;
	short value44;
	bool flag46;
	byte unknown47;
	real distance;
	real minimum;
	byte unknown50[2];
	bool flag52;
};

void function_26c180(long actor_index);

// @retail 0x1b0540
void function_1b0540(long actor_index, s_follow_search_state *state)
{
	s_2605d0_request request;
	memset(&request, 0, sizeof(request));
	s_actor_view *actor = actor_get(actor_index);
	s_follow_search_fields *fields = (s_follow_search_fields *)&request;
	fields->distance = 10.0f;
	fields->minimum = 3.0f;
	request.type = 1;
	fields->flag46 = true;
	request.unknown008 = 4.0f;
	request.unknown00c = 4.0f;
	request.unknown010 = 7.0f;
	if (actor->unknown344 != NONE)
	{
		fields->flag52 = state->flag2;
	}
	else if (state->actor_index != NONE)
	{
		function_26c180(state->actor_index);
		s_actor_view *other = actor_get(state->actor_index);
		s_slot_object_view *unit = object_get(other->unknown018);
		fields->use_point = true;
		fields->point = other->position;
		fields->location_index = other->unknown27c.unknown10;
		fields->location = other->unknown27c.point;
		fields->value44 = *(short *)((byte *)unit + 0x2c);
	}
	if (state->count > 0)
	{
		request.unknown014 = true;
		request.unknown015 = true;
	}
	byte *scratch = ai_scratch_buffer_get();
	s_261d20_entry entry;
	long other_index;
	bool unknown;
	state->reference = function_2605d0(actor_index, &request, (long)&entry, (long)&other_index, scratch, &unknown);
	state->reference = function_2626b0(actor_index, state->reference, other_index, scratch, unknown, false);
	state->flag8 = !REFERENCE_EQUAL(state->reference, g_470fa0) && !unknown;
	state->flag3 = false;
	ai_scratch_buffer_release(scratch);
}


bool function_114b60(short entry_index, short fallback_index, long unit_index, long priority, void const *extra);

// @retail 0x1b0020
void __stdcall function_1b0020(long actor_index, s_slot *slot)
{
    s_actor_view *actor = actor_get(actor_index);
    ++*(long *)((byte *)slot + 0x18);
    if (*(short *)((byte *)slot + 0xc) > 0)
        --*(short *)((byte *)slot + 0xc);
    long unit_index = actor->unknown018;
    byte *unit = (byte *)object_get(unit_index);
    if (*(short *)(unit + *(short *)(unit + 0x342) + 0xc) <= 0)
    {
        if (*(short *)((byte *)slot + 0x22) > 0)
            --*(short *)((byte *)slot + 0x22);
        else
        {
            function_114b60(NONE, 0xc, unit_index, 0xd, 0);
            *(short *)((byte *)slot + 0x22) = (short)real_to_long(slot_random_range(0.5f, 1.5f) * g_510c54->field_2_3);
        }
    }
}
