#include "unknown_11c920.h"
#include "unknown_1d5120.h"
#include <float.h>
// @flags /O2 /arch:SSE /Gr

extern hkWorld *g_51e9a4;
void havok_component_rigid_body_transform_set(long arg_0, s_havok_component *arg_1, hkTransform const *arg_2);

// @retail 0x1d5120
bool function_1d5120(s_havok_component *arg_0, long arg_1, long arg_2, long arg_3,
 real arg_4, long *arg_5, long arg_6)
{
 (void)&arg_0; (void)&arg_1; (void)&arg_2; (void)&arg_3; (void)&arg_4; (void)&arg_5;
 bool local_0 = false;
 _control87(0x9001f, 0x8001f);
 _mm_setcsr(_mm_getcsr() | 0x1f80);
 hkTransform local_1;
 c_contact_query_transform_volume *local_2;
 {
  c_contact_query_transform_info local_3;
  local_1.set(havok_component_rigid_body_get(arg_1, arg_0)->m_motion->m_transform);
  local_3.transform.set(local_1);
  local_3.shape = (void *)arg_3;
  local_3.filter = ((arg_0->object_index + 1) << 16) | arg_2;
  ((real *)&local_1.m_translation.m_quad)[2] += (real)arg_6 * arg_4;
  local_2 = new c_contact_query_transform_volume(&local_3);
 }
 ((c_contact_query_world *)g_51e9a4)->add(local_2);
 bool local_4 = arg_6 > 0;
 long local_5 = arg_6 - (local_4 ? 1 : -1);
 c_contact_presence local_6;
 local_6.found = false;
 while (local_4 ? local_5 >= 0 : local_5 <= 0)
 {
  local_6.found = false;
  ((hkRigidBody *)local_2)->setTransform(local_1);
  ((c_contact_query_dispatch *)local_2)->query(&local_6);
  if (!local_6.found)
  {
   hkTransform local_7;
   local_7.m_rotation.m_col0.set(local_1.m_rotation.m_col0(0), local_1.m_rotation.m_col0(1), local_1.m_rotation.m_col0(2));
   local_7.m_rotation.m_col1.set(local_1.m_rotation.m_col1(0), local_1.m_rotation.m_col1(1), local_1.m_rotation.m_col1(2));
   local_7.m_rotation.m_col2.set(local_1.m_rotation.m_col2(0), local_1.m_rotation.m_col2(1), local_1.m_rotation.m_col2(2));
   hkVector4 local_8;
   local_8.set(local_1.m_translation(0), local_1.m_translation(1), local_1.m_translation(2));
   local_7.m_translation = local_8;
   *arg_5 = arg_6 - local_5;
   havok_component_rigid_body_transform_set(arg_1, arg_0, &local_7);
   local_0 = true;
   break;
  }
  ((real *)&local_1.m_translation.m_quad)[2] += (local_4 ? -1.0f : 1.0f) * arg_4;
  if (local_4) --local_5; else ++local_5;
 }
 ((c_contact_query_world *)g_51e9a4)->remove(local_2);
 havok_reference_remove((c_havok_reference_counted *)local_2);
 _mm_setcsr(_mm_getcsr() & 0xffffffc0);
 _clearfp();
 _control87(0x9001f, 0xfffff);
 return local_0;
}
