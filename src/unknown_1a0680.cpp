#include "unknown_11c920.h"
#include <stddef.h>

// @flags /O2 /Gr

struct s_1a0680
{
    byte field_0[8];
    wchar_t field_8[32];
    byte field_48[0x1e0 - 0x48];
};

bool function_2173d0(wchar_t const *arg_0, long arg_1, void const *arg_2, long arg_3, long arg_4);

// @retail 0x1a0680
bool function_1a0680(s_1a0680 const *arg_0, long arg_1, long arg_2)
{
    bool local_0;
    if (arg_1 != NONE)
        local_0 = function_2173d0(arg_0->field_8, arg_1, arg_0, sizeof(s_1a0680), arg_2);
    else
        local_0 = false;
    return local_0;
}
