// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_239B80.CPP: two-dimensional polygon helpers (convex hulls,
   clipping against lines and point-in-polygon tests over point2f
   arrays) */

#include "cseries.h"
#include "real_math.h"
#include <float.h>
#include <string.h>

#define k_real_epsilon 0.0001f
#define MAXIMUM_POLYGON_POINTS 48

struct vector2f { real i, j; };

/* a line in the plane: points p with dot(n, p) == d */
struct plane2f
{
	vector2f n;
	real d;
};

static inline real dot_product2d(vector2f const *a, point2f const *b)
{
	return a->i * b->x + a->j * b->y;
}

static inline real normalize2d(vector2f *v)
{
	real m = (real)sqrt(v->i * v->i + v->j * v->j);
	if (fabs(m) < k_real_epsilon)
		return 0.f;
	real inv = 1.f / m;
	v->i = inv * v->i;
	v->j = v->j * inv;
	return m;
}

static inline bool points2d_equal(point2f const *a, point2f const *b)
{
	return fabs(a->x - b->x) < k_real_epsilon && fabs(a->y - b->y) < k_real_epsilon;
}

/* two pi, set up at run time */
real g_55e470;

long function_239d50(long count, point2f const *points, point2f *hull);
short function_239e50(short count, point2f const *points, short *indices);

/* 0 when every point is the same, 1 when they are on one line, 2 otherwise */
// @retail 0x239b80
short function_239b80(short count, point2f const *points)
{
	point2f first;
	plane2f line;
	short state = NONE;

	for (short i = 0; state < 2 && i < count; i++)
	{
		switch (state)
		{
		case NONE:
			first = points[i];
			state = 0;
			break;
		case 0:
			line.n.i = first.y - points[i].y;
			line.n.j = points[i].x - first.x;
			if (normalize2d(&line.n) != 0.f)
			{
				line.d = dot_product2d(&line.n, &points[i]);
				state = 1;
			}
			else
			{
				line.d = 0.f;
			}
			break;
		case 1:
			if (!(fabs(dot_product2d(&line.n, &points[i]) - line.d) < k_real_epsilon))
				state = 2;
			break;
		}
	}

	return state;
}

// @retail 0x239cb0
long function_239cb0(long count, point3f const *points, point2f *hull)
{
	point2f points2d[MAXIMUM_POLYGON_POINTS];

	if (count > MAXIMUM_POLYGON_POINTS)
		count = MAXIMUM_POLYGON_POINTS;
	for (long i = 0; i < count; i++)
	{
		points2d[i].x = points[i].x;
		points2d[i].y = points[i].y;
	}

	return function_239d50(count, points2d, hull);
}

// @retail 0x239d50
long function_239d50(long count, point2f const *points, point2f *hull)
{
	point2f unique_points[MAXIMUM_POLYGON_POINTS];
	short indices[MAXIMUM_POLYGON_POINTS];

	memcpy(unique_points, points, sizeof(point2f) * (count > MAXIMUM_POLYGON_POINTS ? MAXIMUM_POLYGON_POINTS : count));
	short unique_count = (short)count;
	for (long i = 0; i < unique_count - 1; i++)
	{
		for (long j = i + 1; j < unique_count; j++)
		{
			if (fabs(unique_points[i].x - unique_points[j].x) < k_real_epsilon &&
				fabs(unique_points[i].y - unique_points[j].y) < k_real_epsilon)
			{
				unique_points[j] = unique_points[unique_count - 1];
				unique_count--;
			}
		}
	}

	short hull_count = function_239e50(unique_count, unique_points, indices);
	if (hull_count >= 3)
	{
		for (long i = 0; i < hull_count; i++)
			hull[i] = unique_points[indices[i]];
	}
	else
	{
		hull_count = 0;
	}
	return hull_count;
}

/* gift wrapping: the indices of the convex hull of the points, starting from
   the lowest */
// @retail 0x239e50
short function_239e50(short count, point2f const *points, short *indices)
{
	short hull_count = 0;

	if (function_239b80(count, points) == 2)
	{
		real previous_angle = 0.f;
		bool left_start = false;
		real lowest_x = FLT_MAX;
		real lowest_y = FLT_MAX;
		short current = count;

		for (short i = 0; i < count; i++)
		{
			if (lowest_y - k_real_epsilon > points[i].y ||
				lowest_y > points[i].y && lowest_x + k_real_epsilon > points[i].x ||
				lowest_y + k_real_epsilon > points[i].y && lowest_x - k_real_epsilon > points[i].x)
			{
				lowest_x = points[i].x;
				lowest_y = points[i].y;
				current = i;
			}
		}

		while (hull_count < count)
		{
			real best_distance = -FLT_MAX;
			indices[hull_count++] = current;
			short next = NONE;
			real best_angle = FLT_MAX;
			real best_delta = FLT_MAX;

			for (short j = 0; j < count; j++)
			{
				real dx = points[current].x - points[j].x;
				real dy = points[current].y - points[j].y;
				real distance = (real)sqrt(dx * dx + dy * dy);
				if (distance > 0.000244140625f)
				{
					real angle = (real)atan2(points[j].y - points[current].y, points[j].x - points[current].x);
					real delta = angle - previous_angle;
					while (delta < -k_real_epsilon)
						delta += g_55e470;
					if (delta < best_delta - k_real_epsilon ||
						!(delta > best_delta + k_real_epsilon) &&
						(j == indices[0] || next != indices[0] && distance > best_distance))
					{
						best_distance = distance;
						best_angle = angle;
						best_delta = delta;
						next = j;
					}
				}
			}

			current = next;
			previous_angle = best_angle;
			if (next == NONE)
				return hull_count;
			if (!left_start)
				left_start = !points2d_equal(&points[next], &points[indices[0]]);
			if (next == indices[0])
				return hull_count;
			if (left_start && points2d_equal(&points[next], &points[indices[0]]))
				return hull_count;
		}

		for (short i = 0; i < hull_count; i++)
		{
			short j;
			for (j = 0; j < i; j++)
			{
				if (indices[j] == indices[i])
				{
					hull_count = i - j;
					memcpy(indices, &indices[j], sizeof(short) * hull_count);
					break;
				}
			}
			if (j < i)
				break;
		}
	}

	return hull_count;
}

// @retail 0x23a160
bool function_23a160(point2f const *point, real radius, short count, point2f const *points)
{
	real radius_squared = radius * radius;
	for (short i = 0; i < count; i++)
	{
		real dx = point->x - points[i].x;
		real dy = point->y - points[i].y;
		long next = (i + 1 >= count) ? 0 : i + 1;
		real ex = points[next].x - points[i].x;
		real ey = points[next].y - points[i].y;
		real length_squared = ex * ex + ey * ey;
		if (length_squared != 0.f)
		{
			real cross = dx * ey - dy * ex;
			if (cross > 0.f && cross * cross > length_squared * radius_squared)
				return false;
		}
	}
	return true;
}

// @retail 0x23a220
bool function_23a220(point2f const *point, short count, point2f const *points, real epsilon)
{
	for (short i = 0; i < count; i++)
	{
		real dy = point->y - points[i].y;
		real dx = point->x - points[i].x;
		long next = (i + 1 >= count) ? 0 : i + 1;
		real ex = points[next].x - points[i].x;
		real ey = points[next].y - points[i].y;
		if (dy * ex - dx * ey < -epsilon)
			return false;
	}
	return true;
}
