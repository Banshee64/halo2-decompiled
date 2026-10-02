// @flags /O2 /Gr
#include "cseries.h"
#include "unknown_26b230.h"
#include "globals.h"
#include "slot_owner.h"

#define OWNER_STATE(index) ((s_slot_owner_entry *)(g_4f55f0->data + ((index) & 0xffff) * sizeof(s_slot_owner_entry)))

// @retail 0x1f8640
byte function_1f8640(long index)
{
	return OWNER_STATE(index)->unknown50c;
}

// @retail 0x1f8660
bool function_1f8660(long index)
{
	s_slot_owner_entry *s = OWNER_STATE(index);
	byte flag = s->unknown50c;
	bool result = false;
	if (flag && s->unknown504 == 1)
		result = true;
	return result;
}

// @retail 0x1f86a0
void function_1f86a0(long index)
{
	s_slot_owner_entry *s = OWNER_STATE(index);
	s->unknown50c = 0;
	s->unknown5ac = NONE;
	s->unknown5b0 = NONE;
	s->unknown5b4 = 0;
	s->unknown5b6 = 0;
	s->unknown4ac = 0;
	s->unknown504 = 0;
}

// @retail 0x1f86f0
bool function_1f86f0(long index)
{
	return OWNER_STATE(index)->unknown504 == 2;
}

// @retail 0x1f8720
bool function_1f8720(long index)
{
	return OWNER_STATE(index)->unknown504 == 3;
}

// @retail 0x1f8750
void function_1f8750(long index)
{
	s_slot_owner_entry *s = OWNER_STATE(index);
	s->unknown508++;
	s->unknown50c = 0;
	s->unknown504 = 3;
}
