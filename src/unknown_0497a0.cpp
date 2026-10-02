// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_0497A0.CPP: view parameter computation */

#include "cseries.h"
#include <math.h>
#include "unknown_0494b0.h"

real_point3d g_4b9da0;
real_vector3d g_4b9dac;
real_vector3d g_4b9db8;
real_vector3d *g_4687a4;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PIN(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

// @retail 0x497a0
void function_0497a0(
	s_view_source *source,
	s_view_result *result,
	s_view_camera *camera,
	s_view_flags *flags,
	real_vector3d const *c_in,
	real_vector3d const *d_in,
	real scale,
	long *mode,
	real_vector4d *old20,
	real_vector4d *old30)
{
	real inv = -1.0f;
	s_view_shape_0 * shape0 = 0;
	s_view_shape_1 * shape1;
	shape1 = 0;
	if (!(0 == source->type_low))
	{
		shape1 = &source->shape1;
	}
	else
	{
		shape0 = &source->shape0;
	}

	bool special = ((0x20 & flags->flags)) && (0x40000 & flags->flags);
	real_point3d v1 = special ? g_4b9da0 : source->v2c;
	real_vector3d v2 = special ? g_4b9dac : camera->v28;
	real_vector3d v3 = special ? g_4b9dac : (shape1 ? shape1->v4 : camera->v28);
	real_vector3d v4 = special ? g_4b9db8 : camera->v34;
	real_vector3d w;

	if (special)
	{
		real_vector3d c;
		c.i = v4.j * v2.k - v4.k * v2.j;
		real s1 = (real)sin(g_4b9da0.x * 5.6f + g_4b9da0.y);
		c.j = v4.k * v2.i - v4.i * v2.k;
		real s0 = (real)((sin(g_4b9da0.x * 8.2f) * cos(g_4b9da0.y * 7.8f) * sin(g_4b9da0.x) + cos(g_4b9da0.y * 7.75f)) * 0.5f);
		c.k = v4.i * v2.j - v4.j * v2.i;
		real amp = s0 * 0.03f;
		v1.x = (v4.i * amp) + v1.x;
		v1.y += amp * v4.j;
		v1.z = v1.z + (amp * v4.k);
		real k = s1 * 0.025f;
		v1.x = (c.i * k) + v1.x;
		v1.y += k * c.j;
		v1.z = (k * c.k) + v1.z;
		w.i = v2.j * v4.k - v2.k * v4.j;
		w.j = v2.k * v4.i - v2.i * v4.k;
		w.k = v2.i * v4.j - v2.j * v4.i;
	}
	else
	{
		w = source->v1c;
	}

	switch (flags->w8e)
	{
		case 1:
			if (shape1)
			{
				inv = -1.0f / (shape1->f2c - shape1->f28);
			}
			break;

		default:
			inv = -1.0f / (shape0 ? shape0->f0c : shape1->f2c);
			break;
	}

	if (source->type != 1)
	{
		result->p0 = v1;
		result->f0c = 1.0f;
		result->p10 = v1;
	}

	switch (source->type)
	{
	case 0:
	{
		result->v20.i = source->v1c.i * inv;
		result->v20.j = (inv * source->v1c.j);
		result->v20.k = inv * source->v1c.k;
		result->v30.i = camera->v34.i * inv;
		result->v30.j = inv * camera->v34.j;
		result->v30.k = inv * camera->v34.k;
		result->f2c = 0.0f;
		result->f3c = 0.0f;
		break;
	}

	case 1:
		break;

	default:
	{
		real_vector3d r;
		r.i = v3.j * v4.k - v3.k * v4.j;
		r.j = v3.k * v4.i - v3.i * v4.k;
		real_vector3d s;
		r.k = v4.j * v3.i - v3.j * v4.i;
		s.i = v3.j * w.k - v3.k * w.j;
		s.j = v3.k * w.i - w.k * v3.i;
		real sx = -1.0f / shape1->f14;
		s.k = w.j * v3.i - v3.j * w.i;
		real sy = -1.0f / shape1->f18;
		real k = inv * 0.5f;
		result->v20.i = k * (r.i * sx + v2.i);
		result->v20.j = (r.j * sx + v2.j) * k;
		result->v20.k = (r.k * sx + v2.k) * k;
		result->v30.i = k * (s.i * sy + v2.i);
		result->v30.j = k * (s.j * sy + v2.j);
		result->v30.k = k * (s.k * sy + v2.k);
		result->f2c = 0.0f;
		result->f3c = 0.0f;
		break;
	}
	}

	switch (flags->w8e)
	{
	default:
	{
		result->v40.i = v2.i * inv;
		result->v40.j = inv * v2.j;
		result->v40.k = v2.k * inv;
		result->f4c = inv * 0.5f;
		result->v50 = *g_4687a4;
		result->f5c = 0.0f;
		break;
	}

	case 1:
	{
		real_vector3d const * up = special ? &g_4b9dac : &camera->v28;
		if (source->type != 1)
		{
			result->v40.i = up->i * inv;
			result->v40.j = inv * up->j;
			result->v40.k = inv * up->k;
			result->f4c = inv * (shape1->f28 + source->f28);
			if (!(0x20 & flags->flags))
			{
				real h = (shape1->f2c - shape1->f28) * 0.025f * 0.5f;
				result->v50.i = h * result->v40.i;
				int tmp0;
				tmp0 = result->v40.j * h;
				result->v50.j = tmp0;
				result->v50.k = result->v40.k * h;
				result->f5c = (source->f28 + 0.0078125f) * h * inv + 0.5f;
			}
			else
			{
				result->v50 = *g_4687a4;
				result->f5c = 1.0f;
			}
		}
		break;
	}
	}

	*old20 = *(real_vector4d *)&result->v20;
	*old30 = *(real_vector4d *)&result->v30;
	result->v20.i = result->v20.i + (camera->f6c * result->v40.i);
	result->v20.j = (result->v40.j * camera->f6c) + result->v20.j;
	result->v20.k += camera->f6c * result->v40.k;
	result->v30.i += result->v40.i * camera->f70;
	result->v30.j = (result->v40.j * camera->f70) + result->v30.j;
	result->v30.k = (result->v30.k + (camera->f70 * result->v40.k));

	real_vector3d c = *c_in;
	real_vector3d d = *d_in;
	real ca = scale * c.i;
	real cb = scale * c.j;
	real cc = c.k * scale;
	real da = (d.i * scale);
	real db = d.j * scale;
	long tmp2 = (((scale * d.k)));
	real dc = tmp2;
	real mc = MAX(ca, MAX(cb, cc));
	real md = MAX(da, MAX(db, dc));
	real m = MAX(mc, md);

	if (m > 2.0f)
	{
		result->f60 = PIN(ca * 0.25f, 0.0f, 1.0f);
		result->f64 = PIN(cb * 0.25f, 0.0f, 1.0f);
		result->f68 = PIN(cc * 0.25f, 0.0f, 1.0f);
		result->f70 = PIN(da * 0.25f, 0.0f, 1.0f);
		result->f74 = PIN(db * 0.25f, 0.0f, 1.0f);
		result->f78 = PIN(dc * 0.25f, 0.0f, 1.0f);
		*mode = 2;
	}
	else if (m > 1.0f)
	{
		result->f60 = PIN(ca * 0.5f, 0.0f, 1.0f);
		result->f64 = PIN(cb * 0.5f, 0.0f, 1.0f);
		result->f68 = PIN(cc * 0.5f, 0.0f, 1.0f);
		result->f70 = PIN(da * 0.5f, 0.0f, 1.0f);
		result->f74 = PIN(db * 0.5f, 0.0f, 1.0f);
		result->f78 = PIN(dc * 0.5f, 0.0f, 1.0f);
		*mode = 1;
	}
	else
	{
		result->f60 = PIN(ca, 0.0f, 1.0f);
		result->f64 = PIN(cb, 0.0f, 1.0f);
		result->f68 = PIN(cc, 0.0f, 1.0f);
		result->f70 = PIN(da, 0.0f, 1.0f);
		result->f74 = PIN(db, 0.0f, 1.0f);
		result->f78 = PIN(dc, 0.0f, 1.0f);
		*mode = 0;
	}
}
