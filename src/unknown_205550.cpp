// @flags /O2 /Gr
#include "unknown_11c920.h"
#include <new>

class c_205558
{
public:
    virtual void function_205551(void) {}
    virtual void function_205552(void) {}
    virtual void function_205553(void) {}
    c_205558() { field_8 = 0; }
    word field_4;
    word field_6;
    long field_8;
};

class c_205550 : public c_205558
{
public:
    virtual void function_205551(void) {}
    c_205550() { field_6 = 0x80; }
    byte field_c[0x90 - 0xc];
};

struct s_205550
{
    c_205550 *field_0;
    long field_4;
};

class c_205559 : public c_205558
{
public:
    c_205559(s_205550 *arg_0, long arg_1)
    {
        *(long volatile *)&field_10 = arg_1;
        *(s_205550 *volatile *)&field_c = arg_0;
        *(dword volatile *)&field_14 = arg_1 | 0x80000000;
    }
    s_205550 *field_c;
    long field_10;
    dword field_14;
};

class c_205554 : public c_205559
{
public:
    virtual void function_205551(void) {}
    c_205554(s_205550 *arg_0, long arg_1) : c_205559(arg_0, arg_1) { field_6 = 0x80; }
};

// @retail 0x205550
void function_205550(void *arg_0)
{
    byte *local_0 = (byte *)arg_0;
    if (*(long *)(local_0 + 0x4c))
    {
        byte *local_1 = *(byte **)(local_0 + 0x50);
        for (long local_2 = 0; local_2 < *(long *)(local_1 + 0x38); local_2++)
            new (local_1 + 0x60 + local_2 * 0x90) c_205550;
        if (*(long *)(local_1 + 0x38) > 0)
        {
            s_205550 *local_3 = (s_205550 *)(local_1 + 0x18);
            for (long local_4 = 0; local_4 < *(long *)(local_1 + 0x38); local_4++)
            {
                c_205550 *local_5 = (c_205550 *)(local_1 + 0x60 + local_4 * 0x90);
                local_3[local_4].field_0 = local_5;
                local_3[local_4].field_4 = 0;
                local_5->field_8 = local_4 + 1;
            }
            new (local_1) c_205554(local_3, *(long *)(local_1 + 0x38));
        }
    }
}
