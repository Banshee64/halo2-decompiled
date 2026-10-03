// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_141590.CPP: real_matrix4x3 and quaternion math (matrix_math.obj) */

#include "cseries.h"
#include "real_math.h"
#include <math.h>
#include <string.h>

struct real_orientation
{
	real_quaternion rotation;
	real_point3d position;
	real scale;
};

real_vector3d *function_11d000(real_vector3d const *v, real_vector3d *out);

__inline void real_point3d_set(real_point3d *point, real x, real y, real z)
{
	point->x = x;
	point->y = y;
	point->z = z;
}

// @retail 0x141590
void function_141590(
	real_matrix4x3 const *in,
	real_matrix4x3 *out)
{
	if (in->scale != 0.f)
	{
		real_vector3d v;
		real t;
		v.i = 0.f - in->position.x;
		v.j = 0.f - in->position.y;
		v.k = 0.f - in->position.z;
		if (in->scale != 1.f)
		{
			real inverse = 1.f / in->scale;
			out->scale = inverse;
			v.i = inverse * v.i;
			v.j = inverse * v.j;
			v.k = inverse * v.k;
		}
		else
		{
			out->scale = 1.f;
		}
		out->forward.i = in->forward.i;
		out->left.j = in->left.j;
		out->up.k = in->up.k;
		t = in->left.i; out->left.i = in->forward.j; out->forward.j = t;
		t = in->up.i; out->up.i = in->forward.k; out->forward.k = t;
		t = in->up.j; out->up.j = in->left.k; out->left.k = t;
		out->position.x = out->forward.i * v.i + out->left.i * v.j + out->up.i * v.k;
		out->position.y = out->forward.j * v.i + out->left.j * v.j + out->up.j * v.k;
		out->position.z = out->forward.k * v.i + out->left.k * v.j + out->up.k * v.k;
	}
	else
	{
		memset(out, 0, sizeof(real_matrix4x3));
	}
}

// @retail 0x1416c0
void function_1416c0(
	real_point3d const *position,
	real_matrix4x3 *out)
{
	out->scale = 1.f;
	out->forward.i = 1.f;
	out->forward.j = 0.f;
	out->forward.k = 0.f;
	out->left.i = 0.f;
	out->left.j = 1.f;
	out->left.k = 0.f;
	out->up.i = 0.f;
	out->up.j = 0.f;
	out->up.k = 1.f;
	out->position = *position;
}

// @retail 0x141ce0
void function_141ce0(
	real a,
	real b,
	real c,
	real_matrix4x3 *out)
{
	real cos_c, sin_c, cos_b, sin_b, cos_a, sin_a, sin_b_sin_c, sin_b_cos_c;
	cos_c = (real)cos(c);
	sin_c = (real)sin(c);
	cos_b = (real)cos(b);
	sin_b = (real)sin(b);
	cos_a = (real)cos(a);
	sin_b_sin_c = sin_b * sin_c;
	sin_b_cos_c = sin_b * cos_c;
	sin_a = (real)sin(a);
	out->scale = 1.f;
	out->forward.j = sin_a * cos_c - sin_b_sin_c * cos_a;
	out->forward.i = cos_a * cos_b;
	out->forward.k = sin_b_cos_c * cos_a + sin_a * sin_c;
	out->left.i = 0.f - sin_a * cos_b;
	out->left.j = sin_b_sin_c * sin_a + cos_a * cos_c;
	out->up.i = 0.f - sin_b;
	out->left.k = cos_a * sin_c - sin_b_cos_c * sin_a;
	out->up.j = 0.f - cos_b * sin_c;
	out->up.k = cos_b * cos_c;
	out->position.x = 0.f;
	out->position.y = 0.f;
	out->position.z = 0.f;
}

// @retail 0x141e10
void function_141e10(
	matrix3x3 *out,
	real_quaternion const *q)
{
	real norm = q->i * q->i + q->j * q->j + q->k * q->k + q->w * q->w;
	real s = norm > 0.0001f ? 2.f / norm : 0.f;
	real xs = q->i * s;
	real ys = q->j * s;
	real zs = q->k * s;
	real wx = q->w * xs;
	real wy = q->w * ys;
	real wz = q->w * zs;
	real xx = q->i * xs;
	real xy = q->i * ys;
	real xz = q->i * zs;
	real yy = q->j * ys;
	real yz = q->j * zs;
	real zz = q->k * zs;
	out->forward.i = 1.f - (yy + zz);
	out->left.i = xy - wz;
	out->forward.j = xy + wz;
	out->up.i = xz + wy;
	out->left.j = 1.f - (xx + zz);
	out->up.j = yz - wx;
	out->forward.k = xz - wy;
	out->left.k = yz + wx;
	out->up.k = 1.f - (xx + yy);
}

