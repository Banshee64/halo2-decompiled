// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "real_math.h"
#include "globals.h"
#include "unknown_13fd90.h"
#include "unknown_19c1d0.h"
#include <stdarg.h>
#include <string.h>
#include <wchar.h>
#include <math.h>

#include "unknown_163110.h"

// @retail 0x163110
s_text_buffer *text_buffer_format(s_text_buffer *buffer, const word *format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	_vsnwprintf((wchar_t *)buffer->text, 0x4f, (const wchar_t *)format, arguments);
	buffer->text[0x4f] = 0;
	return buffer;
}

// @retail 0x163140
void s_text_widget_a::initialize(const s_short_rectangle *rectangle, const real_argb_color *color_a, const real_argb_color *color_b, const word *text, long text_length, bool flag)
{
	s_short_rectangle bounds = *rectangle;
	bounds.right--;
	bounds.bottom--;
	this->bounds = bounds;
	this->color_a = *color_a;
	this->color_b = *color_b;
	unicode_string_copy(this->text, text, text_length);
	text_bounds = bounds;
	text_bounds.top--;
	text_bounds.left += 2;
	text_bounds.right -= 2;
	this->flag = flag;
	valid = 1;
}

// @retail 0x1631f0
void s_text_widget_b::initialize(const s_short_rectangle *rectangle, const real_argb_color *color_a, const real_argb_color *color_b, const word *text, long text_length, bool flag)
{
	s_short_rectangle bounds = *rectangle;
	bounds.right--;
	bounds.bottom--;
	this->bounds = bounds;
	this->color_a = *color_a;
	this->color_b = *color_b;
	unicode_string_copy(this->text, text, text_length);
	text_bounds = bounds;
	text_bounds.top--;
	text_bounds.left += 2;
	text_bounds.right -= 2;
	this->flag = flag;
	valid = 1;
}

// @retail 0x1632a0
void s_text_widget_c::initialize(const s_short_rectangle *rectangle, const real_argb_color *color_a, const real_argb_color *color_b, const word *text, long text_length, bool flag)
{
	s_short_rectangle bounds = *rectangle;
	bounds.right--;
	bounds.bottom--;
	this->bounds = bounds;
	this->color_a = *color_a;
	this->color_b = *color_b;
	unicode_string_copy(this->text, text, text_length);
	text_bounds = bounds;
	text_bounds.top--;
	text_bounds.left += 2;
	text_bounds.right -= 2;
	this->flag = flag;
	valid = 1;
}

// @retail 0x163350
void s_text_widget_d::initialize(const s_short_rectangle *rectangle, const real_argb_color *color_a, const real_argb_color *color_b, const word *text, long text_length, bool flag)
{
	s_short_rectangle bounds = *rectangle;
	bounds.right--;
	bounds.bottom--;
	this->bounds = bounds;
	this->color_a = *color_a;
	this->color_b = *color_b;
	unicode_string_copy(this->text, text, text_length);
	text_bounds = bounds;
	text_bounds.top--;
	text_bounds.left += 2;
	text_bounds.right -= 2;
	this->flag = flag;
	valid = 1;
}

// @retail 0x163610
char *function_163610()
{
	char *result = 0;
	if (g_4e6948 && g_4e6948->state == 1)
	{
		long key0 = g_4e6948->position_a;
		long key1 = g_4e6948->position_b;
		if (key0 == NONE)
		{
			s_entry_a *entry = function_19c320(g_4e6948->name);
			key0 = entry ? entry->key0 : key0;
			entry = function_19c320(g_4e6948->name);
			key1 = NONE;
			if (entry)
				key1 = entry->key1;
		}
		if (key0 != NONE)
		{
			long next = function_19c440(key0, key1);
			if (next != NONE)
			{
				s_entry_a *entry = function_19c270(key0, next);
				if (entry)
					result = entry->name;
			}
		}
	}
	return result;
}

class c_entry_list
{
public:
	c_entry_list(long maximum_count);
	bool add(long a, short b, long c, short d);
	short find(long a);
	void swap(short index0, short index1);

private:
	long maximum_count;
	word count;
	short *shorts_b;
	long *longs_a;
	long *longs_c;
	short *shorts_d;
};

// @retail 0x163c40
c_entry_list::c_entry_list(long maximum_count)
{
	longs_a = new long[maximum_count];
	shorts_b = new short[maximum_count];
	this->maximum_count = maximum_count;
	longs_c = new long[maximum_count];
	shorts_d = new short[maximum_count];
}

