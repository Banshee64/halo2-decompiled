// @flags /O1 /Oi /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_510c4c;
extern s_510c4c *g_510c4c;

struct s_2007b3
{
    char field_0;
    bool field_1;
    char field_2;
    byte field_3;
};

struct s_2007b4
{
    byte field_0[0x18];
    long field_18;
};

PRIVATE __forceinline s_2007b4 *function_2007b4(byte *arg_0, long arg_1)
{
    s_2007b4 *local_0 = *(s_2007b4 **)(arg_0 + 0x404);
    return &local_0[arg_1];
}

long function_1469f0(real arg_0);
long function_1896c0(real arg_0, long arg_1);

// @retail 0x2007b3
void function_2007b3(long arg_0, long arg_1, long arg_2, long arg_3)
{
    long const *local_0 = &arg_2;
    long const *local_1 = &arg_3;
    long local_2 = function_1469f0(0.5f);
    byte *local_3 = (byte *)g_510c4c + arg_0 * 0x6c;
    s_2007b3 *local_4 = (s_2007b3 *)(local_3 + 0x2c) + arg_1;
    for (long local_5 = 0; local_5 < 9; local_5++)
    {
        s_2007b3 *local_6 = (s_2007b3 *)(local_3 + 0x2c) + local_5;
        if (local_5 != arg_1 && local_6->field_0)
        {
            long local_7 = local_6->field_0;
            if ((real)local_7 * g_510c54->rate < 0.25f)
                local_2 += local_7;
            else
                local_2 = local_7;
            break;
        }
    }
    if (local_4->field_0)
    {
        long local_8 = local_4->field_2;
        local_4->field_2 = (char)(*local_0 > local_8 ? *local_0 : local_8);
    }
    else
    {
        if (arg_1 >= 0 && arg_1 < *(long *)((byte *)g_510c94 + 0x400))
        {
            s_2007b4 *local_9 = function_2007b4((byte *)g_510c94, arg_1);
            long local_10 = *(volatile long *)&local_9->field_18;
            if (local_10 != NONE && !local_4->field_1)
                function_1896c0(1.0f, local_10);
        }
        local_4->field_2 = (char)*local_0;
        local_4->field_0 = (char)local_2;
    }
    if (*local_1 != NONE)
    {
        s_2007b3 *local_11 = (s_2007b3 *)(local_3 + 0x2c) + *local_1;
        local_11->field_1 = false;
        local_11->field_2 = 0;
        local_11->field_0 = 0;
    }
}
