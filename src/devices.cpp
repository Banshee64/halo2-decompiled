// @flags /O2 /arch:SSE /Gr
/* DEVICES.CPP: devices (object types 7..9: machines, controls and light
   fixtures) and the device groups that drive their position and power.
   device_groups_initialize and device_groups_dispose (0x106460, 0x106490)
   are in unknown_1061c0.cpp, the script flag setters (0x107590, 0x1075e0)
   in unknown_107590.cpp. */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "object_iterator.h"
#include "unknown_1c62f0.h"

#define DEVICE_TYPE_MASK 0x380

/* a device group (12 bytes, in g_4e0328.groups) */
struct s_device_group
{
	short identifier;
	word flags;
	real value;
	real desired_value;
};

/* the scenario's device groups (a view of g_4e0350) */
struct s_scenario_device_group
{
	byte unknown00[0x20];
	real initial_value;
	byte flags;
	byte unknown25[3];
};

struct s_scenario_device_groups_view
{
	byte unknown00[0xa0];
	long device_group_count;
	s_scenario_device_group *device_groups;
};

/* the device definition (the tag data) */
struct s_device_definition
{
	byte unknown000[0x100];
	long tag_index_100;
	byte unknown104[4];
	long tag_index_108;
};

/* the device (the object data) */
struct s_device
{
	long definition_index;
	byte unknown004[8];
	long next_object_index;
	long first_child_index;
	byte unknown014[0xaa - 0x14];
	byte type;
	byte unknown0ab[0xc2 - 0xab];
	short location_c2;
	long location_c4;
	long location_c8;
	byte unknown0cc[0xd4 - 0xcc];
	long value_d4;
	byte unknown0d8[0x12c - 0xd8];
	dword flags;
	long position_group_index;
	real position;
	real position_velocity;
	long power_group_index;
	real power;
	real power_velocity;
	byte unknown148[4];
	real value_14c;
	real value_150;
	real value_154;
	real value_158;
	real value_15c;
	real value_160;
	real value_164;
	real value_168;
	real value_16c;
	real value_170;
	real value_174;
	real value_178;
	real value_17c;
	real value_180;
	real value_184;
	byte unknown188[4];
	c_animation_channel channels[2];
};

struct s_device_header
{
	byte unknown00[8];
	s_device *device;
};

/* the location the effects and sounds of a device start from */
struct s_device_location
{
	long value_c4;
	long value_c8;
	short value_c2;
};

struct s_tag_group_view
{
	dword group_tag;
};

/* an iteration over the devices: the current device, then the object
   iterator */
struct s_device_iterator
{
	s_device *device;
	s_object_iterator iterator;
};

static inline void device_iterator_new(s_device_iterator *iterator)
{
	function_bae80(&iterator->iterator, DEVICE_TYPE_MASK, 0);
}

static inline bool device_iterator_next(s_device_iterator *iterator)
{
	iterator->device = (s_device *)function_baeb0(&iterator->iterator);
	return iterator->device != NULL;
}

#define DEVICE_GET(index) (((s_device_header *)g_4e0300->data)[(index) & 0xffff].device)
#define DEVICE_GROUP_GET(index) (&((s_device_group *)g_4e0328.groups->data)[(index) & 0xffff])

void function_b7360(long object_index);
void function_b58c0(long index, dword mask);
long function_189060(long object_index, short value, real scale, real_point3d const *position, real_vector3d const *direction, long tag_index);
void function_176780(long object_index, real_vector3d const *velocity, real scale_a, long tag_index, real scale_b, real_point3d const *origin, real_vector3d const *direction);
void device_groups_initialize();
void device_groups_dispose();

static inline void data_make_valid_inlined(s_data_array *data)
{
	data->valid = true;
	data_delete_all(data);
}

static inline void data_make_invalid_inlined(s_data_array *data)
{
	data->valid = false;
}

void __stdcall function_107520(long object_index);
void function_107a30(void);

// @retail 0x1064e0
void function_1064e0(void)
{
	data_make_valid_inlined(g_4e0328.groups);
	function_107a30();
}

// @retail 0x106500
void function_106500(void)
{
	data_make_invalid_inlined(g_4e0328.groups);
}

