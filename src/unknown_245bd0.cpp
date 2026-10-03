// @flags /O2 /Gr /arch:SSE
/* UNKNOWN_245BD0.CPP: collision tests of points and rays against capsules, spheres
   and flat prisms, plus a few small object accessors */

#include "cseries.h"
#include "real_math.h"
#include "data_array.h"
#include "globals.h"
#include <math.h>

#define k_real_epsilon 0.0001f

real function_30bf0(real_vector3d *v);
void datum_delete(s_data_array *data, long datum_index);
matrix3x3 *function_142da0(real yaw, real pitch, real roll, matrix3x3 *out);
matrix3x3 *function_142eb0(matrix3x3 const *a, matrix3x3 const *b, matrix3x3 *out);
real_vector3d *function_143070(real_vector3d const *v, matrix3x3 const *m, real_vector3d *out);

extern s_data_array *g_51ec84;

short const g_440b94[6][3] =
{
	{ 2, 1, 0 },
	{ 1, 2, 0 },
	{ 0, 2, 1 },
	{ 2, 0, 1 },
	{ 1, 0, 2 },
	{ 0, 1, 2 },
};

/* a segment (origin and vector) with a radius */
struct s_capsule
{
	byte unknown00[0x10];
	real_point3d origin;
	real_vector3d vector;
	real radius;
};

struct s_sphere
{
	byte unknown00[0x10];
	real_point3d center;
	real radius;
};

/* a slab: a plane, a thickness, and a convex polygon on the plane projected
   onto two axes */
struct s_prism
{
	byte unknown00[0x10];
	real_plane3d plane;
	real thickness;
	short axis;
	byte side;
	byte unknown27;
	long point_count;
	real_point2d points[1];
};

/* projects a point on a prism's plane onto the two axes the prism was flattened along */
PRIVATE void prism_project(real const *q, short axis, byte side, real_point2d *out)
{
	out->x = q[g_440b94[side + axis * 2][0]];
	out->y = q[g_440b94[side + axis * 2][1]];
}

PRIVATE real dot_product3d(real_vector3d const *a, real_vector3d const *b)
{
	return a->i * b->i + a->j * b->j + a->k * b->k;
}

PRIVATE real magnitude_squared3d(real_vector3d const *v)
{
	return v->i * v->i + v->j * v->j + v->k * v->k;
}

PRIVATE void vector3d_from_points3d(real_point3d const *p0, real_point3d const *p1, real_vector3d *out)
{
	out->i = p1->x - p0->x;
	out->j = p1->y - p0->y;
	out->k = p1->z - p0->z;
}

PRIVATE real plane_distance_to_point(real_plane3d const *plane, real_point3d const *point)
{
	return plane->i * point->x + plane->j * point->y + plane->k * point->z - plane->d;
}

// @retail 0x245bd0
bool function_245bd0(
	real_point3d const *point,
	s_capsule const *capsule,
	real_plane3d *plane,
	real *distance)
{
	real_vector3d d;
	vector3d_from_points3d(&capsule->origin, point, &d);
	real dot = dot_product3d(&capsule->vector, &d);
	if (dot >= g_45dbd8)
	{
		real length2 = magnitude_squared3d(&capsule->vector);
		if (length2 >= dot)
		{
			real d2 = magnitude_squared3d(&d);
			if (d2 * length2 - dot * dot < capsule->radius * capsule->radius * length2)
			{
				if (length2 > g_45dbd8)
				{
					real t = dot / length2;
					real_vector3d n;
					n.i = d.i - capsule->vector.i * t;
					n.j = d.j - capsule->vector.j * t;
					n.k = d.k - capsule->vector.k * t;
					plane->n = n;
				}
				else
				{
					plane->n = d;
				}
				real magnitude = function_30bf0(&plane->n);
				if (magnitude == 0.f)
				{
					plane->i = 0.f;
					plane->j = 0.f;
					plane->k = 1.f;
				}
				plane->d = capsule->origin.z * plane->k + capsule->origin.y * plane->j + capsule->origin.x * plane->i + capsule->radius;
				*distance = capsule->radius - magnitude;
				return true;
			}
		}
	}
	return false;
}

