#include "cseries.h"
#include <xmmintrin.h>
#include "globals.h"

// @flags /O2 /Gr /arch:SSE

struct s_frame_info
{
	dword count : 12;
	dword start : 20;
};

struct s_animation_data
{
	byte unknown00[0xc];
	long vector_offset;
	long scale_offset;
	long rotation_stride;
	long vector_stride;
	long scale_stride;
	long unknown20;
	long unknown24;
	long unknown28;
	long unknown2c;
	dword rotation_frame_info[1];
};

struct s_animation_output
{
	real_quaternion rotation;
	real_vector3d vector;
	real scale;
};

long g_504468;

// @retail 0x28c510
void function_28c510()
{
	g_5044c0->scale = *(real *)((byte *)g_504480 + g_504480->scale_offset + g_5044bc * 4);
}

// @retail 0x28c530
void function_28c530()
{
	real_quaternion *quaternions = (real_quaternion *)((byte *)g_504480 + g_504480->rotation_stride * g_5044b4 + 0x20);
	real_quaternion *result = &g_5044c0->rotation;
	real t = g_50446c;
	real_quaternion *b = quaternions + g_504468;
	real_quaternion *a = quaternions + g_504464;

	__asm
	{
		mov eax, a
		mov ecx, b
		movss xmm3, t
		movaps xmm1, [eax]
		movaps xmm2, [ecx]
		shufps xmm3, xmm3, 0
		subps xmm2, xmm1
		mulps xmm2, xmm3
		addps xmm2, xmm1
		movaps xmm0, xmm2
		mulps xmm0, xmm2
		movaps xmm7, xmm0
		shufps xmm7, xmm7, 0x4e
		addps xmm0, xmm7
		movaps xmm6, xmm0
		shufps xmm6, xmm6, 0x11
		addps xmm0, xmm6
		rsqrtps xmm0, xmm0
		mov edx, result
		mulps xmm2, xmm0
		movaps [edx], xmm2
	}
}

// @retail 0x28c5d0
void function_28c5d0()
{
	static real_vector3d vector_a;
	static real_vector3d vector_b;
	real_vector3d *vectors = (real_vector3d *)((byte *)g_504480 + (g_504480->vector_offset + g_504480->vector_stride * g_5044b8));
	real t = g_50446c;

	vector_a = vectors[g_504464];
	vector_b = vectors[g_504468];
	g_5044c0->vector.i = (vector_b.i - vector_a.i) * t + vector_a.i;
	g_5044c0->vector.j = (vector_b.j - vector_a.j) * t + vector_a.j;
	g_5044c0->vector.k = (vector_b.k - vector_a.k) * t + vector_a.k;
}

// @retail 0x28c6b0
void function_28c6b0()
{
	dword *scales = (dword *)((byte *)g_504480 + g_504480->scale_stride * g_5044bc + g_504480->scale_offset);
	dword bits_a = scales[g_504464];
	dword bits_b = scales[g_504468];
	real a = *(real *)&bits_a;
	real b = *(real *)&bits_b;

	g_5044c0->scale = (b - a) * g_50446c + a;
}

// @retail 0x28c710
void function_28c710()
{
	real_quaternion *quaternions = (real_quaternion *)((byte *)g_504480 + g_504480->rotation_stride * g_5044b4);
	real_quaternion *source = quaternions + (g_504464 + 2);
	real_quaternion *destination = &g_5044c0->rotation;

	*destination = *source;
}

// @retail 0x28c750
void function_28c750()
{
	s_animation_data *data = g_504480;
	real_vector3d *source = (real_vector3d *)((byte *)data + (data->vector_stride * g_5044b8 + g_504464 * 12 + data->vector_offset));
	real_vector3d *destination = &g_5044c0->vector;

	*destination = *source;
}

