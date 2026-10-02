// @flags /O2 /Gr
#include "cseries.h"

#pragma pack(push, 1)
struct c_unknown_05b040
{
	byte unknown0000[0x49c4];
	byte value49c4;
	byte unknown49c5[3];
	long value49c8;
	byte unknown49cc[0x49fd - 0x49cc];
	byte flag49fd;
	byte unknown49fe[2];
	byte data4a00[4];
	byte unknown4a04[0x4d08 - 0x4a04];
	long value4d08;
	long value4d0c;
	byte flag4d10;
	byte unknown4d11[0x4da0 - 0x4d11];
	long value4da0;
	long value4da4;
	byte unknown4da8[0x5e20 - 0x4da8];
	long value5e20;
	byte unknown5e24[0x741c - 0x5e24];
	long state;

	byte get_value_49c4();
	long get_value_49c8();
	byte *get_data_4a00();
	long get_value_5e20();
	bool get_values_4d08(long *a, long *b, byte **c);
	__int64 get_values_4da0();
};
#pragma pack(pop)

// @retail 0x5b040
byte c_unknown_05b040::get_value_49c4()
{
	byte result = 0;

	if (state > 2 && state <= 8)
	{
		result = value49c4;
	}

	return result;
}

// @retail 0x5b060
long c_unknown_05b040::get_value_49c8()
{
	long result = NONE;

	if (state > 2 && state <= 8)
	{
		result = value49c8;
	}

	return result;
}

// @retail 0x5b080
byte *c_unknown_05b040::get_data_4a00()
{
	byte *result = 0;

	if (state > 2 && state <= 8)
	{
		if (flag49fd)
		{
			result = data4a00;
		}
	}

	return result;
}

// @retail 0x5b0b0
long c_unknown_05b040::get_value_5e20()
{
	long result = NONE;

	if (state > 2 && state <= 8)
	{
		result = value5e20;
	}

	return result;
}

// @retail 0x5b0d0
bool c_unknown_05b040::get_values_4d08(long *a, long *b, byte **c)
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (value4d0c != NONE && flag4d10)
		{
			*a = value4d08;
			*b = value4d0c;
			*c = &flag4d10;
			result = true;
		}
	}

	return result;
}

// @retail 0x5b120
__int64 c_unknown_05b040::get_values_4da0()
{
	long values[2] = { NONE, NONE };

	if (state > 2 && state <= 8)
	{
		values[0] = value4da0;
		values[1] = value4da4;
	}

	return *(__int64 *)values;
}