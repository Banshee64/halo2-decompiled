#include "unknown_11c920.h"
#include "unknown_0259d0.h"
#include "slot_handler.h"
#include <math.h>
// @flags /O2 /arch:SSE /Gr

struct s_collision_result_1697c0
{
    long field_0;
    real field_4;
    point3f field_8;
    byte field_14[0x24 - 0x14];
    short field_24;
    byte field_26[0x40 - 0x26];
    long field_40;
    byte field_44[0x5c - 0x44];
};
bool __stdcall function_1697c0(long arg_0, point3f const *arg_1, vector3f const *arg_2,
    long arg_3, long arg_4, s_collision_result_1697c0 *arg_5);

// @retail 0x1fe410
void function_1fe410(point3f *arg_0, real arg_1)
{
    point3f local_0;
    local_0.x = g_4687b0->i * 1.5f + arg_0->x;
    local_0.y = g_4687b0->j * 1.5f + arg_0->y;
    local_0.z = g_4687b0->k * 1.5f + arg_0->z;
    real local_1 = slot_random() * 6.2831854820251465f - 3.1415927410125732f;
    point3f local_2;
    local_2.x = (real)cos(local_1) * arg_1 + local_0.x;
    local_2.y = (real)sin(local_1) * arg_1 + local_0.y;
    local_2.z = arg_1 * 0.0f + local_0.z;
    vector3f local_3;
    vector3d_from_points3d(arg_0, &local_0, &local_3);
    s_collision_result_1697c0 local_4;
    local_4.field_24 = NONE;
    if (function_1697c0(0x15808c2f, arg_0, &local_3, NONE, NONE, &local_4))
        local_0 = *arg_0;
    vector3d_from_points3d(&local_0, &local_2, &local_3);
    if (function_1697c0(0x5808c2f, &local_0, &local_3, NONE, NONE, &local_4))
    {
        real local_5 = local_4.field_4 * arg_1 - 0.1f;
        if (local_5 < 0.0f)
            local_5 = 0.0f;
        local_2.x = local_3.i * local_5 + local_0.x;
        local_2.y = local_3.j * local_5 + local_0.y;
        local_2.z = local_3.k * local_5 + local_0.z;
    }
    *arg_0 = local_2;
}
