// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"

struct s_e58e0_object_header
{
    byte unknown00[8];
    byte *unit;
};

// @retail 0xe58e0
bool function_e58e0(long unit_index)
{
    byte *unit =
        ((s_e58e0_object_header *)g_4e0300->data)[unit_index & 0xffff].unit;

    long weapon_index = *(long *)(unit + 0x3e8);
    bool result = false;

    if (weapon_index != NONE)
    {
        word flags = *(word *)(unit + 0xc0);

        byte flag_1 = (byte)(flags >> 1);
        byte flag_2 = (byte)(flags >> 2);

        if (flag_1 & 1)
            result = true;
        else if (flag_2 & 1)
            result = true;
        else if (weapon_index == *(long *)(unit + 0x3e4))
            result = true;
    }

    return result;
}