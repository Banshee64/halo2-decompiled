// @flags /O2 /Ob1 /Gr
/* UNKNOWN_1130A0.CPP */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_107590.h"

/* the units (a local view of the object data) */
struct s_unit_1130a0
{
	long definition_index;
	byte unknown004[0x134 - 4];
	dword unit_flags;
	byte unknown138[0x1f6 - 0x138];
	char index1f6;
	char index1f7;
	byte unknown1f8[0x342 - 0x1f8];
	short state_offset;
};

struct s_object_header_1130a0
{
	byte unknown00[8];
	s_unit_1130a0 *object;
};

// @retail 0x1130a0
void function_1130a0(long unit_index, bool flag)
{
	s_unit_1130a0 *unit = ((s_object_header_1130a0 *)g_4e0300->data)[unit_index & 0xffff].object;
	bool enable = flag;
	if (enable && (unit->index1f6 == NONE || unit->index1f7 == NONE))
		enable = false;
	if (enable)
		unit->unit_flags |= 0x1000000;
	else
		unit->unit_flags &= ~0x1000000;
}

// @retail 0x114480
long function_114480(long unit_index)
{
	s_unit_1130a0 *unit = ((s_object_header_1130a0 *)g_4e0300->data)[unit_index & 0xffff].object;
	return *(long *)((byte *)unit + unit->state_offset);
}

PRIVATE __forceinline long *unit_state_handle_114440(s_unit_1130a0 *unit)
{
	return (long *)((byte *)unit + unit->state_offset);
}

// @retail 0x114440
long function_114440(long unit_index)
{
	long result = 0;
	s_unit_1130a0 *unit = ((s_object_header_1130a0 *)g_4e0300->data)[unit_index & 0xffff].object;
	long index = *unit_state_handle_114440(unit);
	if (index != NONE)
		result = *(long *)(g_4e3b44[index & 0xffff].bytes + 0x14);
	return result;
}

struct s_state_entry_1144a0
{
	byte unknown00[0xc];
	long tag_index;
};

struct s_state_tag_1144a0
{
	byte unknown00[0xc];
	long count;
	s_state_entry_1144a0 *entries;
};

bool function_20b5c0(long definition_index);

// @retail 0x1144a0
bool function_1144a0(long unit_index, short entry_index, long *tag_index)
{
	s_unit_1130a0 *unit = ((s_object_header_1130a0 *)g_4e0300->data)[unit_index & 0xffff].object;
	long index = *unit_state_handle_114440(unit);
	long found = NONE;
	bool result = false;
	if (index != NONE)
	{
		s_state_tag_1144a0 *tag = (s_state_tag_1144a0 *)g_4e3b44[index & 0xffff].bytes;
		if (entry_index >= 0 && entry_index < tag->count)
		{
			found = tag->entries[entry_index].tag_index;
			if (found != NONE)
				result = !function_20b5c0(found);
		}
	}
	if (*tag_index)
		*tag_index = found;
	return result;
}

struct s_unit_state_114ec0
{
	long tag_index;
	long value4;
	long value8;
};

struct s_model_variant_114ec0
{
	long name;
	byte unknown04[0x2c - 4];
	long value;
	byte unknown30[4];
	long tag_index;
};

struct s_model_114ec0
{
	byte unknown00[0x50];
	long variant_count;
	s_model_variant_114ec0 *variants;
	byte unknown58[0x90 - 0x58];
	long tag_index;
	byte unknown94[0xa0 - 0x94];
	long value;
};

PRIVATE __forceinline void unit_state_tag_set_114ec0(long unit_index, long index)
{
	long value = *(long *)(g_4e3b44[index & 0xffff].bytes + 0x14);
	s_unit_1130a0 *unit = ((s_object_header_1130a0 *)g_4e0300->data)[unit_index & 0xffff].object;
	s_unit_state_114ec0 *state = (s_unit_state_114ec0 *)((byte *)unit + unit->state_offset);
	state->tag_index = index;
	state->value4 = value;
}

// @retail 0x114ec0
void function_114ec0(long unit_index, long name)
{
	s_unit_1130a0 *unit = ((s_object_header_1130a0 *)g_4e0300->data)[unit_index & 0xffff].object;
	s_unit_state_114ec0 *state = (s_unit_state_114ec0 *)((byte *)unit + unit->state_offset);
	long model_index = *(long *)(g_4e3b44[unit->definition_index & 0xffff].bytes + 0x38);
	s_model_114ec0 *model = (s_model_114ec0 *)g_4e3b44[model_index & 0xffff].bytes;
	state->tag_index = model->tag_index;
	state->value8 = model->value;
	for (short i = 0; i < model->variant_count; i++)
	{
		s_model_variant_114ec0 *variant = &model->variants[i];
		if (variant->name == name && variant->tag_index != NONE)
		{
			unit_state_tag_set_114ec0(unit_index, variant->tag_index);
			if (variant->value && variant->value != NONE)
				state->value8 = variant->value;
			break;
		}
		if (variant->tag_index != NONE && state->tag_index == NONE)
		{
			unit_state_tag_set_114ec0(unit_index, variant->tag_index);
			if (variant->value && variant->value != NONE)
				state->value8 = variant->value;
		}
	}
}
