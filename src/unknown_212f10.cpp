// @flags /O2 /Ob1 /Gr
#include "unknown_11c920.h"
#include "unknown_19d220.h"
#include "unknown_2accd0.h"

bool function_19d650(s_game_variant *arg_0);
bool function_2173d0(long arg_0, wchar_t const *arg_1, void *arg_2, dword arg_3, s_saved_game_file_task *arg_4);

// @retail 0x212f10
bool function_212f10(long arg_0, s_game_variant *arg_1, s_saved_game_file_task *arg_2)
{
    (void)&arg_2;
    bool local_0 = true;
    if (arg_0 != NONE)
    {
        function_19d650(arg_1);
        local_0 = function_2173d0(arg_0, arg_1->name, arg_1, sizeof(*arg_1), arg_2);
    }
    return local_0;
}
