#include "unknown_11c920.h"
#include "globals.h"

// @flags /O2 /Gr

void function_28fd00(long arg_0);
void function_2952e0(long arg_0);

// @retail 0x211640
void function_211640(long arg_0)
{
    byte *local_0 = *(byte **)(g_4e0300->data + (arg_0 & 0xffff) * 12 + 8);
    switch (*(long *)(local_0 + 0x134))
    {
    case 0:
        function_28fd00(arg_0);
        break;
    case 1:
        function_2952e0(arg_0);
        break;
    }
}

void __stdcall function_28fe50(long arg_0);

// @retail 0x2116c0
void function_2116c0(long arg_0, long arg_1, long arg_2)
{
    (void)&arg_1;
    (void)&arg_2;
    byte *local_0 = *(byte **)(g_4e0300->data + (arg_0 & 0xffff) * 12 + 8);
    if (*(long *)(local_0 + 0x134) == 0)
        function_28fe50(arg_0);
}
