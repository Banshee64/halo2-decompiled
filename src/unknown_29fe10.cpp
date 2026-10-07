#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "slot_handler.h"
#include "object_iterator.h"
#include "unit_requests.h"
#include <string.h>
// @flags /O2 /Ob1 /arch:SSE /Gr

bool __stdcall function_beb30(long arg_0);
void __stdcall function_b8540(long arg_0);

struct s_29fe10 { byte field_0[0x2c]; long field_2c; };
struct s_29fe11 { s_slot_object_view *field_0; s_type_f1af8e field_4; };

PRIVATE __forceinline void function_29fe72(s_type_f1af8e *arg_0, dword arg_1)
{
    arg_0->signature = 0x86868686;
    if (!arg_1) arg_1 = NONE;
    arg_0->type_mask = arg_1;
    arg_0->flags = 0;
    arg_0->index = 0;
    arg_0->object_index = NONE;
}

// @retail 0x29fe10
void __stdcall function_29fe10(long arg_0)
{
    if (arg_0 & 2)
    {
        s_record_pool_iterator local_0;
        local_0.data = g_4e8c24;
        local_0.index = NONE;
        s_29fe10 *local_1;
        while ((local_1 = (s_29fe10 *)data_iterator_next_inlined(&local_0)) != NULL)
        {
            long local_2 = local_1->field_2c;
            if (local_2 != NONE)
            {
                long local_3 = local_2;
                long local_4;
                do
                {
                    local_4 = local_3;
                    local_3 = object_get(local_3)->parent_index;
                } while (local_3 != NONE);
                if (local_4 != local_2)
                {
                    s_unit_request local_5;
                    memset(&local_5, 0, sizeof(local_5));
                    local_5.type = 0x1e;
                    function_e6900(local_2, &local_5);
                }
            }
        }
    }
    s_29fe11 local_6;
    function_29fe72(&local_6.field_4, arg_0);
    while ((local_6.field_0 = (s_slot_object_view *)function_baeb0(&local_6.field_4)) != NULL)
        if (local_6.field_0->parent_index == NONE && !function_beb30(local_6.field_4.object_index))
            function_b8540(local_6.field_4.object_index);
}
