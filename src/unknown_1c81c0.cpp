#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include "unknown_1fb7e0.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

void function_b9fc0(long arg_0, vector3f *arg_1, vector3f *arg_2);

PRIVATE inline real function_1c81c1(point3f const *arg_0, point3f const *arg_1)
{
 real local_0 = arg_0->x - arg_1->x;
 real local_1 = arg_0->y - arg_1->y;
 real local_2 = arg_0->z - arg_1->z;
 local_0 *= local_0;
 local_0 += local_1*local_1;
 local_0 += local_2*local_2;
 return (real)sqrt(local_0);
}

// @retail 0x1c81c0
void function_1c81c0()
{
 long local_0 = 0;
 long local_1 = 2;
 do
 {
  s_ai_player *local_2 = &g_4f55cc[local_0];
  if (local_2->player_index != NONE)
  {
   byte *local_3 = g_4e8c24->data + (local_2->player_index & 0xffff) * 0x21c;
   if (*(long *)(local_3 + 0x2c) != NONE)
   {
    byte *local_4 = (byte *)havok_object_get(*(long *)(local_3 + 0x2c));
    if (local_2->unknown0a > 0 && local_2->unit_index != NONE)
    {
     byte *local_5 = (byte *)havok_object_get(local_2->unit_index);
     if (!(function_1c81c1((point3f *)(local_5 + 0x30), (point3f *)(local_4 + 0x30)) - *(real *)(local_5 + 0x3c) < 3.0f))
     {
      if (--local_2->unknown0a <= 0)
      {
       local_2->unit_index = NONE;
       local_2->unknown08 = NONE;
       local_2->unknown0a = 0;
      }
     }
    }
    else local_2->unknown0a = 0;
    if (*(short *)(local_4 + 0x1fc) != NONE && g_4e0300->data[(*(long *)(local_4 + 0x14) & 0xffff)*12 + 3] == 1)
    {
     long local_9 = *(long const volatile *)(local_4 + 0x14);
     byte *local_10 = (byte *)havok_object_get(local_9);
     vector3f local_11;
     vector3f local_12;
     function_b9fc0(local_9, &local_12, &local_11);
     if (*(long *)(local_10 + 0x248) == *(long *)(local_3 + 0x2c))
     {
      if (local_10[0x34f] > 0 && (signed char)((byte *)local_2)[0x18] > g_510c54->field_2_3 && local_11.k > 0.7071067690849304f)
       function_20ba60(0x55, *(long *)(local_3 + 0x2c), NONE, NONE, NONE, NULL);
      else if (local_11.k < 0.0f && !((byte *)local_2)[0x19])
       ((byte *)local_2)[0x19] = function_20ba60(0x57, *(long *)(local_3 + 0x2c), NONE, NONE, NONE, NULL);
     }
     ((byte *)local_2)[0x18] = local_10[0x34c];
    }
    else
    {
     ((byte *)local_2)[0x18] = 0;
     ((byte *)local_2)[0x19] = 0;
    }
   }
  }
  ++local_0;
 }
 while (--local_1);
}
