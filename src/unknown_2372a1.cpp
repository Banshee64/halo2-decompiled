#include "cseries.h"

// @flags /O1 /arch:SSE /Gr

union field_param
{
	long i;
	real r;
};

typedef void (__stdcall *field_info_proc)(long *type, long *offset, field_param *param);

void __stdcall function_2372a1(long *type, long *offset, field_param *param);
void __stdcall function_2372c2(long *type, long *offset, field_param *param);
void __stdcall function_2372e3(long *type, long *offset, field_param *param);
void __stdcall function_23730a(long *type, long *offset, field_param *param);
void __stdcall function_23732b(long *type, long *offset, field_param *param);
void __stdcall function_237352(long *type, long *offset, field_param *param);
void __stdcall function_237379(long *type, long *offset, field_param *param);
void __stdcall function_237397(long *type, long *offset, field_param *param);

// Retail table of field descriptor callbacks, indexed by field id. Only the
// entries whose functions are in this file are filled in; the rest (null here)
// belong to neighbouring batches.
field_info_proc g_470828[0x70] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, function_2372a1, function_2372c2, function_2372e3, function_23730a, function_23732b, 0, function_237379,
	0, function_237397, function_237352, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// @retail 0x2372a1
void __stdcall function_2372a1(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 9;
}

// @retail 0x2372c2
void __stdcall function_2372c2(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 10;
}

// @retail 0x2372e3
void __stdcall function_2372e3(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd5;
	param->r = 1.0f;
}

// @retail 0x23730a
void __stdcall function_23730a(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 11;
}

// @retail 0x23732b
void __stdcall function_23732b(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd6;
	param->r = 1.0f;
}

// @retail 0x237352
void __stdcall function_237352(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd7;
	param->r = 1.0f;
}

// @retail 0x237379
void __stdcall function_237379(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 0;
}

// @retail 0x237397
void __stdcall function_237397(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0xb4;
	param->r = 1.0f;
}

// @retail 0x2373be
void function_2373be(long index, void *base, long value)
{
	field_info_proc proc;
	long type;
	long offset;
	field_param param;

	proc = (index >= 0 && index < 0x70) ? g_470828[index] : 0;
	if (base && proc)
	{
		proc(&type, &offset, &param);

		switch (type)
		{
		case 0:
			if (value)
				*(word *)((byte *)base + offset) |= (word)(1 << param.i);
			else
				*(word *)((byte *)base + offset) &= ~(word)(1 << param.i);
			break;
		case 1:
			if (value)
				*(dword *)((byte *)base + offset) |= (1 << param.i);
			else
				*(dword *)((byte *)base + offset) &= ~(1 << param.i);
			break;
		case 2:
			*(byte *)((byte *)base + offset) = (value != 0);
			break;
		case 3:
		{
			long r;
			real v = 1.0f / param.r;
			__asm
			{
				fld v
				fistp r
			}
			*(byte *)((byte *)base + offset) = (byte)(r * value);
			break;
		}
		case 4:
		{
			long r;
			real v = 1.0f / param.r;
			__asm
			{
				fld v
				fistp r
			}
			*(word *)((byte *)base + offset) = (word)(r * value);
			break;
		}
		case 5:
		{
			long r;
			real v = 1.0f / param.r;
			__asm
			{
				fld v
				fistp r
			}
			*(long *)((byte *)base + offset) = r * value;
			break;
		}
		case 6:
			*(real *)((byte *)base + offset) = (1.0f / param.r) * (real)value;
			break;
		}
	}
}
