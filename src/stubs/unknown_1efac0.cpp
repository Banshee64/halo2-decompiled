// stub for the c_a virtual in the library range
#include "unknown_1efac0.h"
#include "unknown_1eb350.h"
#include <xmmintrin.h>

/* a distinct body, so that the linker does not fold this stub into another empty virtual */
static volatile long g_stub_2d7240;

// @stub 0x2d7240
void c_a::v2(bool enable)
{
	g_stub_2d7240 = enable;
}

// @stub 0x2df680
c_shape_library_base_a::~c_shape_library_base_a() {}

// @stub 0x2fc420
c_shape_library_base_b::~c_shape_library_base_b() {}

struct c_transformed_point
{
	__m128 value;
	void transform(const void *matrix, const __m128 *point);
};

// @stub 0x2da610
void c_transformed_point::transform(const void *matrix, const __m128 *point) {}

// @stub 0x2fe730
void __cdecl function_2fe730(const void *points, long count, long stride, void *output) {}
struct s_surface_key_array
{
 dword *keys;
 long count;
};

struct c_query_transform
{
 __m128 axes[3];
 __m128 position;
 void inverse_product(const c_query_transform *a, const c_query_transform *b);
};

struct c_query_point
{
 __m128 value;
 void inverse_transform(const c_query_transform *matrix, const __m128 *point);
};

struct s_query_bounds
{
 __m128 minimum, maximum;
};

struct c_surface_query_library
{
 void query_sphere(const __m128 *sphere, s_surface_key_array *keys);
 void query_box(const c_query_transform *matrix, const __m128 *extent, real tolerance, s_surface_key_array *keys);
 void query_bounds(const s_query_bounds *bounds, s_surface_key_array *keys);
};


// @stub 0x2da4d0
void c_query_transform::inverse_product(const c_query_transform *a, const c_query_transform *b) {}
// @stub 0x2da660
void c_query_point::inverse_transform(const c_query_transform *matrix, const __m128 *point) {}
// @stub 0x2df5e0
void c_surface_query_library::query_sphere(const __m128 *sphere, s_surface_key_array *keys) {}
// @stub 0x2df610
void c_surface_query_library::query_box(const c_query_transform *matrix, const __m128 *extent, real tolerance, s_surface_key_array *keys) {}
// @stub 0x2df640
void c_surface_query_library::query_bounds(const s_query_bounds *bounds, s_surface_key_array *keys) {}

class c_havok_reference_counted;
struct c_child_transform
{
 c_child_transform(c_havok_reference_counted *child);
};

// @stub 0x2de890
c_child_transform::c_child_transform(c_havok_reference_counted *child) {}
