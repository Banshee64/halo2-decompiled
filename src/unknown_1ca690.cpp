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

struct s_1ca693
{
 byte field_0[0xc];
 real field_c;
 real field_10;
 byte field_14[2];
 bool field_16;
 bool field_17;
 long field_18;
 short field_1c;
 short field_1e;
 long field_20;
 s_1ca691 *field_24;
 short field_28;
 short field_2a;
 long field_2c;
 long field_30;
};

// @retail 0x1ca690
void __stdcall function_1ca690(long arg_0, void const *arg_1, long arg_2, long arg_3, long arg_4)
{
 s_1ca693 local_11;
 if (g_4f55d0->active)
 {
  local_11.field_30 = function_146650();
  do {} while (false);
  do {} while (false);
  do {} while (false);
  if ((short)arg_3 > 0)
  {
   local_11.field_18 = local_11.field_30 - function_1469f0(1.0f);
   local_11.field_20 = local_11.field_30 - function_1469f0(1.0f);
   local_11.field_2c = local_11.field_20 - function_1469f0(3.0f);
   local_11.field_24 = NULL;
   local_11.field_17 = true;
   local_11.field_28 = NONE;
   local_11.field_1c = ((s_1ca692 *)g_4f55d0)->field_3c;
   for (; local_11.field_1c != ((s_1ca692 *)g_4f55d0)->field_3e; local_11.field_1c = (local_11.field_1c + 1) & 31)
   {
    local_11.field_16 = false;
    if ((short)arg_2 == ((s_1ca692 *)g_4f55d0)->field_40[local_11.field_1c].field_0 &&
        distance_sq3f(&((s_1ca692 *)g_4f55d0)->field_40[local_11.field_1c].field_8, (point3f const *)arg_1) < 1.0f)
     local_11.field_16 = true;
    if (((s_1ca692 *)g_4f55d0)->field_40[local_11.field_1c].field_14 <= local_11.field_2c)
    {
     ((s_1ca692 *)g_4f55d0)->field_40[local_11.field_1c].field_0 = NONE;
     if (local_11.field_1c == ((s_1ca692 *)g_4f55d0)->field_3c)
      ((s_1ca692 *)g_4f55d0)->field_3c = (local_11.field_1c + 1) & 31;
     else
      local_11.field_28 = local_11.field_1c;
    }
    else if (local_11.field_16)
    {
     local_11.field_24 = &((s_1ca692 *)g_4f55d0)->field_40[local_11.field_1c];
     local_11.field_24->field_4 = (short)(local_11.field_24->field_4 + 1);
     local_11.field_17 = local_11.field_24->field_14 < local_11.field_18;
     if (local_11.field_24->field_14 < local_11.field_20)
     {
      local_11.field_24->field_8 = *(point3f const *)arg_1;
      do {} while (false);
     }
     else
     {
      local_11.field_10 = 1.0f / local_11.field_24->field_4;
      local_11.field_c = 1.0f - local_11.field_10;
      local_11.field_24->field_8.x = local_11.field_c * local_11.field_24->field_8.x + local_11.field_10 * ((point3f const *)arg_1)->x;
      local_11.field_24->field_8.y = local_11.field_c * local_11.field_24->field_8.y + local_11.field_10 * ((point3f const *)arg_1)->y;
      local_11.field_24->field_8.z = local_11.field_c * local_11.field_24->field_8.z + local_11.field_10 * ((point3f const *)arg_1)->z;
     }
     break;
    }
   }
   if (!local_11.field_24)
   {
    if (local_11.field_28 == NONE)
    {
     local_11.field_1c = ((s_1ca692 *)g_4f55d0)->field_3e;
     ((s_1ca692 *)g_4f55d0)->field_3e = (((s_1ca692 *)g_4f55d0)->field_3e + 1) & 31;
     if (((s_1ca692 *)g_4f55d0)->field_3e == ((s_1ca692 *)g_4f55d0)->field_3c)
      ((s_1ca692 *)g_4f55d0)->field_3c = (((s_1ca692 *)g_4f55d0)->field_3c + 1) & 31;
    }
    else
     local_11.field_1c = local_11.field_28;
    local_11.field_24 = &((s_1ca692 *)g_4f55d0)->field_40[local_11.field_1c];
    local_11.field_24->field_0 = (short)arg_2;
    local_11.field_24->field_2 = (short)arg_3;
    local_11.field_24->field_4 = 1;
    local_11.field_24->field_8 = *(point3f const *)arg_1;
    local_11.field_24->field_14 = local_11.field_30;
   }
   if (local_11.field_17)
    ((void (__stdcall *)(long, long, long, long, s_1fbac0_event *))function_1fbac0)(NONE, arg_0, 3, 1, (s_1fbac0_event *)local_11.field_24);
  }
 }
}
