// @flags /O2 /Gr

#include "unknown_11c920.h"
#include "globals.h"

struct s_e5240_object
{
    byte unknown00[0x3dc];
    byte state_3dc;
};

struct s_e5240_object_header
{
    byte unknown00[8];
    s_e5240_object *object;
};

bool function_e4050(long object_index);

// @retail 0xe5240
bool function_e5240(long object_index)
{
    s_e5240_object *object =
        ((s_e5240_object_header *)g_4e0300->data)[object_index & 0xffff].object;

    bool result = false;

    if (object->state_3dc == 1)
        result = function_e4050(object_index);
    return result;
}