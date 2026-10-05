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
