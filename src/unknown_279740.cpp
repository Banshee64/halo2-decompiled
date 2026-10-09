// @flags /O2 /Ob1 /Gr
#include "unknown_11c920.h"
#include "unknown_122870.h"


#define PIN(value, lower, upper) ((value) < (lower) ? (lower) : (value) > (upper) ? (upper) : (value))

// @retail 0x279740
bool __cdecl function_279740(long value)
{
    long local_0 = value;
    if (value < (long)0x80061000)
        local_0 = (long)0x80061000;
    else
    {
        bool local_1 = cache_file_globals.loaded;
        long local_2 = cache_file_globals.header.unknown1c;
        bool const volatile *local_3 = &local_1;
        if (value > (long)0x80061000 + (local_1 ? local_2 : 0))
            local_0 = (long)0x80061000 + (*local_3 ? local_2 : 0);
    }
    return local_0 == value;
}
