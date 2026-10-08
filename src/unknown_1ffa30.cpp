#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_0259d0.h"
// @flags /O2 /arch:SSE /Gr

void *function_1e5380(long arg_0);
bool function_1ff200(short arg_0, point3f const *arg_1, real arg_2, point3f const *arg_3,
    real const *arg_4, bool arg_5, vector3f *arg_6, real *arg_7, real *arg_8,
    vector3f *arg_9, real *arg_10);
bool function_1c9290(long arg_0, point3f const *arg_1, vector3f const *arg_2,
    real arg_3, real arg_4, long arg_5, bool arg_6);

// @retail 0x1ffa30
bool __stdcall function_1ffa30(s_type_c3b527 const *arg_1, long arg_0, long arg_2, long arg_3, bool arg_4)
{
    bool local_10 = false;
    s_type_c3b527 const *local_9 = *(s_type_c3b527 const *const volatile *)&arg_1;
    s_actor_view *local_0 = actor_get(arg_0);
    byte *local_1 = (byte *)function_1e5380(arg_0);
    point3f local_2;
    if (local_9->output_index != NONE)
    {
        if (!function_2104b0(local_9->output_index, &local_9->point, &local_2))
            local_2 = local_9->point;
    }
    else
        local_2 = local_9->point;
    if (local_1)
    {
        point3f local_3 = *(point3f *)((byte *)local_0 + 0x22c);
        vector3f local_4;
        real local_5;
        real local_6;
        vector3f local_7;
        real local_8;
        if (function_1ff200(*(short *)(local_1 + 4), &local_3, *(real *)(local_1 + 0x14),
            &local_2, NULL, false, &local_4, &local_5, &local_6, &local_7, &local_8) &&
            function_1c9290(arg_0, &local_3, &local_7, local_6, local_8, arg_3, local_0->unknown26c != NONE))
        {
            *(short *)((byte *)local_0 + 0x7c4) = g_510c54->field_2_3;
            *(s_type_c3b527 *)((byte *)local_0 + 0x7d0) = *local_9;
            *(long *)((byte *)local_0 + 0x7e0) = arg_2;
            *(long *)((byte *)local_0 + 0x7e4) = arg_3;
            *(vector3f *)((byte *)local_0 + 0x7e8) = local_4;
            *(real *)((byte *)local_0 + 0x7f4) = local_5;
            *(bool *)((byte *)local_0 + 0x7c6) = false;
            *(bool *)((byte *)local_0 + 0x7cc) = arg_4;
            local_10 = true;
        }
    }
    return local_10;
}
