// @flags /O2 /Gr
#include "cseries.h"

struct s_a0c4_object
{
	byte unknown00[0x2098];
	void *vtable2098;
	byte unknown209c[0xa0ac - 0x209c];
	void *vtable_a0ac;
};

struct s_flagged
{
	byte unknown00[0x29];
	bool field29;
};

class c_object_slot
{
public:
	virtual void slot0() {}
	virtual void slot1() {}
	virtual void slot2() {}
	virtual void slot3() {}
	virtual void slot4() {}
	virtual void slot5() {}
	virtual void slot6() {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void slot10() {}
	virtual void slot11() {}
	virtual void slot12() {}
	virtual void dispose(long flags) {}
};

struct s_4d87f8
{
	c_object_slot *object;
	long unknown04;
	byte unknown08;

	~s_4d87f8()
	{
		if (object)
		{
			object->dispose(1);
			object = 0;
		}
	}
};

byte g_5291a0[4];
byte g_450cb8[4];
byte g_450d60[4];
s_a0c4_object *g_4d87e8;
s_flagged *g_4d87f0;
s_flagged *g_4d87ec;
void *g_4d87d4;
void *g_4d87d8;
void *g_4d87dc;
s_4d87f8 *g_4d87f8;
void *g_4d87fc;
long g_4d87d0;
long g_4d87e0;
byte g_4d87e4;
long g_4d87c4;
long g_4d87c8;
long g_4d87cc;

void __stdcall function_78880(void *p);

// @retail 0x81f80
void function_81f80(void)
{
	function_78880(g_5291a0);

	if (g_4d87e8)
	{
		g_4d87e8->vtable_a0ac = g_450cb8;
		g_4d87e8->vtable2098 = g_450d60;
		g_4d87e8 = 0;
	}
	if (g_4d87f0)
	{
		g_4d87f0->field29 = false;
		g_4d87f0 = 0;
	}
	if (g_4d87ec)
	{
		g_4d87ec->field29 = false;
		g_4d87ec = 0;
	}
	if (g_4d87d4)
	{
		g_4d87d4 = 0;
	}
	if (g_4d87d8)
	{
		g_4d87d8 = 0;
	}
	if (g_4d87dc)
	{
		g_4d87dc = 0;
	}
	if (g_4d87f8)
	{
		delete g_4d87f8;
		g_4d87f8 = 0;
	}
	if (g_4d87fc)
	{
		g_4d87fc = 0;
	}
	g_4d87d0 = 0;
	g_4d87e0 = 0;
	g_4d87e4 = 0;
	g_4d87c4 = 0;
	g_4d87c8 = 0;
	g_4d87cc = 0;
}