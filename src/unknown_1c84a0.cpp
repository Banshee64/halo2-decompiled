#include "unknown_11c920.h"
#include "globals.h"
#include "squads.h"
#include "unknown_1e46c0.h"
// @flags /O2 /Gr

void __stdcall function_1e2990(long arg_0, bool arg_1);

// @retail 0x1c84a0
void function_1c84a0(long arg_0, bool arg_1)
{
 if (g_4f55d0->active)
 {
  if (arg_0 != NONE)
  {
   s_squad_actor_iterator local_0;
   function_204d30(&local_0, arg_0);
   long local_1 = local_0.next_actor_index;
   while (g_4f55d0->active && local_1 != NONE)
   {
    byte *local_2 = g_4f55f0->data + (local_1 & 0xffff) * 0x888;
    long local_3 = local_1;
    local_1 = *(long *)(local_2 + 0x20);
    function_1e2990(local_3, (bool)arg_1);
   }
  }
  else
  {
   s_actor_iterator local_4;
   function_x66da2b(&local_4, false);
   while (function_1e46c0(&local_4))
    function_1e2990(local_4.actor_index, (bool)arg_1);
  }
 }
}

struct s_1c88c0
{
 byte field_0;
 byte field_1[3];
 long field_4;
 long field_8;
};
struct s_1c88c1
{
 short field_0;
 short field_2;
 s_1c88c0 field_4[1];
};

// @retail 0x1c88c0
long __stdcall function_1c88c0(long arg_0, void *arg_1, long arg_2, bool *arg_3, void *arg_4, long arg_5)
{
 long local_0 = 0;
 s_1c88c1 *local_1 = (s_1c88c1 *)arg_1;
 if (local_1->field_2 < local_1->field_0)
 {
  s_1c88c0 *local_2 = &local_1->field_4[local_1->field_2];
  if (local_2->field_0)
  {
   local_0 = 1;
   function_1e2990(local_2->field_4, true);
  }
  else
  {
   long local_3 = local_2->field_4;
   local_0 = *(short *)(g_51e9d8->data + (local_3 & 0xffff) * 0x98 + 0xa);
   function_1c84a0(local_3, 1);
  }
  ++local_1->field_2;
 }
 *arg_3 = local_1->field_2 < local_1->field_0;
 return local_0;
}