// @retail 0x28c790
void function_28c790()
{
	real_quaternion *quaternions = (real_quaternion *)((byte *)g_504480 + g_504480->rotation_stride * g_5044b4 + 0x20);
	real inverse_t = 1.0f - g_50446c;
	real t = g_50446c;
	real_quaternion *result = &g_5044c0->rotation;
	short *b = (short *)quaternions + g_504468 * 4;
	short *a = (short *)quaternions + g_504464 * 4;

	__asm
	{
		mov ecx, a
		mov edx, b
		mov eax, result
		movq mm3, qword ptr [ecx]
		punpcklwd mm1, mm3
		punpckhwd mm2, mm3
		psrad mm1, 0x10
		psrad mm2, 0x10
		cvtpi2ps xmm1, mm1
		cvtpi2ps xmm2, mm2
		movq mm3, qword ptr [edx]
		punpcklwd mm1, mm3
		punpckhwd mm2, mm3
		psrad mm1, 0x10
		psrad mm2, 0x10
		movss xmm5, inverse_t
		movss xmm6, t
		shufps xmm5, xmm5, 0
		shufps xmm6, xmm6, 0
		cvtpi2ps xmm3, mm1
		cvtpi2ps xmm4, mm2
		movlhps xmm1, xmm2
		movlhps xmm3, xmm4
		mulps xmm1, xmm5
		mulps xmm3, xmm6
		addps xmm1, xmm3
		emms
		movaps xmm0, xmm1
		mulps xmm0, xmm1
		movaps xmm3, xmm0
		shufps xmm3, xmm3, 0x4e
		addps xmm0, xmm3
		movaps xmm4, xmm0
		shufps xmm4, xmm4, 0x11
		addps xmm0, xmm4
		rsqrtps xmm0, xmm0
		mov eax, result
		mulps xmm1, xmm0
		movaps [eax], xmm1
	}
}

// @retail 0x28c880
void function_28c880()
{
	static real_vector3d vector_a;
	static real_vector3d vector_b;
	real_vector3d *vectors = (real_vector3d *)((byte *)g_504480 + (g_504480->vector_offset + g_504480->vector_stride * g_5044b8));
	real t = g_50446c;

	vector_a = vectors[g_504464];
	vector_b = vectors[g_504468];
	g_5044c0->vector.i = (vector_b.i - vector_a.i) * t + vector_a.i;
	g_5044c0->vector.j = (vector_b.j - vector_a.j) * t + vector_a.j;
	g_5044c0->vector.k = (vector_b.k - vector_a.k) * t + vector_a.k;
}

