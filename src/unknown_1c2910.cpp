#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1efac0.h"
#include "unknown_1eb350.h"
#include "havok_reference.h"
#include <new>
// @flags /O2 /arch:SSE /Gr

struct s_1c2910 : c_a
{
 long field_8;
 s_1c2910() { unknown06 = 1; field_8 = 0; }
};
class c_1c2910 : public s_1c2910
{
public:
 static void operator delete(void *arg_0)
 {
  g_480118->allocate((long)arg_0, ((c_1c2910 *)arg_0)->flags, 0x22);
 }
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
extern bool g_47f05a;
extern c_havok_reference_counted *g_4f55b0;
void __stdcall function_1e95d0(char const *arg_0);
void function_1e9650();
void function_1edc10();
void function_1c3b70();

PRIVATE __forceinline long function_1c2911()
{
 return *(long *)((byte *)g_4e0348 + 0x208);
}

// @retail 0x1c2910
void function_1c2910()
{
 function_1e95d0("building havok representation for bsp");
 if (!g_47f05a) function_1edc10();
 function_1c3b70();
 if (*(long *)((byte *)g_4e0348 + 0x204) > 0)
 {
  c_1c2910 *local_0 = (c_1c2910 *)((c_1d7390 *)g_480118)->function_1d7390(0xc, 0x22);
  c_1d7390 *local_2 = (c_1d7390 *)g_480118;
  local_0->flags = 0xc;
  local_0 = new (local_0) c_1c2910;
  c_shape_library_base_a *local_1 = (c_shape_library_base_a *)local_2->function_1d7390(0x14, 0x22);
  local_1->allocation_size = 0x14;
  local_1 = new (local_1) c_shape_library_base_a((c_havok_reference_counted *)local_0, function_1c2911());
  g_4f55b0 = (c_havok_reference_counted *)local_1;
  havok_reference_remove((c_havok_reference_counted *)local_0);
 }
 function_1e9650();
}
