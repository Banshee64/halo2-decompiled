// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "unknown_2accd0.h"

extern long g_55c154;
void function_125d60(void);
bool function_2173d0(long arg_0, wchar_t const *arg_1, void *arg_2, dword arg_3, s_saved_game_file_task *arg_4);

// @retail 0x216240
bool function_216240(long arg_0, void *arg_1, long arg_2, wchar_t *arg_3)
{
    (void)&arg_3;
    bool local_0 = false;
    s_saved_game_file_task local_1;
    s_saved_game_file_task *local_2 = &local_1;
    if (function_2173d0(arg_0, arg_3, arg_1, arg_2, local_2))
    {
        if (!*(volatile bool *)&local_2->done)
        {
            while (!*(volatile bool *)&local_2->done)
            {
                SwitchToThread();
                function_125d60();
            }
        }
        local_0 = local_2->succeeded;
        if (!local_0)
            g_55c154 = local_2->state;
    }
    return local_0;
}
