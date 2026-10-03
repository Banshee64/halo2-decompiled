// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_1428B0.CPP: matrix math (matrix_math.obj) */

#include "cseries.h"
#include "unknown_1428b0.h"
#include <math.h>

// @retail 0x142b80
matrix3x3 *matrix3x3_transpose(
	matrix3x3 const *in,
	matrix3x3 *out)
{
	if (in == out)
	{
		real t;
		t = in->forward.j; out->forward.j = in->left.i; out->left.i = t;
		t = in->forward.k; out->forward.k = in->up.i; out->up.i = t;
		t = in->left.k; out->left.k = in->up.j; out->up.j = t;
	}
	else
	{
		out->forward.i = in->forward.i;
		out->forward.j = in->left.i;
		out->forward.k = in->up.i;
		out->left.i = in->forward.j;
		out->left.j = in->left.j;
		out->left.k = in->up.j;
		out->up.i = in->forward.k;
		out->up.j = in->left.k;
		out->up.k = in->up.k;
	}
	return out;
}
__inline real_vector3d *cross_product3d(
	real_vector3d const *a,
	real_vector3d const *b,
	real_vector3d *result)
{
	result->i = a->j * b->k - a->k * b->j;
	result->j = a->k * b->i - a->i * b->k;
	result->k = a->i * b->j - a->j * b->i;
	return result;
}

// @retail 0x142d10
matrix3x3 *function_142d10(
	real_vector3d const *up,
	real_vector3d const *forward,
	matrix3x3 *out)
{
	out->forward = *forward;
	out->left.k = up->i * forward->j - up->j * forward->i;
	out->left.j = up->k * forward->i - up->i * forward->k;
	out->left.i = up->j * forward->k - up->k * forward->j;
	out->up = *up;
	return out;
}

// @retail 0x142eb0
matrix3x3 *function_142eb0(
	matrix3x3 const *a,
	matrix3x3 const *b,
	matrix3x3 *out)
{
	matrix3x3 temp;
	if (b == out)
	{
		temp = *b;
		b = &temp;
	}
	if (a == out)
	{
		temp = *a;
		a = &temp;
	}
	out->forward.i = b->forward.i * a->forward.i + b->left.i * a->forward.j + b->up.i * a->forward.k;
	out->forward.j = b->forward.j * a->forward.i + b->left.j * a->forward.j + b->up.j * a->forward.k;
	out->forward.k = b->forward.k * a->forward.i + b->left.k * a->forward.j + b->up.k * a->forward.k;
	out->left.i = b->forward.i * a->left.i + b->left.i * a->left.j + b->up.i * a->left.k;
	out->left.j = b->forward.j * a->left.i + b->left.j * a->left.j + b->up.j * a->left.k;
	out->left.k = b->forward.k * a->left.i + b->left.k * a->left.j + b->up.k * a->left.k;
	out->up.i = b->forward.i * a->up.i + b->left.i * a->up.j + b->up.i * a->up.k;
	out->up.j = b->forward.j * a->up.i + b->left.j * a->up.j + b->up.j * a->up.k;
	out->up.k = b->forward.k * a->up.i + b->left.k * a->up.j + b->up.k * a->up.k;
	return out;
}

// @retail 0x143070
real_vector3d *function_143070(
	real_vector3d const *v,
	matrix3x3 const *m,
	real_vector3d *out)
{
	real_vector3d temp;
	if (v == out)
	{
		temp = *v;
		v = &temp;
	}
	out->i = m->forward.i * v->i + m->left.i * v->j + m->up.i * v->k;
	out->j = m->forward.j * v->i + m->left.j * v->j + m->up.j * v->k;
	out->k = m->forward.k * v->i + m->left.k * v->j + m->up.k * v->k;
	return out;
}

