// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include <new>

class c_1d7390
{
public:
	virtual void slot0() = 0;
	virtual void slot1() = 0;
	virtual void slot2() = 0;
	virtual void slot3() = 0;
	virtual void *function_1d7390(long arg_0, long arg_1) = 0;
};

struct s_2dc030
{
	byte field_0[8];
	long field_8;
	real field_c;
	real field_10;
	s_2dc030();
};

class c_2dbfc0
{
public:
	byte field_0[4];
	word field_4;
	byte field_6[0x3a];
	c_2dbfc0(long arg_0, void *arg_1, real arg_2);
	void function_2dbcf0(s_2dc030 const *arg_0);
};

// @retail 0x1d7390
c_2dbfc0 *function_1d7390(long arg_0, void *arg_1, real arg_2)
{
	(void)&arg_1;
	(void)&arg_2;
	c_2dbfc0 *local_0 = (c_2dbfc0 *)((c_1d7390 *)g_480118)->function_1d7390(0x40, 0x22);
	local_0->field_4 = 0x40;
	local_0 = new (local_0) c_2dbfc0(arg_0, arg_1, 10.0f);
	s_2dc030 local_1;
	local_1.field_c = arg_2;
	local_1.field_8 = arg_0;
	local_1.field_10 = arg_2 * 0.25f;
	local_0->function_2dbcf0(&local_1);
	return local_0;
}
