// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_181800.CPP: whether a sphere lies outside, across or inside a
   frustum given by its bounds and six planes */

#include "cseries.h"
#include "real_math.h"

struct s_frustum_bounds
{
	real x0;
	real x1;
	real y0;
	real y1;
	real z0;
	real z1;
};

struct s_sphere_frustum
{
	byte unknown00[0x60];
	s_frustum_bounds bounds;
	real_plane3d planes[6];
};

// @retail 0x181800
long function_181800(s_sphere_frustum const *frustum, real_point3d const *center, real radius)
{
	if (center->x - radius > frustum->bounds.x1 ||
		center->y - radius > frustum->bounds.y1 ||
		center->z - radius > frustum->bounds.z1 ||
		frustum->bounds.x0 > center->x + radius ||
		frustum->bounds.y0 > radius + center->y ||
		frustum->bounds.z0 > radius + center->z)
	{
		return 0;
	}

	real distance0 = frustum->planes[0].k * center->z + frustum->planes[0].j * center->y + frustum->planes[0].i * center->x - frustum->planes[0].d;
	if (distance0 > radius)
	{
		return 0;
	}
	real distance1 = frustum->planes[1].k * center->z + frustum->planes[1].j * center->y + frustum->planes[1].i * center->x - frustum->planes[1].d;
	if (distance1 > radius)
	{
		return 0;
	}
	real distance2 = frustum->planes[2].k * center->z + frustum->planes[2].j * center->y + frustum->planes[2].i * center->x - frustum->planes[2].d;
	if (distance2 > radius)
	{
		return 0;
	}
	real distance3 = frustum->planes[3].k * center->z + frustum->planes[3].j * center->y + frustum->planes[3].i * center->x - frustum->planes[3].d;
	if (distance3 > radius)
	{
		return 0;
	}
	real distance4 = frustum->planes[4].k * center->z + frustum->planes[4].j * center->y + frustum->planes[4].i * center->x - frustum->planes[4].d;
	if (distance4 > radius)
	{
		return 0;
	}
	real distance5 = frustum->planes[5].k * center->z + frustum->planes[5].j * center->y + frustum->planes[5].i * center->x - frustum->planes[5].d;
	if (!(distance5 > radius))
	{
		return distance0 < -radius && distance1 < -radius && distance2 < -radius && distance3 < -radius && distance5 < -radius ? 2 : 1;
	}
	return 0;
}
