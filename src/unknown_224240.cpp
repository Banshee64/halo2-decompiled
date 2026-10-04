// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_224240.h"

struct s_interface_element
{
	byte unknown00[0x38];
};

struct s_range_view
{
	byte unknown00;
	byte unknown01 : 4;
	byte type : 4;
	byte unknown02[2];
	real minimum;
	real maximum;
};

struct s_interface_sub
{
	byte unknown00[4];
	s_range_view *range;

	real get_range_maximum()
	{
		real minimum = (range->type == 0) ? range->minimum : 0.0f;
		real maximum = (range->type == 0) ? range->maximum : 1.0f;
		return (minimum > maximum) ? minimum : maximum;
	}
};

struct s_interface_definition
{
	unsigned long flag0 : 1;
	unsigned long flag1 : 1;
	unsigned long flag2 : 1;
	unsigned long flag3 : 1;
	unsigned long flag4 : 1;
	unsigned long flag5 : 1;
	unsigned long flag6 : 1;
	unsigned long flag7 : 1;
	unsigned long flag8 : 1;
	unsigned long flag9 : 1;
	unsigned long flag10 : 1;
	unsigned long flag11 : 1;
	unsigned long flag12 : 1;
	unsigned long flag13 : 1;
	unsigned long flag14 : 1;
	unsigned long flag15 : 1;
	unsigned long flags_high : 16;
	byte unknown04[0x18];
	long tag_index;
	byte unknown20[0x24];
	s_interface_sub sub44;
	byte unknown4c[0x14];
	long value60;
	long value64;
	long value68;
	long value6c;
	long value70;
	long value74;
	long value78;
	s_interface_element *elements7c;
	long value80;
	long value84;
	s_interface_element *elements88;
	long value8c;
	long value90;
};

class c_interface_base
{
public:
	virtual long v0() { return 0; }
	virtual long v1() { return 0; }
	virtual long v2() { return 0; }
	virtual long v3() { return 0; }
	virtual long v4() { return NONE; }
	virtual long v5() { return NONE; }
	virtual long v6() { return 0; }
	virtual void *v7(long index) { return 0; }
	virtual long v8() { return 0; }
	virtual long v9() { return 0; }
	virtual long v10() { return 0; }
	virtual void v11(long a, long b, long c, long d) { }
	virtual bool v12() { return false; }
	virtual bool v13() { return false; }
	virtual bool v14() { return false; }
	virtual bool v15() { return false; }
	virtual bool v16() { return false; }
	virtual bool v17() { return false; }
	virtual bool v18() { return false; }
	virtual real v19() { return 0.0f; }
	virtual void v20(long tag_index) { }

	long tag_index;
	s_interface_definition *data;
};

class c_interface_a : public c_interface_base
{
public:
	virtual long v0();
	virtual long v1();
	virtual long v2();
	virtual long v3();
	virtual long v4();
	virtual long v5();
	virtual long v6();
	virtual void *v7(long index);
	virtual long v8();
	virtual long v9();
	virtual long v10();
	virtual void v11(long a, long b, long c, long d);
	virtual bool v12();
	virtual bool v13();
	virtual bool v14();
	virtual bool v15();
	virtual bool v16();
	virtual bool v17();
	virtual bool v18();
	virtual real v19();
	virtual void v20(long index);
};

class c_interface_b : public c_interface_base
{
public:
	virtual long v0();
	virtual long v1();
	virtual long v2();
	virtual long v3();
	virtual long v4();
	virtual long v5();
	virtual long v6();
	virtual void *v7(long index);
	virtual long v8();
	virtual long v9();
	virtual long v10();
	virtual void v11(long a, long b, long c, long d);
	virtual bool v12();
	virtual bool v13();
	virtual bool v14();
	virtual bool v15();
	virtual bool v16();
	virtual bool v17();
	virtual bool v18();
	virtual real v19();
	virtual void v20(long index);
};
// @retail 0x224cf0
long c_interface_a::v0()
{
	return data->value60;
}

// @retail 0x224d00
long c_interface_a::v1()
{
	return data->value64;
}

