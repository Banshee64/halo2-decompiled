// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_23BC90.CPP: the cameras that follow a unit: the first person
   camera, the camera of a unit's seat and the following camera
   (0x23bc90..0x23cea0) */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "globals.h"
#include "data_array.h"

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);

/* a unit as its camera sees it */
struct s_camera_unit_view
{
	long tag_index;
	byte unknown04[0x10];
	long parent_index;
	byte unknown18[0x1fc - 0x18];
	short seat_index;
};

struct s_camera_object_view
{
	long tag_index;
};

/* a vehicle seat (0xb0 bytes) and the camera it gives its unit */
struct s_camera_seat_view
{
	dword flag0 : 1;
	dword flag1 : 1;
	dword flag2 : 1;
	dword flag3 : 1;
	dword flag4 : 1;
	byte unknown04[0x60 - 4];
	byte camera[0xb0 - 0x60];
};

struct s_camera_vehicle_tag_view
{
	byte unknown000[0x1c8];
	long seat_count;
	s_camera_seat_view *seats;
};

struct s_camera_unit_tag_view
{
	byte unknown00[0xd4];
	byte camera[4];
};

struct s_camera_object_header_view
{
	byte unknown00[8];
	s_camera_unit_view *object;
};

/* the camera of the unit: its seat's when the seat has one, else its own */
// @retail 0x23c270
void *function_23c270(long unit_index)
{
	s_camera_unit_view *unit = ((s_camera_object_header_view *)g_4e0300->data)[unit_index & 0xffff].object;
	s_tag_instance *tags = g_4e3b44;
	void *camera = NULL;

	if (unit->parent_index != NONE)
	{
		s_camera_object_view *vehicle = (s_camera_object_view *)function_badc0(unit->parent_index, 2);

		if (vehicle)
		{
			s_camera_vehicle_tag_view *definition = (s_camera_vehicle_tag_view *)tags[vehicle->tag_index & 0xffff].flags;
			s_camera_seat_view *seat = &definition->seats[unit->seat_index];

			if (TEST_FIELD_BIT(seat->flag2) || TEST_FIELD_BIT(seat->flag0) || TEST_FIELD_BIT(seat->flag4))
			{
				camera = seat->camera;
			}
		}
	}
	if (!camera)
	{
		camera = ((s_camera_unit_tag_view *)tags[unit->tag_index & 0xffff].flags)->camera;
	}
	return camera;
}