// @retail 0x141f60
real_quaternion *function_141f60(
	matrix3x3 const *matrix,
	real_quaternion *out)
{
	real const *m = (real const *)matrix;
	real trace = matrix->left.j + matrix->forward.i + matrix->up.k;
	if (trace > 0.f)
	{
		real root = (real)(sqrt(trace + 1.f) * 0.5f);
		real inverse = 0.25f / root;
		out->w = root;
		out->i = (m[5] - m[7]) * inverse;
		out->j = (m[6] - m[2]) * inverse;
		out->k = (m[1] - m[3]) * inverse;
	}
	else
	{
		long i = 0, j, k;
		real root, inverse;
		if (m[4] > m[0])
			i = 1;
		if (m[8] > m[i * 4])
			i = 2;
		j = (i + 1) % 3;
		k = (i + 2) % 3;
		root = (real)(sqrt(m[i * 4] - m[j * 4] - m[k * 4] + 1.f) * 0.5f);
		inverse = 0.25f / root;
		out->n[i] = root;
		out->w = (m[j * 3 + k] - m[k * 3 + j]) * inverse;
		out->n[j] = (m[i * 3 + j] + m[j * 3 + i]) * inverse;
		out->n[k] = (m[i * 3 + k] + m[k * 3 + i]) * inverse;
	}
	if (out->w < 0.f)
	{
		out->w = 0.f - out->w;
		out->i = 0.f - out->i;
		out->j = 0.f - out->j;
		out->k = 0.f - out->k;
	}
	return out;
}

// @retail 0x1420f0
void function_1420f0(
	real_vector3d const *forward,
	real_vector3d const *up,
	real_point3d const *position,
	real_matrix4x3 *out)
{
	out->scale = 1.f;
	out->forward = *forward;
	real_vector3d left;
	left.k = forward->j * up->i - forward->i * up->j;
	left.j = up->k * forward->i - forward->k * up->i;
	left.i = forward->k * up->j - up->k * forward->j;
	out->left.k = left.k;
	out->left.j = left.j;
	out->left.i = left.i;
	out->up = *up;
	real_point3d_set(&out->position, 0.f, 0.f, 0.f);
	out->position = *position;
}

real g_45dbdc = 0.0001f;
static const real g_45dbc0 = 1.f;
__declspec(align(16)) static const unsigned long g_453750[4] = {0x80000000, 0, 0, 0x80000000};

// @retail 0x1421f0
void __stdcall function_1421f0(
	real_matrix4x3 *out,
	real_orientation const *orientation)
{
	static real epsilon = g_45dbdc;
	__asm
	{
		mov eax, orientation
		mov ecx, out
		movlps xmm0, qword ptr [eax]
		movhps xmm0, qword ptr [eax + 8]
		movaps xmm7, xmm0
		mulps xmm0, xmm7
		movhlps xmm1, xmm0
		addps xmm0, xmm1
		movaps xmm1, xmm0
		shufps xmm1, xmm1, 0x55
		addss xmm0, xmm1
		comiss xmm0, epsilon
		ja big
		movss xmm0, g_45dbc0
		movlps qword ptr [ecx + 4], xmm0
		movhps qword ptr [ecx + 0xc], xmm0
		movlps qword ptr [ecx + 0x14], xmm0
		movhps qword ptr [ecx + 0x1c], xmm0
		movss dword ptr [ecx + 0x24], xmm0
		jmp done
	big:
		rcpss xmm1, xmm0
		mulss xmm0, xmm1
		mulss xmm0, xmm1
		addss xmm1, xmm1
		subss xmm1, xmm0
		addss xmm1, xmm1
		shufps xmm1, xmm1, 0
		mulps xmm7, xmm1
		movss xmm4, dword ptr [eax]
		shufps xmm4, xmm4, 0
		mulps xmm4, xmm7
		movss xmm5, dword ptr [eax + 0xc]
		shufps xmm5, xmm5, 0
		mulps xmm5, xmm7
		movss xmm6, dword ptr [eax + 4]
		shufps xmm6, xmm6, 0
		mulps xmm6, xmm7
		movhlps xmm7, xmm7
		mulss xmm7, dword ptr [eax + 8]
		movaps xmm0, xmm4
		shufps xmm0, xmm0, 0x99
		movaps xmm1, xmm5
		shufps xmm1, xmm1, 0x66
		xorps xmm1, xmmword ptr g_453750
		addps xmm0, xmm1
		movhps qword ptr [ecx + 8], xmm0
		movss dword ptr [ecx + 0x10], xmm0
		movlps qword ptr [ecx + 0x18], xmm0
		movhlps xmm2, xmm6
		shufps xmm6, xmm6, 0x55
		movss xmm0, g_45dbc0
		subss xmm0, xmm7
		movss xmm1, xmm0
		subss xmm0, xmm6
		movss dword ptr [ecx + 4], xmm0
		subss xmm1, xmm4
		movss dword ptr [ecx + 0x14], xmm1
		movss xmm3, g_45dbc0
		subss xmm3, xmm4
		subss xmm3, xmm6
		movss dword ptr [ecx + 0x24], xmm3
		movss xmm7, xmm2
		subss xmm7, xmm5
		movss dword ptr [ecx + 0x20], xmm7
		addss xmm2, xmm5
		movss dword ptr [ecx + 0x18], xmm2
	done:
		movss xmm0, dword ptr [eax + 0x1c]
		movss dword ptr [ecx], xmm0
		movlps xmm0, qword ptr [eax + 0x10]
		movlps qword ptr [ecx + 0x28], xmm0
		movss xmm0, dword ptr [eax + 0x18]
		movss dword ptr [ecx + 0x30], xmm0
	}
}

