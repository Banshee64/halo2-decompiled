#include "unknown_11c920.h"
#include "unknown_1cec30.h"
// @flags /O2 /arch:SSE /Gr

struct c_transformed_point
{
 __m128 value;
 void transform(const void *matrix, const __m128 *point);
};

struct s_1ce8d0
{
 hkRigidBody *field_0;
 hkRigidBody *field_4;
 __m128 field_10;
 __m128 field_20;
 real field_30;
 void function_1ce8d0(hkRigidBody *arg_1, hkRigidBody *arg_2, const __m128 *arg_3, const __m128 *arg_4);
};

// @retail 0x1ce8d0
void s_1ce8d0::function_1ce8d0(hkRigidBody *arg_1, hkRigidBody *arg_2, const __m128 *arg_3, const __m128 *arg_4)
{
 hkRigidBody *const *local_5 = &arg_2;
 field_0 = arg_1;
 field_4 = *local_5;
 field_10 = *arg_3;
 field_20 = *arg_4;
 c_transformed_point local_0, local_1;
 local_0.transform(&arg_1->m_motion->m_transform, arg_3);
 local_1.transform(&(*local_5)->m_motion->m_transform, arg_4);
 __m128 local_2 = _mm_sub_ps(local_0.value, local_1.value);
 local_2 = _mm_mul_ps(local_2, local_2);
 __m128 local_3 = _mm_add_ss(_mm_shuffle_ps(local_2, local_2, 0x55), local_2);
 local_2 = _mm_add_ss(_mm_shuffle_ps(local_2, local_2, 0xaa), local_3);
 local_2 = _mm_sqrt_ss(local_2);
 real local_4;
 _mm_store_ss(&local_4, local_2);
 field_30 = local_4;
}