// @retail 0x224d10
long c_interface_a::v2()
{
	return data->value68;
}

// @retail 0x224d20
long c_interface_a::v3()
{
	return data->value6c;
}

// @retail 0x224240
long c_interface_a::v4()
{
	return NONE;
}

long c_interface_a::v5()
{
	return NONE;
}

// @retail 0x224d30
long c_interface_a::v6()
{
	return data->value78;
}

// @retail 0x224d50
long c_interface_a::v8()
{
	return data->value70;
}

// @retail 0x224d60
long c_interface_a::v9()
{
	return data->value74;
}

// @retail 0x224d70
long c_interface_a::v10()
{
	return *(long *)((byte *)g_4e3b44[data->tag_index & 0xffff].data + 0x24);
}

// @retail 0x224d90
void c_interface_a::v11(long a, long b, long c, long d)
{
	function_224db0(data, a, b, c, d);
}

// @retail 0x2242e0
bool c_interface_a::v12()
{
	return TEST_FIELD_BIT(data->flag14);
}

// @retail 0x2242f0
bool c_interface_a::v13()
{
	return TEST_FIELD_BIT(data->flag9);
}

// @retail 0x224300
bool c_interface_a::v14()
{
	return TEST_FIELD_BIT(data->flag10);
}

// @retail 0x224310
bool c_interface_a::v15()
{
	return TEST_FIELD_BIT(data->flag11);
}

// @retail 0x224320
bool c_interface_a::v16()
{
	return TEST_FIELD_BIT(data->flag12);
}

// @retail 0x224330
bool c_interface_a::v17()
{
	return TEST_FIELD_BIT(data->flag7);
}

// @retail 0x224340
bool c_interface_a::v18()
{
	return TEST_FIELD_BIT(data->flag8);
}

// @retail 0x224da0
real c_interface_a::v19()
{
	return 1.0f;
}

// @retail 0x2243a0
void c_interface_a::v20(long index)
{
	tag_index = index;
	data = (s_interface_definition *)g_4e3b44[index & 0xffff].data;
}

long c_interface_b::v0()
{
	return data->value6c;
}

long c_interface_b::v1()
{
	return data->value70;
}

long c_interface_b::v2()
{
	return data->value74;
}

long c_interface_b::v3()
{
	return data->value78;
}

long c_interface_b::v4()
{
	return NONE;
}

long c_interface_b::v5()
{
	return NONE;
}

// @retail 0x224250
long c_interface_b::v6()
{
	return data->value84;
}

// @retail 0x224280
long c_interface_b::v8()
{
	return (long)data->elements7c;
}

// @retail 0x224290
long c_interface_b::v9()
{
	return data->value80;
}

// @retail 0x2242a0
long c_interface_b::v10()
{
	return data->value90;
}

// @retail 0x2242b0
void c_interface_b::v11(long a, long b, long c, long d)
{
	function_2243c0(data, a, b, tag_index, c, d);
}

bool c_interface_b::v12()
{
	return TEST_FIELD_BIT(data->flag14);
}

bool c_interface_b::v13()
{
	return TEST_FIELD_BIT(data->flag9);
}

bool c_interface_b::v14()
{
	return TEST_FIELD_BIT(data->flag10);
}

bool c_interface_b::v15()
{
	return TEST_FIELD_BIT(data->flag11);
}

bool c_interface_b::v16()
{
	return TEST_FIELD_BIT(data->flag12);
}

bool c_interface_b::v17()
{
	return TEST_FIELD_BIT(data->flag7);
}

bool c_interface_b::v18()
{
	return TEST_FIELD_BIT(data->flag8);
}

// @retail 0x224350
real c_interface_b::v19()
{
	return data->sub44.get_range_maximum();
}

void c_interface_b::v20(long index)
{
	tag_index = index;
	data = (s_interface_definition *)g_4e3b44[index & 0xffff].data;
}

// @retail 0x224d40
void *c_interface_a::v7(long index)
{
	return &data->elements7c[index];
}

// @retail 0x224260
void *c_interface_b::v7(long index)
{
	return &data->elements88[index];
}
