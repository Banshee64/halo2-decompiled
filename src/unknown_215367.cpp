// @flags /O1 /Oi /Gr
#include "unknown_11c920.h"
#include "unknown_19d220.h"
#include "unknown_2accd0.h"
#include <string.h>

class c_slots_215541;
struct s_storage_request;
s_storage_request *__stdcall function_215454(c_slots_215541 *arg_0, byte arg_1);
void __stdcall function_2154b4(void *arg_0);
void function_2154cd(long arg_0, s_storage_request *arg_1, long arg_2);
bool function_212f10(long arg_0, s_game_variant *arg_1, s_saved_game_file_task *arg_2);
bool function_212ec0(s_game_variant *arg_0, long arg_1);

// @retail 0x215367
void __stdcall function_215367(long arg_0, long arg_1, void *arg_2, long arg_3)
{
    s_storage_request *local_0 = NULL;
    s_storage_request *local_2 = function_215454((c_slots_215541 *)arg_3, 2);
    local_0 = local_2;
    if (local_2)
    {
        s_game_variant *local_1 = (s_game_variant *)((byte *)local_2 + 0x114);
        memcpy(local_1, arg_2, sizeof(*local_1));
        if (function_212f10(arg_1, local_1, (s_saved_game_file_task *)local_2))
        {
            if (!arg_3)
                function_2154cd(arg_0, local_2, 0x13000714);
        }
        else if (!arg_3)
        {
            function_2154b4(local_2);
            local_0 = NULL;
        }
    }
    if (!local_0)
        function_212ec0((s_game_variant *)arg_2, arg_1);
}
