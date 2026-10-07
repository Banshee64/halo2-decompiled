#include "unknown_11c920.h"
#include "unknown_1cec30.h"
// @flags /O2 /arch:SSE /Gr

class c_30b080
{
public:
 void function_30b080(__m128 const *arg_0);
};
void __cdecl function_1ce210(__m128 const *arg_0, __m128 const *arg_1,
 __m128 const *arg_2, real arg_3, __m128 *arg_4);

struct s_1cfb90
{
 hkVector4 field_0;
 hkVector4 field_10;
 hkVector4 field_20;
 hkTransform field_30;
 __m128 field_70[2];
};

// @retail 0x1cfb90
void function_1cfb90(s_havok_component *arg_0, transform4x3f const *arg_1, void const *arg_2)
{
 if (*(long volatile *)&arg_0->unknown9c)
 {
  real const *local_0 = (real const *)arg_2;
  s_1cfb90 local_1;
  local_1.field_20.set((local_0[0] + local_0[1]) * 0.5f, (local_0[2] + local_0[3]) * 0.5f,
   (local_0[5] + local_0[4]) * 0.5f);
  local_1.field_0.set((local_0[1] - local_0[0]) * 0.5f, (local_0[3] - local_0[2]) * 0.5f,
   (local_0[5] - local_0[4]) * 0.5f);
  local_1.field_30.m_rotation.m_col0.set(arg_1->forward.i, arg_1->forward.j, arg_1->forward.k);
  local_1.field_30.m_rotation.m_col1.set(arg_1->left.i, arg_1->left.j, arg_1->left.k);
  local_1.field_30.m_rotation.m_col2.set(arg_1->up.i, arg_1->up.j, arg_1->up.k);
  local_1.field_10.set(arg_1->position.x, arg_1->position.y, arg_1->position.z);
  local_1.field_30.m_translation = local_1.field_10;
  function_1ce210((__m128 const *)&local_1.field_30, &local_1.field_0.m_quad, &local_1.field_20.m_quad, 0.0f, local_1.field_70);
  ((c_30b080 *)*(long volatile *)&arg_0->unknown9c)->function_30b080(local_1.field_70);
 }
}