// @retail 0x245d80
bool function_245d80(
	s_prism const *prism,
	real_point3d const *point,
	real *distance,
	real_plane3d *out)
{
	real height = plane_distance_to_point(&prism->plane, point);
	if (height >= 0.f && prism->thickness > height)
	{
		real offset = 0.f - height;
		real q[3];
		q[0] = offset * prism->plane.i + point->x;
		q[1] = prism->plane.j * offset + point->y;
		q[2] = prism->plane.k * offset + point->z;
		real_point2d p2;
		prism_project(q, prism->axis, prism->side, &p2);
		long count = prism->point_count;
		long i = 0;
		if (count > 0)
		{
			do
			{
				real_point2d const *a = &prism->points[i];
				real ax = a->x - p2.x;
				real ay = a->y - p2.y;
				long next = (i + 1 >= count) ? 0 : i + 1;
				real_point2d const *b = &prism->points[next];
				real bx = b->x - p2.x;
				real by = b->y - p2.y;
				if (0.f > by * ax - ay * bx)
				{
					return false;
				}
				i++;
			} while (i < count);
		}
		out->n = prism->plane.n;
		out->d = prism->plane.d + prism->thickness;
		*distance = prism->thickness - height;
		return true;
	}
	return false;
}

// @retail 0x2461a0
bool function_2461a0(
	real_point3d const *point,
	s_sphere const *sphere,
	real_vector3d const *direction,
	real_plane3d *plane,
	real *t)
{
	real_vector3d d;
	vector3d_from_points3d(point, &sphere->center, &d);
	bool result = false;
	bool hit = true;
	real c = magnitude_squared3d(&d) - sphere->radius * sphere->radius;
	if (c > 0.f)
	{
		hit = false;
		real b = dot_product3d(direction, &d);
		if (b > 0.f)
		{
			real a = magnitude_squared3d(direction);
			real discriminant = b * b - a * c;
			if (discriminant >= g_45dbd8)
			{
				real root = b - (real)sqrt(discriminant);
				if (!(a < root))
				{
					*t = root / a;
					hit = true;
				}
			}
		}
	}
	else
	{
		*t = 0.f;
	}
	if (hit)
	{
		result = true;
		real time = *t;
		plane->i = direction->i * time - d.i;
		plane->j = direction->j * time - d.j;
		plane->k = direction->k * time - d.k;
		if (function_30bf0(&plane->n) == 0.f)
		{
			plane->i = 0.f;
			plane->j = 0.f;
			plane->k = 1.f;
		}
		plane->d = sphere->center.z * plane->k + sphere->center.y * plane->j + sphere->center.x * plane->i + sphere->radius;
	}
	return result;
}

