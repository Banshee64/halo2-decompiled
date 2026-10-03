#include "cseries.h"
#include "globals.h"

// @flags /O2 /Gr /arch:SSE

/* UNKNOWN_28C470.CPP: the channel decoders of the uncompressed static data
   codec (the definition at 0x47fb40 holds them with function_28c510): a
   quantized rotation and a translation per node */

struct s_animation_data
{
	byte unknown00[0xc];
	long vector_offset;
};

struct s_animation_output
{
	real_quaternion rotation;
	real_vector3d vector;
	real scale;
};

// @retail 0x28c470
void function_28c470()
{
	short *a = (short *)((byte *)g_504480 + g_5044b4 * 8 + 0x20);
	real_quaternion *result = &g_5044c0->rotation;

	__asm
	{
		mov ecx, a
		mov eax, result
		movq mm3, [ecx]
		punpcklwd mm1, mm3
		punpckhwd mm2, mm3
		psrad mm1, 0x10
		psrad mm2, 0x10
		cvtpi2ps xmm1, mm1
		cvtpi2ps xmm2, mm2
		emms
		movlhps xmm1, xmm2
		movaps xmm0, xmm1
		mulps xmm0, xmm1
		movaps xmm3, xmm0
		shufps xmm3, xmm3, 0x4e
		addps xmm0, xmm3
		movaps xmm4, xmm0
		shufps xmm4, xmm4, 0x11
		addps xmm0, xmm4
		rsqrtps xmm0, xmm0
		mulps xmm1, xmm0
		movaps [eax], xmm1
	}
}

// @retail 0x28c4e0
void function_28c4e0()
{
	real_vector3d *source = (real_vector3d *)((byte *)g_504480 + (g_5044b8 * 12 + g_504480->vector_offset));
	real_vector3d *destination = &g_5044c0->vector;

	*destination = *source;
}
