// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_181E90.CPP: whether the ratio of two lengths lies outside the
   allowed range */

#include "cseries.h"
#include "real_math.h"
#include <xmmintrin.h>

/* the smallest allowed ratio (set at run time) */
real g_54e870;

// @retail 0x181e90
bool function_181e90(real numerator, real denominator)
{
	if (!(numerator < 0.001f) && !(denominator < 0.001f))
	{
		real ratio = numerator / denominator;
		real pinned = ratio < g_54e870 ? g_54e870 : (ratio > 16.0f ? 16.0f : ratio);
		if (pinned != ratio)
		{
			return true;
		}
	}
	return false;
}

struct s_scale_definition;
struct s_scale_owner
{
	byte unknown00[0x3c];
	s_scale_definition *definition;
	real get_inverse_scale() const;
};

// @retail 0x181ee0
void function_181ee0(s_scale_owner const *a, s_scale_owner const *b, real *scale_a, real *scale_b)
{
	real inverse_a = a->get_inverse_scale();
	real inverse_b = b->get_inverse_scale();

	*scale_a = 1.0f;
	*scale_b = 1.0f;
	if (!(inverse_a < 0.001f) && !(inverse_b < 0.001f))
	{
		real ratio = inverse_a / inverse_b;
		if (ratio > 0.001f)
		{
			if (ratio < g_54e870)
			{
				*scale_a = ratio / g_54e870;
				*scale_b = 1.0f;
			}
			else if (ratio > 16.0f)
			{
				*scale_a = 16.0f / ratio;
				*scale_b = 1.0f;
			}
		}
	}
}
/* a Havok transform: three rotation columns and a translation */
struct s_havok_transform
{
	__m128 rotation[3];
	__m128 translation;
};

static inline void havok_vector4_set(__m128 *vector, real x, real y, real z, real w)
{
	vector->m128_f32[0] = x;
	vector->m128_f32[1] = y;
	vector->m128_f32[2] = z;
	vector->m128_f32[3] = w;
}

// @retail 0x181f80
void function_181f80(s_havok_transform *transform, real_matrix4x3 const *matrix)
{
	havok_vector4_set(&transform->rotation[0], matrix->forward.i, matrix->forward.j, matrix->forward.k, 0.0f);
	havok_vector4_set(&transform->rotation[1], matrix->left.i, matrix->left.j, matrix->left.k, 0.0f);
	havok_vector4_set(&transform->rotation[2], matrix->up.i, matrix->up.j, matrix->up.k, 0.0f);
	transform->translation = _mm_set_ps(0.0f, matrix->position.z, matrix->position.y, matrix->position.x);
}