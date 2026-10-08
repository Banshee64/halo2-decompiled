// @flags /O1 /Oi /Gr
#include "unknown_11c920.h"
#include "unknown_2accd0.h"
#include <string.h>

class c_slots_215541;
struct s_player_profile;
struct s_player_profile_settings;
struct s_storage_request;

s_storage_request *__stdcall function_215454(c_slots_215541 *arg_0, bool arg_1);
void __stdcall function_2154b4(void *arg_0);
void function_2154cd(s_storage_request *arg_0, long arg_1, long arg_2);
bool function_1a0660(long arg_0, s_player_profile *arg_1);
bool function_1a0680(long arg_0, s_player_profile *arg_1, s_saved_game_file_task *arg_2);

// @retail 0x2153dd
void __stdcall function_2153dd(long arg_0, long arg_1, s_player_profile_settings *arg_2, long arg_3)
{
    s_storage_request *local_0 = function_215454((c_slots_215541 *)arg_3, true);
    if (local_0)
    {
        s_player_profile *local_1 = (s_player_profile *)((byte *)local_0 + 0x114);
        memcpy(local_1, arg_2, 0x1e0);
        if (function_1a0680(arg_1, local_1, (s_saved_game_file_task *)local_0))
        {
            if (!arg_3)
                function_2154cd(local_0, arg_0, 0x15000715);
        }
        else if (!arg_3)
        {
            function_2154b4(local_0);
            local_0 = NULL;
        }
    }
    if (!local_0)
        function_1a0660(arg_1, (s_player_profile *)arg_2);
}
