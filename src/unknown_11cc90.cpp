// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_11CC90.CPP: angles between vectors */

#include "cseries.h"
#include "globals.h"
#include "unknown_11cc90.h"
#include <math.h>


// @retail 0x11cc90
real function_11cc90(
	real_vector2d const *a,
	real_vector2d const *b)
{
	real d = a->i * b->i + a->j * b->j;
	real angle;
	d = d < -1.f ? -1.f : d;
	d = d > 1.f ? 1.f : d;
	angle = (real)acos(d);
	if (a->j * b->i - a->i * b->j < 0.f)
		return 0.f - angle;
	return angle;
}

// @retail 0x11cd20
real function_11cd20(
	real_vector2d const *a,
	real_vector2d const *b)
{
	real result = 0.f;
	real l = (b->i * b->i + b->j * b->j) * (a->i * a->i + a->j * a->j);
	if (l != 0.f)
	{
		real d = b->i * a->i + b->j * a->j;
		real c = d / l * d * 2.f - 1.f;
		c = c < -1.f ? -1.f : c;
		c = c > 1.f ? 1.f : c;
		result = (real)acos(c) * 0.5f;
		if (d < 0.f)
			result = g_5476c4 - result;
	}
	return result;
}

// @retail 0x11ce20
real function_11ce20(
	real_vector3d const *a,
	real_vector3d const *b)
{
	real result = 0.f;
	real l = (b->i * b->i + b->j * b->j + b->k * b->k) * (a->i * a->i + a->j * a->j + a->k * a->k);
	if (l != 0.f)
	{
		real d = b->k * a->k + b->j * a->j + b->i * a->i;
		real c = d / l * d * 2.f - 1.f;
		c = c < -1.f ? -1.f : c;
		c = c > 1.f ? 1.f : c;
		result = (real)acos(c) * 0.5f;
		if (d < 0.f)
			result = g_5476c4 - result;
	}
	return result;
}

// @retail 0x11cf50
real function_11cf50(
	real_vector3d const *a,
	real_vector3d const *b)
{
	real d, angle;
	if (((long const *)a)[0] == ((long const *)b)[0] && ((long const *)a)[1] == ((long const *)b)[1] && ((long const *)a)[2] == ((long const *)b)[2])
		return 0.f;
	d = a->k * b->k + a->j * b->j + a->i * b->i;
	d = d < -1.f ? -1.f : d;
	d = d > 1.f ? 1.f : d;
	angle = (real)acos(d);
	return angle;
}

// @retail 0x11d000
real_vector3d *function_11d000(
	real_vector3d const *v,
	real_vector3d *out)
{
	real x = (real)fabs(v->i);
	real y = (real)fabs(v->j);
	real z = (real)fabs(v->k);
	if (y >= x && z >= x)
	{
		out->i = 0.f;
		out->j = v->k;
		out->k = 0.f - v->j;
	}
	else if (z >= y)
	{
		out->i = 0.f - v->k;
		out->j = 0.f;
		out->k = v->i;
	}
	else
	{
		out->i = v->j;
		out->j = 0.f - v->i;
		out->k = 0.f;
	}
	return out;
}

// @retail 0x11d090
real_vector3d *function_11d090(
	real_vector3d const *v,
	real_vector3d *out)
{
	real_vector3d w;
	real length;
	w.i = v->j;
	w.j = 0.f - v->i;
	w.k = 0.f;
	length = (real)sqrt(w.j * w.j + w.i * w.i + w.k * w.k);
	if (!(0.0001f > fabs(length)) && length != 0.f)
	{
		real inv = 1.f / length;
		w.i = inv * w.i;
		w.j = inv * w.j;
		w.k = inv * w.k;
	}
	else
	{
		w = *g_4687a8;
	}
	out->i = v->k * w.j - w.k * v->j;
	out->j = v->i * w.k - v->k * w.i;
	out->k = w.i * v->j - v->i * w.j;
	return out;
}
