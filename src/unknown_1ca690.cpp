#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "unknown_1fb7e0.h"
#include "globals.h"
// @flags /Od /arch:SSE /Gr

struct s_1ca691
{
 short field_0;
 short field_2;
 short field_4;
 short field_6;
 point3f field_8;
 long field_14;
};
struct s_1ca692
{
 byte field_0[0x3c];
 short field_3c;
 short field_3e;
 s_1ca691 field_40[32];
};
long function_146650();
long function_1469f0(real seconds);
real distance_sq3f(point3f const *a, point3f const *b);
void __stdcall function_1fbac0(long unknown, long unit_index, bool unknown2, long unknown3, s_1fbac0_event *event);

// @retail 0x1ca690
void __stdcall function_1ca690(long arg_0, void const *arg_1, long arg_2, long arg_3, long arg_4)
{
 if (g_4f55d0->active)
 {
  long local_0 = function_146650();
  do {} while (false);
  do {} while (false);
  do {} while (false);
  if ((short)arg_3 > 0)
  {
   long local_1 = local_0 - function_1469f0(1.0f);
   long local_2 = local_0 - function_1469f0(1.0f);
   long local_3 = local_2 - function_1469f0(3.0f);
   s_1ca691 *local_4 = NULL;
   bool local_5 = true;
   short local_6 = NONE;
   short local_7 = ((s_1ca692 *)g_4f55d0)->field_3c;
   for (; local_7 != ((s_1ca692 *)g_4f55d0)->field_3e; local_7 = (local_7 + 1) & 31)
   {
    bool local_8 = false;
    if ((short)arg_2 == ((s_1ca692 *)g_4f55d0)->field_40[local_7].field_0 &&
        distance_sq3f(&((s_1ca692 *)g_4f55d0)->field_40[local_7].field_8, (point3f const *)arg_1) < 1.0f)
     local_8 = true;
    if (((s_1ca692 *)g_4f55d0)->field_40[local_7].field_14 <= local_3)
    {
     ((s_1ca692 *)g_4f55d0)->field_40[local_7].field_0 = NONE;
     if (local_7 == ((s_1ca692 *)g_4f55d0)->field_3c)
      ((s_1ca692 *)g_4f55d0)->field_3c = (local_7 + 1) & 31;
     else
      local_6 = local_7;
    }
    else if (local_8)
    {
     local_4 = &((s_1ca692 *)g_4f55d0)->field_40[local_7];
     local_4->field_4 = (short)(local_4->field_4 + 1);
     local_5 = local_4->field_14 < local_1;
     if (local_4->field_14 < local_2)
     {
      local_4->field_8 = *(point3f const *)arg_1;
      do {} while (false);
     }
     else
     {
      real local_9 = 1.0f / local_4->field_4;
      real local_10 = 1.0f - local_9;
      local_4->field_8.x = local_10 * local_4->field_8.x + local_9 * ((point3f const *)arg_1)->x;
      local_4->field_8.y = local_10 * local_4->field_8.y + local_9 * ((point3f const *)arg_1)->y;
      local_4->field_8.z = local_10 * local_4->field_8.z + local_9 * ((point3f const *)arg_1)->z;
     }
     break;
    }
   }
   if (!local_4)
   {
    if (local_6 == NONE)
    {
     local_7 = ((s_1ca692 *)g_4f55d0)->field_3e;
     ((s_1ca692 *)g_4f55d0)->field_3e = (((s_1ca692 *)g_4f55d0)->field_3e + 1) & 31;
     if (((s_1ca692 *)g_4f55d0)->field_3e == ((s_1ca692 *)g_4f55d0)->field_3c)
      ((s_1ca692 *)g_4f55d0)->field_3c = (((s_1ca692 *)g_4f55d0)->field_3c + 1) & 31;
    }
    else
     local_7 = local_6;
    local_4 = &((s_1ca692 *)g_4f55d0)->field_40[local_7];
    local_4->field_0 = (short)arg_2;
    local_4->field_2 = (short)arg_3;
    local_4->field_4 = 1;
    local_4->field_8 = *(point3f const *)arg_1;
    local_4->field_14 = local_0;
   }
   if (local_5)
    ((void (__stdcall *)(long, long, long, long, s_1fbac0_event *))function_1fbac0)(NONE, arg_0, 3, 1, (s_1fbac0_event *)local_4);
  }
 }
}
