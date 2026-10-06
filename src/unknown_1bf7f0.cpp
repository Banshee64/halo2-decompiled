// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "slot_handler.h"

/* the vehicle helpers of the slot types from 0x1bf990 */

struct s_seat_flags
{
	dword unknown0 : 11;
	dword bit11 : 1;
	dword unknown12 : 20;
};

/* a seat of a vehicle's tag (0xb0 bytes) */
struct s_vehicle_seat
{
	s_seat_flags flags;
	byte unknown04[0x3e - 0x4];
	short unknown3e;
	byte unknown40[0xb0 - 0x40];
};

struct s_vehicle_tag
{
	byte unknown000[0x1c8];
	long seat_count;
	s_vehicle_seat *seats;
};

bool function_1b8eb0(long actor_index, long object_index, short seat_index, bool ignore_reserved);

struct s_seat_approach_result
{
	point3f point;
	vector3f direction;
	real score;
	bool close;
	bool facing;
	bool approaching;
};

bool function_1baae0(long actor_index, long object_index, short seat_index, bool ignore_bonus,
	bool ignore_reserved, bool vertical, s_seat_approach_result *out);

// @retail 0x1bf890
short function_1bf890(long actor_index, long object_index, short seat_index, bool vertical)
{
	s_vehicle_tag *tag = (s_vehicle_tag *)g_4e3b44[object_get(object_index)->tag_index & 0xffff].bytes;
	real best_score = 0.0f;
	short result = NONE;
	for (short i = 0; i < tag->seat_count; i++)
	{
		if (i != seat_index)
		{
			s_vehicle_seat *seat = &tag->seats[i];
			if (TEST_FIELD_BIT(seat->flags.bit11) && (seat->unknown3e == seat_index || seat->unknown3e == NONE))
			{
				s_seat_approach_result approach;
				if (function_1baae0(actor_index, object_index, i, false, false, vertical, &approach))
				{
					if (seat->unknown3e == NONE)
						approach.score *= 0.5f;
					if (approach.score > best_score)
					{
						best_score = approach.score;
						result = i;
					}
				}
			}
		}
	}
	return result;
}

/* whether the actor can take a seat of the vehicle other than seat_index
   (one tied to it, or to no seat) */
// @retail 0x1bf7f0
bool function_1bf7f0(long actor_index, short seat_index, long object_index)
{
	s_vehicle_tag *tag = (s_vehicle_tag *)g_4e3b44[object_get(object_index)->tag_index & 0xffff].bytes;
	bool result = false;

	for (short i = 0; i < tag->seat_count; i++)
	{
		if (i != seat_index)
		{
			s_vehicle_seat *seat = &tag->seats[i];

			if (TEST_FIELD_BIT(seat->flags.bit11) && (seat->unknown3e == seat_index || seat->unknown3e == NONE) &&
				function_1b8eb0(actor_index, object_index, i, false))
			{
				result = true;
				break;
			}
		}
	}
	return result;
}
