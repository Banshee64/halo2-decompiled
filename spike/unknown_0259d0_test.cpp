/*
REAL_MATH_TEST.CPP: callers for unknown_0259d0.cpp's functions, and the test
image's entry point.
*/

extern "C" int _fltused = 0;

typedef float real;

union point3f
{
	real n[3];
	struct { real x, y, z; };
};

real distance3d(point3f const *a, point3f const *b);
real function_259d0(unsigned long *seed, char const *file, long line, real lower_bound, real upper_bound);

point3f g_points[8];
unsigned long g_seed;
volatile real g_out, g_lower, g_upper;

void caller_a(void)
{
	g_out = distance3d(&g_points[0], &g_points[1]);
	g_out = function_259d0(&g_seed, __FILE__, __LINE__, g_lower, g_upper);
}

void caller_b(point3f const *point)
{
	if (distance3d(point, &g_points[3]) > g_lower)
	{
		g_out = function_259d0(&g_seed, __FILE__, __LINE__, 0.f, g_upper);
	}
}

extern "C" int entry(void)
{
	caller_a();
	caller_b(&g_points[5]);
	return 0;
}
