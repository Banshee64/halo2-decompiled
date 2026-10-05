/* UNKNOWN_2C0D00.CPP: a list of up to 64 circular obstacles on the ground
   plane that a path is steered around: adding them, finding the ones a point
   or a ray hits, and grouping the ones that overlap */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "unknown_2c0d00.h"
#include <math.h>
#include <string.h>

// @flags /O2 /arch:SSE /Gr


/* the nearest obstacle a ray hits */
struct s_obstacle_hit
{
	real distance;
	short index;
	short group;
};

static inline void vector2d_from_points2d(point2f const *p0, point2f const *p1, point2f *out)
{
	out->x = p1->x - p0->x;
	out->y = p1->y - p0->y;
}

static inline real dot_product2d(point2f const *a, point2f const *b)
{
	return a->x * b->x + a->y * b->y;
}

static inline real magnitude_squared2d(point2f const *v)
{
	return v->x * v->x + v->y * v->y;
}

static inline bool bit_vector_test(dword const *bits, long index)
{
	return (bits[index >> 5] & (1 << (index & 31))) != 0;
}

static inline void bit_vector_set(dword *bits, long index)
{
	bits[index >> 5] |= 1 << (index & 31);
}

/* the two directions, turned either way from the direction, that just touch
   a circle of the radius at the distance, and how far along them it is
   touched */
// @retail 0x2c0c40
void tangent_directions(point2f const *direction, point2f *left, point2f *right, real distance, real radius, real *tangent_length)
{
	real sine = 1.0f;
	real cosine;
	real negative_sine;

	if (distance > 0.0f)
	{
		sine = radius / distance;
		if (sine > 1.0f)
		{
			sine = 1.0f;
		}
	}
	cosine = (real)sqrt(1.0f - sine * sine);
	negative_sine = 0.0f - sine;
	left->y = direction->x * negative_sine + cosine * direction->y;
	left->x = direction->x * cosine - negative_sine * direction->y;
	right->y = direction->x * sine + cosine * direction->y;
	right->x = cosine * direction->x - sine * direction->y;
	*tangent_length = cosine * distance;
}

static inline real normalize2d(point2f *v)
{
	real magnitude = (real)sqrt(magnitude_squared2d(v));

	if (fabs(magnitude) < 0.0001f)
	{
		magnitude = 0.0f;
	}
	else
	{
		real inverse = 1.0f / magnitude;

		v->x *= inverse;
		v->y *= inverse;
	}
	return magnitude;
}

/* the directions from the point that pass the obstacle, grown by the radius,
   on either side */
// @retail 0x2c1d90
void obstacle_tangent_directions(point2f const *point, s_obstacle_list const *list, short index, real radius,
	point2f *left, point2f *right, real *tangent_length)
{
	s_obstacle const *obstacle = &list->obstacles[index];
	point2f direction;
	real distance;

	vector2d_from_points2d(point, &obstacle->center, &direction);
	distance = normalize2d(&direction);
	if (distance > 0.0f)
	{
		tangent_directions(&direction, left, right, distance, obstacle->radius + radius + 0.00390625f, tangent_length);
	}
	else
	{
		left->x = 0.0f;
		left->y = -1.0f;
		right->x = 0.0f;
		right->y = 1.0f;
		*tangent_length = 0.0f;
	}
}

// @retail 0x2c0d00
bool obstacle_list_add(s_obstacle_list *list, word flags, point2f const *center, long object_index, real radius)
{
	s_obstacle *obstacle;

	if (list->count == 64)
	{
		return false;
	}
	obstacle = &list->obstacles[list->count++];
	if (flags & 1)
	{
		list->flag0_count++;
	}
	if (flags & 8)
	{
		list->flag3_count++;
	}
	obstacle->flags = flags;
	obstacle->group = NONE;
	obstacle->object_index = object_index;
	obstacle->center = *center;
	obstacle->radius = radius;
	return true;
}

/* the first obstacle other than the one ignored whose circle, grown by the
   radius, holds the point */
