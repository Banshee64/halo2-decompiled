#include "unknown_11c920.h"
#include <math.h>

// @flags /O2 /Gr /arch:SSE

// @retail 0x29f180
void __stdcall function_29f180(byte *dest, byte *src, byte **cursor)
{
	*(long *)(dest + 0x0) = *(char *)(src + 4);
	*cursor += 6;
}

// @retail 0x29f1a0
void __stdcall function_29f1a0(byte *dest, byte *src, byte **cursor)
{
	*(short *)(dest + 0x4) = *(char *)(src + 4);
	*cursor += 6;
}

// @retail 0x29f1c0
void __stdcall function_29f1c0(byte *dest, byte *src, byte **cursor)
{
	*(long *)(dest + 0x10) = *(word *)(src + 4);
	*cursor += 6;
}

// @retail 0x29f1e0
void __stdcall function_29f1e0(byte *dest, byte *src, byte **cursor)
{
	dest[8] = src[4];
	*cursor += 6;
}

// @retail 0x29f200
void __stdcall function_29f200(byte *dest, byte *src, byte **cursor)
{
	dest[9] = src[4];
	*cursor += 6;
}

struct vec3 { real x, y, z; };

static inline void copy_vec3(vec3 *d, const vec3 *s)
{
	*d = *s;
}

// @retail 0x29f220
void __stdcall function_29f220(byte *dest, real *src, byte **cursor)
{
	*(real *)(dest + 0x14) = src[1];
	*(real *)(dest + 0x18) = src[2];
	*(real *)(dest + 0x1c) = 0.0f;
	*cursor += 12;
}

// @retail 0x29f250
void __stdcall function_29f250(byte *dest, byte *src, byte **cursor)
{
	copy_vec3((vec3 *)(dest + 0x28), (vec3 *)(src + 4));
	*cursor += 16;
}

// @retail 0x29f280
void __stdcall function_29f280(byte *dest, byte *src, byte **cursor)
{
	copy_vec3((vec3 *)(dest + 0x34), (vec3 *)(src + 4));
	*cursor += 16;
}

// @retail 0x29f2b0
void __stdcall function_29f2b0(byte *dest, byte *src, byte **cursor)
{
	copy_vec3((vec3 *)(dest + 0x40), (vec3 *)(src + 4));
	*cursor += 16;
}

// @retail 0x29f2e0
void __stdcall function_29f2e0(byte *dest, word *src, byte **cursor)
{
	real *f = (real *)src;
	real cp = (real)cos(f[2]);
	vec3 v;
	v.x = (real)cos(f[1]) * cp;
	v.y = (real)sin(f[1]) * cp;
	v.z = (real)sin(f[2]);
	if (src[0] != 0x15)
	{
		copy_vec3((vec3 *)(dest + 0x28), &v);
	}
	if (src[0] != 0x14)
	{
		copy_vec3((vec3 *)(dest + 0x34), &v);
	}
	if (src[0] != 0x13)
	{
		copy_vec3((vec3 *)(dest + 0x40), &v);
	}
	*cursor += 12;
}

// @retail 0x29f370
void __stdcall function_29f370(byte *dest, word *src, byte **cursor)
{
	if (src[0] != 0xe)
	{
		copy_vec3((vec3 *)(dest + 0x28), (vec3 *)(src + 2));
	}
	if (src[0] != 0xd)
	{
		copy_vec3((vec3 *)(dest + 0x34), (vec3 *)(src + 2));
	}
	if (src[0] != 0xc)
	{
		copy_vec3((vec3 *)(dest + 0x40), (vec3 *)(src + 2));
	}
	*cursor += 16;
}

// Retail keeps these in a table of per-field readers (at 0x46d0b0 in the XBE);
// the table keeps the parameters on the stack.
void *const g_46d0b0_table[] =
{
	(void *)function_29f180,
	(void *)function_29f1a0,
	(void *)function_29f1c0,
	(void *)function_29f1e0,
	(void *)function_29f220,
	0,
	0,
	(void *)function_29f250,
	(void *)function_29f280,
	(void *)function_29f2b0,
	(void *)function_29f370,
	(void *)function_29f200,
	(void *)function_29f2e0,
};
