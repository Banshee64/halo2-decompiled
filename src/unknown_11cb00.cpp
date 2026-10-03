// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_11CB00.CPP: language and region codes, quaternion and ray math */

#include "cseries.h"
#include "globals.h"
#include "unknown_11cb00.h"
#include "unknown_11cc90.h"
#include <math.h>
#include <xtl.h>

long g_47ff34 = -1;

void function_11d610(real_quaternion *q);
void function_11d820(real_quaternion const *a, real_quaternion const *b, real_quaternion *out);
short function_120850(real_vector3d const *v);

// @retail 0x11cb00
char const *function_11cb00(
	long language)
{
	struct
	{
		long language;
		char const *name;
	} const names[9] =
	{
		{ 0, "" },
		{ 1, "jpn" },
		{ 2, "de" },
		{ 3, "fr" },
		{ 4, "sp" },
		{ 5, "it" },
		{ 6, "kor" },
		{ 7, "cht" },
		{ 8, "pt" }
	};
	char const *name = "";
	dword i = 0;

	do
	{
		if (names[i].language == language)
		{
			name = names[i].name;
			break;
		}
		i++;
	}
	while (i < 9);
	return name;
}

// @retail 0x11cbb0
long function_11cbb0()
{
	if (g_47ff34 == NONE)
	{
		long region = 0;

		switch (XGetGameRegion())
		{
		case XC_GAME_REGION_NA:
			region = 0;
			break;
		case XC_GAME_REGION_JAPAN:
			region = 1;
			break;
		case XC_GAME_REGION_RESTOFWORLD:
			region = 2;
			break;
		case XC_GAME_REGION_MANUFACTURING:
			region = 3;
			break;
		}
		g_47ff34 = region;
	}
	return g_47ff34;
}

// @retail 0x11d950
void function_11d950(
	real_quaternion const *b,
	real_quaternion const *a,
	real_quaternion *out,
	real t)
{
	real s = 1.f - t;
	real dot = a->k * b->k + a->j * b->j + b->w * a->w + b->i * a->i;

	if (dot < 0.f)
		t = 0.f - t;
	out->i = b->i * s + a->i * t;
	out->j = a->j * t + s * b->j;
	out->k = a->k * t + s * b->k;
	out->w = b->w * s + t * a->w;
	function_11d610(out);
}

// @retail 0x11da10
void function_11da10(
	real_quaternion const *a,
	real_quaternion const *b,
	real_quaternion *out,
	real t)
{
	real omega;
	real s = 1.f - t;
	real dot = a->j * b->j + a->k * b->k + b->i * a->i + a->w * b->w;
	real sin_omega;
	real ta;
	real tb;

	if (dot > 1.f)
		dot = 1.f;
	else if (-1.f > dot)
		dot = -1.f;
	omega = (real)acos(dot);
	sin_omega = (real)sin(omega);
	if (!(0.0001f > (real)fabs(sin_omega)))
	{
		real inv = 1.f / sin_omega;

		ta = (real)sin(omega * t) * inv;
		tb = (real)sin(omega * s) * inv;
		if (dot < 0.f)
			ta = 0.f - ta;
	}
	else
	{
		long q;

		__asm
		{
			fld t
			fistp q
		}
		if (0.f > (real)q)
		{
			ta = 0.f;
			tb = 1.f - ta;
		}
		else
		{
			__asm
			{
				fld t
				fistp q
			}
			if ((real)q > 1.f)
			{
				ta = 1.f;
				tb = 1.f - ta;
			}
			else
			{
				__asm
				{
					fld t
					fistp q
				}
				ta = (real)q;
				tb = 1.f - ta;
			}
		}
	}
	out->i = b->i * tb + a->i * ta;
	out->j = a->j * ta + tb * b->j;
	out->k = a->k * ta + tb * b->k;
	out->w = a->w * ta + b->w * tb;
}

// rotates a point by a unit quaternion
__inline void quaternion_rotate_point(
	real_quaternion const *q,
	real_point3d const *v,
	real_point3d *out)
{
	real d = (q->j * v->y + v->x * q->i + v->z * q->k) * 2.f;
	real w2 = q->w * 2.f;
	real m = q->w * q->w * 2.f - 1.f;
	real cx = q->j * v->z - v->y * q->k;
	real cy = v->x * q->k - q->i * v->z;
	real cz = q->i * v->y - q->j * v->x;

	out->x = cx * w2 + v->x * m + q->i * d;
	out->y = q->j * d + cy * w2 + m * v->y;
	out->z = v->z * m + cz * w2 + d * q->k;
}

