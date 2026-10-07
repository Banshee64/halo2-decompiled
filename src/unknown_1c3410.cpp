// @flags /O2 /Ob1 /Gr /GL-
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1efac0.h"

class c_1c3450;
void function_1c3450(c_1c3450 *arg_0);

class c_1c3410
{
public:
 c_1c3410 *function_1c3410(dword arg_0);
 dword field_0;
 word field_4;
};

// @retail 0x1c3410
c_1c3410 *c_1c3410::function_1c3410(dword arg_0)
{
 function_1c3450((c_1c3450 *)this);
 if (arg_0 & 1) g_480118->allocate((long)this, field_4, 0x22);
 return this;
}

class c_1c58d0
{
public:
 c_1c3410 *function_1c58d0(dword arg_0);
};

// @retail 0x1c58d0
c_1c3410 *c_1c58d0::function_1c58d0(dword arg_0)
{
 return ((c_1c3410 *)((dword)this - 0xc))->function_1c3410(arg_0);
}

class c_1c58e0
{
public:
 c_1c3410 *function_1c58e0(dword arg_0);
};

// @retail 0x1c58e0
c_1c3410 *c_1c58e0::function_1c58e0(dword arg_0)
{
 return ((c_1c3410 *)((dword)this - 0x10))->function_1c3410(arg_0);
}
