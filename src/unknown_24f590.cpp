#include "unknown_11c920.h"
#include "unknown_1946f0.h"

// @flags /O2 /Ob1 /arch:SSE /Gr

long icosahedron_nearest_face(vector3f const *direction, real *cosine);

/* The planes used to project a direction onto its nearest face. */
PRIVATE plane3f const g_475340[20] =
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

PRIVATE __forceinline void direction_face_inverse(s_direction_face const *face, vector3f const *point, vector3f *out)
{
	if (face->scale != 0.f)
	{
		real x = point->i - face->origin.i;
		real y = point->j - face->origin.j;
		real z = point->k - face->origin.k;
		if (face->scale != 1.f)
		{
			real inverse = 1.f / face->scale;
			x = inverse * x;
			y = inverse * y;
			z = inverse * z;
		}
		out->i = face->axes[0].k * z + face->axes[0].j * y + face->axes[0].i * x;
		out->j = face->axes[1].k * z + face->axes[1].j * y + face->axes[1].i * x;
		out->k = face->axes[2].k * z + face->axes[2].j * y + face->axes[2].i * x;
	}
	else
	{
		out->i = 0.f;
		out->j = 0.f;
		out->k = 0.f;
	}
}

PRIVATE __forceinline void direction_scale(vector3f const *vector, real scale, vector3f *out)
{
	out->i = vector->i * scale;
	out->j = vector->j * scale;
	out->k = vector->k * scale;
}

/* Projects a unit direction and packs its face and two six-bit coordinates. */
// @retail 0x24f590
long __fastcall function_24f590(vector3f const *direction)
{
	real cosine;
	long index = icosahedron_nearest_face(direction, &cosine);
	vector3f point;
	real scale = g_475340[index].d / cosine;
	direction_scale(direction, scale, &point);
	direction_face_inverse(&g_475480[index], &point, &point);
	direction_scale(&point, 0.5f, &point);
	return (((index << 6) | ((long)(point.j * 63.f) & 0x3f)) << 6) |
		((long)(point.i * 63.f) & 0x3f);
}