// @retail 0x106680
void __stdcall function_106680(long device_index)
{
	s_device *device = DEVICE_GET(device_index);

	if (device->position_group_index != NONE && (DEVICE_GROUP_GET(device->position_group_index)->flags & 4))
		datum_delete(g_4e0328.groups, device->position_group_index);
	device->position_group_index = NONE;
	if (device->power_group_index != NONE && (DEVICE_GROUP_GET(device->power_group_index)->flags & 4))
		datum_delete(g_4e0328.groups, device->power_group_index);
	device->power_group_index = NONE;
	device->value_14c = 0.0f;
	device->value_150 = 0.0f;
	device->value_154 = 0.0f;
	device->value_158 = 0.0f;
	device->value_164 = 0.0f;
	device->value_15c = 0.0f;
	device->value_160 = 0.0f;
	device->value_16c = 0.0f;
	device->value_170 = 0.0f;
	device->value_174 = 0.0f;
	device->value_178 = 0.0f;
	device->value_184 = 0.0f;
	device->value_17c = 0.0f;
	device->value_180 = 0.0f;
}

// @retail 0x106510
bool __stdcall function_106510(long device_index, long a, long b)
{
	s_device *device = DEVICE_GET(device_index);

	device->power_group_index = NONE;
	device->position_group_index = NONE;
	device->channels[0].reset();
	device->channels[1].reset();
	device->value_14c = 0.0f;
	device->value_150 = 0.0f;
	device->value_154 = 0.0f;
	device->value_158 = 0.0f;
	device->value_164 = 0.0f;
	device->value_15c = 0.0f;
	device->value_160 = 0.0f;
	device->value_16c = 0.0f;
	device->value_170 = 0.0f;
	device->value_174 = 0.0f;
	device->value_178 = 0.0f;
	device->value_184 = 0.0f;
	device->value_17c = 0.0f;
	device->value_180 = 0.0f;
	return true;
}

// @retail 0x106780
void __stdcall function_106780(long device_index)
{
	function_106680(device_index);
}

// @retail 0x1070d0
long function_1070d0(short group_index)
{
	long result = NONE;
	long pinned = group_index < 0 ? 0 : (group_index > ((s_scenario_device_groups_view *)g_4e0350)->device_group_count - 1 ? ((s_scenario_device_groups_view *)g_4e0350)->device_group_count - 1 : group_index);

	if (pinned == group_index)
	{
		result = data_datum_index(g_4e0328.groups, group_index);
		if (DEVICE_GROUP_GET(result)->flags & 4)
			result = NONE;
	}
	return result;
}

// @retail 0x107430
void function_107430(long group_index, real value)
{
	if (group_index != NONE)
	{
		if (0.0f > value)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;
		DEVICE_GROUP_GET(group_index)->value = value;

		s_device_iterator iterator;
		device_iterator_new(&iterator);
		while (device_iterator_next(&iterator))
		{
			s_device *device = iterator.device;

			if (device->position_group_index == group_index)
			{
				function_107520(iterator.iterator.object_index);
				device->position = value;
				device->position_velocity = 0.0f;
				function_b7360(iterator.iterator.object_index);
			}
			if (device->power_group_index == group_index)
			{
				function_107520(iterator.iterator.object_index);
				device->power = value;
				device->power_velocity = 0.0f;
				function_b7360(iterator.iterator.object_index);
			}
		}
	}
}

// @retail 0x107980
void function_107980(long object_index, long tag_index)
{
	if (tag_index != NONE)
	{
		s_device *device = DEVICE_GET(object_index);
		s_device_location location;

		location.value_c4 = device->location_c4;
		location.value_c8 = device->location_c8;
		location.value_c2 = device->location_c2;
		switch (((s_tag_group_view *)&g_4e3b44[(short)tag_index])->group_tag)
		{
		case 'effe':
			function_176780(object_index, (real_vector3d const *)&location, device->power, tag_index, device->position, NULL, NULL);
			break;
		case 'snd!':
			function_189060(object_index, NONE, 1.0f, g_468788, g_4687a8, tag_index);
			break;
		}
	}
}

// @retail 0x1071e0
bool __stdcall function_1071e0(long group_index, real value)
{
	bool result = false;

	if (0.0f > value)
		value = 0.0f;
	else if (value > 1.0f)
		value = 1.0f;
	if (group_index != NONE)
	{
		s_device_group *group = DEVICE_GROUP_GET(group_index);

		if (group->value != value && (!(group->flags & 1) || !(group->flags & 2)))
		{
			group->flags |= 2;
			group->value = value;
			result = true;

			s_device_iterator iterator;
			device_iterator_new(&iterator);
			while (device_iterator_next(&iterator))
			{
				s_device *device = iterator.device;
				s_device_definition *definition = (s_device_definition *)g_4e3b44[device->definition_index & 0xffff].bytes;

				if (device->position_group_index == group_index)
				{
					function_107980(iterator.iterator.object_index, value > 0.0f ? definition->tag_index_108 : definition->tag_index_100);
					function_b7360(iterator.iterator.object_index);
				}
				if (device->power_group_index == group_index)
				{
					s_device *object = DEVICE_GET(iterator.iterator.object_index);
					if (object->value_d4 != NONE)
						function_b58c0(object->value_d4, 0x800);
					function_b7360(iterator.iterator.object_index);
				}
			}
		}
	}
	return result;
}

