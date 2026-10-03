#include "cseries.h"

// @flags /O2 /Gr

/* UNKNOWN_191270.CPP: the voice and sound settings and their string ids */

// @retail 0x1914f0
long function_1914f0(long string_id)
{
	long result = NONE;

	switch (string_id)
	{
	case 0x1300012d:
		result = 0xd;
		break;
	case 0x14000135:
		result = 0xe;
		break;
	case 0x1600013c:
		result = 0x1b;
		break;
	case 0x1600013d:
		result = 0x1c;
		break;
	case 0x1600013e:
		result = 0x1d;
		break;
	case 0x1600013f:
		result = 0x1e;
		break;
	}

	return result;
}

// @retail 0x191660
dword function_191660(long string_id, long index)
{
	dword result = 0;

	if (index == NONE)
	{
		switch (string_id)
		{
		case 0x1300012d:
			result = 2;
			break;
		case 0x18000136:
			result = 0x30;
			break;
		case 0x17000137:
			result = 0xc0;
			break;
		case 0x11000138:
			result = 0xf0;
			break;
		}
	}
	else if (string_id == 0x11000138)
	{
		switch (index)
		{
		case 0:
			result = 0x10;
			break;
		case 1:
			result = 0x20;
			break;
		case 2:
			result = 0x40;
			break;
		case 3:
			result = 0x80;
			break;
		}
	}

	return result;
}
