// @flags /O2 /Gr
#include "cseries.h"
#include <string.h>

#pragma pack(push, 1)
struct s_unknown_8bytes
{
	long a;
	long b;
};

struct s_unknown_108
{
	long data[27];
};

struct s_unknown_3648
{
	long data[0x390];
};

struct c_unknown_05c490
{
	byte unknown0000[0x4978];
	long update_count;
	byte unknown497c[0x4994 - 0x497c];
	long value4994;
	byte flag4998;
	s_unknown_8bytes data4999;
	byte unknown49a1[0x49c4 - 0x49a1];
	byte flag49c4;
	byte unknown49c5[0x4da0 - 0x49c5];
	long value4da0;
	long value4da4;
	long value4da8;
	byte unknown4dac[0x4f20 - 0x4dac];
	byte flag4f20;
	byte unknown4f21[3];
	s_unknown_108 data4f24;
	s_unknown_3648 data4f90;
	byte unknown5dd0[0x5e20 - 0x5dd0];
	long value5e20;
	byte unknown5e24[0x741c - 0x5e24];
	long state;

	bool set_value_4994(long value);
	bool set_values_4da0(long a, long b);
	bool clear_flag_49c4();
	bool set_value_4da8(long value);
	bool set_data_4f24(const s_unknown_108 *a, const s_unknown_3648 *b);
	bool set_data_4999(const s_unknown_8bytes *data);
	bool set_value_5e20(long value);
};
#pragma pack(pop)

// @retail 0x5c490
bool c_unknown_05c490::set_value_4994(long value)
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			value4994 = value;
			update_count++;
			result = true;
		}
		else
		{
			// retail keeps this dead store; cause unknown
			volatile long unused = state;
		}
	}

	return result;
}

// @retail 0x5c4e0
bool c_unknown_05c490::set_values_4da0(long a, long b)
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			value4da0 = a;
			update_count++;
			value4da4 = b;
			result = true;
		}
		else
		{
			// retail keeps this dead store; cause unknown
			volatile long unused = state;
		}
	}

	return result;
}

// @retail 0x5c530
bool c_unknown_05c490::clear_flag_49c4()
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			flag49c4 = 0;
			update_count++;
			result = true;
		}
		else
		{
			// retail keeps this dead store; cause unknown
			volatile long unused = state;
		}
	}

	return result;
}

// @retail 0x5c570
bool c_unknown_05c490::set_value_4da8(long value)
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			value4da8 = value;
			update_count++;
			result = true;
		}
		else
		{
			// retail keeps this dead store; cause unknown
			volatile long unused = state;
		}
	}

	return result;
}

// @retail 0x5c5c0
bool c_unknown_05c490::set_data_4f24(const s_unknown_108 *a, const s_unknown_3648 *b)
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			if (a)
			{
				flag4f20 = 1;
				data4f24 = *a;
				data4f90 = *b;
			}
			else
			{
				flag4f20 = 0;
				memset(&data4f24, 0, sizeof(data4f24));
				memset(&data4f90, 0, sizeof(data4f90));
			}
			update_count++;
			result = true;
		}
		else
		{
			// retail keeps this dead store; cause unknown
			volatile long unused = state;
		}
	}

	return result;
}

// @retail 0x5c660
bool c_unknown_05c490::set_data_4999(const s_unknown_8bytes *data)
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			if (!data)
			{
				flag4998 = 0;
			}
			else
			{
				flag4998 = 1;
				data4999 = *data;
			}
			update_count++;
			result = true;
		}
		else
		{
			// retail keeps this dead store; cause unknown
			volatile long unused = state;
		}
	}

	return result;
}

// @retail 0x5c6d0
bool c_unknown_05c490::set_value_5e20(long value)
{
	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			value5e20 = value;
			update_count++;
		}
		else
		{
			// retail keeps this dead store; cause unknown
			volatile long unused = state;
		}
	}

	return false;
}