// @retail 0x246360
bool function_246360(
	real_vector3d const *direction,
	s_capsule const *capsule,
	real_point3d const *start,
	real *t,
	real_plane3d *plane)
{
	real_vector3d const *u = direction;
	real_vector3d const *v = &capsule->vector;
	real a = magnitude_squared3d(v);
	real uv = dot_product3d(v, u);
	real uu = magnitude_squared3d(u);
	real den = uu * a - uv * uv;
	if (den == g_45dbd8)
	{
		return false;
	}
	real_vector3d w;
	vector3d_from_points3d(&capsule->origin, start, &w);
	real uw = dot_product3d(u, &w);
	real vw = dot_product3d(v, &w);
	real b = vw * uv - uw * a;
	real c = (magnitude_squared3d(&w) - capsule->radius * capsule->radius) * a - vw * vw;
	real disc = b * b - c * den;
	if (0.f > disc)
	{
		return false;
	}
	real root = (real)sqrt(disc);
	real inv = 1.f / den;
	real t_enter = (b - root) * inv;
	real t_exit = (root + b) * inv;
	if (t_enter > 1.f || 0.f > t_exit)
	{
		return false;
	}
	if (0.f > t_enter)
	{
		t_enter = 0.f;
	}
	if (t_exit > 1.f)
	{
		t_exit = 1.f;
	}
	if (uv == 0.f)
	{
		if (0.f > vw || vw > a)
		{
			return false;
		}
	}
	else
	{
		real inv_uv = 1.f / uv;
		real s0 = 0.f - inv_uv * vw;
		real s1 = (a - vw) * inv_uv;
		if (uv > 0.f)
		{
			if (s0 > t_enter)
			{
				t_enter = s0;
			}
			if (t_exit > s1)
			{
				t_exit = s1;
			}
		}
		else
		{
			if (s1 > t_enter)
			{
				t_enter = s1;
			}
			if (t_exit > s0)
			{
				t_exit = s0;
			}
		}
		if (t_enter > t_exit)
		{
			return false;
		}
	}
	*t = t_enter;
	real_vector3d q;
	q.i = u->i * t_enter + w.i;
	q.j = u->j * t_enter + w.j;
	q.k = u->k * t_enter + w.k;
	real along = (v->k * q.k + v->j * q.j + q.i * v->i) / a;
	real back = 0.f - along;
	plane->i = v->i * back + q.i;
	plane->j = v->j * back + q.j;
	plane->k = v->k * back + q.k;
	if (function_30bf0(&plane->n) == 0.f)
	{
		plane->i = 1.f;
		plane->j = 0.f;
		plane->k = 0.f;
	}
	plane->d = capsule->origin.z * plane->k + capsule->origin.y * plane->j + capsule->origin.x * plane->i + capsule->radius;
	return true;
}

// @retail 0x2466b0
bool function_2466b0(
	s_prism const *prism,
	real_point3d const *start,
	real_vector3d const *direction,
	real *t,
	real_plane3d *out)
{
	real a = dot_product3d(&prism->plane.n, direction);
	real b = plane_distance_to_point(&prism->plane, start);
	real t0 = 0.f;
	real t1 = 1.f;
	if (!(fabs(a) < k_real_epsilon))
	{
		real inv = 1.f / a;
		real s0 = 0.f - inv * b;
		real s1 = 0.f - (b - prism->thickness) * inv;
		if (a > 0.f)
		{
			if (s0 > 0.f)
			{
				t0 = s0;
			}
			if (1.f > s1)
			{
				t1 = s1;
			}
		}
		else
		{
			if (s1 > 0.f)
			{
				t0 = s1;
			}
			if (1.f > s0)
			{
				t1 = s0;
			}
		}
		if (t0 > t1)
		{
			return false;
		}
	}
	else if (0.f > b || b >= prism->thickness)
	{
		return false;
	}
	real nb = 0.f - b;
	real q0[3];
	q0[0] = prism->plane.i * nb + start->x;
	q0[1] = prism->plane.j * nb + start->y;
	q0[2] = prism->plane.k * nb + start->z;
	real na = 0.f - a;
	real q1[3];
	q1[0] = na * prism->plane.i + direction->i;
	q1[1] = prism->plane.j * na + direction->j;
	q1[2] = prism->plane.k * na + direction->k;
	real_point2d s2;
	prism_project(q0, prism->axis, prism->side, &s2);
	real_point2d d2;
	prism_project(q1, prism->axis, prism->side, &d2);
	long count = prism->point_count;
	long i = 0;
	if (count > 0)
	{
		do
		{
			long next = i + 1;
			if (next >= count)
			{
				next = 0;
			}
			real_point2d const *cur = &prism->points[i];
			real_point2d const *nxt = &prism->points[next];
			real ex = nxt->x - cur->x;
			real ey = nxt->y - cur->y;
			real denom = ey * d2.x - d2.y * ex;
			real num = (s2.y - cur->y) * ex - ey * (s2.x - cur->x);
			if (!(fabs(denom) < k_real_epsilon))
			{
				real r = num / denom;
				if (denom < 0.f)
				{
					if (r > t0)
					{
						t0 = r;
					}
				}
				else
				{
					if (t1 > r)
					{
						t1 = r;
					}
				}
				if (t0 > t1)
				{
					return false;
				}
			}
			else if (0.f > num)
			{
				return false;
			}
			i++;
		} while (i < count);
	}
	*t = t0;
	out->n = prism->plane.n;
	out->d = prism->plane.d + prism->thickness;
	return true;
}