// @retail 0x142da0
matrix3x3 *function_142da0(
	real yaw,
	real pitch,
	real roll,
	matrix3x3 *out)
{
	real cos_roll = (real)cos(roll);
	real sin_roll = (real)sin(roll);
	real cos_pitch = (real)cos(pitch);
	real sin_pitch = (real)sin(pitch);
	real cos_yaw = (real)cos(yaw);
	real sin_yaw = (real)sin(yaw);
	real cc = cos_yaw * cos_roll;
	real cs = cos_yaw * sin_roll;
	real sc = sin_yaw * cos_roll;
	real ss = sin_yaw * sin_roll;
	out->forward.i = cos_yaw * cos_pitch;
	out->forward.j = sin_yaw * cos_pitch;
	out->left.i = 0.f - cs * sin_pitch - sc;
	out->left.j = cc - ss * sin_pitch;
	out->forward.k = sin_pitch;
	out->left.k = cos_pitch * sin_roll;
	out->up.i = ss - cc * sin_pitch;
	out->up.j = 0.f - sc * sin_pitch - cs;
	out->up.k = cos_pitch * cos_roll;
	return out;
}
// @retail 0x142bf0
matrix3x3 *function_142bf0(
	matrix3x3 const *in,
	real scale,
	matrix3x3 *out)
{
	matrix3x3 temp;
	real one_over_scale = 1.f / scale;
	real const *m;
	real *r = (real *)out;
	short i, j;
	if (in == out)
	{
		temp = *in;
		in = &temp;
	}
	m = (real const *)in;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			short i1 = i < 2 ? i + 1 : 0;
			short i2 = i > 0 ? i - 1 : 2;
			short j1 = j < 2 ? j + 1 : 0;
			short j2 = j > 0 ? j - 1 : 2;
			r[j * 3 + i] = (m[i1 * 3 + j1] * m[i2 * 3 + j2] - m[i1 * 3 + j2] * m[i2 * 3 + j1]) * (1.f / scale);
		}
	}
	return out;
}

