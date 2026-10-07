#include "unknown_11c920.h"
#include "unknown_182d90.h"
#include "unknown_1cec30.h"
#include "havok_reference.h"
#include <new>
// @flags /O2 /arch:SSE /Gr

struct s_extent_bounds
{
	__m128 lower;
	__m128 upper;
};

class c_extent_shape
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual long shape_kind() { return 0; }
	virtual void bounds(s_extent_transform const *transform, real tolerance, s_extent_bounds *result) {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void slot10() {}
	virtual long first_key() { return NONE; }
	virtual long next_key(long key) { return NONE; }
	virtual c_extent_shape *child(long key, void *buffer) { return 0; }
	virtual void slot14() {}
	virtual long vertex_count() { return 0; }
};

class c_1d7390
{
public:
	virtual void slot0() = 0;
	virtual void slot1() = 0;
	virtual void slot2() = 0;
	virtual void slot3() = 0;
	virtual void *function_1d7390(long arg_0, long arg_1) = 0;
};


struct c_child_transform : c_havok_reference_counted
{
 dword user;
 long field_c;
 hkTransform transform;
 c_child_transform(c_havok_reference_counted *child);
};
class c_physics_shape_list
{
public:
 c_physics_shape_list(c_havok_reference_counted **shapes, long count);
};
class hkShape;
class c_2dbfc0;
bool function_182180(hkShape const *shape);
real __stdcall function_182aa0(c_extent_shape *shape, real *minimum, real *maximum);
c_2dbfc0 *function_1d7390(long arg_0, void *arg_1, real arg_2);
extern real g_47f05c;

struct s_1d7401
{
 byte field_0[0xc];
 c_extent_shape *field_c;
 hkTransform field_10;
};

struct s_1d7400
{
 c_extent_shape *function_1d7400(c_extent_shape *arg_0, void *arg_1, real arg_2);
};

// @retail 0x1d7400
c_extent_shape *s_1d7400::function_1d7400(c_extent_shape *arg_0, void *arg_1, real arg_2)
{
 (void)&arg_0; (void)&arg_1; (void)&arg_2;
 real local_1, local_0;
 function_182aa0(arg_0, &local_1, &local_0);
 real local_2 = (local_0 * arg_2 * 0.5f + g_47f05c) * g_510c54->field_2_3;
 long local_3 = arg_0->shape_kind();
 if (local_3 != 2 && (local_3 <= 9 || local_3 > 11)) goto local_23;
 {
  c_extent_shape *local_4[32];
  long local_5 = 0;
  for (long local_6 = arg_0->first_key(); local_6 != NONE; local_6 = arg_0->next_key(local_6))
  {
   __m128 local_7[16];
   c_extent_shape *local_8 = arg_0->child(local_6, local_7);
   if (local_8->shape_kind() == 0x11)
    local_4[local_5] = local_8;
   else if (local_8->shape_kind() == 0x15)
   {
    c_extent_shape *local_9 = ((s_1d7401 *)local_8)->field_c;
    if (function_182180((hkShape *)local_9))
    {
     c_havok_reference_counted *local_10 = (c_havok_reference_counted *)function_1d7390((long)local_9, arg_1, local_2);
     void *local_17 = ((c_1d7390 *)g_480118)->function_1d7390(0x50, 0x22);
     c_child_transform *local_18 = local_17 ? new (local_17) c_child_transform(local_10) : NULL;
     local_18->transform = ((s_1d7401 *)local_8)->field_10;
     havok_reference_remove(local_10);
     local_4[local_5] = (c_extent_shape *)local_18;
    }
    else
    {
     c_havok_reference_counted *local_11 = (c_havok_reference_counted *)function_1d7400(local_9, arg_1, arg_2);
     void *local_19 = ((c_1d7390 *)g_480118)->function_1d7390(0x50, 0x22);
     c_child_transform *local_20 = local_19 ? new (local_19) c_child_transform(local_11) : NULL;
     local_20->transform = ((s_1d7401 *)local_8)->field_10;
     havok_reference_remove(local_11);
     local_4[local_5] = (c_extent_shape *)local_20;
    }
   }
   else if (function_182180((hkShape *)local_8))
    local_4[local_5] = (c_extent_shape *)function_1d7390((long)local_8, arg_1, local_2);
   else
    local_4[local_5] = function_1d7400(local_8, arg_1, arg_2);
   ++local_5;
  }
  void *local_12 = ((c_1d7390 *)g_480118)->function_1d7390(0x38, 0x22);
  ((c_havok_reference_counted *)local_12)->allocation_size = 0x38;
  c_extent_shape *local_13 = (c_extent_shape *)new (local_12) c_physics_shape_list((c_havok_reference_counted **)local_4, local_5);
  for (long local_14 = 0; local_14 < local_5; ++local_14)
   if (local_4[local_14]->shape_kind() != 0x11)
    havok_reference_remove((c_havok_reference_counted *)local_4[local_14]);
  return local_13;
 }
 local_23:
 {
 if (arg_0->shape_kind() == 0x11) return arg_0;
 if (arg_0->shape_kind() == 0x15)
 {
  c_havok_reference_counted *local_15 = (c_havok_reference_counted *)function_1d7390((long)((s_1d7401 *)arg_0)->field_c, arg_1, local_2);
  void *local_21 = ((c_1d7390 *)g_480118)->function_1d7390(0x50, 0x22);
  c_child_transform *local_22 = local_21 ? new (local_21) c_child_transform(local_15) : NULL;
  local_22->transform = ((s_1d7401 *)arg_0)->field_10;
  havok_reference_remove(local_15);
  return (c_extent_shape *)local_22;
 }
 return (c_extent_shape *)function_1d7390((long)arg_0, arg_1, local_2);
 }
 }
