#include "unknown_11c920.h"
#include "slot_handler.h"
#include "unknown_0259d0.h"
#include "unknown_2626b0.h"
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
void function_caf90(long arg_0, point3f *arg_1);
bool function_29d6c0(vector3f *arg_0, s_reference arg_1);
real normalize2d(point2f *arg_0);
long function_baf80(long arg_0);

// @retail 0x1f9580
bool function_1f9580(long arg_0, s_reference arg_1)
{
    s_actor_view *local_0 = actor_get(arg_0);
    s_262b40_result *local_1 = function_262b40(arg_1);
    bool local_2 = false;
    if (local_1 && (((byte *)local_1)[0xe] & 0x20) && local_0->unknown018 != NONE)
    {
        point3f local_3;
        point3f local_4;
        function_caf90(local_0->unknown018, &local_3);
        function_210850((s_type_c3b527 const *)local_1, &local_4);
        vector3f local_5;
        vector3d_from_points3d(&local_4, &local_3, &local_5);
        real local_6 = local_5.k * local_5.k + local_5.j * local_5.j + local_5.i * local_5.i;
        if (local_6 < 12.25f)
        {
            vector3f local_7;
            function_29d6c0(&local_7, arg_1);
            point2f local_8;
            local_8.x = local_7.i;
            local_8.y = local_7.j;
            if (normalize2d(&local_8) > 0.0f &&
                (local_3.y - local_4.y) * local_8.y + (local_3.x - local_4.x) * local_8.x > 1.2f &&
                local_0->unknown018 != NONE)
            {
                vector3f local_9;
                vector3d_from_points3d(&local_3, &local_4, &local_9);
                s_collision_result_1697c0 local_10;
                local_10.field_24 = NONE;
                if (!function_1697c0(0x1808c2d, &local_3, &local_9,
                    function_baf80(local_0->unknown018), NONE, &local_10) ||
                    local_10.field_4 >= 1.0f ||
                    (1.0f - local_10.field_4) * (1.0f - local_10.field_4) * local_6 < 0.1f)
                    local_2 = true;
            }
        }
    }
    return local_2;
}
