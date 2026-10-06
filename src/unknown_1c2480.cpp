// @flags /O2 /Ob1 /Gr /GL-
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1efac0.h"

// @retail 0x1c2480
void __cdecl function_1c2480(void *arg_0)
{
	g_480118->allocate((long)arg_0, 0x50, 0x22);
}

struct s_1c24d0
{
	long field_0;
	word field_4;
};

// @retail 0x1c24d0
void __cdecl function_1c24d0(void *arg_0)
{
	g_480118->allocate((long)arg_0, ((s_1c24d0 *)arg_0)->field_4, 0x1c);
}

// @retail 0x1c2560
void __cdecl function_1c2560(void *arg_0)
{
	g_480118->allocate((long)arg_0, ((s_1c24d0 *)arg_0)->field_4, 0x22);
}

class c_1c2520
{
public:
	virtual void function_1c2520() = 0;
	virtual ~c_1c2520() {}
};

// @retail 0x1c2520 deleting c_1c2520

class c_1c2540
{
public:
	virtual void function_1c2540() = 0;
	virtual ~c_1c2540() {}
};

// @retail 0x1c2540 deleting c_1c2540

struct s_1c3450 : c_a
{
	long field_8;
};

class c_1c3450 : public s_1c3450, public c_1c2520, public c_1c2540
{
public:
	virtual void function_1c2520() = 0;
	virtual void function_1c2540() = 0;
};

// @retail 0x1c3450
void function_1c3450(c_1c3450 *arg_0)
{
	static_cast<c_1c2540 *>(arg_0)->c_1c2540::~c_1c2540();
	static_cast<c_1c2520 *>(arg_0)->c_1c2520::~c_1c2520();
	static_cast<c_a *>(arg_0)->c_a::~c_a();
}

// @retail 0x1c3440
void function_1c3440(c_1c3450 *arg_0)
{
	function_1c3450(arg_0);
}

class c_2de7b0
{
public:
	virtual ~c_2de7b0();
	byte field_4[0x4c];
};

class c_1c24a0 : public c_2de7b0
{
public:
	static void operator delete(void *arg_0)
	{
		g_480118->allocate((long)arg_0, 0x50, 0x22);
	}
};

// @retail 0x1c24a0 deleting c_1c24a0