// @retail 0x163c80
bool c_entry_list::add(long a, short b, long c, short d)
{
	if (count < maximum_count - 1)
	{
		longs_a[count] = a;
		shorts_b[count] = b;
		longs_c[count] = c;
		shorts_d[count] = d;
		count++;
		return true;
	}
	return false;
}

// @retail 0x163d20
short c_entry_list::find(long a)
{
	for (long i = 0; i < count; i++)
	{
		if (longs_a[i] == a)
			return (short)i;
	}
	return NONE;
}

// @retail 0x163d50
void c_entry_list::swap(short index0, short index1)
{
	long a = longs_a[index0];
	longs_a[index0] = longs_a[index1];
	longs_a[index1] = a;
	short b = shorts_b[index0];
	shorts_b[index0] = shorts_b[index1];
	shorts_b[index1] = b;
	long c = longs_c[index0];
	longs_c[index0] = longs_c[index1];
	longs_c[index1] = c;
	short d = shorts_d[index0];
	shorts_d[index0] = shorts_d[index1];
	shorts_d[index1] = d;
}

struct s_view
{
	byte unknown00[4];
	real_matrix4x3 matrix;
	byte unknown38[0x54 - 0x38];
	real_point3d point54;
	byte unknown60[0x84 - 0x60];
	byte flag84;
	byte flag85;
	byte unknown86[0xb0 - 0x86];
	real margin;
	real_vector3d vector_b4;
	byte unknownc0[0xe4 - 0xc0];
	real_vector3d vector_e4;
};

struct s_bounds3d
{
	real x0, x1, y0, y1, z0, z1;
};

s_bounds3d *g_4687e0;

static void transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *out)
{
	real x = point->x;
	real y = point->y;
	real z = point->z;
	if (matrix->scale != 1.0f)
	{
		x = matrix->scale * x;
		y = matrix->scale * y;
		z = matrix->scale * z;
	}
	out->x = matrix->up.i * z + matrix->left.i * y + matrix->forward.i * x + matrix->position.x;
	out->y = matrix->up.j * z + matrix->left.j * y + matrix->forward.j * x + matrix->position.y;
	out->z = matrix->up.k * z + matrix->left.k * y + matrix->forward.k * x + matrix->position.z;
}

// @retail 0x1650a0
bool function_1650a0(const s_view *view, const real_point3d *point, s_bounds3d *bounds, real radius)
{
	bool result = true;
	real_point3d p;
	transform_point(&view->matrix, point, &p);
	if (p.z > radius)
		result = false;
	else
	{
	long i = 0;
	real z2 = p.z * p.z;
	s_bounds3d b;
	real *bn = &b.x0;
	for (; i < 2; i++)
	{
		real d = (real)sqrt(p.n[i] * p.n[i] + z2);
		if (radius >= d)
		{
			bn[i * 2] = -10000.0f;
			bn[i * 2 + 1] = 10000.0f;
		}
		else
		{
			real a = (real)asin(radius / d);
			real angle = (real)atan2(p.n[i], -p.z);
			real lo = angle - a;
			if (lo < -1.5707964f)
				bn[i * 2] = -10000.0f;
			else
				bn[i * 2] = (real)tan(lo);
			real hi = angle + a;
			if (hi > 1.5707964f)
				bn[i * 2 + 1] = 10000.0f;
			else
				bn[i * 2 + 1] = (real)tan(hi);
		}
	}
	real z_lo = p.z - radius;
	b.z0 = z_lo > -1.5258800e-5f ? -1.5258800e-5f : z_lo;
	real z_hi = p.z + radius;
	b.z1 = z_hi > -1.5258800e-5f ? -1.5258800e-5f : z_hi;
	*bounds = b;
	}
	return result;
}
struct s_plane
{
	real_vector3d normal;
	real d;
};

