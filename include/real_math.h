/* REAL_MATH.H: points, vectors and the math on them */

#ifndef REAL_MATH_H
#define REAL_MATH_H

union real_point3d
{
	real n[3];
	struct { real x, y, z; };
};

union real_vector3d
{
	real n[3];
	struct { real i, j, k; };

	/* 0x24f7b0 (thiscall on a unit direction): true when both quantize to the
	   same 17 bit index, or decode to points within 0.05 of each other */
	bool quantized_equal(real_vector3d const *other) const;
};

struct real_rgb_color { real red, green, blue; };
struct real_argb_color { real alpha, red, green, blue; };
struct real_hsv_color { real hue, saturation, value; };

struct real_point2d { real x, y; };
struct real_bounds { real lo, hi; };
struct real_rectangle2d { real x0, x1, y0, y1; };

union real_quaternion
{
	real n[4];
	struct { real i, j, k, w; };
};

struct matrix3x3
{
	real_vector3d forward;
	real_vector3d left;
	real_vector3d up;
};

/* scale, three axis vectors and a position: 13 reals */
struct real_matrix4x3
{
	real scale;
	union
	{
		matrix3x3 rotation;
		struct
		{
			real_vector3d forward;
			real_vector3d left;
			real_vector3d up;
		};
	};
	real_point3d position;
};

struct real_plane3d
{
	union
	{
		real_vector3d n;
		struct { real i, j, k; };
	};
	real d;
};

/* a float to integer truncation (cvttss2si), as retail's inlined casts */
__forceinline long real_truncate(real x)
{
	__asm
	{
		movss xmm0, x
		cvttss2si eax, xmm0
	}
}

real distance3d(real_point3d const *a, real_point3d const *b);
real _real_random_range(dword *seed, char const *file, long line, real lower_bound, real upper_bound);

#endif
