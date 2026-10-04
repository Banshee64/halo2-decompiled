/* UNIT_REQUESTS.H: the requests sent to a unit (src/unknown_0e6900.cpp) */

#ifndef UNIT_REQUESTS_H
#define UNIT_REQUESTS_H

#include "cseries.h"
#include "real_math.h"

/* the request function_e6900 passes to the actor's unit: its type, then
   arguments by type (0x20 bytes) */
struct s_unit_request
{
	long type;
	union
	{
		struct
		{
			short unknown4;
			bool unknown6;
		} type1a;
		struct
		{
			bool unknown4;
			bool unknown5;
		} type17;
		struct
		{
			bool unknown4;
		} type1d;
		struct
		{
			bool unknown4;
		} type20;
		struct
		{
			bool has_vector;
			byte unknown5[3];
			real_vector3d vector;
		} type35;
		struct
		{
			byte mode;
			byte unknown5[3];
			long animation;
			bool has_target;
			byte unknownd[3];
			long target[2];
		} type19;
		struct
		{
			real_point3d point;
			real_vector3d facing;
			short unknown1c;
		} type25;
		struct
		{
			long object_index;
			short seat_index;
			bool unknowna;
			bool unknownb;
		} type1c;
		byte arguments[0x20 - 0x4];
	};
};

bool function_e6900(long unit_index, s_unit_request *request);
/* a request with no arguments */
bool function_e68c0(long type, long unit_index);

#endif