// @retail 0x1652e0
bool function_1652e0(s_view *view, real margin, const real_point3d *points, long point_count, real_point3d *projected_points, short *projected_count, s_bounds3d *bounds, bool use_plane0, bool use_plane1)
{
	real_point3d local_points[32];
	byte outside[32];
	byte segment_culled[16];
	s_plane planes[2];
	bool culled = true;

	use_plane1 &= (view->flag84 || view->flag85);
	*bounds = *g_4687e0;

	real near_distance = view->margin + margin;
	if (near_distance < 1.52588e-5f)
		near_distance = 1.52588e-5f;

	memset(segment_culled, 0, sizeof(segment_culled));

	planes[0].normal = *(real_vector3d *)&view->point54;
	real dot0 = view->vector_e4.j * planes[0].normal.j + view->vector_e4.k * planes[0].normal.k + view->vector_e4.i * planes[0].normal.i;
	real_vector3d negated;
	negated.i = 0.0f - view->point54.x;
	negated.j = 0.0f - view->point54.y;
	negated.k = 0.0f - view->point54.z;
	planes[0].normal.i = 0.0f - planes[0].normal.i;
	planes[0].normal.j = 0.0f - planes[0].normal.j;
	planes[0].normal.k = 0.0f - planes[0].normal.k;
	planes[0].d = 0.0f - dot0 + near_distance;

	planes[1].normal = negated;
	real dot1 = view->vector_b4.j * planes[1].normal.j + view->vector_b4.k * planes[1].normal.k + view->vector_b4.i * planes[1].normal.i;
	planes[1].normal.i = 0.0f - planes[1].normal.i;
	planes[1].normal.j = 0.0f - planes[1].normal.j;
	planes[1].normal.k = 0.0f - planes[1].normal.k;
	planes[1].d = 0.0f - dot1;

	memcpy(local_points, points, point_count * sizeof(real_point3d));

	for (long pass = 0; pass < 2; pass++)
	{
		s_plane *plane;
		if (pass == 0)
		{
			if (!use_plane0)
				continue;
			plane = &planes[0];
		}
		else
		{
			if (pass == 1 && !use_plane1)
				continue;
			plane = &planes[1];
		}

		long i;
		for (i = 0; i < point_count; i++)
		{
			real_point3d *point = &local_points[i];
			outside[i] = plane->normal.i * point->x + plane->normal.k * point->z + plane->normal.j * point->y - plane->d > -0.0001f;
		}

		long segment_count = point_count / 2;
		if (segment_count > 0)
		{
		long segment = 0;
		do
		{
			byte outside0 = outside[segment * 2];
			segment_culled[segment] |= (!outside0 && !outside[segment * 2 + 1]);
			if (!segment_culled[segment] && outside0 != outside[segment * 2 + 1])
			{
				real_point3d *p0 = &local_points[segment * 2];
				real_point3d *p1 = &local_points[segment * 2 + 1];
				real dx = p1->x - p0->x;
				real dz = p1->z - p0->z;
				real dy = p1->y - p0->y;
				real denominator = dx * plane->normal.i + dz * plane->normal.k + plane->normal.j * dy;
				real t;
				if (denominator != 0.0f)
					t = (plane->normal.j * p0->y + p0->z * plane->normal.k + plane->normal.i * p0->x - plane->d) * (-1.0f / denominator);
				else
					t = 0.0f;
				real_point3d intersection;
				intersection.x = t * dx + p0->x;
				intersection.y = dy * t + p0->y;
				intersection.z = dz * t + p0->z;
				if (outside0)
					*p1 = intersection;
				else
					*p0 = intersection;
			}
			segment++;
		} while (segment < segment_count);
		}
	}

	long segment_count = point_count / 2;
	long total = 0;
	real_point3d *cursor = projected_points;
	for (long segment = 0; segment < segment_count; segment++)
	{
		if (!segment_culled[segment])
		{
			culled = false;
			total += 2;
			real_point3d *output = cursor;
			cursor += 2;
			real_point3d *point = &local_points[segment * 2];
			long k = 2;
			do
			{
				real_point3d t;
				transform_point(&view->matrix, point, &t);
				real w = t.z;
				if (w > -1.52588e-5f)
					w = -1.52588e-5f;
				real factor = -1.0f / w;
				real_point3d s;
				s.x = t.x * factor;
				s.y = t.y * factor;
				s.z = t.z;
				if (projected_points)
				{
					*output = s;
					output++;
				}
				if (bounds->x0 > s.x)
					bounds->x0 = s.x;
				if (s.x > bounds->x1)
					bounds->x1 = s.x;
				if (bounds->y0 > s.y)
					bounds->y0 = s.y;
				if (s.y > bounds->y1)
					bounds->y1 = s.y;
				if (bounds->z0 > s.z)
					bounds->z0 = s.z;
				if (s.z > bounds->z1)
					bounds->z1 = s.z;
				point++;
			} while (--k);
		}
	}

	if (projected_count)
		*projected_count = (short)total;
	return !culled;
}