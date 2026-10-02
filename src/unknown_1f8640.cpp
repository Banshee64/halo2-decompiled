// @flags /O2 /Gr
#include "cseries.h"
#include "unknown_26b230.h"
#include "globals.h"

struct s_owner_state
{
	byte unknown0[0x4ac];
	short unknown4ac;
	byte unknown4ae[0x56];
	short unknown504;
	byte unknown506[2];
	short unknown508;
	byte unknown50a[2];
	byte unknown50c;
	byte unknown50d[0x9f];
	long unknown5ac;
	short unknown5b0;
	byte unknown5b2[2];
	short unknown5b4;
	short unknown5b6;
	byte unknown5b8[0x2d0];
};

#define OWNER_STATE(index) ((s_owner_state *)(g_4f55f0->data + ((index) & 0xffff) * sizeof(s_owner_state)))

// @retail 0x1f8640
byte function_1f8640(long index)
{
	return OWNER_STATE(index)->unknown50c;
}

// @retail 0x1f8660
bool function_1f8660(long index)
{
	s_owner_state *s = OWNER_STATE(index);
	byte flag = s->unknown50c;
	bool result = false;
	if (flag && s->unknown504 == 1)
		result = true;
	return result;
}

// @retail 0x1f86a0
void function_1f86a0(long index)
{
	s_owner_state *s = OWNER_STATE(index);
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
	s_owner_state *s = OWNER_STATE(index);
	s->unknown508++;
	s->unknown50c = 0;
	s->unknown504 = 3;
}
