// @flags /O2 /arch:SSE /Ob1 /Gr
/* UNKNOWN_107370.CPP: the device power setter of the script functions.
   Retail calls it out of line from its three callers (0xa3fd0, 0xcfc90 and
   the script function 0x2a48c0), so it sits in its own /Ob1 file apart
   from devices.cpp. */

#include "cseries.h"
#include "globals.h"

/* the devices (a local view of the object data, as in devices.cpp) */
struct s_device_107370
{
	byte unknown000[0x12c];
	dword flags;
	byte unknown130[0x13c - 0x130];
	long power_group_index;
};

struct s_device_107370_header
{
	byte unknown00[8];
	s_device_107370 *device;
};

void function_b7360(long object_index);
void function_107430(long group_index, real value);

// @retail 0x107370
void function_107370(long device_index, real value)
{
	if (device_index != NONE)
	{
		s_device_107370 *device = ((s_device_107370_header *)g_4e0300->data)[device_index & 0xffff].device;
		if (device->power_group_index != NONE)
			function_107430(device->power_group_index, value);
		device->flags |= 0x80;
		function_b7360(device_index);
	}
}
