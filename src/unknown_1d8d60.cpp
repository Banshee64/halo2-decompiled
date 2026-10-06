// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include <xmmintrin.h>

struct s_1d8d60
{
	__m128 field_0;
	void function_1d8d60();
};

// @retail 0x1d8d60
void s_1d8d60::function_1d8d60()
{
	__m128 local_0 = field_0;
	__m128 local_1 = _mm_mul_ps(local_0, local_0);
	__m128 local_2 = _mm_add_ps(_mm_shuffle_ps(local_1, local_1, 0x4e), local_1);
	local_2 = _mm_add_ps(local_2, _mm_shuffle_ps(local_2, local_2, 0xb1));
	__m128 local_3 = _mm_rsqrt_ss(local_2);
	__m128 local_4 = _mm_mul_ss(_mm_mul_ss(local_2, local_3), local_3);
	__m128 local_5 = _mm_sub_ss(_mm_set_ss(3.0f), local_4);
	__m128 local_6 = _mm_mul_ss(_mm_mul_ss(_mm_set_ss(0.5f), local_3), local_5);
	field_0 = _mm_mul_ps(_mm_shuffle_ps(local_6, local_6, 0), local_0);
}