// @retail 0x1429d0
int __stdcall function_1429d0(
	real_matrix4x3 const *matrix,
	long count,
	real_point3d const *source,
	real_point3d *destination)
{
	__asm
	{
		mov ecx, matrix
		movss xmm0, dword ptr [ecx + 4]
		movss xmm1, dword ptr [ecx + 0x10]
		movss xmm2, dword ptr [ecx + 0x1c]
		movss xmm3, dword ptr [ecx + 0x28]
		movhps xmm0, qword ptr [ecx + 8]
		movhps xmm1, qword ptr [ecx + 0x14]
		movhps xmm2, qword ptr [ecx + 0x20]
		movhps xmm3, qword ptr [ecx + 0x2c]
		movss xmm4, dword ptr [ecx]
		shufps xmm4, xmm4, 0
		mulps xmm0, xmm4
		mulps xmm1, xmm4
		mulps xmm2, xmm4
		mov ecx, count
		cmp ecx, 0
		jle done
		mov edx, source
		mov eax, destination
		sub eax, edx
	again:
		movss xmm4, dword ptr [edx]
		movss xmm5, dword ptr [edx + 4]
		movss xmm6, dword ptr [edx + 8]
		shufps xmm4, xmm4, 0
		shufps xmm5, xmm5, 0
		shufps xmm6, xmm6, 0
		mulps xmm4, xmm0
		mulps xmm5, xmm1
		mulps xmm6, xmm2
		addps xmm4, xmm5
		addps xmm6, xmm3
		addps xmm4, xmm6
		movss dword ptr [edx + eax], xmm4
		movhps qword ptr [edx + eax + 4], xmm4
		add edx, 0xc
		dec ecx
		jne again
	done:
	}
}
// @retail 0x142a60
int __fastcall function_142a60(
	real_matrix4x3 const *a,
	real_matrix4x3 const *b,
	real_matrix4x3 *result)
{
	__asm
	{
		mov eax, result
		movss xmm7, dword ptr [ecx]
		shufps xmm7, xmm7, 0
		movss xmm3, dword ptr [edx]
		movss xmm0, dword ptr [edx + 0x28]
		movss xmm1, dword ptr [edx + 0x2c]
		movss xmm2, dword ptr [edx + 0x30]
		shufps xmm0, xmm0, 0
		shufps xmm1, xmm1, 0
		shufps xmm2, xmm2, 0
		mulss xmm3, xmm7
		movss xmm4, dword ptr [ecx + 4]
		movhps xmm4, qword ptr [ecx + 8]
		movss xmm5, dword ptr [ecx + 0x10]
		movhps xmm5, qword ptr [ecx + 0x14]
		movss xmm6, dword ptr [ecx + 0x1c]
		movhps xmm6, qword ptr [ecx + 0x20]
		mulps xmm0, xmm4
		mulps xmm1, xmm5
		mulps xmm2, xmm6
		addps xmm0, xmm1
		addps xmm0, xmm2
		mulps xmm0, xmm7
		movss xmm1, dword ptr [ecx + 0x28]
		movhps xmm1, qword ptr [ecx + 0x2c]
		addps xmm0, xmm1
		movss xmm2, dword ptr [edx + 0x24]
		movss dword ptr [eax + 0x28], xmm0
		movhps qword ptr [eax + 0x2c], xmm0
		movss xmm1, dword ptr [edx + 0x20]
		movss xmm0, dword ptr [edx + 0x1c]
		shufps xmm2, xmm2, 0
		shufps xmm1, xmm1, 0
		shufps xmm0, xmm0, 0
		mulps xmm2, xmm6
		mulps xmm1, xmm5
		mulps xmm0, xmm4
		addps xmm1, xmm2
		addps xmm0, xmm1
		movss xmm2, dword ptr [edx + 0x18]
		movss dword ptr [eax + 0x1c], xmm0
		movhps qword ptr [eax + 0x20], xmm0
		movss xmm1, dword ptr [edx + 0x14]
		movss xmm0, dword ptr [edx + 0x10]
		shufps xmm2, xmm2, 0
		shufps xmm1, xmm1, 0
		shufps xmm0, xmm0, 0
		mulps xmm2, xmm6
		mulps xmm1, xmm5
		mulps xmm0, xmm4
		addps xmm1, xmm2
		addps xmm0, xmm1
		movss xmm2, dword ptr [edx + 0xc]
		movss dword ptr [eax + 0x10], xmm0
		movhps qword ptr [eax + 0x14], xmm0
		movss xmm1, dword ptr [edx + 8]
		movss xmm0, dword ptr [edx + 4]
		shufps xmm2, xmm2, 0
		shufps xmm1, xmm1, 0
		shufps xmm0, xmm0, 0
		mulps xmm2, xmm6
		mulps xmm1, xmm5
		mulps xmm0, xmm4
		addps xmm1, xmm2
		addps xmm0, xmm1
		movss dword ptr [eax + 4], xmm0
		movhps qword ptr [eax + 8], xmm0
		movss dword ptr [eax], xmm3
	}
}
// @retail 0x1428b0
real_plane3d *function_1428b0(
	real_matrix4x3 const *matrix,
	real_plane3d const *plane,
	real_plane3d *out)
{
	real vi, vj, vk;
	if (matrix->scale == 0.f)
	{
		out->d = 0.f;
	}
	else
	{
		out->d = plane->d - (matrix->position.z * plane->n.k + matrix->position.y * plane->n.j + matrix->position.x * plane->n.i);
		if (matrix->scale != 1.f)
		{
			out->d = out->d / matrix->scale;
		}
	}
	vi = plane->n.i;
	vj = plane->n.j;
	vk = plane->n.k;
	if (matrix->scale != 1.f)
	{
		real inv = 1.f / matrix->scale;
		vi = inv * vi;
		vj = inv * vj;
		vk = inv * vk;
	}
	out->n.i = matrix->rotation.forward.k * vk + matrix->rotation.forward.j * vj + matrix->rotation.forward.i * vi;
	out->n.j = matrix->rotation.left.k * vk + matrix->rotation.left.j * vj + matrix->rotation.left.i * vi;
	out->n.k = matrix->rotation.up.k * vk + matrix->rotation.up.j * vj + matrix->rotation.up.i * vi;
	return out;
}
// @retail 0x142570
real_point3d *matrix4x3_transform_point(
	real_matrix4x3 const *matrix,
	real_point3d const *point,
	real_point3d *result)
{
	real x = point->x;
	real y = point->y;
	real z = point->z;
	if (matrix->scale != 1.f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	result->x = matrix->up.i * z + matrix->left.i * y + matrix->forward.i * x + matrix->position.x;
	result->y = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	result->z = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x + matrix->position.z;
	return result;
}