// @retail 0x28c960
void function_28c960()
{
	real_quaternion *result = &g_5044c0->rotation;
	real_quaternion *quaternions = (real_quaternion *)((byte *)g_504480 + g_504480->rotation_stride * g_5044b4 + 0x20);
	short *a = (short *)quaternions + g_504464 * 4;

	__asm
	{
		mov ecx, a
		mov eax, result
		movq mm3, qword ptr [ecx]
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

// @retail 0x28c9e0
void function_28c9e0()
{
	s_animation_data *data = g_504480;
	dword frame_info = data->rotation_frame_info[g_5044b4];
	long start = frame_info >> 12;
	long count = frame_info & 0xfff;
	byte *keys = (byte *)data + data->rotation_stride + start;
	short *quaternions = (short *)((byte *)data + data->unknown20 + start * 8);
	long low = 0;
	long high = count;
	short *a;
	real_quaternion *result;

	while (high > low + 1)
	{
		long middle = (low + high) >> 1;

		if ((dword)keys[middle] <= (dword)g_504464)
		{
			low = middle;
		}
		else
		{
			high = middle;
		}
	}

	_mm_prefetch((char *)(quaternions + low * 4), _MM_HINT_T0);
	a = quaternions + low * 4;
	result = &g_5044c0->rotation;
	keys += low;

	if (low + 1 < count)
	{
		real x = g_504464 + g_50446c;
		real weight_a = keys[1] - x;
		real weight_b = x - keys[0];
		short *b = a + 4;

		__asm
		{
			mov ecx, a
			mov edx, b
			mov eax, result
			movq mm3, qword ptr [ecx]
			punpcklwd mm1, mm3
			punpckhwd mm2, mm3
			psrad mm1, 0x10
			psrad mm2, 0x10
			cvtpi2ps xmm1, mm1
			cvtpi2ps xmm2, mm2
			movq mm3, qword ptr [edx]
			punpcklwd mm1, mm3
			punpckhwd mm2, mm3
			psrad mm1, 0x10
			psrad mm2, 0x10
			movss xmm5, weight_a
			movss xmm6, weight_b
			shufps xmm5, xmm5, 0
			shufps xmm6, xmm6, 0
			cvtpi2ps xmm3, mm1
			cvtpi2ps xmm4, mm2
			movlhps xmm1, xmm2
			movlhps xmm3, xmm4
			mulps xmm1, xmm5
			mulps xmm3, xmm6
			addps xmm1, xmm3
			emms
			movaps xmm0, xmm1
			mulps xmm0, xmm1
			movaps xmm3, xmm0
			shufps xmm3, xmm3, 0x4e
			addps xmm0, xmm3
			movaps xmm4, xmm0
			shufps xmm4, xmm4, 0x11
			addps xmm0, xmm4
			rsqrtps xmm0, xmm0
			mov eax, result
			mulps xmm1, xmm0
			movaps [eax], xmm1
		}
	}
	else
	{
		__asm
		{
			mov ecx, a
			mov eax, result
			movq mm3, qword ptr [ecx]
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
}

// @retail 0x28cb70
void function_28cb70()
{
	s_animation_data *data = g_504480;
	dword frame_info = *(dword *)((byte *)data + (data->vector_offset + g_5044b8 * 4));
	long start = frame_info >> 12;
	long count = frame_info & 0xfff;
	byte *keys = (byte *)data + data->vector_stride + start;
	real_vector3d *vectors = (real_vector3d *)((byte *)data + data->unknown24 + start * 12);
	long low = 0;
	long high = count;
	real_vector3d *vector;
	real_vector3d *destination;

	while (high > low + 1)
	{
		long middle = (low + high) >> 1;

		if ((dword)keys[middle] <= (dword)g_504464)
		{
			low = middle;
		}
		else
		{
			high = middle;
		}
	}

	destination = &g_5044c0->vector;
	_mm_prefetch((char *)(vectors + low), _MM_HINT_T0);
	vector = vectors + low;
	*destination = *vector;
	keys += low;

	if (low + 1 < count)
	{
		real x = g_504464 + g_50446c;
		real difference = (real)(keys[1] - keys[0]);
		real reciprocal;
		real weight;

		__asm
		{
			rcpss xmm0, difference
			movss reciprocal, xmm0
		}

		weight = reciprocal * (x - keys[0]);
		real_vector3d next;

		vector++;
		next = *vector;
		destination->i = (next.i - destination->i) * weight + destination->i;
		destination->j = (next.j - destination->j) * weight + destination->j;
		destination->k = (next.k - destination->k) * weight + destination->k;
	}
}

// @retail 0x28ccc0
void function_28ccc0()
{
	s_animation_data *data = g_504480;
	dword frame_info = *(dword *)((byte *)data + (data->scale_offset + g_5044bc * 4));
	long start = frame_info >> 12;
	long count = frame_info & 0xfff;
	byte *keys = (byte *)data + data->scale_stride + start;
	dword *scales = (dword *)((byte *)data + data->unknown28 + start * 4);
	long low = 0;
	long high = count;
	dword *scale;
	s_animation_output *result;

	while (high > low + 1)
	{
		long middle = (low + high) >> 1;

		if ((dword)keys[middle] <= (dword)g_504464)
		{
			low = middle;
		}
		else
		{
			high = middle;
		}
	}

	_mm_prefetch((char *)(scales + low), _MM_HINT_T0);
	scale = scales + low;
	result = g_5044c0;
	*(dword *)&result->scale = *scale;
	keys += low;

	if (low + 1 < count)
	{
		real x = g_504464 + g_50446c;
		real difference = (real)(keys[1] - keys[0]);
		real reciprocal;
		real weight;

		__asm
		{
			rcpss xmm0, difference
			movss reciprocal, xmm0
		}

		weight = reciprocal * (x - keys[0]);
		dword bits_next = scale[1];
		real next = *(real *)&bits_next;

		result->scale = (next - result->scale) * weight + result->scale;
	}
}
