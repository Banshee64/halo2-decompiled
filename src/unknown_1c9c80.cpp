#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"
// @flags /O2 /arch:SSE /Gr

struct s_prop_node;
struct s_type_f95cd3;
struct s_ai_player;
long function_1c9580(long arg_0, bool arg_1);
long function_25d770(long arg_0, long arg_1);
s_type_f95cd3 *function_25d740(s_prop_node *arg_0);
bool function_1df560(short arg_0, short arg_1);
void function_1df950(short arg_0, short arg_1);
s_ai_player *ai_player_get(long arg_0);
long function_baf40(long arg_0);
void __stdcall function_1e2570(long arg_0, word arg_1, long arg_2, real arg_3, long arg_4);

// @retail 0x1c9c80
void function_1c9c80(long arg_0, long arg_1, word arg_2, real arg_3, long arg_4, bool arg_5)
{
 (void)&arg_2;
 (void)&arg_3;
 (void)&arg_4;
 (void)&arg_5;
 if (!g_4f55d0->active || !(arg_3 >= 0.0f)) return;
 long local_0 = function_1c9580(arg_1, arg_2 != 9);
 byte *local_1 = local_0 == NONE ? NULL : (byte *)havok_object_get(local_0);
 byte *local_2 = (byte *)havok_object_get(arg_0);
 if (local_1 && *(long *)(local_1 + 0x12c) != NONE)
 {
  long local_3 = function_25d770(*(long *)(local_1 + 0x12c), arg_0);
  if (local_3 != NONE)
  {
   byte *local_4 = g_502418->data + (local_3 & 0xffff) * 0x3c;
   byte *local_5 = (byte *)function_25d740((s_prop_node *)local_4);
   byte *local_6 = g_50241c->data + (*(long *)(local_4 + 8) & 0xffff) * 0xc4;
   if (local_5) *(real *)(local_5 + 0x54) += 0.2f;
   *(real *)(local_6 + 0x28) += arg_3;
  }
  if (*(long *)(local_1 + 0x13c) != NONE && *(short *)(local_2 + 0x1fc) != NONE &&
      !function_1df560(*(short *)(local_1 + 0x138), *(short *)(local_2 + 0x138)))
  {
   byte *local_7 = (byte *)ai_player_get(*(long *)(local_1 + 0x13c));
   if (local_7)
   {
    *(long *)(local_7 + 0x10) = g_510c54->game_time;
    *(long *)(local_7 + 0x14) = function_baf40(*(long *)(local_2 + 0x14));
   }
  }
 }
 if (*(long *)(local_2 + 0x12c) != NONE)
 {
  if (!arg_5 && arg_2 != 1)
   function_1e2570(*(long *)(local_2 + 0x12c), arg_2, local_0, arg_3, arg_4);
  if (local_1) function_1df950(*(short *)(local_1 + 0x138), *(short *)(local_2 + 0x138));
 }
}
