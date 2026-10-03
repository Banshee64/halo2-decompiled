// @flags /O2 /Gr
/* UNKNOWN_2116F0.CPP: listing the seats of a unit that pass a filter (outside
   functions lane A's AI script functions need) */

#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"
#include "units.h"

/* the scenario's seat filters (g_4e0350 +0x228): an object definition and a
   bit vector of its seats */
struct s_seat_filter
{
	long definition_index;
	dword seats[1];
};

struct s_scenario_seat_filters_view
{
	byte unknown000[0x22c];
	s_seat_filter *seat_filters;
};

// @retail 0x211830
bool function_211830(long filter_range, long object_index, long seat_index)
{
	long const *seat_reference = &seat_index;
	bool result = false;
	long definition_index = object_get(object_index)->tag_index;

	if (filter_range == NONE)
		return true;

	long first = filter_range & 0xffff;
	long count = (dword)filter_range >> 16;
	for (long i = 0; i < count; i++)
	{
		s_seat_filter *filter = &((s_scenario_seat_filters_view *)g_4e0350)->seat_filters[first + i];

		if (filter->definition_index == definition_index)
		{
			result = (filter->seats[*seat_reference >> 5] & (1 << (*seat_reference & 0x1f))) != 0;
			break;
		}
	}

	return result;
}

// @retail 0x2116f0
short function_2116f0(long unit_index, long filter_range, long seat_type, long occupancy, s_object_seat *results, long maximum_count)
{
	long result_count = 0;
	s_object_seat seats[64];
	short seat_count = 0;

	function_c8a40(unit_index, seats, &seat_count, 64);
	for (long i = 0; i < seat_count; i++)
	{
		s_object_seat *seat = &seats[i];
		bool valid = true;

		if (result_count >= maximum_count)
			break;

		if (!function_211830(filter_range, seat->object_index, seat->seat_index))
			continue;

		switch (seat_type)
		{
		case 0:
			valid = !TEST_FIELD_BIT(seat->definition->flags.bit2);
			break;
		case 1:
			valid = TEST_FIELD_BIT(seat->definition->flags.bit3);
			break;
		case 2:
			valid = !TEST_FIELD_BIT(seat->definition->flags.bit2) && !TEST_FIELD_BIT(seat->definition->flags.bit3);
			break;
		case 3:
			valid = TEST_FIELD_BIT(seat->definition->flags.bit2);
			break;
		}
		if (!valid)
			continue;

		switch (occupancy)
		{
		case 1:
			valid = unit_seat_get_occupant(seat->object_index, seat->seat_index) != NONE;
			break;
		case 2:
			valid = unit_seat_get_occupant(seat->object_index, seat->seat_index) == NONE;
			break;
		}
		if (!valid)
			continue;

		*results++ = *seat;
		result_count++;
	}

	return (short)result_count;
}
