// @flags /O2 /Ob1 /Gr
/* UNKNOWN_107590.CPP: device flag setters of the script functions */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_107590.h"

/* the devices (a local view of the object data) */
struct s_device_107590
{
	byte unknown000[0x1cc];
	dword flags;
};

struct s_object;
s_object *function_badc0(long object_index, dword type_mask);
void function_b7360(long object_index);

// @retail 0x107590
void function_107590(long device_index, bool flag)
{
	s_device_107590 *device = (s_device_107590 *)function_badc0(device_index, 0x80);
	if (device)
	{
		if (!flag)
			device->flags |= 1;
		else
			device->flags &= ~1;
		function_b7360(device_index);
	}
}

// @retail 0x1075e0
void function_1075e0(long device_index, bool flag)
{
	s_device_107590 *device = (s_device_107590 *)function_badc0(device_index, 0x80);
	if (device)
	{
		if (!flag)
			device->flags |= 2;
		else
			device->flags &= ~2;
		function_b7360(device_index);
	}
}