/* the object at 0x246eb0: clamp(|vector| - 0.5, 0, 1) of the vector at +0x28 */
struct s_object_246eb0
{
	byte unknown00[0x28];
	real_vector3d vector;
};

// @retail 0x246eb0
real function_246eb0(s_object_246eb0 const *object)
{
	real r = (real)sqrt(object->vector.i * object->vector.i + object->vector.j * object->vector.j + object->vector.k * object->vector.k) - 0.5f;
	if (r < 0.f)
	{
		return 0.f;
	}
	if (r > 1.f)
	{
		r = 1.f;
	}
	return r;
}

struct s_object_2470e0
{
	byte unknown00[0x9c];
	real_vector3d vector9c;
	real unknowna8;
	real unknownac;
};

// @retail 0x2470e0
real_vector3d *function_2470e0(s_object_2470e0 *object)
{
	return &object->vector9c;
}

// @retail 0x2470f0
real *function_2470f0(s_object_2470e0 *object)
{
	return &object->unknowna8;
}

struct s_object_247120
{
	byte unknown00[0xc];
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word flag8 : 1;
	word flag9 : 1;
	word flag10 : 1;
	word flag11 : 1;
	word flag12 : 1;
	word flag13 : 1;
	word flag14 : 1;
	word flag15 : 1;
	byte unknown0e[8];
	word bit0 : 1;
	word bit1 : 1;
	word bit2 : 1;
	word bit3 : 1;
	word bit4 : 1;
	word bit5 : 1;
	word bit6 : 1;
	word bit7 : 1;
	word bit8 : 1;
	word bit9 : 1;
	word bit10 : 1;
	word bit11 : 1;
	word bit12 : 1;
	word bit13 : 1;
	word bit14 : 1;
	word bit15 : 1;
};

// @retail 0x247120
long function_247120(s_object_247120 const *object)
{
	return object->bit7;
}

// @retail 0x247130
long function_247130(s_object_247120 const *object)
{
	return TEST_FIELD_BIT(object->flag9);
}

struct s_frame_2477b0
{
	byte unknown00[0x10];
	matrix3x3 rotation;
	real_point3d position;
};

// @retail 0x2477b0
void function_2477b0(
	s_object_2470e0 *object,
	s_frame_2477b0 *frame,
	real_matrix4x3 const *source)
{
	if (source)
	{
		frame->rotation.forward = source->forward;
		frame->rotation.left = source->left;
		frame->rotation.up = source->up;
		frame->position = source->position;
		real_vector3d offset;
		function_143070(&object->vector9c, &frame->rotation, &offset);
		if (!(fabs(object->unknownac) < k_real_epsilon) || !(fabs(object->unknowna8) < k_real_epsilon))
		{
			matrix3x3 rotation;
			function_142da0(object->unknowna8, object->unknownac, 0.f, &rotation);
			function_142eb0(&rotation, &frame->rotation, &frame->rotation);
		}
		frame->position.x += offset.i;
		frame->position.y += offset.j;
		frame->position.z += offset.k;
	}
}

struct s_effect_owner
{
	word unknown00;
	word unknown02;
	long first_effect;
};

struct s_effect_datum
{
	byte unknown00[4];
	long next;
	byte unknown08[0x38];
};

// @retail 0x2483b0
void function_2483b0(s_effect_owner *owner)
{
	long index = owner->first_effect;
	if (index != NONE)
	{
		long next;
		do
		{
			next = ((s_effect_datum *)g_51ec84->data)[index & 0xffff].next;
			datum_delete(g_51ec84, index);
			index = next;
		} while (next != NONE);
	}
	owner->unknown02 = 0;
	owner->first_effect = NONE;
}
