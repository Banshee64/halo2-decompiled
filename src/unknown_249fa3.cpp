// @flags /O1 /Gr
#include "cseries.h"
#include "loop_allocator.h"
#include "unknown_19b516.h"

struct s_459a60
{
	byte unknown00[0x38];
	void method_13ee20(bool flag);
};

s_459a60 g_459a60[3];
long g_470a60;
long g_51ea10;
long g_54e5d4;

void function_24a190(void *p);
void function_249fcc(c_unknown_249fa3 *p);
void function_24a150(void *p);

// @retail 0x18f3d4 deleting c_unknown_249fa3

// @retail 0x18f40e
c_unknown_249fa3::~c_unknown_249fa3()
{
}

// @retail 0x249fa3
void c_unknown_249fa3::slot1()
{
	g_470a60 = NONE;
	function_24a190(this);
	((c_widget *)this)->c_widget::v9();
	function_249fcc(this);
}

// @retail 0x249fc0
void c_unknown_249fa3::slot2()
{
	g_470a60 = NONE;
	((c_widget *)this)->c_widget::v10();
}

// @retail 0x24a01f
void c_unknown_249fa3::slot3()
{
	g_459a60[1].method_13ee20(true);
	g_459a60[2].method_13ee20(true);
	g_459a60[0].method_13ee20(true);

	if (g_51ea10 <= 0)
	{
		if ((field8c != NONE && g_54e5d4 == NONE) || (field8a && g_470a60 == NONE))
		{
			function_24a150(this);
			field8a = false;
		}
		field8c = g_54e5d4;
		slot24();
	}
	((c_widget *)this)->c_widget::v11();
}