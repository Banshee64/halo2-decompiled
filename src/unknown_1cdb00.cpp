#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"
#include "unknown_1cafc0.h"
// @flags /O2 /arch:SSE /Gr

struct s_anim_data;
struct s_index_pair;
extern bool g_46fbf4;
bool function_0b6760(s_index_pair const *arg_0);
void c_animation_channel_data_get(c_animation_channel const *arg_0, s_animation_data *arg_1);
void function_20ab60(s_anim_data *arg_1, vector3f *arg_0, real *arg_2);
bool __stdcall function_1cd8a0(s_animation_state *arg_0, long arg_1, vector3f const *arg_2);

PRIVATE __forceinline void function_1cdb01(s_anim_data *arg_0, vector3f *arg_1, real *arg_2)
{
 function_20ab60(arg_0, arg_1, arg_2);
}

// @retail 0x1cdb00
bool __stdcall function_1cdb00(s_animation_state *arg_0, long arg_1, vector3f const *arg_2)
{
 (void)&arg_1; (void)&arg_2;
 bool local_0 = false;
 if ((arg_1 == NONE || !(((byte *)havok_object_get(arg_1))[0xb3] > 0)) && !(arg_0->unknown6e & 1))
 {
  c_animation_channel *local_1 = &arg_0->channels[0];
  c_animation_channel *local_2;
  s_animation *local_3;
  if (g_46fbf4 && local_1->graph_tag_index != NONE && local_1->animation_id.index != NONE &&
   (local_3 = local_1->function_1c6440()) != NULL && local_3->frame_info_type &&
   function_0b6760((s_index_pair const *)(local_2 = &arg_0->channels[1])) &&
   local_2->function_1c6440() && local_2->function_1c6440()->frame_count > 0)
  {
   s_animation_data local_4;
   vector3f local_5;
   real local_6;
   c_animation_channel_data_get(local_1, &local_4);
   function_1cdb01((s_anim_data *)&local_4, &local_5, &local_6);
   real local_7 = arg_2->i * arg_2->i + arg_2->j * arg_2->j + arg_2->k * arg_2->k;
   if (local_7 > 0.0f)
   {
    real local_11 = local_7;
    *(long *)&local_11 = (*(long *)&local_11 >> 1) + 0x1fc00000;
    real local_8 = local_11;
    if (local_8 < 0.0f) local_8 = 0.0f;
    else if (local_8 > 1.0f) local_8 = 1.0f;
    real local_9 = (real)g_510c54->field_2_3 * local_8;
    vector3f local_10;
    local_10.i = local_5.i * local_9;
    local_10.j = local_5.j * local_9;
    local_10.k = local_5.k * local_9;
    local_0 = function_1cd8a0(arg_0, arg_1, &local_10);
   }
  }
  if (!local_0) arg_0->unknown80 = 0.0f;
 }
 return local_0;
}