// @retail 0x2c1c00
short obstacle_list_find_containing(s_obstacle_list const *list, short ignore_index, point2f const *point, real radius)
{
	short count = list->count;

	for (short i = 0; i < count; i++)
	{
		if (i != ignore_index)
		{
			s_obstacle const *obstacle = &list->obstacles[i];
			point2f offset;
			real distance;

			real distance_squared;

			vector2d_from_points2d(point, &obstacle->center, &offset);
			distance_squared = magnitude_squared2d(&offset);
			distance = obstacle->radius + radius;
			if (distance * distance >= distance_squared)
			{
				return i;
			}
		}
	}
	return NONE;
}

/* the nearest obstacle, grown by the radius, that the ray from the origin
   hits within the maximum distance */
// @retail 0x2c1c70
bool obstacle_list_cast_ray(s_obstacle_list const *list, short ignore_index, point2f const *origin, point2f const *direction,
	real radius, real maximum_distance, bool ignore_flagged, s_obstacle_hit *hit)
{
	hit->distance = maximum_distance;
	hit->index = NONE;
	hit->group = NONE;
	for (short i = 0; i < list->count; i++)
	{
		if (i != ignore_index)
		{
			s_obstacle const *obstacle = &list->obstacles[i];

			if (!ignore_flagged || !(obstacle->flags & 1))
			{
				point2f offset;
				real along;
				real distance;

				vector2d_from_points2d(origin, &obstacle->center, &offset);
				distance = obstacle->radius + radius;
				along = dot_product2d(direction, &offset);
				if (along > 0.0f)
				{
					real outside = magnitude_squared2d(&offset) - distance * distance;
					real hit_distance;

					if (outside <= 0.0f)
					{
						hit_distance = 0.0f;
					}
					else
					{
						real discriminant = along * along - outside;

						if (discriminant >= 0.0f)
						{
							hit_distance = (real)(along - sqrt(discriminant));
						}
						else
						{
							continue;
						}
					}
					if (hit->distance > hit_distance)
					{
						hit->distance = hit_distance;
						hit->index = i;
						hit->group = obstacle->group;
					}
				}
			}
		}
	}
	return hit->index != NONE;
}

/* marks the obstacles that overlap the first one, directly or through
   others, each grown by the radius */
// @retail 0x2c1e70
void obstacle_list_flood(short first_index, dword *bits, real radius, s_obstacle_list const *list)
{
	memset(bits, 0, ((list->count + 31) >> 5) * sizeof(dword));
	if (first_index != NONE)
	{
		short stack[64];
		short stack_count;

		bit_vector_set(bits, first_index);
		stack[0] = first_index;
		stack_count = 1;
		do
		{
			s_obstacle const *current = &list->obstacles[stack[--stack_count]];

			for (short i = 0; i < list->count; i++)
			{
				if (!bit_vector_test(bits, i))
				{
					s_obstacle const *other = &list->obstacles[i];
					point2f offset;
					real distance;

					distance = (other->radius + radius) + (current->radius + radius);
					vector2d_from_points2d(&current->center, &other->center, &offset);
					if (distance * distance >= magnitude_squared2d(&offset))
					{
						bit_vector_set(bits, i);
						stack[stack_count++] = i;
					}
				}
			}
		}
		while (stack_count > 0);
	}
}

/* numbers the groups of overlapping obstacles */
// @retail 0x2c1fb0
void obstacle_list_group(s_obstacle_list *list, real radius)
{
	short i;

	list->group_count = 0;
	for (i = 0; i < list->count; i++)
	{
		list->obstacles[i].group = NONE;
	}
	for (i = 0; i < list->count; i++)
	{
		if (list->obstacles[i].group == NONE)
		{
			dword bits[2];
			short group = list->group_count++;

			obstacle_list_flood(i, bits, radius, list);
			for (short j = 0; j < list->count; j++)
			{
				if (bit_vector_test(bits, j))
				{
					list->obstacles[j].group = group;
				}
			}
		}
	}
}
