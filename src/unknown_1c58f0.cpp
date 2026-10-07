// @flags /O2 /arch:SSE /Gr /GL-
#include "unknown_11c920.h"
#include <xmmintrin.h>

struct s_1c58f0
{
	__m128 field_0;
	s_1c58f0(real arg_0);
};

// @retail 0x1c58f0
s_1c58f0::s_1c58f0(real arg_0)
{
	field_0 = _mm_set_ss(arg_0);
}
