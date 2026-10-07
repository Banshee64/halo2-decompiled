#include "unknown_11c920.h"
#include "globals.h"
#include "squads.h"

// @flags /O2 /Ob1 /Gr

void function_291740(short arg_0, long arg_1);

// @retail 0x202e30
void function_202e30(short arg_0, long arg_1, byte arg_2)
{
    byte const *local_0 = &arg_2;
    arg_1 &= 0xffff;
    byte *local_1 = g_51e9dc->data + arg_1 * 0x38;
    local_1[0x16] = *local_0;
    *(short *)(local_1 + 0x14) = arg_0;
    *(long *)(local_1 + 0x18) = g_510c54->game_time;
    *(short *)(local_1 + 0x1c) = NONE;
    *(short *)(local_1 + 0x1e) = NONE;
    if (arg_0 != NONE)
        function_291740(arg_0, arg_1 | 0x40000000);
}
