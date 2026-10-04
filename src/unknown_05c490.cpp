// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "unknown_059ad0.h"
#include <string.h>

// @retail 0x5c490
bool c_class_58d20::set_value_4994(long value)
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
bool c_class_58d20::set_values_4da0(long a, long b)
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
bool c_class_58d20::clear_value_49c4()
{
	bool result = false;

	if (state > 2 && state <= 8)
	{
		if (state == 5 || state == 6 || state == 7 || state == 8)
		{
			value49c4 = 0;
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
bool c_class_58d20::set_value_4da8(long value)
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
bool c_class_58d20::set_data_4f24(const s_unknown_108 *a, const s_unknown_3648 *b)
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
bool c_class_58d20::set_data_4999(const s_long_pair *data)
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
bool c_class_58d20::set_value_5e20(long value)
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
