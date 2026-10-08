// @flags /O1 /Oi /Gr
#include "screen_widgets.h"
#include <new>

class c_18f4bb
{
public:
    c_18f4bb(long arg_0, long arg_1, word arg_2);
    byte field_0[0x6c];
    bool field_6c;
    byte field_6d[0x11dc - 0x6d];
    bool field_11dc;
    byte field_11dd[3];
};

// @retail 0x18f42d
c_class_1473c9 *__stdcall function_18f42d(s_screen_parameters *arg_0)
{
    c_18f4bb *local_0;
    void *local_1 = function_1a47fd(sizeof(c_18f4bb));
    if (local_1)
        local_0 = new (local_1) c_18f4bb(arg_0->a, arg_0->b, arg_0->user_flags);
    else
        local_0 = 0;
    local_0->field_6c = true;
    local_0->field_11dc = false;
    ((c_class_1473c9 *)local_0)->function_147f6d(arg_0);
    return (c_class_1473c9 *)local_0;
}

// @retail 0x18f474
c_class_1473c9 *__stdcall function_18f474(s_screen_parameters *arg_0)
{
    c_18f4bb *local_0;
    void *local_1 = function_1a47fd(sizeof(c_18f4bb));
    if (local_1)
        local_0 = new (local_1) c_18f4bb(arg_0->a, arg_0->b, arg_0->user_flags);
    else
        local_0 = 0;
    local_0->field_6c = true;
    local_0->field_11dc = true;
    ((c_class_1473c9 *)local_0)->function_147f6d(arg_0);
    return (c_class_1473c9 *)local_0;
}