// @retail 0x11dbb0
void function_11dbb0(
	real_quaternion_transform *out,
	real_quaternion_transform const *a,
	real_quaternion_transform const *b)
{
	real_quaternion_transform temp;

	if (a == out)
	{
		temp = *a;
		a = &temp;
	}
	if (b == out)
	{
		temp = *b;
		b = &temp;
	}
	function_11d820(&a->rotation, &b->rotation, &out->rotation);
	quaternion_rotate_point(&a->rotation, &b->position, &out->position);
	if (a->scale != 1.f)
	{
		out->position.x *= a->scale;
		out->position.y *= a->scale;
		out->position.z *= a->scale;
		out->scale = b->scale * a->scale;
	}
	else
	{
		out->scale = b->scale;
	}
	out->position.x = a->position.x + out->position.x;
	out->position.y = a->position.y + out->position.y;
	out->position.z = a->position.z + out->position.z;
}

// @retail 0x11dd80
void function_11dd80(
	real_quaternion_transform *out,
	real_quaternion_transform const *in)
{
	real inv = 1.f;

	out->rotation = in->rotation;
	out->rotation.w = out->rotation.w * -1.f;
	{
		real_quaternion const *q = &out->rotation;
		real_point3d const *v = &in->position;
		real d = (v->x * q->i + v->y * q->j + q->k * v->z) * 2.f;
		real m = q->w * q->w * 2.f - 1.f;
		real w2 = q->w * 2.f;
		real cy = q->k * v->x - q->i * v->z;
		real cz = v->y * q->i - v->x * q->j;
		real cx = q->j * v->z - v->y * q->k;

		out->position.x = cx * w2 + v->x * m + q->i * d;
		out->position.y = v->y * m + q->j * d + cy * w2;
		out->position.z = q->k * d + cz * w2 + m * v->z;
	}
	if (0.f == in->scale)
		inv = 10000.f;
	else if (1.f != in->scale)
		inv = 1.f / in->scale;
	out->position.x = out->position.x * inv;
	out->position.y = out->position.y * inv;
	out->position.z = out->position.z * inv;
	out->position.x = out->position.x * -1.f;
	out->position.y = out->position.y * -1.f;
	out->position.z = out->position.z * -1.f;
	out->scale = inv;
}

// @retail 0x11df30
real_vector2d *function_11df30(
	real_vector2d *angles,
	real_vector3d const *v)
{
	angles->i = (real)atan2(v->j, v->i);
	angles->j = (real)atan2(v->k, sqrt(v->j * v->j + v->i * v->i));
	return angles;
}

// @retail 0x11e000
real function_11e000(
	real_point3d const *b,
	real_point3d const *a,
	real_vector3d const *d)
{
	if (d->j * d->j + d->k * d->k + d->i * d->i > 0.0001f)
	{
		real_vector3d v;
		real t;

		v.k = b->z - a->z;
		v.i = b->x - a->x;
		v.j = b->y - a->y;
		t = (d->i * v.i + v.k * d->k + v.j * d->j) / (d->j * d->j + d->k * d->k + d->i * d->i);
		real nt;
		real x;
		real y;
		real z;

		if (0.f > t)
			t = 0.f;
		else if (t > 1.f)
			t = 1.f;
		nt = 0.f - t;
		x = d->i * nt + v.i;
		y = nt * d->j + v.j;
		z = nt * d->k + v.k;
		return z * z + y * y + x * x;
	}
	else
	{
		real_vector3d u;

		u.j = a->y - b->y;
		u.i = a->x - b->x;
		u.k = a->z - b->z;
		return u.k * u.k + u.j * u.j + u.i * u.i;
	}
}

// @retail 0x11e5e0
bool function_11e5e0(
	real_point3d const *origin,
	real_point3d const *center,
	real_vector3d const *direction,
	real radius)
{
	real_vector3d delta;
	real c;

	delta.i = origin->x - center->x;
	delta.j = origin->y - center->y;
	delta.k = origin->z - center->z;
	c = delta.k * delta.k + delta.j * delta.j + delta.i * delta.i - radius * radius;

	if (c < 0.f)
		return true;
	else
	{
		real_vector3d d = *direction;
		real b = d.k * delta.k + d.j * delta.j + d.i * delta.i;

		if (b >= 0.f)
			return false;
		else
		{
			real a = d.k * d.k + d.j * d.j + d.i * d.i;
			real disc = b * b - a * c;

			if (disc <= 0.f)
				return false;
			else
			{
				real e = 0.f - a - b;

				if (e < 0.f)
					return true;
				return disc > e * e;
			}
		}
	}
}

