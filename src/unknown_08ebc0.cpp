// @flags /O2 /Gr
#include "cseries.h"
#include <string.h>
#include "online_message_entries.h"

struct s_named_entry
{
	byte unknown0[0xc];
	char name[0x10];
};

struct s_entry_header
{
	long unknown0;
	long unknown4;
	long unknown8;
};

// @retail 0x8ebc0
const char *function_08ebc0(s_named_entry *entry)
{
	const char *name = "";
	if (entry)
	{
		name = entry->name;
	}
	return name;
}

// @retail 0x8ebd0
void function_08ebd0(s_entry *entry, s_entry_source *source)
{
	memset(entry, 0, sizeof(s_entry));
	*(s_entry_header *)entry = *(s_entry_header *)source;
	char *name = entry->name;
	strncpy(name, source->name, 0x10);
	name[0xf] = 0;
	if (source->low & 0x1)
	{
		entry->flags |= 0x1;
	}
	else
	{
		entry->flags &= ~0x1;
	}
	if (source->low & 0x2)
	{
		entry->flags |= 0x2;
	}
	else
	{
		entry->flags &= ~0x2;
	}
	if (source->low & 0x4)
	{
		entry->flags |= 0x4;
	}
	else
	{
		entry->flags &= ~0x4;
	}
	if (source->low & 0x8)
	{
		entry->flags |= 0x8;
	}
	else
	{
		entry->flags &= ~0x8;
	}
	if (source->low & 0x10)
	{
		entry->flags |= 0x10;
	}
	else
	{
		entry->flags &= ~0x10;
	}
	if (source->low & 0x20)
	{
		entry->flags |= 0x20;
	}
	else
	{
		entry->flags &= ~0x20;
	}
	if (source->low & 0x40)
	{
		entry->flags |= 0x40;
	}
	else
	{
		entry->flags &= ~0x40;
	}
	if (source->low & 0x80)
	{
		entry->flags |= 0x80;
	}
	else
	{
		entry->flags &= ~0x80;
	}
	if (source->all & 0x100)
	{
		entry->flags |= 0x100;
	}
	else
	{
		entry->flags &= ~0x100;
	}
	if (source->all & 0xff000000)
	{
		entry->flags |= 0x200;
	}
	else
	{
		entry->flags &= ~0x200;
	}

	switch (source->type)
	{
	case 2:
		entry->flags |= 0x400;
		break;
	case 3:
		entry->flags |= 0x800;
		break;
	case 4:
		entry->flags |= 0x1000;
		break;
	case 5:
		entry->flags |= 0x2000;
		break;
	case 6:
		entry->flags |= 0x4000;
		break;
	case 7:
		entry->flags |= 0x8000;
		break;
	case 1:
		entry->flags |= 0x10000;
		break;
	}

	entry->unknown20 = source->unknown20;
	entry->unknown24 = source->unknown28;
	entry->unknown28 = source->unknown10;
	entry->unknown2c = source->unknown14;
	entry->unknown30 = source->unknown18;
	entry->unknown34 = source->unknown1c;
	entry->unknown38 = source->unknown2c;
	entry->unknown3a = source->unknown2e;
}

// @retail 0x8eda0
long first_person_animation_type_from_weapon_state(long state)
{
	switch (state)
	{
	case 0:
		return 0x9c1;
	case 1:
		return 0x3c2;
	case 2:
		return 0x4c3;
	case 3:
		return 0x6c4;
	case 4:
		return 0x4c5;
	case 5:
		return 0x581;
	case 6:
		return 0x681;
	case 7:
		return 0xb81;
	case 8:
		return 0x481;
	case 9:
		return 0x682;
	case 10:
		return 0x783;
	case 11:
		return 0x384;
	case 12:
		return 0xb85;
	case 13:
		return 0x586;
	case 14:
		return 0x787;
	case 15:
		return 0x388;
	case 16:
		return 0x389;
	case 17:
		return 0x38a;
	case 18:
		return 0x38b;
	case 19:
		return 0x38c;
	case 20:
		return 0x68d;
	case 21:
		return 0x68e;
	case 22:
		return 0x590;
	case 23:
		return 0x791;
	default:
		return NONE;
	}
}

// @retail 0x8eeb0
void function_08eeb0(s_state_block *block)
{
	block->unknownc = 0;
	memset(&block->unknownc, 0, sizeof(block->unknownc));  /* retail stores this twice */
	block->unknown210 = 0;
	block->unknown20c = 0;
	block->unknown4 = 0;
	block->unknown8 = 0;
	block->unknown21c = 0;
	block->unknown218 = NONE;
	block->active = 1;
}