__inline real normalize3d(real_vector3d *v)
{
	real m = (real)sqrt(v->i * v->i + v->j * v->j + v->k * v->k);
	if (!(fabs(m) < 0.0001f))
	{
		real inv = 1.f / m;
		v->i = inv * v->i;
		v->j = v->j * inv;
		v->k = inv * v->k;
		return m;
	}
	return 0.f;
}

// @retail 0x142390
void function_142390(
	real_plane3d const *plane,
	real_matrix4x3 *out)
{
	real_vector3d w;
	real_point3d position;
	function_11d000(&plane->n, &w);
	normalize3d(&w);
	position.x = plane->d * plane->n.i;
	position.y = plane->n.j * plane->d;
	position.z = plane->n.k * plane->d;
	function_1420f0(&w, &plane->n, &position, out);
}

// @retail 0x142570
real_point3d *matrix4x3_transform_point(
	real_matrix4x3 const *matrix,
	real_point3d const *point,
	real_point3d *out)
{
	/* retail passes the matrix on the stack (ret 4) while 0x142640, with the
	   same body, takes it in ecx: the parameter's address is taken here, and
	   the optimizer removes the indirection only after LTCG has chosen the
	   convention. Its callers' conventions (0x2104b0 ...) follow from it. */
	real_matrix4x3 const *const *matrix_reference = &matrix;
	real x = point->x;
	real y = point->y;
	real z = point->z;
	if ((*matrix_reference)->scale != 1.f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	out->x = matrix->up.i * z + matrix->left.i * y + matrix->forward.i * x + matrix->position.x;
	out->y = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	out->z = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x + matrix->position.z;
	return out;
}

// @retail 0x142640
real_vector3d *function_142640(
	real_matrix4x3 const *matrix,
	real_vector3d const *vector,
	real_vector3d *out)
{
	real x = vector->i;
	real y = vector->j;
	real z = vector->k;
	if (matrix->scale != 1.f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	out->i = matrix->up.i * z + matrix->left.i * y + matrix->forward.i * x;
	out->j = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x;
	out->k = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x;
	return out;
}

// @retail 0x142700
real_point3d *function_142700(
	real_matrix4x3 const *matrix,
	real_point3d const *point,
	real_point3d *out)
{
	if (matrix->scale != 0.f)
	{
		real x = point->x - matrix->position.x;
		real y = point->y - matrix->position.y;
		real z = point->z - matrix->position.z;
		if (matrix->scale != 1.f)
		{
			real inverse = 1.f / matrix->scale;
			x = inverse * x;
			y = inverse * y;
			z = inverse * z;
		}
		out->x = matrix->forward.k * z + matrix->forward.j * y + matrix->forward.i * x;
		out->y = matrix->left.k * z + matrix->left.j * y + matrix->left.i * x;
		out->z = matrix->up.k * z + matrix->up.j * y + matrix->up.i * x;
	}
	else
	{
		out->x = 0.f;
		out->y = 0.f;
		out->z = 0.f;
	}
	return out;
}

// @retail 0x1427f0
real_vector3d *matrix4x3_inverse_transform_vector(
	real_matrix4x3 const *matrix,
	real_vector3d const *vector,
	real_vector3d *out)
{
	real x = vector->i;
	real y = vector->j;
	real z = vector->k;
	if (matrix->scale != 1.f)
	{
		real inverse = 1.f / matrix->scale;
		x = inverse * x;
		y = inverse * y;
		z = inverse * z;
	}
	out->i = matrix->forward.k * z + matrix->forward.j * y + matrix->forward.i * x;
	out->j = matrix->left.k * z + matrix->left.j * y + matrix->left.i * x;
	out->k = matrix->up.k * z + matrix->up.j * y + matrix->up.i * x;
	return out;
}

// @retail 0x143250
void function_143250(
	real *out,
	unsigned long n,
	real const *a,
	real const *b)
{
	unsigned long i, j;
	for (i = 0; i < n; i++)
	{
		out[i] = b[0] * a[i * n];
		for (j = 1; j < n; j++)
			out[i] = b[j] * a[i * n + j] + out[i];
	}
}