// @retail 0x11e6f0
real function_11e6f0(
	real_point3d const *origin,
	real_point3d const *center,
	real_vector3d const *direction,
	real radius)
{
	real_vector3d delta;
	real c;

	delta.i = origin->x - center->x;
	delta.j = origin->y - center->y;
	delta.k = origin->z - center->z;
	c = delta.k * delta.k + delta.j * delta.j + delta.i * delta.i - radius * radius;

	if (c < 0.f)
		return 0.f;
	else
	{
		real_vector3d d = *direction;
		real b = d.k * delta.k + d.j * delta.j + d.i * delta.i;

		if (b >= 0.f)
			return 3.4028235e38f;
		else
		{
			real a = d.k * d.k + d.j * d.j + d.i * d.i;
			real disc = b * b - a * c;

			if (disc <= 0.f)
				return 3.4028235e38f;
			return (0.f - b - (real)sqrt(disc)) / a;
		}
	}
}

static const short k_axis_indices[6][3] =
{
	{ 2, 1, 0 },
	{ 1, 2, 0 },
	{ 0, 2, 1 },
	{ 2, 0, 1 },
	{ 1, 0, 2 },
	{ 0, 1, 2 }
};

// @retail 0x11e800
bool function_11e800(
	real_point3d const *a,
	real_point3d const *b,
	real_point3d const *c,
	real_point3d const *p,
	real *u,
	real *v)
{
	real w[3];
	real e2[3];
	real e1[3];
	real_vector3d n;
	real d;

	w[0] = p->x - a->x;
	w[1] = p->y - a->y;
	w[2] = p->z - a->z;
	e1[0] = b->x - a->x;
	e1[1] = b->y - a->y;
	e1[2] = b->z - a->z;
	e2[0] = c->x - a->x;
	e2[1] = c->y - a->y;
	e2[2] = c->z - a->z;
	n.i = e1[2] * e2[1] - e1[1] * e2[2];
	n.j = e2[2] * e1[0] - e1[2] * e2[0];
	n.k = e1[1] * e2[0] - e2[1] * e1[0];
	d = n.j * w[1] + n.k * w[2] + n.i * w[0];
	if ((n.k * n.k + n.j * n.j + n.i * n.i) * 0.0001f > d * d)
	{
		short axis = function_120850(&n);
		bool positive = n.n[axis] > 0.f;
		short const *indices = k_axis_indices[axis * 2 + positive];
		long i0 = indices[0];
		long i1 = indices[1];
		real w1 = w[i1];
		real e2_1 = e2[i1];
		real e2_0 = e2[i0];
		real w0 = w[i0];
		real ca = w1 * e2_0 - e2_1 * w0;

		if (ca > -0.0001f)
		{
			real e1_1 = e1[i1];
			real e1_0 = e1[i0];
			real cb = e1_1 * w0 - w1 * e1_0;

			if (cb > -0.0001f)
			{
				real cd = e1_1 * e2_0 - e2_1 * e1_0;

				if (-0.0001f > cd || cd > 0.0001f)
				{
					if (cd + 0.0001f >= cb + ca)
					{
						real inv = 1.f / cd;

						*u = inv * cb;
						*v = inv * ca;
						return true;
					}
				}
			}
		}
	}
	return false;
}

// @retail 0x11f9a0
long rectangle3d_build_vertices(
	real_rectangle3d const *rectangle,
	long maximum_vertex_count,
	real_point3d vertices[])
{
	vertices[0].x = rectangle->x0;
	vertices[0].y = rectangle->y0;
	vertices[0].z = rectangle->z0;
	vertices[1].x = rectangle->x1;
	vertices[1].y = rectangle->y0;
	vertices[1].z = rectangle->z0;
	vertices[2].x = rectangle->x0;
	vertices[2].y = rectangle->y1;
	vertices[2].z = rectangle->z0;
	vertices[3].x = rectangle->x1;
	vertices[3].y = rectangle->y1;
	vertices[3].z = rectangle->z0;
	vertices[4].x = rectangle->x0;
	vertices[4].y = rectangle->y0;
	vertices[4].z = rectangle->z1;
	vertices[5].x = rectangle->x1;
	vertices[5].y = rectangle->y0;
	vertices[5].z = rectangle->z1;
	vertices[6].x = rectangle->x0;
	vertices[6].y = rectangle->y1;
	vertices[6].z = rectangle->z1;
	vertices[7].x = rectangle->x1;
	vertices[7].y = rectangle->y1;
	vertices[7].z = rectangle->z1;
	return 8;
}