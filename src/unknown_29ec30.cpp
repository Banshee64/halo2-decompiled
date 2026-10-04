#include "cseries.h"

// @flags /O2 /Ob1 /Gr /arch:SSE

// @retail 0x29ec30
void __stdcall function_29ec30(long a, byte *dest, long c, char **cursor)
{
	*(long *)(dest + 0x0) = **cursor;
	(*cursor)++;
}

// @retail 0x29ec50
void __stdcall function_29ec50(long a, byte *dest, long c, char **cursor)
{
	*(short *)(dest + 0x4) = **cursor;
	(*cursor)++;
}

// @retail 0x29ec70
void __stdcall function_29ec70(long a, byte *dest, long c, long **cursor)
{
	*(long *)(dest + 0x10) = **cursor;
	(*cursor)++;
}

// @retail 0x29ec90
void __stdcall function_29ec90(long a, byte *dest, long c, byte **cursor)
{
	dest[8] = **cursor;
	*cursor += 2;
}

// @retail 0x29ecb0
void __stdcall function_29ecb0(long a, byte *dest, long c, byte **cursor)
{
	dest[9] = **cursor;
	*cursor += 2;
}

// @retail 0x29ecd0
void __stdcall function_29ecd0(long a, byte *dest, long c, real **cursor)
{
	real *p = *cursor;
	*(real *)(dest + 0x14) = p[0];
	*(real *)(dest + 0x18) = p[1];
	*(real *)(dest + 0x1c) = 0.0f;
	*cursor += 2;
}

// Retail has these in a table of per-field readers (at 0x46cf78 in the XBE,
// continuing with functions not decompiled yet); the table keeps the
// parameters on the stack.
void *const g_29ec30_table[] =
{
	(void *)function_29ec30,
	(void *)function_29ec50,
	(void *)function_29ec70,
	(void *)function_29ec90,
	(void *)function_29ecb0,
	(void *)function_29ecd0,
};

// @retail 0x29ed00
void function_29ed00(const char *data, short *controller)
{
	controller[0] += data[0];
	if (controller[0] > 1000)
		controller[0] -= 1000;
	else if (controller[0] < -1000)
		controller[0] += 1000;
	controller[1] += data[1];
}
