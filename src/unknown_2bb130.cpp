/* UNKNOWN_2BB130.CPP: the face of an icosahedron nearest a direction */

#include "unknown_11c920.h"
#include "unknown_0259d0.h"

// @flags /O2 /arch:SSE /Gr

/* the planes of the icosahedron's twenty faces */
plane3f const g_475340[20] =
{
	{ 0.356822014f, 0.934171975f, 0.0f, 1.51152301f },
	{ -0.356822014f, 0.934171975f, 0.0f, 1.51152301f },
	{ 0.0f, 0.356822014f, 0.934171975f, 1.51152301f },
	{ 0.57735002f, 0.57735002f, 0.57735002f, 1.51152301f },
	{ -0.57735002f, 0.57735002f, 0.57735002f, 1.51152301f },
	{ 0.0f, 0.356822014f, -0.934171975f, 1.51152301f },
	{ 0.57735002f, 0.57735002f, -0.57735002f, 1.51152301f },
	{ -0.57735002f, 0.57735002f, -0.57735002f, 1.51152301f },
	{ 0.356822014f, -0.934171975f, 0.0f, 1.51152301f },
	{ -0.356822014f, -0.934171975f, 0.0f, 1.51152301f },
	{ 0.0f, -0.356822014f, 0.934171975f, 1.51152301f },
	{ 0.57735002f, -0.57735002f, 0.57735002f, 1.51152301f },
	{ -0.57735002f, -0.57735002f, 0.57735002f, 1.51152301f },
	{ 0.0f, -0.356822014f, -0.934171975f, 1.51152301f },
	{ 0.57735002f, -0.57735002f, -0.57735002f, 1.51152301f },
	{ -0.57735002f, -0.57735002f, -0.57735002f, 1.51152301f },
	{ 0.934171975f, 0.0f, 0.356822014f, 1.51152301f },
	{ 0.934171975f, 0.0f, -0.356822014f, 1.51152301f },
	{ -0.934171975f, 0.0f, 0.356822014f, 1.51152301f },
	{ -0.934171975f, 0.0f, -0.356822014f, 1.51152301f },
};

/* the faces that can be nearest a direction in each octant (bit 0: x not
   positive, bit 1: y, bit 2: z) */
long const g_475a70[8][4] =
{
	{ 0, 2, 3, 16 },
	{ 1, 2, 4, 18 },
	{ 8, 10, 11, 16 },
	{ 9, 10, 12, 18 },
	{ 0, 5, 6, 17 },
	{ 1, 5, 7, 19 },
	{ 8, 13, 14, 17 },
	{ 9, 13, 15, 19 },
};

/* the face whose normal is nearest the direction, and the cosine to it */
// @retail 0x2bb130
long icosahedron_nearest_face(vector3f const *direction, real *cosine)
{
	long octant = (direction->i > 0.0 ? 0 : 1) | (direction->j > 0.0 ? 0 : 2) | (direction->k > 0.0 ? 0 : 4);
	real best = -1.0f;
	long result = NONE;

	for (long i = 0; i < 4; i++)
	{
		long face = g_475a70[octant][i];
		plane3f const *plane = &g_475340[face];
		real dot = direction->i * plane->i + plane->j * direction->j + plane->k * direction->k;

		if (dot > best)
		{
			best = dot;
			result = face;
		}
	}
	*cosine = best;
	return result;
}
