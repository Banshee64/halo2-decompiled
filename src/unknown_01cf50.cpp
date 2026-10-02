// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "globals.h"
#include <xtl.h>

/* the texture stages: [0] the textures set on the device (0x51f3c8), [1] the
   textures wanted (0x51f3d8) */
IDirect3DBaseTexture8 *g_51f3c8[2][4];

// @retail 0x1cf50
void function_1cf50()
{
	IDirect3DBaseTexture8 **wanted = g_51f3c8[1];
	for (long stage = 0; stage < 4; wanted++, stage++)
	{
		IDirect3DBaseTexture8 **current = wanted - 4;
		if (*current != *wanted)
		{
			D3DDevice_SetTexture(stage, *wanted);
			*current = *wanted;
		}
	}
}

struct s_597d0_object
{
	byte unknown00[0x741c];
	long field_741c;
};

s_597d0_object *g_527364;
s_597d0_object *g_52736c;

// @retail 0x597d0
bool function_597d0(s_597d0_object **out)
{
	bool result = false;
	long mode = 0;
	if (g_527330)
		mode = g_527334;

	switch (mode)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		result = false;
		if (g_527330)
		{
			s_597d0_object *object = g_527364;
			if (object->field_741c)
			{
				if (out)
					*out = object;
				result = true;
			}
		}
		break;
	case 7:
	case 8:
	case 9:
		result = false;
		if (g_527330)
		{
			s_597d0_object *object = g_52736c;
			if (object->field_741c)
			{
				if (out)
					*out = object;
				result = true;
			}
		}
		break;
	}
	return result;
}

struct s_69470_item
{
	byte unknown00[2];
	byte type;
};

struct s_69470_iterator
{
	dword mask;
	long index;
};

struct s_69470_container
{
	byte unknown00[0x40];
	s_69470_item *items[15];
};

// @retail 0x69470
bool function_69470(s_69470_iterator *iterator, s_69470_container *container, s_69470_item **out)
{
	bool result = false;

	while (iterator->index >= 0)
	{
		if (iterator->index >= 15)
			break;
		s_69470_item *item = container->items[iterator->index++];
		if (item && (iterator->mask & (1 << item->type)))
		{
			*out = item;
			result = true;
			break;
		}
	}
	return result;
}
