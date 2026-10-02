// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_11EED0.CPP: motion smoothing and quantization helpers */

#include "cseries.h"
#include "globals.h"
#include <math.h>

real function_30bf0(real_vector3d *v);

// @retail 0x11eed0
bool function_11eed0(
	real *velocity,
	real *position,
	real dt,
	bool wrap,
	real target,
	real a,
	real b,
	real lo,
	real hi)
{
	real v0 = *velocity;
	real p0 = *position;
	real dt2 = dt * dt;
	real orig = target;
	bool reached = false;
	real d = target - p0;
	real limit;
	real accel;
	real newv;
	real newp;
	real half;

	target = d;
	if (wrap)
	{
		half = (hi - lo) * 0.5f;
		if (d > half)
			d -= half * 2.f;
		else if (0.f - half > d)
			d = half * 2.f + d;
		target = d;
	}

	limit = a * dt;
	if (!(a * a * b > limit))
		limit = a * a * b;

	if (limit >= (real)fabs(target - v0 * a))
	{
		real c = orig;
		if (lo > c)
			c = lo;
		else if (c > hi)
			c = hi;
		reached = true;
		*velocity = 0.f;
		*position = c;
	}
	else
	{
		real x = (real)fabs(target) * b;
		x = x + x;
		target = x >= dt2 ? dt : (real)sqrt(x);
		if (0.f > d)
			target = 0.f - target;

		accel = (target - v0) / a;
		if ((real)fabs(accel) > b)
		{
			long s = accel == 0.f ? 0 : (0.f > accel ? -1 : 1);
			accel = (real)s * b;
		}

		{
			real dv = accel * a;
			newv = dv + v0;
			newp = (dv * 0.5f + newv) * a + p0;
		}
		if (wrap)
		{
			if (lo > newp)
				newp = hi - lo + newp;
			else if (newp > hi)
				newp = newp - (hi - lo);
		}
		if (lo > newp)
			newp = lo;
		else if (newp > hi)
			newp = hi;
		*velocity = newv;
		*position = newp;
	}
	return reached;
}

// @retail 0x11f0d0
void function_11f0d0(
	real_vector3d *position,
	real_vector3d *forward,
	real_vector3d const *target,
	real rate,
	real max_angle,
	real scale)
{
	real_vector3d axis;
	real_vector3d diff;
	real d;
	real angle;
	real step;
	real dist2;
	real theta;

	if (0.f >= scale && 0.f >= max_angle)
	{
		*position = *g_4687a4;
		*forward = *target;
		return;
	}

	d = target->k * forward->k + target->j * forward->j + forward->i * target->i;
	d = d < -1.f ? -1.f : d;
	d = d > 1.f ? 1.f : d;
	angle = (real)acos(d);
	angle = angle * scale;
	angle = angle + angle;
	max_angle = angle >= max_angle * max_angle ? max_angle : (real)sqrt(angle);

	axis.i = target->k * forward->j - forward->k * target->j;
	axis.j = target->i * forward->k - target->k * forward->i;
	axis.k = forward->i * target->j - target->i * forward->j;
	function_30bf0(&axis);

	axis.j = axis.j * max_angle;
	diff.j = axis.j - position->j;
	axis.k = axis.k * max_angle;
	axis.i = axis.i * max_angle;
	diff.k = axis.k - position->k;
	diff.i = axis.i - position->i;
	step = rate * scale;
	dist2 = diff.k * diff.k + diff.j * diff.j + diff.i * diff.i;

	if (step * step * 5.f > dist2)
	{
		if (step > max_angle)
		{
			*position = *g_4687a4;
			*forward = *target;
			return;
		}
		*position = axis;
	}
	else
	{
		real s = step / (real)sqrt(dist2);
		position->i = diff.i * s + position->i;
		position->j = diff.j * s + position->j;
		position->k = diff.k * s + position->k;
	}

	axis = *position;
	theta = function_30bf0(&axis) * rate;
	if (theta != 0.f)
	{
		real s = (real)sin(theta);
		real c = (real)cos(theta);
		real dot = (forward->k * axis.k + forward->j * axis.j + forward->i * axis.i) * (1.f - c);
		real cx = forward->k * axis.i - forward->i * axis.k;
		real cy = forward->i * axis.j - forward->j * axis.i;
		real cz = forward->j * axis.k - forward->k * axis.j;
		real nx = forward->i * c + axis.i * dot - cz * s;
		real ny = forward->j * c + axis.j * dot - cx * s;
		real nz = forward->k * c + axis.k * dot - cy * s;
		forward->i = nx;
		forward->j = ny;
		forward->k = nz;
		function_30bf0(forward);
	}
}

// @retail 0x11f470
byte function_11f470(real lo, real hi, real value)
{
	real range = hi - lo;
	byte result = 0;

	if (range > 0.0001f)
	{
		result = (byte)((value - lo) / range * 255.f);
		while (result > 0)
		{
			real v;
			if (result == 255)
				v = hi;
			else
				v = range * ((real)result * (1.f / 255.f)) + lo;
			if (!(v > value))
				break;
			result--;
		}
		return result;
	}
	return 0;
}

// @retail 0x11f4f0
void function_11f4f0(
	long bits,
	real const *in,
	real_bounds const *ranges,
	long *out)
{
	long i = 0;
	long max = (1 << bits) - 1;

	do
	{
		real v;
		long q;
		real lo;
		real hi;

		if (ranges[i].lo > in[i])
			v = ranges[i].lo;
		else if (in[i] > ranges[i].hi)
			v = ranges[i].hi;
		else
			v = in[i];
		lo = ranges[i].lo;
		hi = ranges[i].hi;
		v = (v - lo) / ((hi - lo) / (real)max);
		__asm
		{
			fld v
			fistp q
		}
		out[i] = q;
		i++;
	}
	while (i < 3);
}

// @retail 0x11f580
void function_11f580(
	long bits,
	real *out,
	real const *ranges,
	long const *in)
{
	long i = 0;
	long max = (1 << bits) - 1;

	do
	{
		long q = in[i];
		real hi = ranges[i * 2 + 1];
		real lo = ranges[i * 2];
		real v;

		if (q == 0)
			v = lo;
		else if (q >= max)
			v = hi;
		else
			v = ((real)(max - q) * lo + (real)q * hi) / (real)max;
		out[i] = v;
		i++;
	}
	while (i < 3);
}


// @retail 0x11f5f0
void function_11f5f0(
	real_rectangle2d *bounds,
	real_point2d const *points,
	long count)
{
	long i;

	for (i = 0; i < count; i++)
	{
		real_point2d const *p = points + i;
		if (bounds->x0 > p->x)
			bounds->x0 = p->x;
		if (p->x > bounds->x1)
			bounds->x1 = p->x;
		if (bounds->y0 > p->y)
			bounds->y0 = p->y;
		if (p->y > bounds->y1)
			bounds->y1 = p->y;
	}
}