// @retail 0x107140
bool function_107140(long device_index, real value)
{
	bool result = false;

	if (device_index != NONE)
	{
		s_device *device = DEVICE_GET(device_index);
		if (device->power_group_index != NONE)
			result = function_1071e0(device->power_group_index, value);
		function_b7360(device_index);
	}
	return result;
}

// @retail 0x107190
void function_107190(long device_index, real value)
{
	if (device_index != NONE)
	{
		s_device *device = DEVICE_GET(device_index);
		function_107520(device_index);
		device->position = value;
		function_b7360(device_index);
		function_1071e0(device->position_group_index, value);
	}
}

// @retail 0x107370
void function_107370(long device_index, real value)
{
	if (device_index != NONE)
	{
		s_device *device = DEVICE_GET(device_index);
		if (device->power_group_index != NONE)
			function_107430(device->power_group_index, value);
		device->flags |= 0x80;
		function_b7360(device_index);
	}
}

// @retail 0x1073c0
void function_1073c0(void)
{
	s_data_iterator iterator;
	s_device_group *group;

	iterator.data = g_4e0328.groups;
	iterator.index = NONE;
	while ((group = (s_device_group *)data_iterator_next_inlined(&iterator)) != NULL)
	{
		function_107430(iterator.datum_index, group->desired_value);
		group->flags &= ~2;
	}
}

// @retail 0x107520
void __stdcall function_107520(long object_index)
{
	s_device *device = DEVICE_GET(object_index);

	if ((1 << device->type) & DEVICE_TYPE_MASK)
	{
		device->flags |= 4;
		function_b7360(object_index);
	}
	for (long child_index = device->first_child_index; child_index != NONE; child_index = DEVICE_GET(child_index)->next_object_index)
		function_107520(child_index);
}

// @retail 0x107630
void function_107630(long group_index, bool flag)
{
	if (group_index != NONE)
	{
		s_device_group *group = DEVICE_GROUP_GET(group_index);

		if (flag)
			group->flags |= 1;
		else
			group->flags &= ~1;
		group->flags &= ~2;

		s_device_iterator iterator;
		device_iterator_new(&iterator);
		while (device_iterator_next(&iterator))
		{
			s_device *device = iterator.device;

			if (device->position_group_index == group_index)
				function_b7360(iterator.iterator.object_index);
			if (device->power_group_index == group_index)
				function_b7360(iterator.iterator.object_index);
		}
	}
}

// @retail 0x107870
bool function_107870(long device_index)
{
	s_device *device = DEVICE_GET(device_index);
	bool result = false;

	if (device->power_group_index != NONE)
	{
		s_device_group *power_group = DEVICE_GROUP_GET(device->power_group_index);
		s_device_group *position_group = DEVICE_GROUP_GET(device->position_group_index);
		word flags = power_group->flags;
		bool active = true;

		if ((flags & 1) && !(flags & 2))
			active = false;
		if (device->flags & 2)
			active = false;
		result = position_group->value == 1.0f && active;
	}
	return result;
}

// @retail 0x107a30
void function_107a30(void)
{
	s_scenario_device_groups_view *scenario = (s_scenario_device_groups_view *)g_4e0350;

	for (long i = 0; i < scenario->device_group_count; i++)
	{
		s_scenario_device_group *scenario_group = &scenario->device_groups[i];
		word flags = 0;
		real value;

		if (scenario_group->flags & 1)
			flags = 1;
		value = scenario_group->initial_value;

		long group_index = datum_new(g_4e0328.groups);
		if (group_index != NONE)
		{
			s_device_group *group = DEVICE_GROUP_GET(group_index);
			group->value = value;
			group->desired_value = value;
			group->flags = flags;
		}
	}
}

/* the device object type definition */
struct s_device_type_definition
{
	char const *name;
	dword group_tag;
	short datum_size;
	short unknown0a;
	long unknown0c;
	void (*initialize)(void);
	void (*dispose)(void);
	void (*initialize_for_new_map)(void);
	void (*dispose_from_old_map)(void);
	void *unknown20[3];
	bool (__stdcall *handler2c)(long, long, long);
	void *handler30;
	void (__stdcall *handler34)(long);
	void *handler38;
	void (__stdcall *handler3c)(long);
};

s_device_type_definition g_468248 =
{
	"device",
	'devi',
	0x1cc,
	NONE,
	NONE,
	device_groups_initialize,
	device_groups_dispose,
	function_1064e0,
	function_106500,
	{ 0, 0, 0 },
	function_106510,
	0,
	function_106680,
	0,
	function_106780
};
