// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_131E50.CPP: real color to pixel32 conversion */

#include "cseries.h"

struct real_argb_color
{
	real alpha;
	real red;
	real green;
	real blue;
};

struct real_rgb_color
{
	real red;
	real green;
	real blue;
};

// @retail 0x131e50
dword __cdecl real_argb_color_to_pixel32(const real_argb_color *color)
{
	long b, g, r, a;
	real scale = 255.0f;
	dword pixel = 0;

	__asm
	{
		mov edx, color
		fld dword ptr [edx]
		fld dword ptr [edx + 4]
		fld dword ptr [edx + 8]
		fld dword ptr [edx + 12]
		fld scale
		fmul st(4), st(0)
		fmul st(3), st(0)
		fmul st(2), st(0)
		fmulp st(1), st(0)
		fistp b
		fistp g
		fistp r
		fistp a
		mov edx, b
		mov ebx, g
		mov ecx, r
		mov eax, a
		shl ebx, 8
		shl ecx, 0x10
		shl eax, 0x18
		or edx, ebx
		or edx, ecx
		or edx, eax
		mov pixel, edx
	}
	return pixel;
}

// @retail 0x131ed0
dword __cdecl real_rgb_color_to_pixel32(const real_rgb_color *color)
{
	real scale = 255.0f;
	dword pixel = 0;

	__asm
	{
		mov edx, color
		fld dword ptr [edx]
		fld dword ptr [edx + 4]
		fld dword ptr [edx + 8]
		fld scale
		fmul st(3), st(0)
		fmul st(2), st(0)
		fmulp st(1), st(0)
		fistp pixel
		and pixel, 0xff
		mov edx, pixel
		fistp pixel
		and pixel, 0xff
		shl pixel, 8
		or edx, pixel
		fistp pixel
		and pixel, 0xff
		shl pixel, 0x10
		or edx, pixel
		mov pixel, edx
	}
	return pixel;
}

// @retail 0x131f40
dword __cdecl real_a_rgb_color_to_pixel32(real alpha, const real_rgb_color *color)
{
	real scale = 255.0f;
	dword pixel = 0;

	__asm
	{
		mov edx, color
		fld alpha
		fld dword ptr [edx]
		fld dword ptr [edx + 4]
		fld dword ptr [edx + 8]
		fld scale
		fmul st(4), st(0)
		fmul st(3), st(0)
		fmul st(2), st(0)
		fmulp st(1), st(0)
		fistp pixel
		and pixel, 0xff
		mov edx, pixel
		fistp pixel
		and pixel, 0xff
		shl pixel, 8
		or edx, pixel
		fistp pixel
		and pixel, 0xff
		shl pixel, 0x10
		or edx, pixel
		fistp pixel
		shl pixel, 0x18
		or edx, pixel
		mov pixel, edx
	}
	return pixel;
}

// @retail 0x131fc0
dword __cdecl real_alpha_to_pixel32(real alpha)
{
	real scale = 255.0f;
	dword pixel = 0;

	__asm
	{
		fld alpha
		fld scale
		fmulp st(1), st(0)
		fistp pixel
		shl pixel, 0x18
	}
	return pixel;
}
